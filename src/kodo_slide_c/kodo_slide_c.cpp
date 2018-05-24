// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodo_slide_c.h"

#include <cstring>
#include <cstdint>
#include <cassert>
#include <string>

#include <kodo_slide/encoder.hpp>
#include <kodo_slide/decoder.hpp>

struct kslide_decoder
{
    kslide_decoder(kodo_slide::decoder decoder) :
        m_impl(decoder)
    { }
    kodo_slide::decoder m_impl;
};

struct kslide_decoder_factory
{
    kodo_slide::decoder::factory m_impl;
};

struct kslide_encoder
{
    kslide_encoder(kodo_slide::encoder encoder) :
        m_impl(encoder)
    { }
    kodo_slide::encoder m_impl;
};

struct kslide_encoder_factory
{
    kodo_slide::encoder::factory m_impl;
};

int32_t kslide_field_to_c_field(kodo_slide::finite_field field_id)
{
    switch (field_id)
    {
    case kodo_slide::finite_field::binary:
        return kslide_binary;
    case kodo_slide::finite_field::binary4:
        return kslide_binary4;
    case kodo_slide::finite_field::binary8:
        return kslide_binary8;
    case kodo_slide::finite_field::binary16:
        return kslide_binary16;
    default:
        assert(false && "Unknown field");
        return kslide_binary;
    }
}

kodo_slide::finite_field c_field_to_kslide_field(int32_t c_field)
{
    switch (c_field)
    {
    case kslide_binary:
        return kodo_slide::finite_field::binary;
    case kslide_binary4:
        return kodo_slide::finite_field::binary4;
    case kslide_binary8:
        return kodo_slide::finite_field::binary8;
    case kslide_binary16:
        return kodo_slide::finite_field::binary16;
    default:
        assert(false && "Unknown field");
        return kodo_slide::finite_field::binary;
    }
}

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

int32_t kslide_encoder_factory_field(kslide_encoder_factory_t* factory)
{
    assert(factory != nullptr);
    return kslide_field_to_c_field(factory->m_impl.field());
}

void kslide_encoder_factory_set_field(
    kslide_encoder_factory_t* factory, int32_t c_field)
{
    assert(factory != nullptr);
    factory->m_impl.set_field(c_field_to_kslide_field(c_field));
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
    return new kslide_encoder_t(factory->m_impl.build());
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

int32_t kslide_decoder_factory_field(kslide_decoder_factory_t* factory)
{
    assert(factory != nullptr);
    return kslide_field_to_c_field(factory->m_impl.field());
}

void kslide_decoder_factory_set_field(
    kslide_decoder_factory_t* factory, int32_t c_field)
{
    assert(factory != nullptr);
    factory->m_impl.set_field(c_field_to_kslide_field(c_field));
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

    assert(factory != nullptr);
    return new kslide_decoder_t(factory->m_impl.build());
}

void kslide_delete_decoder(kslide_decoder_t* decoder)
{
    assert(decoder != nullptr);
    delete decoder;
}

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
    return decoder->m_impl.stream_symbols();
}

uint64_t kslide_decoder_push_front_symbol(
    kslide_decoder_t* decoder, uint8_t* data)
{
    assert(decoder != nullptr);
    assert(data != nullptr);

    return decoder->m_impl.push_front_symbol(data);
}

uint32_t kslide_decoder_symbols_decoded(kslide_decoder_t* decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.symbols_decoded();
}

uint32_t kslide_decoder_stream_lower_bound(kslide_decoder_t* decoder)
{
    assert(decoder != nullptr);
    return decoder->m_impl.stream_lower_bound();
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
    return encoder->m_impl.stream_symbols();
}

uint64_t kslide_encoder_push_front_symbol(
    kslide_encoder_t* encoder, uint8_t* data)
{
    assert(encoder != nullptr);
    assert(data != nullptr);

    return encoder->m_impl.push_front_symbol(data);
}

uint32_t kslide_encoder_stream_lower_bound(kslide_encoder_t* encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.stream_lower_bound();
}

uint32_t kslide_encoder_coefficient_vector_size(kslide_encoder_t* encoder)
{
    assert(encoder != nullptr);
    return encoder->m_impl.coefficient_vector_size();
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
