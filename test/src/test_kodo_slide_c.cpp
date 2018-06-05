// Copyright Steinwurf ApS 2016.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodo_slide_c/kodo_slide_c.h>

#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

TEST(test_kodo_slide_c, factory_api)
{
    uint32_t symbol_size = 1300;
    int32_t field = kslide_binary8;
    auto decoder_factory = kslide_new_decoder_factory();
    auto encoder_factory = kslide_new_encoder_factory();

    EXPECT_EQ(field, kslide_decoder_factory_field(decoder_factory));
    EXPECT_EQ(field, kslide_encoder_factory_field(encoder_factory));

    for (auto new_field : { kslide_binary, kslide_binary4, kslide_binary8, kslide_binary16 })
    {
        kslide_decoder_factory_set_field(decoder_factory, new_field);
        kslide_encoder_factory_set_field(encoder_factory, new_field);
        EXPECT_EQ(new_field, kslide_decoder_factory_field(decoder_factory));
        EXPECT_EQ(new_field, kslide_encoder_factory_field(encoder_factory));
    }

    EXPECT_EQ(symbol_size, kslide_encoder_factory_symbol_size(encoder_factory));
    EXPECT_EQ(symbol_size, kslide_decoder_factory_symbol_size(decoder_factory));

    uint32_t new_symbol_size = 300;

    kslide_encoder_factory_set_symbol_size(encoder_factory, new_symbol_size);
    EXPECT_EQ(new_symbol_size, kslide_encoder_factory_symbol_size(encoder_factory));

    kslide_decoder_factory_set_symbol_size(decoder_factory, new_symbol_size);
    EXPECT_EQ(new_symbol_size, kslide_decoder_factory_symbol_size(decoder_factory));

    kslide_delete_decoder_factory(decoder_factory);
    kslide_delete_encoder_factory(encoder_factory);
}

void randomize_buffer(uint8_t* data, uint32_t size)
{
    assert(data != nullptr);
    for (uint32_t i = 0; i < size; ++i)
    {
        data[i] = rand();
    }
}

uint32_t accumulate_buffer(uint8_t* data, uint64_t size)
{
    uint32_t result = 0;
    for (uint32_t i = 0; i < size; i++)
        result += data[i];
    return result;
}


typedef struct
{
    uint32_t m_symbol_size;
    uint32_t m_capacity;
    uint8_t* m_data;
}  symbol_storage;

symbol_storage* symbol_storage_alloc(uint32_t capacity, uint32_t symbol_size)
{
    assert(symbol_size > 0);
    assert(capacity > 0);

    symbol_storage* storage = (symbol_storage*) malloc(sizeof(symbol_storage));
    storage->m_symbol_size = symbol_size;
    storage->m_capacity = capacity;
    storage->m_data = (uint8_t*) malloc(symbol_size * capacity);
    memset(storage->m_data, 0, symbol_size * capacity);

    return storage;
}

void symbol_storage_free(symbol_storage* storage)
{
    assert(storage != 0);

    free(storage->m_data);
    free(storage);
}

void symbol_storage_randomize(symbol_storage* storage)
{
    assert(storage != nullptr);
    uint32_t size = storage->m_symbol_size * storage->m_capacity;
    randomize_buffer(storage->m_data, size);
}

uint8_t* symbol_storage_symbol(symbol_storage* storage, uint32_t index)
{
    assert(storage != nullptr);
    uint32_t mapped_index = index % storage->m_capacity;
    return storage->m_data + (storage->m_symbol_size * mapped_index);
}

typedef struct
{
    uint32_t m_n = 0;
    uint32_t m_k = 0;
    uint32_t m_position = 0;
}  rate_controller;

rate_controller rate_controller_init(uint32_t n, uint32_t k)
{
    rate_controller controller;
    controller.m_n = n;
    controller.m_k = k;
    controller.m_position = 0;
    return controller;
}

void rate_controller_advance(rate_controller& controller)
{
    assert(controller.m_position < controller.m_n);

    controller.m_position = (controller.m_position + 1) % controller.m_n;

    assert(controller.m_position < controller.m_n);
}

