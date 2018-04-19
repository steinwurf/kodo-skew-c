// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodo_slide_c.h"

#include <cstring>
#include <cstdint>
#include <cassert>
#include <string>

#include <kodo_slide/details/stack_encoder.hpp>
#include <kodo_slide/details/stack_decoder.hpp>

struct kslide_decoder
{
    kodo_slide::details::stack_decoder m_impl;
};

struct kslide_decoder_factory
{
    kodo_slide::details::stack_decoder::config m_impl;
};

struct kslide_encoder
{
    kodo_slide::details::stack_encoder m_impl;
};

struct kslide_encoder_factory
{
    kodo_slide::details::stack_encoder::config m_impl;
};

// struct kodo_slide_encoder_factory
// {
//     template<class... Args>
//     kodo_slide_encoder_factory(Args&&... args) :
//         m_encoder_factory(std::forward<Args>(args)...)
//     { }

//     kodo_slide_c::on_the_fly_encoder_stack::config m_encoder_factory;
// };

// fifi::api::field build_finite_field(int32_t finite_field_id)
// {
//     switch (finite_field_id)
//     {
//         case kodo_slide_binary:
//             return fifi::api::field::binary;
//         case kodo_slide_binary4:
//             return fifi::api::field::binary4;
//         case kodo_slide_binary8:
//             return fifi::api::field::binary8;
//     default:
//         assert(false && "Unknown field");
//         return fifi::api::field::binary;
//     }
// }

//------------------------------------------------------------------
// ENCODER FACTORY API
//------------------------------------------------------------------

kslide_encoder_factory_t* kslide_new_encoder_factory()
{
    return new kslide_encoder_factory;
}

void kslide_delete_encoder_factory(kslide_encoder_factory_t* factory)
{
    assert(factory != nullptr);
    delete factory;
}

uint32_t kslide_encoder_factory_symbol_size(kslide_encoder_factory_t* factory)
{
    assert(factory != nullptr);
    return factory->m_impl.symbol_size();
}

void kslide_encoder_factory_set_symbol_size(
    kslide_encoder_factory_t* factory, uint32_t symbol_size)
{
    assert(factory != nullptr);
    factory->m_impl.set_symbol_size(symbol_size);
}

kslide_encoder_t* kslide_encoder_factory_build(
    kslide_encoder_factory_t* factory)
{
    assert(factory != nullptr);

    kslide_encoder_t* encoder = new kslide_encoder_t;
    encoder->m_impl.initialize(factory->m_impl);
    return encoder;
}

void kslide_delete_encoder(kslide_encoder_t* encoder)
{
    assert(encoder != nullptr);
    delete encoder;
}

//------------------------------------------------------------------
// DECODER FACTORY API
//------------------------------------------------------------------

kslide_decoder_factory_t* kslide_new_decoder_factory()
{
    return new kslide_decoder_factory;
}

void kslide_delete_decoder_factory(kslide_decoder_factory_t* factory)
{
    assert(factory != nullptr);
    delete factory;
}

uint32_t kslide_decoder_factory_symbol_size(kslide_decoder_factory_t* factory)
{
    assert(factory != nullptr);
    return factory->m_impl.symbol_size();
}


void kslide_decoder_factory_set_symbol_size(
    kslide_decoder_factory_t* factory, uint32_t symbol_size)
{
    assert(factory != nullptr);
    factory->m_impl.set_symbol_size(symbol_size);
}

kslide_decoder_t* kslide_decoder_factory_build(
    kslide_decoder_factory_t* factory)
{
    assert(factory != nullptr);

    kslide_decoder_t* decoder = new kslide_decoder_t;
    decoder->m_impl.initialize(factory->m_impl);
    return decoder;
}

void kslide_delete_decoder(kslide_decoder_t* decoder)
{
    assert(decoder != nullptr);
    delete decoder;
}

