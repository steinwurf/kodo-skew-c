// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <stdint.h>

// #if defined(_MSC_VER)
//     #if defined(KODO_SLIDE_STATIC)
//         // When building a static library, KODO_SLIDE_API should be blank
//         #define KODO_SLIDE_API
//     #elif defined(KODO_SLIDE_DLL_EXPORTS)
//         // When building the DLL, the API symbols must be exported
//         #define KODO_SLIDE_API __declspec(dllexport)
//     #else
//         // When a program uses the DLL, the API symbols must be imported
//         #define KODO_SLIDE_API __declspec(dllimport)
//     #endif
// #else
//     #if __GNUC__ >= 4
//         // When building a shared library, only the API symbols with the 'default'
//         // visibility should be exported to hide all other symbols. All source
//         // files should be compiled with the '-fvisibility=hidden' and
//         // '-fvisibility-inlines-hidden' flags to achieve this.
//         #define KODO_SLIDE_API __attribute__ ((visibility ("default")))
//     #else
//         #define KODO_SLIDE_API
//     #endif
// #endif

// #ifdef __cplusplus
// extern "C" {
// #endif

// //------------------------------------------------------------------
// // kodo-slide-c TYPES
// //------------------------------------------------------------------

// /// Opaque pointer used for the encoder and decoder factories
// typedef struct kodo_slide_encoder_factory* kodo_slide_encoder_factory_t;
typedef struct kodo_slide_decoder_factory* kodo_slide_decoder_factory_t;

// /// Opaque pointer used for decoders, encoders
// typedef struct kodo_slide_encoder* kodo_slide_encoder_t;
typedef struct kodo_slide_decoder* kodo_slide_decoder_t;

// /// Enum specifying the available finite fields
// /// Note: the size of the enum type cannot be guaranteed, so the int32_t type
// /// is used in the API calls to pass the enum values
// typedef enum
// {
//     kodo_slide_binary,
//     kodo_slide_binary4,
//     kodo_slide_binary8
// }
// kodo_slide_finite_field;

// //------------------------------------------------------------------
// // ENCODER FACTORY API
// //------------------------------------------------------------------

// /// Build a new encoder factory
// /// @param finite_field The finite field that should be used by the encoder.
// /// @param symbols The maximum number of symbols supported by encoders
// ///        built with this factory.
// /// @param symbol_size The maximum symbol size in bytes supported by
// ///        encoders built using the returned factory
// /// @return A new factory capable of building encoders using the
// ///         selected parameters.
// KODO_SLIDE_API
// kodo_slide_encoder_factory_t kodo_slide_encoder_factory_construct(
//     int32_t finite_field_id, uint32_t symbols, uint32_t symbol_size);

// /// Deallocate and release the memory consumed by a factory
// /// @param factory The factory which should be deallocated
// KODO_SLIDE_API
// void kodo_slide_encoder_factory_destruct(kodo_slide_encoder_factory_t factory);

// /// Return the number of symbols in a block
// /// @param factory The factory to query
// /// @return the number of symbols in a block
// KODO_SLIDE_API
// uint32_t kodo_slide_encoder_factory_symbols(kodo_slide_encoder_factory_t factory);

// /// Return the symbol size in bytes
// /// @param factory The factory to query
// /// @return the symbol size in bytes
// KODO_SLIDE_API
// uint32_t kodo_slide_encoder_factory_symbol_size(kodo_slide_encoder_factory_t factory);

// /// Set the number of symbols
// /// @param factory The factory which should be configured
// /// @param symbols the number of symbols
// KODO_SLIDE_API
// void kodo_slide_encoder_factory_set_symbols(kodo_slide_encoder_factory_t factory, uint32_t symbols);

// /// Set the symbol size
// /// @param factory The factory which should be configured
// /// @param the symbol size in bytes
// KODO_SLIDE_API
// void kodo_slide_encoder_factory_set_symbol_size(kodo_slide_encoder_factory_t factory, uint32_t symbol_size);

// /// Build the actual encoder
// /// @param factory The encoder factory which should be used to build the encoder
// /// @return pointer to an instantiation of an encoder
// KODO_SLIDE_API
// kodo_slide_encoder_t kodo_slide_encoder_factory_build(kodo_slide_encoder_factory_t factory);