bool rate_controller_generate_data(rate_controller& controller)
{
    return controller.m_position < controller.m_k;
}


TEST(test_kodo_slide_c, basic_api)
{
    uint32_t symbols = 100U;
    uint32_t symbol_size = 750U;

    kslide_decoder_factory_t* decoder_factory = kslide_new_decoder_factory();
    kslide_encoder_factory_t* encoder_factory = kslide_new_encoder_factory();

    kslide_decoder_factory_set_symbol_size(decoder_factory, symbol_size);
    kslide_encoder_factory_set_symbol_size(encoder_factory, symbol_size);

    EXPECT_EQ(
        kslide_decoder_factory_symbol_size(decoder_factory),
        kslide_encoder_factory_symbol_size(encoder_factory));

    kslide_decoder_t* decoder = kslide_decoder_factory_build(decoder_factory);
    kslide_encoder_t* encoder = kslide_encoder_factory_build(encoder_factory);

    EXPECT_EQ(symbol_size, kslide_decoder_symbol_size(decoder));
    EXPECT_EQ(symbol_size, kslide_encoder_symbol_size(encoder));

    // Allocate memory for the decoder
    symbol_storage* decoder_storage = symbol_storage_alloc(symbols, symbol_size);
    symbol_storage* encoder_storage = symbol_storage_alloc(symbols, symbol_size);

    // Fill the encoder storage with random data
    symbol_storage_randomize(encoder_storage);

    // Provide the decoder with storage
    for (uint32_t i = 0; i < symbols; ++i)
    {
        uint8_t* symbol = symbol_storage_symbol(decoder_storage, i);
        kslide_decoder_push_front_symbol(decoder, symbol);
    }

    uint32_t iterations = 0U;
    uint32_t max_iterations = 1000U;
    uint32_t symbols_decoded = 0;

    while (symbols_decoded < symbols && iterations < max_iterations)
    {
        if (kslide_encoder_stream_symbols(encoder) < symbols && rand() % 2)
        {
            uint8_t* symbol = symbol_storage_symbol(
                encoder_storage, kslide_encoder_stream_symbols(encoder));

            kslide_encoder_push_front_symbol(encoder, symbol);
        }

        if (kslide_encoder_stream_symbols(encoder) == 0)
        {
            continue;
        }

        kslide_encoder_set_window(encoder,
                                  kslide_encoder_stream_lower_bound(encoder),
                                  kslide_encoder_stream_symbols(encoder));

        kslide_decoder_set_window(decoder,
                                  kslide_encoder_stream_lower_bound(encoder),
                                  kslide_encoder_stream_symbols(encoder));

        uint8_t* coefficients = (uint8_t*) malloc(
            kslide_encoder_coefficient_vector_size(encoder));

        uint8_t* symbol = (uint8_t*) malloc(
            kslide_encoder_symbol_size(encoder));

        uint32_t seed = rand();

        kslide_encoder_set_seed(encoder, seed);
        kslide_encoder_generate(encoder, coefficients);

        kslide_encoder_write_symbol(encoder, symbol, coefficients);
        kslide_decoder_read_symbol(decoder, symbol, coefficients);

        free(coefficients);
        free(symbol);

        symbols_decoded = kslide_decoder_symbols_decoded(decoder);
        ++iterations;
    }

    EXPECT_TRUE(iterations != max_iterations);

    EXPECT_EQ(memcmp(decoder_storage->m_data, encoder_storage->m_data,
                     symbols*symbol_size), 0);


    kslide_delete_decoder(decoder);
    kslide_delete_encoder(encoder);

    symbol_storage_free(decoder_storage);
    symbol_storage_free(encoder_storage);

    kslide_delete_decoder_factory(decoder_factory);
    kslide_delete_encoder_factory(encoder_factory);
}


