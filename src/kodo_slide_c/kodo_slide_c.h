// Copyright Steinwurf ApS 2018.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <stdint.h>

#if defined(_MSC_VER)
    #if defined(KODO_SLIDE_C_STATIC)
        // When building a static library, KODO_SLIDE_API should be blank
        #define KODO_SLIDE_API
    #elif defined(KODO_SLIDE_C_DLL_EXPORTS)
        // When building the DLL, the API symbols must be exported
        #define KODO_SLIDE_API __declspec(dllexport)
    #else
        // When a program uses the DLL, the API symbols must be imported
        #define KODO_SLIDE_API __declspec(dllimport)
    #endif
#else
    #if __GNUC__ >= 4
        // When building a shared library, only the API symbols with the 'default'
        // visibility should be exported to hide all other symbols. All source
        // files should be compiled with the '-fvisibility=hidden' and
        // '-fvisibility-inlines-hidden' flags to achieve this.
        #define KODO_SLIDE_API __attribute__ ((visibility ("default")))
    #else
        #define KODO_SLIDE_API
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// //------------------------------------------------------------------
// // kodo-slide-c TYPES
// //------------------------------------------------------------------

// /// Opaque pointer used for the encoder and decoder factories
typedef struct kslide_encoder_factory kslide_encoder_factory_t;
typedef struct kslide_decoder_factory kslide_decoder_factory_t;

// /// Opaque pointer used for decoders, encoders
typedef struct kslide_encoder kslide_encoder_t;
typedef struct kslide_decoder kslide_decoder_t;

// /// Enum specifying the available finite fields
// /// Note: the size of the enum type cannot be guaranteed, so the int32_t type
// /// is used in the API calls to pass the enum values
typedef enum
{
    kslide_binary,
    kslide_binary4,
    kslide_binary8,
    kslide_binary16
}
kslide_finite_field;

// //------------------------------------------------------------------
// // ENCODER FACTORY API
// //------------------------------------------------------------------

/// Build a new encoder factory
/// @return Pointer to the new encoder factory
KODO_SLIDE_API
kslide_encoder_factory_t* kslide_new_encoder_factory();

/// Deallocate and release the memory consumed by a factory
/// @param factory The factory which should be deallocated
KODO_SLIDE_API
void kslide_delete_encoder_factory(kslide_encoder_factory_t* factory);

/// @param factory The factory to query
/// @return the finite field  to use.
KODO_SLIDE_API
int32_t kslide_encoder_factory_field(kslide_encoder_factory_t* factory);

/// Set the finite field
/// @param factory The factory which should be configured
/// @param c_field The finite field to use.
KODO_SLIDE_API
void kslide_encoder_factory_set_field(kslide_encoder_factory_t* factory,
                                      int32_t c_field);

/// @param factory The factory to query
/// @return the symbol size in bytes
KODO_SLIDE_API
uint32_t kslide_encoder_factory_symbol_size(kslide_encoder_factory_t* factory);

/// Set the symbol size
/// @param factory The factory which should be configured
/// @param symbol_size The symbol size in bytes.
KODO_SLIDE_API
void kslide_encoder_factory_set_symbol_size(kslide_encoder_factory_t* factory,
                                            uint32_t symbol_size);

/// Build the actual encoder
/// @param factory The encoder factory which should be used to build the encoder
/// @return pointer to an instantiation of an encoder
KODO_SLIDE_API
kslide_encoder_t* kslide_encoder_factory_build(
    kslide_encoder_factory_t* factory);

/// Deallocate and release the memory consumed by an encoder
/// @param encoder The encoder which should be deallocated
KODO_SLIDE_API
void kslide_delete_encoder(kslide_encoder_t* encoder);

//------------------------------------------------------------------
// DECODER FACTORY API
//------------------------------------------------------------------

/// Build a new decoder factory
/// @return A new factory capable of building decoders using the
///         selected parameters.
KODO_SLIDE_API
kslide_decoder_factory_t* kslide_new_decoder_factory();

/// Deallocate and release the memory consumed by a factory
/// @param factory The factory which should be deallocated
KODO_SLIDE_API
void kslide_delete_decoder_factory(kslide_decoder_factory_t* factory);

/// @param factory The factory to query
/// @return the finite field  to use.
KODO_SLIDE_API
int32_t kslide_decoder_factory_field(kslide_decoder_factory_t* factory);

/// Set the finite field
/// @param factory The factory which should be configured
/// @param c_field The finite field to use.
KODO_SLIDE_API
void kslide_decoder_factory_set_field(kslide_decoder_factory_t* factory,
                                      int32_t c_field);

