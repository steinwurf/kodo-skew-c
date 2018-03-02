// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "kodo_slide_c.h"

// #include <cstring>
// #include <cstdint>
// #include <cassert>
// #include <string>

// #include <storage/storage.hpp>
// #include <kodo_slide/on_the_fly_codes.hpp>
// #include "on_the_fly_encoder_stack.hpp"
// #include "on_the_fly_decoder_stack.hpp"

// struct kodo_slide_decoder
// {
//     kodo_slide_c::on_the_fly_decoder_stack m_decoder;
// };

// struct kodo_slide_decoder_factory
// {
//     template<class... Args>
//     kodo_slide_decoder_factory(Args&&... args) :
//         m_decoder_factory(std::forward<Args>(args)...)
//     { }
//     kodo_slide_c::on_the_fly_decoder_stack::config m_decoder_factory;
// };

// struct kodo_slide_encoder
// {
//     kodo_slide_c::on_the_fly_encoder_stack m_encoder;
// };

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

// //------------------------------------------------------------------
// // ENCODER FACTORY API
// //------------------------------------------------------------------

// kodo_slide_encoder_factory_t kodo_slide_encoder_factory_construct(
//     int32_t finite_field_id, uint32_t symbols, uint32_t symbol_size)
// {
//     auto finite_field = build_finite_field(finite_field_id);
//     return new kodo_slide_encoder_factory(finite_field, symbols, symbol_size);
// }

// void kodo_slide_encoder_factory_destruct(kodo_slide_encoder_factory_t factory)
// {
//     auto api = (kodo_slide_encoder_factory*) factory;
//     delete api;
// }

// uint32_t kodo_slide_encoder_factory_symbols(kodo_slide_encoder_factory_t factory)
// {
//     auto api = (kodo_slide_encoder_factory*) factory;
//     return api->m_encoder_factory.symbols();
// }

// uint32_t kodo_slide_encoder_factory_symbol_size(kodo_slide_encoder_factory_t factory)
// {
//     auto api = (kodo_slide_encoder_factory*) factory;
//     return api->m_encoder_factory.symbol_size();
// }

// void kodo_slide_encoder_factory_set_symbols(kodo_slide_encoder_factory_t factory, uint32_t symbols)
// {
//     auto api = (kodo_slide_encoder_factory*) factory;
//     api->m_encoder_factory.set_symbols(symbols);
// }

// void kodo_slide_encoder_factory_set_symbol_size(
//     kodo_slide_encoder_factory_t factory, uint32_t symbol_size)
// {
//     auto api = (kodo_slide_encoder_factory*) factory;
//     api->m_encoder_factory.set_symbol_size(symbol_size);
// }

// kodo_slide_encoder_t kodo_slide_encoder_factory_build(kodo_slide_encoder_factory_t factory)
// {
//     auto api = (kodo_slide_encoder_factory*) factory;
//     auto encoder = new kodo_slide_encoder();
//     encoder->m_encoder.initialize(api->m_encoder_factory);
//     return encoder;
// }

// void kodo_slide_encoder_destruct(kodo_slide_encoder_t encoder)
// {
//     auto api = (kodo_slide_encoder*) encoder;
//     delete api;
// }

// //------------------------------------------------------------------
// // DECODER FACTORY API
// //------------------------------------------------------------------

// kodo_slide_decoder_factory_t kodo_slide_decoder_factory_construct(
//     int32_t finite_field_id, uint32_t symbols, uint32_t symbol_size)
// {
//     auto finite_field = build_finite_field(finite_field_id);
//     return new kodo_slide_decoder_factory(finite_field, symbols, symbol_size);
// }

// void kodo_slide_decoder_factory_destruct(kodo_slide_decoder_factory_t factory)
// {
//     auto api = (kodo_slide_decoder_factory*) factory;
//     delete api;
// }

// uint32_t kodo_slide_decoder_factory_symbols(kodo_slide_decoder_factory_t factory)
// {
//     auto api = (kodo_slide_decoder_factory*) factory;
//     return api->m_decoder_factory.symbols();
// }

// uint32_t kodo_slide_decoder_factory_symbol_size(kodo_slide_decoder_factory_t factory)
// {
//     auto api = (kodo_slide_decoder_factory*) factory;
//     return api->m_decoder_factory.symbol_size();
// }

// void kodo_slide_decoder_factory_set_symbols(kodo_slide_decoder_factory_t factory, uint32_t symbols)
// {
//     auto api = (kodo_slide_decoder_factory*) factory;
//     api->m_decoder_factory.set_symbols(symbols);
// }