// Test that everything works when sliding the window - also with losses. Since
// we only move the window and stream forward we may not decode all symbols.
TEST(test_kodo_slide_c, slide_window)
{
    srand(time(0));

    // Set the capacity of the decoder (this is the number of encoded symbols
    // that are used in the decoding process).
    uint32_t capacity = 10U;

    // Set the window size (this is the number of symbols included in an
    // encoded symbol).
    uint32_t window_symbols = 5U;

    // The size of a symbol in bytes
    uint32_t symbol_size = 160U;

    // Maximum number of interations
    uint32_t max_iterations = 1000U;

    kslide_decoder_factory_t* decoder_factory = kslide_new_decoder_factory();
    kslide_encoder_factory_t* encoder_factory = kslide_new_encoder_factory();

    kslide_encoder_factory_set_symbol_size(encoder_factory, symbol_size);
    kslide_decoder_factory_set_symbol_size(decoder_factory, symbol_size);

    auto encoder = kslide_encoder_factory_build(encoder_factory);
    auto decoder = kslide_decoder_factory_build(decoder_factory);

    // Allocate our finite memory for decoding
    symbol_storage* decoder_storage = symbol_storage_alloc(capacity, symbol_size);

    uint32_t source_symbols_index = 0;
    uint32_t source_symbol_count = max_iterations;
    uint8_t** source_symbols =
        (uint8_t**)calloc(source_symbol_count, sizeof(uint8_t*));

    // Provide the decoder with storage
    for (uint32_t i = 0; i < capacity; ++i)
    {
        uint8_t* symbol = symbol_storage_symbol(decoder_storage, i);
        kslide_decoder_push_front_symbol(decoder, symbol);
    }

    // Initialize our rate controller
    rate_controller control = rate_controller_init(6, 3);

    // Make sure we will not hang on bugs that cause infinite loops
    uint32_t iterations = 0;

    // Counter for keeping track of the number of decoded symbols
    uint32_t decoded = 0;

    while (decoded < 100U && iterations < max_iterations)
    {
        // Update loop state
        ++iterations;
        // Manage the encoder's window
        if (rate_controller_generate_data(control))
        {
            // Create a new source symbol
            uint8_t* source_symbol = (uint8_t*)malloc(symbol_size);
            source_symbols[source_symbols_index] = source_symbol;
            source_symbols_index++;
            assert(source_symbols_index < source_symbol_count);

            randomize_buffer(source_symbol, symbol_size);

            if (kslide_encoder_window_symbols(encoder) == window_symbols)
            {
                // If window is full - pop a symbol before pushing a new one
                kslide_encoder_pop_back_symbol(encoder);
            }
            randomize_buffer(source_symbol, symbol_size);
            kslide_encoder_push_front_symbol(encoder, source_symbol);
        }

        // Choose a seed for this encoding
        uint32_t seed = rand();

        // Encode a symbol
        kslide_encoder_set_window(
            encoder,
            kslide_encoder_stream_lower_bound(encoder),
            kslide_encoder_stream_symbols(encoder));

        uint8_t* coefficients =
            (uint8_t*)calloc(1, kslide_encoder_coefficient_vector_size(encoder));

        uint8_t* symbol = (uint8_t*)calloc(1, kslide_encoder_symbol_size(encoder));

        kslide_encoder_set_seed(encoder, seed);
        kslide_encoder_generate(encoder, coefficients);
        kslide_encoder_write_symbol(encoder, symbol, coefficients);

        rate_controller_advance(control);

        if (rand() % 2)
        {
            free(coefficients);
            free(symbol);
            // Simulate 50% packet loss
            continue;
        }

        // Move the decoders's window / stream if needed
        while (kslide_decoder_stream_upper_bound(decoder) <
               kslide_encoder_stream_upper_bound(encoder))
        {
            uint32_t lower_bound = kslide_decoder_stream_lower_bound(decoder);
            uint8_t* decoder_symbol = symbol_storage_symbol(decoder_storage, lower_bound);

            if (kslide_decoder_is_symbol_decoded(decoder, lower_bound))
            {
                ++decoded;
                // Compare with corresponding source symbol
                uint8_t* source_symbol = source_symbols[lower_bound];
                EXPECT_EQ(0, memcmp(decoder_symbol, source_symbol, symbol_size));
            }

            uint32_t pop_index = kslide_decoder_pop_back_symbol(decoder);
            assert(pop_index == lower_bound);

            // Moves the decoder's upper bound
            kslide_decoder_push_front_symbol(decoder, decoder_symbol);
        }

        // Decode the symbol
        kslide_decoder_set_window(
            decoder,
            kslide_encoder_window_lower_bound(encoder),
            kslide_encoder_window_symbols(encoder));

        kslide_decoder_set_seed(decoder, seed);
        kslide_decoder_generate(decoder, coefficients);
        kslide_decoder_read_symbol(decoder, symbol, coefficients);

        free(coefficients);
        free(symbol);
    }
    SCOPED_TRACE(testing::Message() << "decoded = " << decoded);

    EXPECT_LT(iterations, max_iterations);
    EXPECT_GE(decoded, 100U);

    for (uint32_t i = 0; i < max_iterations; i++)
    {
        free(source_symbols[i]);
    }
    free(source_symbols);
    symbol_storage_free(decoder_storage);
    kslide_delete_encoder(encoder);
    kslide_delete_encoder_factory(encoder_factory);
    kslide_delete_decoder(decoder);
    kslide_delete_decoder_factory(decoder_factory);
}