// /// Deallocate and release the memory consumed by an encoder
// /// @param encoder The encoder which should be deallocated
// KODO_SLIDE_API
// void kodo_slide_encoder_destruct(kodo_slide_encoder_t encoder);

// //------------------------------------------------------------------
// // DECODER FACTORY API
// //------------------------------------------------------------------

// /// Build a new decoder factory
// /// @param finite_field The finite field that should be used by the decoder.
// /// @param symbols The maximum number of symbols supported by decoders
// ///        built with this factory.
// /// @param symbol_size The maximum symbol size in bytes supported by
// ///        decoders built using the returned factory
// /// @return A new factory capable of building decoders using the
// ///         selected parameters.
// KODO_SLIDE_API
// kodo_slide_decoder_factory_t kodo_slide_decoder_factory_construct(int32_t finite_field_id, uint32_t symbols, uint32_t symbol_size);

// /// Deallocate and release the memory consumed by a factory
// /// @param factory The factory which should be deallocated
// KODO_SLIDE_API
// void kodo_slide_decoder_factory_destruct(kodo_slide_decoder_factory_t factory);

// /// Return the number of symbols in a block
// /// @param factory The factory to query
// /// @return the number of symbols in a block
// KODO_SLIDE_API
// uint32_t kodo_slide_decoder_factory_symbols(kodo_slide_decoder_factory_t factory);

// /// Return the symbol size in bytes
// /// @param factory The factory to query
// /// @return the symbol size in bytes
// KODO_SLIDE_API
// uint32_t kodo_slide_decoder_factory_symbol_size(kodo_slide_decoder_factory_t factory);

// /// Set the number of symbols
// /// @param factory The factory which should be configured
// /// @param symbols the number of symbols
// KODO_SLIDE_API
// void kodo_slide_decoder_factory_set_symbols(kodo_slide_decoder_factory_t factory, uint32_t symbols);

// /// Set the symbol size
// /// @param factory The factory which should be configured
// /// @param the symbol size in bytes
// KODO_SLIDE_API
// void kodo_slide_decoder_factory_set_symbol_size(kodo_slide_decoder_factory_t factory, uint32_t symbol_size);

// /// Build the actual decoder
// /// @param factory The decoder factory which should be used to build the decoder
// /// @return pointer to an instantiation of an decoder
// KODO_SLIDE_API
// kodo_slide_decoder_t kodo_slide_decoder_factory_build(kodo_slide_decoder_factory_t factory);

// /// Deallocate and release the memory consumed by an decoder
// /// @param decoder The decoder which should be deallocated
// KODO_SLIDE_API
// void kodo_slide_decoder_destruct(kodo_slide_decoder_t decoder);

// //------------------------------------------------------------------
// // PAYLOAD API DECODER
// //------------------------------------------------------------------

// /// Return the payload size of an decoder, which is the expected size of a given
// /// payload.
// /// @param decoder The decoder to query.
// /// @return The payload size in bytes
// KODO_SLIDE_API
// uint32_t kodo_slide_decoder_payload_size(kodo_slide_decoder_t decoder);

// /// Read symbol from the given payload buffer.
// /// @param decoder The decoder to use.
// /// @param payload The buffer storing the payload of an encoded symbol.
// ///        The payload buffer may be changed by this operation,
// ///        so it cannot be reused. If the payload is needed at several places,
// ///        make sure to keep a copy of the original payload.
// KODO_SLIDE_API
// void kodo_slide_decoder_read_payload(kodo_slide_decoder_t decoder, uint8_t* payload);

// //------------------------------------------------------------------
// // PAYLOAD API ENCODER
// //------------------------------------------------------------------

// /// Return the payload size of an encoder, which is the size of a generated
// /// payload.
// /// @param encoder The encoder to query.
// /// @return The payload size in bytes
// KODO_SLIDE_API
// uint32_t kodo_slide_encoder_payload_size(kodo_slide_encoder_t encoder);

// /// Write a symbol into the provided payload buffer.
// /// @param encoder The encoder to use.
// /// @param payload The buffer which should contain the symbol.
// /// @return The total bytes used from the payload buffer
// KODO_SLIDE_API
// uint32_t kodo_slide_encoder_write_payload(kodo_slide_encoder_t encoder, uint8_t* payload);