// void kodo_slide_decoder_factory_set_symbol_size(
//     kodo_slide_decoder_factory_t factory, uint32_t symbol_size)
// {
//     auto api = (kodo_slide_decoder_factory*) factory;
//     api->m_decoder_factory.set_symbol_size(symbol_size);
// }

// kodo_slide_decoder_t kodo_slide_decoder_factory_build(kodo_slide_decoder_factory_t factory)
// {
//     auto api = (kodo_slide_decoder_factory*) factory;
//     auto decoder = new kodo_slide_decoder();
//     decoder->m_decoder.initialize(api->m_decoder_factory);
//     return decoder;
// }

// void kodo_slide_decoder_destruct(kodo_slide_decoder_t decoder)
// {
//     auto api = (kodo_slide_decoder*) decoder;
//     delete api;
// }

// //------------------------------------------------------------------
// // PAYLOAD API DECODER
// //------------------------------------------------------------------

// uint32_t kodo_slide_decoder_payload_size(kodo_slide_decoder_t decoder)
// {
//     auto api = (kodo_slide_decoder*) decoder;
//     return api->m_decoder.payload_size();
// }

// void kodo_slide_decoder_read_payload(kodo_slide_decoder_t decoder, uint8_t* payload)
// {
//     auto api = (kodo_slide_decoder*) decoder;
//     api->m_decoder.read_payload(payload);
// }

// //------------------------------------------------------------------
// // PAYLOAD API ENCODER
// //------------------------------------------------------------------

// uint32_t kodo_slide_encoder_payload_size(kodo_slide_encoder_t encoder)
// {
//     auto api = (kodo_slide_encoder*) encoder;
//     return api->m_encoder.payload_size();
// }

// uint32_t kodo_slide_encoder_write_payload(kodo_slide_encoder_t encoder, uint8_t* payload)
// {
//     auto api = (kodo_slide_encoder*) encoder;
//     return api->m_encoder.write_payload(payload);
// }

// //------------------------------------------------------------------
// // SYMBOL STORAGE API DECODER
// //------------------------------------------------------------------

// uint32_t kodo_slide_decoder_block_size(kodo_slide_decoder_t decoder)
// {
//     auto api = (kodo_slide_decoder*) decoder;
//     return api->m_decoder.block_size();
// }

// uint32_t kodo_slide_decoder_symbol_size(kodo_slide_decoder_t decoder)
// {
//     auto api = (kodo_slide_decoder*) decoder;
//     return api->m_decoder.symbol_size();
// }

// uint32_t kodo_slide_decoder_symbols(kodo_slide_decoder_t decoder)
// {
//     auto api = (kodo_slide_decoder*) decoder;
//     return api->m_decoder.symbols();
// }

// void kodo_slide_decoder_set_mutable_symbols(
//     kodo_slide_decoder_t decoder, uint8_t* data, uint32_t size)
// {
//     auto api = (kodo_slide_decoder*) decoder;
//     api->m_decoder.set_mutable_symbols(storage::storage(data, size));
// }

// //------------------------------------------------------------------
// // SYMBOL STORAGE API ENCODER
// //------------------------------------------------------------------

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

// //------------------------------------------------------------------
// // DECODER API
// //------------------------------------------------------------------

// uint8_t kodo_slide_decoder_is_complete(kodo_slide_decoder_t decoder)
// {
//     auto api = (kodo_slide_decoder*) decoder;
//     return api->m_decoder.is_complete();
// }

// uint32_t kodo_slide_decoder_rank(kodo_slide_decoder_t decoder)
// {
//     auto api = (kodo_slide_decoder*) decoder;
//     return api->m_decoder.rank();
// }

// //------------------------------------------------------------------
// // ENCODER API
// //------------------------------------------------------------------

// uint8_t kodo_slide_is_systematic_on(kodo_slide_encoder_t encoder)
// {
//     auto api = (kodo_slide_encoder*) encoder;
//     return api->m_encoder.is_systematic_on();
// }

// void kodo_slide_set_systematic_on(kodo_slide_encoder_t encoder)
// {
//     auto api = (kodo_slide_encoder*) encoder;
//     api->m_encoder.set_systematic_on();
// }

// void kodo_slide_encoder_set_systematic_off(kodo_slide_encoder_t encoder)
// {
//     auto api = (kodo_slide_encoder*) encoder;
//     api->m_encoder.set_systematic_off();
// }