TEST(test_kodo_slide_c, encoder_api)
{
    srand(time(0));

    uint32_t symbol_size = 200U;

    kslide_encoder_factory_t* factory = kslide_new_encoder_factory();
    kslide_encoder_factory_set_symbol_size(factory, symbol_size);

    kslide_encoder_t* encoder = kslide_encoder_factory_build(factory);

    uint8_t* symbol = (uint8_t*) calloc(1, symbol_size);

    while (kslide_encoder_stream_lower_bound(encoder) < 100)
    {
        kslide_encoder_push_front_symbol(encoder, symbol);

        kslide_encoder_set_window(
            encoder,
            kslide_encoder_stream_lower_bound(encoder),
            kslide_encoder_stream_symbols(encoder));

        EXPECT_GE(kslide_encoder_coefficient_vector_size(encoder), 1U);
        uint8_t* coefficients = (uint8_t*) malloc(
            kslide_encoder_coefficient_vector_size(encoder));

        kslide_encoder_set_seed(encoder, rand());
        kslide_encoder_generate(encoder, coefficients);

        // The coefficient vector must not be all zero. To check this we simply
        // sum the values in the vector and see that it is > 0.
        uint32_t coefficients_sum = accumulate_buffer(
            coefficients, kslide_encoder_coefficient_vector_size(encoder));

        EXPECT_NE(coefficients_sum, 0U);

        if (symbol_size != kslide_encoder_symbol_size(encoder))
        {
            free(symbol);
            symbol_size = kslide_encoder_symbol_size(encoder);
            symbol = (uint8_t*) malloc(symbol_size);
        }
        uint64_t index = kslide_encoder_window_lower_bound(encoder);
        kslide_encoder_write_source_symbol(encoder, symbol, index);
        kslide_encoder_write_symbol(encoder, symbol, coefficients);

        kslide_encoder_pop_back_symbol(encoder);
        free(coefficients);
    }
    free(symbol);

    kslide_delete_encoder(encoder);
    kslide_delete_encoder_factory(factory);
}