// //------------------------------------------------------------------
// // SYMBOL STORAGE API DECODER
// //------------------------------------------------------------------

// /// Return the block size of an decoder.
// /// @param decoder The decoder to query.
// /// @return The block size, i.e. the total size in bytes that this decoder
// ///         operates on.
// KODO_SLIDE_API
// uint32_t kodo_slide_decoder_block_size(kodo_slide_decoder_t decoder);

// /// Return the symbol size of the decoder.
// /// @param decoder The decoder to check
// /// @return The size of a symbol in bytes
// KODO_SLIDE_API
// uint32_t kodo_slide_decoder_symbol_size(kodo_slide_decoder_t decoder);

// /// Return the number of symbols in a block (i.e. the generation size).
// /// @param decoder The decoder to check
// /// @return The number of symbols
// KODO_SLIDE_API
// uint32_t kodo_slide_decoder_symbols(kodo_slide_decoder_t decoder);

// /// Specify the data buffer where the decoder should store the decoded
// /// symbols.
// /// This will specify the storage for all symbols.
// /// @param decoder The decoder which will decode the data
// /// @param data The buffer that should contain the decoded symbols
// /// @param size The size of the buffer to be decoded
// KODO_SLIDE_API
// void kodo_slide_decoder_set_mutable_symbols(
//     kodo_slide_decoder_t decoder, uint8_t* data, uint32_t size);

// //------------------------------------------------------------------
// // SYMBOL STORAGE API ENCODER
// //------------------------------------------------------------------

// /// Return the block size of an encoder.
// /// @param encoder The encoder to query.
// /// @return The block size, i.e. the total size in bytes that this encoder
// ///         operates on.
// KODO_SLIDE_API
// uint32_t kodo_slide_encoder_block_size(kodo_slide_encoder_t encoder);

// /// Return the symbol size of the encoder.
// /// @param encoder The encoder to check
// /// @return The size of a symbol in bytes
// KODO_SLIDE_API
// uint32_t kodo_slide_encoder_symbol_size(kodo_slide_encoder_t encoder);

// /// Return the number of symbols in a block (i.e. the generation size).
// /// @param encoder The encoder to check
// /// @return The number of symbols
// KODO_SLIDE_API
// uint32_t kodo_slide_encoder_symbols(kodo_slide_encoder_t encoder);

// /// Specify the source data for all symbols. This will specify all
// /// symbols.
// /// @param encoder The encoder which will encode the data
// /// @param data The buffer containing the data to be encoded
// /// @param size The size of the buffer to be encoded
// KODO_SLIDE_API
// void kodo_slide_encoder_set_const_symbols(
//     kodo_slide_encoder_t encoder, uint8_t* data, uint32_t size);

// //------------------------------------------------------------------
// // DECODER API
// //------------------------------------------------------------------

// /// Check whether decoding is complete.
// /// @param decoder The decoder to query
// /// @return Non-zero value if the decoding is complete, otherwise 0
// KODO_SLIDE_API
// uint8_t kodo_slide_decoder_is_complete(kodo_slide_decoder_t decoder);

// /// Return the rank of a decoder indicates how many symbols have been decoded
// /// or partially decoded.
// /// @param decoder The decoder to query
// /// @return The rank of the decoder
// KODO_SLIDE_API
// uint32_t kodo_slide_decoder_rank(kodo_slide_decoder_t decoder);

// //------------------------------------------------------------------
// // ENCODER API
// //------------------------------------------------------------------

// /// Return whether the encoder is in the systematic mode, i.e. if it will
// /// initially send the original source symbols with a simple header.
// /// @param encoder The encoder
// /// @return Non-zero if the encoder is in the systematic mode, otherwise 0
// KODO_SLIDE_API
// uint8_t kodo_slide_is_systematic_on(kodo_slide_encoder_t encoder);

// /// Switch the systematic encoding on
// /// @param encoder The encoder
// KODO_SLIDE_API
// void kodo_slide_set_systematic_on(kodo_slide_encoder_t encoder);

// /// Switch the systematic encoding off
// /// @param encoder The encoder
// KODO_SLIDE_API
// void kodo_slide_encoder_set_systematic_off(kodo_slide_encoder_t encoder);

// #ifdef __cplusplus
// }
// #endif