/// Return the symbol size in bytes
/// @param factory The factory to query
/// @return the symbol size in bytes
KODO_SLIDE_API
uint32_t kslide_decoder_factory_symbol_size(kslide_decoder_factory_t* factory);

/// Set the symbol size
/// @param factory The factory which should be configured
/// @param symbol_size The symbol size in bytes
KODO_SLIDE_API
void kslide_decoder_factory_set_symbol_size(kslide_decoder_factory_t* factory,
                                            uint32_t symbol_size);

/// Build the actual decoder
/// @param factory The decoder factory which should be used to build the decoder
/// @return pointer to an instantiation of an decoder
KODO_SLIDE_API
kslide_decoder_t* kslide_decoder_factory_build(
    kslide_decoder_factory_t* factory);

/// Deallocate and release the memory consumed by an decoder
/// @param decoder The decoder which should be deallocated
KODO_SLIDE_API
void kslide_delete_decoder(kslide_decoder_t* decoder);

//------------------------------------------------------------------
// DECODER API
//------------------------------------------------------------------

/// @param decoder The decoder to query
/// @return the symbol size in bytes
KODO_SLIDE_API
uint32_t kslide_decoder_symbol_size(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The number of symbols in the stream
KODO_SLIDE_API
uint32_t kslide_decoder_stream_symbols(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The number of symbols in the stream
KODO_SLIDE_API
uint32_t kslide_decoder_stream_lower_bound(kslide_decoder_t* decoder);

/// Adds a symbol to the front of the decoder. Increments the stream front
/// index.
/// @param decoder The decoder to push to.
/// @param data The data pointer to push.
/// @return The stream index of the symbol being added.
KODO_SLIDE_API
uint64_t kslide_decoder_push_front_symbol(
    kslide_decoder_t* decoder, uint8_t* data);

/// @param decoder The decoder to query
/// @return The number of symbols decoded
KODO_SLIDE_API
uint32_t kslide_decoder_symbols_decoded(kslide_decoder_t* decoder);


/// @param decoder The decoder to query
/// @param lower_bound The index of the "oldest" symbol in the window
/// @param symbols The number of symbols in the window
KODO_SLIDE_API
void kslide_decoder_set_window(kslide_decoder_t* decoder,
                               uint32_t lower_bound, uint32_t symbols);

KODO_SLIDE_API
void kslide_decoder_read_symbol(kslide_decoder_t* decoder, uint8_t* symbol,
                                uint8_t* coefficients);

//------------------------------------------------------------------
// ENCODER API
//------------------------------------------------------------------

/// @param encoder The encoder to query
/// @return the symbol size in bytes
KODO_SLIDE_API
uint32_t kslide_encoder_symbol_size(kslide_encoder_t* encoder);

/// @param decoder The encoder to query
/// @return The number of symbols in the stream
KODO_SLIDE_API
uint32_t kslide_encoder_stream_symbols(kslide_encoder_t* encoder);

/// @param encoder The encoder to query
/// @return The number of symbols in the stream
KODO_SLIDE_API
uint32_t kslide_encoder_stream_lower_bound(kslide_encoder_t* encoder);

/// @param encoder The encoder to query
/// @return The size of a coefficient vector in bytes
KODO_SLIDE_API
uint32_t kslide_encoder_coefficient_vector_size(kslide_encoder_t* encoder);

/// Adds a symbol to the front of the encoder. Increments the stream front
/// index.
/// @param encoder The encoder to push to.
/// @param data The data pointer to push.
/// @return The stream index of the symbol being added.
KODO_SLIDE_API
uint64_t kslide_encoder_push_front_symbol(
    kslide_encoder_t* encoder, uint8_t* data);

KODO_SLIDE_API
void kslide_encoder_set_seed(kslide_encoder_t* encoder, uint32_t seed);

KODO_SLIDE_API
void kslide_encoder_generate(kslide_encoder_t* encoder, uint8_t* data);

KODO_SLIDE_API
void kslide_encoder_write_symbol(kslide_encoder_t* encoder, uint8_t* symbol,
                                 const uint8_t* coefficients);

/// @param encoder The encoder to query
/// @param lower_bound The index of the "oldest" symbol in the window
/// @param symbols The number of symbols in the window
KODO_SLIDE_API
void kslide_encoder_set_window(kslide_encoder_t* encoder,
                               uint32_t lower_bound, uint32_t symbols);

#ifdef __cplusplus
}
#endif

// Clean up define
#undef KODO_SLIDE_API