TEST(test_kodo_slide_c, decoder_api)
{
    srand(time(0));

    uint32_t symbol_size = 200U;

    kslide_decoder_factory_t* factory = kslide_new_decoder_factory();
    kslide_decoder_factory_set_symbol_size(factory, symbol_size);

    kslide_decoder_t* decoder = kslide_decoder_factory_build(factory);

    EXPECT_EQ(symbol_size, kslide_decoder_symbol_size(decoder));

    uint8_t* input_symbol = (uint8_t*) calloc(1, symbol_size);
    uint8_t* output_symbol = (uint8_t*) calloc(1, symbol_size);

    while (kslide_decoder_stream_lower_bound(decoder) < 100)
    {
        if (rand() % 2)
        {
            kslide_decoder_push_front_symbol(decoder, output_symbol);
        }

        if (kslide_decoder_stream_symbols(decoder) == 0)
        {
            continue;
        }

        kslide_decoder_set_window(
            decoder,
            kslide_decoder_stream_lower_bound(decoder),
            kslide_decoder_stream_symbols(decoder));
        EXPECT_GE(kslide_decoder_coefficient_vector_size(decoder), 1U);

        uint8_t* coefficients = (uint8_t*) malloc(
            kslide_decoder_coefficient_vector_size(decoder));

        kslide_decoder_set_seed(decoder, rand());
        kslide_decoder_generate(decoder, coefficients);

        // The coefficient vector must not be all zero. To check this we simply
        // sum the values in the vector and see that it is not equal to 0.
        uint32_t coefficients_sum = accumulate_buffer(
            coefficients, kslide_decoder_coefficient_vector_size(decoder));
        EXPECT_NE(coefficients_sum, 0U);

        kslide_decoder_read_symbol(decoder, input_symbol, coefficients);

        if (kslide_decoder_stream_symbols(decoder) > 0 && rand() % 2)
        {
            kslide_decoder_pop_back_symbol(decoder);
        }
        free(coefficients);
    }
    free(input_symbol);
    free(output_symbol);

    kslide_delete_decoder(decoder);
    kslide_delete_decoder_factory(factory);
}


