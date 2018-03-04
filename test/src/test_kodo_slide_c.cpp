// Copyright Steinwurf ApS 2016.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <kodo_slide_c/kodo_slide_c.h>

#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

// TEST(test_kodo_slide_c, factory_api)
// {
//     uint32_t symbols = 50;
//     uint32_t symbol_size = 750;
//     auto decoder_factory = kodo_slide_decoder_factory_construct(
//         kodo_slide_binary8, symbols, symbol_size);

//     auto encoder_factory = kodo_slide_encoder_factory_construct(
//         kodo_slide_binary8, symbols, symbol_size);

//     EXPECT_EQ(symbols, kodo_slide_decoder_factory_symbols(decoder_factory));
//     EXPECT_EQ(symbols, kodo_slide_encoder_factory_symbols(encoder_factory));

//     EXPECT_EQ(symbol_size, kodo_slide_encoder_factory_symbol_size(encoder_factory));
//     EXPECT_EQ(symbol_size, kodo_slide_decoder_factory_symbol_size(decoder_factory));

//     uint32_t new_symbols = 25;

//     kodo_slide_encoder_factory_set_symbols(encoder_factory, new_symbols);
//     EXPECT_EQ(new_symbols, kodo_slide_encoder_factory_symbols(encoder_factory));

//     kodo_slide_decoder_factory_set_symbols(decoder_factory, new_symbols);
//     EXPECT_EQ(new_symbols, kodo_slide_decoder_factory_symbols(decoder_factory));

//     uint32_t new_symbol_size = 300;

//     kodo_slide_encoder_factory_set_symbol_size(encoder_factory, new_symbol_size);
//     EXPECT_EQ(new_symbol_size, kodo_slide_encoder_factory_symbol_size(encoder_factory));

//     kodo_slide_decoder_factory_set_symbol_size(decoder_factory, new_symbol_size);
//     EXPECT_EQ(new_symbol_size, kodo_slide_decoder_factory_symbol_size(decoder_factory));

//     kodo_slide_decoder_factory_destruct(decoder_factory);
//     kodo_slide_encoder_factory_destruct(encoder_factory);
// }

typedef struct
{
    uint32_t m_symbol_size;
    uint32_t m_symbols;
    uint8_t* m_data;
}  symbol_storage;

symbol_storage* alloc_storage(uint32_t symbol_size, uint32_t symbols)
{
    assert(symbol_size > 0);
    assert(symbols > 0);

    symbol_storage* storage = (symbol_storage*) malloc(sizeof(symbol_storage));
    storage->m_symbol_size = symbol_size;
    storage->m_symbols = symbols;
    storage->m_data = (uint8_t*) malloc(symbol_size*symbols);
    memset(storage->m_data, 0, symbol_size*symbols);

    return storage;
}

void free_storage(symbol_storage* storage)
{
    assert(storage != 0);

    free(storage->m_data);
    free(storage);
}

void randomize_storage(symbol_storage* storage)
{
    assert(storage != 0);

    uint32_t size = storage->m_symbol_size * storage->m_symbols;
    for(uint32_t i = 0; i < size; ++i)
    {
        storage->m_data[i] = rand();
    }
}

uint8_t* storage_symbol(symbol_storage* storage, uint32_t index)
{
    assert(storage != 0);
    assert(index < storage->m_symbols);

    return storage->m_data + (index * storage->m_symbol_size);
}

TEST(test_kodo_slide_c, api)
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
    symbol_storage* decoder_storage = alloc_storage(symbol_size, symbols);
    symbol_storage* encoder_storage = alloc_storage(symbol_size, symbols);

    // Fill the encoder storage with random data
    randomize_storage(encoder_storage);

    // Provide the decoder with storage
    for(uint32_t i = 0; i < symbols; ++i)
    {
        uint8_t* symbol = storage_symbol(decoder_storage, i);
        kslide_decoder_push_front_symbol(decoder, symbol);
    }

    uint32_t iterations = 0U;
    uint32_t max_iterations = 1000U;
    uint32_t symbols_decoded = 0;

    while(symbols_decoded < symbols && iterations < max_iterations)
    {
        if (kslide_encoder_stream_symbols(encoder) < symbols && rand() % 2)
        {
            uint8_t* symbol = storage_symbol(
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
            kslide_encoder_coefficients_vector_size(encoder));

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
        symbols*symbol_size), 0U);


    kslide_delete_decoder(decoder);
    kslide_delete_encoder(encoder);
    kslide_delete_decoder_factory(decoder_factory);
    kslide_delete_encoder_factory(encoder_factory);
}