//------------------------------------------------------------------
// SYMBOL STORAGE API ENCODER
//------------------------------------------------------------------

// uint32_t kodo_slide_encoder_block_size(kodo_slide_encoder_t encoder)
// {
//     auto api = (kodo_slide_encoder*) encoder;
//     return api->m_encoder.block_size();
// }

// uint32_t kodo_slide_encoder_symbol_size(kodo_slide_encoder_t encoder)
// {
//     auto api = (kodo_slide_encoder*) encoder;
//     return api->m_encoder.symbol_size();
// }

// uint32_t kodo_slide_encoder_symbols(kodo_slide_encoder_t encoder)
// {
//     auto api = (kodo_slide_encoder*) encoder;
//     return api->m_encoder.symbols();
// }

// void kodo_slide_encoder_set_const_symbols(
//     kodo_slide_encoder_t encoder, uint8_t* data, uint32_t size)
// {
//     auto api = (kodo_slide_encoder*) encoder;
//     api->m_encoder.set_const_symbols(storage::storage(data, size));
// }

//------------------------------------------------------------------
// DECODER API
//------------------------------------------------------------------
uint32_t kslide_decoder_symbol_size(kslide_decoder_t* decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.symbol_size();
}

uint32_t kslide_decoder_stream_symbols(kslide_decoder_t* decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.stream_range().symbols().get();
}

uint64_t kslide_decoder_push_front_symbol(
    kslide_decoder_t* decoder, uint8_t* data)
{
    assert(decoder != nullptr);
    assert(data != nullptr);

    return decoder->m_impl.push_front_symbol(data).get();
}

uint32_t kslide_decoder_symbols_decoded(kslide_decoder_t* decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.symbols_decoded().get();
}

uint32_t kslide_decoder_stream_lower_bound(kslide_decoder_t* decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.stream_range().lower_bound().get();
}

void kslide_decoder_set_window(kslide_decoder_t* decoder,
                               uint32_t lower_bound, uint32_t symbols)
{
    assert(decoder != nullptr);
    decoder->m_impl.set_window(lower_bound, symbols);
}

void kslide_decoder_read_symbol(kslide_decoder_t* decoder, uint8_t* symbol,
                                uint8_t* coefficients)
{
    assert(decoder != nullptr);
    decoder->m_impl.read_symbol(symbol, coefficients);
}

//------------------------------------------------------------------
// ENCODER API
//------------------------------------------------------------------
uint32_t kslide_encoder_symbol_size(kslide_encoder_t* encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.symbol_size();
}

uint32_t kslide_encoder_stream_symbols(kslide_encoder_t* encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.stream_range().symbols().get();
}

uint64_t kslide_encoder_push_front_symbol(
    kslide_encoder_t* encoder, uint8_t* data)
{
    assert(encoder != nullptr);
    assert(data != nullptr);

    return encoder->m_impl.push_front_symbol(data).get();
}

uint32_t kslide_encoder_stream_lower_bound(kslide_encoder_t* encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.stream_range().lower_bound().get();
}

uint32_t kslide_encoder_coefficients_vector_size(kslide_encoder_t* encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.window_size().get();
}

void kslide_encoder_set_window(kslide_encoder_t* encoder,
                               uint32_t lower_bound, uint32_t symbols)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_window(lower_bound, symbols);
}

void kslide_encoder_set_seed(kslide_encoder_t* encoder, uint32_t seed)
{
    assert(encoder != nullptr);
    encoder->m_impl.set_seed(seed);
}

void kslide_encoder_generate(kslide_encoder_t* encoder, uint8_t* data)
{
    assert(encoder != nullptr);
    assert(data != nullptr);

    encoder->m_impl.generate(data);
}

void kslide_encoder_write_symbol(kslide_encoder_t* encoder, uint8_t* symbol,
                                 const uint8_t* coefficients)
{
    encoder->m_impl.write_symbol(symbol, coefficients);
}