void mix_coded_uncoded(kslide_finite_field field)
{
    srand(time(0));

    // Set the capacity of the decoder (this is the number of encoded symbols
    // that are used in the decoding process).
    uint32_t capacity = 15U;

    // Set the window size (this is the number of symbols included in an
    // encoded symbol).
    uint32_t window_symbols = 5U;

    // The size of a symbol in bytes
    uint32_t symbol_size = 160U;

    // Maximum number of interations
    uint32_t max_iterations = 1000U;

    kslide_decoder_factory_t* decoder_factory = kslide_new_decoder_factory();
    kslide_encoder_factory_t* encoder_factory = kslide_new_encoder_factory();

    kslide_encoder_factory_set_symbol_size(encoder_factory, symbol_size);
    kslide_decoder_factory_set_symbol_size(decoder_factory, symbol_size);

    kslide_encoder_factory_set_field(encoder_factory, field);
    kslide_decoder_factory_set_field(decoder_factory, field);

    auto encoder = kslide_encoder_factory_build(encoder_factory);
    auto decoder = kslide_decoder_factory_build(decoder_factory);

    // Cache for all original source symbols added to the encoder - such that
    // we can check that they are decoded correctly.
    uint32_t source_symbols_index = 0;
    uint32_t source_symbol_count = max_iterations;
    uint8_t** source_symbols =
        (uint8_t**)calloc(source_symbol_count, sizeof(uint8_t*));

    // Allocate our finite memory for decoding
    symbol_storage* decoder_storage = symbol_storage_alloc(capacity, symbol_size);

    // Provide the decoder with storage
    for (uint32_t i = 0; i < capacity; ++i)
    {
        uint8_t* symbol = symbol_storage_symbol(decoder_storage, i);
        kslide_decoder_push_front_symbol(decoder, symbol);
    }

    // Initialize our rate controller
    rate_controller control = rate_controller_init(8, 3);

    // Make sure we will not hang on bugs that cause infinite loops
    uint32_t iterations = 0;

    // Counter for keeping track of the number of decoded symbols
    uint32_t decoded = 0;

    while (decoded < 100U && iterations < max_iterations)
    {
        // Update loop state
        ++iterations;
        // Manage the encoder's window
        if (rate_controller_generate_data(control))
        {
            // Create a new source symbol
            uint8_t* source_symbol = (uint8_t*)malloc(symbol_size);
            source_symbols[source_symbols_index] = source_symbol;
            source_symbols_index++;
            assert(source_symbols_index < source_symbol_count);

            randomize_buffer(source_symbol, symbol_size);

            if (kslide_encoder_stream_symbols(encoder) == window_symbols)
            {
                // If window is full - pop a symbol before pushing a new one
                kslide_encoder_pop_back_symbol(encoder);
            }

            kslide_encoder_push_front_symbol(encoder, source_symbol);
        }

        // Uncoded or coded
        bool coded = rand() % 2;
        uint64_t random_index = 0;

        // Choose a seed for this encoding
        uint32_t seed = rand();

        // Encode a symbol
        kslide_encoder_set_window(
            encoder,
            kslide_encoder_stream_lower_bound(encoder),
            kslide_encoder_stream_symbols(encoder));


        uint8_t* coefficients = (uint8_t*) malloc(
            kslide_encoder_coefficient_vector_size(encoder));
        uint8_t* symbol = (uint8_t*)malloc(kslide_encoder_symbol_size(encoder));

        if (coded)
        {
            kslide_encoder_set_seed(encoder, seed);
            kslide_encoder_generate(encoder, coefficients);
            kslide_encoder_write_symbol(encoder, symbol, coefficients);
        }
        else
        {
            // Warning: This approach is biased towards the lower end.
            uint64_t max_index = kslide_encoder_window_upper_bound(encoder) - 1;
            uint64_t min_index = kslide_encoder_window_lower_bound(encoder);
            random_index = rand() % (max_index - min_index + 1) + min_index;

            kslide_encoder_write_source_symbol(encoder, symbol, random_index);
        }

        rate_controller_advance(control);

        if (rand() % 2)
        {
            free(coefficients);
            free(symbol);
            // Simulate 50% packet loss
            continue;
        }

        // Move the decoders's window / stream if needed
        while (kslide_decoder_stream_upper_bound(decoder) <
               kslide_encoder_stream_upper_bound(encoder))
        {
            uint32_t lower_bound = kslide_decoder_stream_lower_bound(decoder);
            uint8_t* decoder_symbol = symbol_storage_symbol(decoder_storage, lower_bound);

            if (kslide_decoder_is_symbol_decoded(decoder, lower_bound))
            {
                ++decoded;

                // Compare with corresponding source symbol
                uint8_t* source_symbol = source_symbols[lower_bound];
                EXPECT_EQ(0, memcmp(decoder_symbol, source_symbol, symbol_size));
            }

            uint32_t pop_index = kslide_decoder_pop_back_symbol(decoder);
            assert(pop_index == lower_bound);

            // Moves the decoder's upper bound
            kslide_decoder_push_front_symbol(decoder, decoder_symbol);
        }

        // Decode the symbol
        kslide_decoder_set_window(
            decoder,
            kslide_encoder_window_lower_bound(encoder),
            kslide_encoder_window_symbols(encoder));

        if (coded)
        {
            kslide_decoder_set_seed(decoder, seed);
            kslide_decoder_generate(decoder, coefficients);
            kslide_decoder_read_symbol(decoder, symbol, coefficients);
        }
        else
        {
            kslide_decoder_read_source_symbol(decoder, symbol, random_index);
        }

        free(coefficients);
        free(symbol);
    }
    SCOPED_TRACE(testing::Message() << "decoded = " << decoded);

    EXPECT_LT(iterations, max_iterations);
    EXPECT_GE(decoded, 100U);

    symbol_storage_free(decoder_storage);

    for (uint32_t i = 0; i < max_iterations; i++)
    {
        free(source_symbols[i]);
    }
    free(source_symbols);
    kslide_delete_encoder(encoder);
    kslide_delete_encoder_factory(encoder_factory);
    kslide_delete_decoder(decoder);
    kslide_delete_decoder_factory(decoder_factory);
}

// Test that everything works when sliding the window - also with losses. Since
// we only move the window and stream forward we may not decode all symbols.
TEST(test_code, mix_coded_uncoded)
{
    mix_coded_uncoded(kslide_binary);
    mix_coded_uncoded(kslide_binary4);
    mix_coded_uncoded(kslide_binary8);
    mix_coded_uncoded(kslide_binary16);
}
