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
/// @return The current specified symbol size in bytes.
KODO_SLIDE_API
uint32_t kslide_encoder_factory_symbol_size(kslide_encoder_factory_t* factory);

/// @param factory The factory to configure
/// @param symbol_size The symbol size in bytes.
KODO_SLIDE_API
void kslide_encoder_factory_set_symbol_size(kslide_encoder_factory_t* factory,
                                            uint32_t symbol_size);

/// @param factory The factory to query
/// @return The finite field used.
KODO_SLIDE_API
int32_t kslide_encoder_factory_field(kslide_encoder_factory_t* factory);

/// @param factory The factory to configure
/// @param c_field The finite field to use.
KODO_SLIDE_API
void kslide_encoder_factory_set_field(kslide_encoder_factory_t* factory,
                                      int32_t c_field);

/// @param factory The factory to use
/// @return A new decoder.
KODO_SLIDE_API
kslide_encoder_t* kslide_encoder_factory_build(
    kslide_encoder_factory_t* factory);

/// @param factory The factory to initialize the encoder
/// @param encoder Initialize a encoder with the factory settings. After
///        calling initialize the encoder will be ready for use.
KODO_SLIDE_API
void kslide_encoder_factory_initialize(
    kslide_encoder_factory_t* factory, kslide_encoder_t* encoder);

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
/// @return The current specified symbol size in bytes.
KODO_SLIDE_API
uint32_t kslide_decoder_factory_symbol_size(kslide_decoder_factory_t* factory);

/// @param factory The factory to configure
/// @param symbol_size Sets the size of a symbol in bytes
KODO_SLIDE_API
void kslide_decoder_factory_set_symbol_size(kslide_decoder_factory_t* factory,
                                            uint32_t symbol_size);

/// @param factory The factory to query
/// @return the finite field to use.
KODO_SLIDE_API
int32_t kslide_decoder_factory_field(kslide_decoder_factory_t* factory);

/// @param factory The factory to configure
/// @param c_field The finite field to use.
KODO_SLIDE_API
void kslide_decoder_factory_set_field(kslide_decoder_factory_t* factory,
                                      int32_t c_field);

/// @param factory The factory to use
/// @return A new decoder.
KODO_SLIDE_API
kslide_decoder_t* kslide_decoder_factory_build(
    kslide_decoder_factory_t* factory);

/// @param factory The factory to initialize the decoder
/// @param decoder Initialize a decoder with the factory settings. After
///        calling initialize the decoder will be ready for use.
KODO_SLIDE_API
void kslide_decoder_factory_initialize(
    kslide_decoder_factory_t* factory, kslide_decoder_t* decoder);

/// Deallocate and release the memory consumed by an decoder
/// @param decoder The decoder which should be deallocated
KODO_SLIDE_API
void kslide_delete_decoder(kslide_decoder_t* decoder);

//------------------------------------------------------------------
// ENCODER API
//------------------------------------------------------------------

/// @param encoder The encoder to query
/// @return The size of a symbol in the stream in bytes.
KODO_SLIDE_API
uint32_t kslide_encoder_symbol_size(kslide_encoder_t* encoder);

/// @param decoder The encoder to query
/// @return The total number of symbols available in memory at the encoder.
///         The number of symbols in the coding window MUST be less than
///         or equal to this number. The total range of valid symbol
///         indices is:
///
///             for (uint64_t i = 0; i < kslide_encoder_stream_symbols(); ++i)
///             {
///               std::cout << i + kslide_encoder_stream_lower_bound() << "\n";
///             }
///
KODO_SLIDE_API
uint32_t kslide_encoder_stream_symbols(kslide_encoder_t* encoder);

/// @param encoder The encoder to query
/// @return The index of the oldest symbol known by the encoder. This symbol
///         may not be inside the window but can be included in the window
///         if needed.
KODO_SLIDE_API
uint32_t kslide_encoder_stream_lower_bound(kslide_encoder_t* encoder);

/// @param encoder The encoder to query
/// @return The upper bound of the stream. The range of valid symbol indices
///         goes from [encoder::stream_lower_bound(),
///         encoder::stream_upper_bound()). Note the stream is a half-open
///         interval. Going from encoder::stream_lower_bound() to
///         encoder::stream_upper_bound() - 1.
KODO_SLIDE_API
uint32_t kslide_encoder_stream_upper_bound(kslide_encoder_t* encoder);

/// Adds a new symbol to the front of the encoder. Increments the number of
/// symbols in the stream and increases the upper bound of the stream.
///
/// @param encoder The encoder to push to.
/// @param symbol Pointer to the symbol. Note, the caller must ensure that
///        the memory of the symbol remains valid as long as the symbol is
///        included in the stream. The caller is responsible for freeing the
///        memory if needed. Once the symbol is popped from the stream.
/// @return The stream index of the symbol being added.
KODO_SLIDE_API
uint64_t kslide_encoder_push_front_symbol(
    kslide_encoder_t* encoder, uint8_t* data);

/// Remove the "oldest" symbol from the stream. Increments the
/// lower bound of the stream.
/// @param encoder The encoder to pop from.
/// @return The index of the symbol being removed
KODO_SLIDE_API
uint64_t kslide_encoder_pop_back_symbol(kslide_encoder_t* encoder);

/// @param encoder The encoder to query.
/// @return The number of symbols currently in the coding window. The
///         window must be within the bounds of the stream.
KODO_SLIDE_API
uint64_t kslide_encoder_window_symbols(kslide_encoder_t* encoder);

/// @param encoder The encoder to query.
/// @return The index of the "oldest" symbol in the coding window.
KODO_SLIDE_API
uint64_t kslide_encoder_window_lower_bound(kslide_encoder_t* encoder);

/// @param encoder The encoder to query.
/// @return The upper bound of the window. The range of valid symbol indices
///         goes from [encoder::window_lower_bound(),
///         encoder::window_upper_bound()). Note the window is a half-open
///         interval. Going from encoder::window_lower_bound() to
///         encoder::window_upper_bound() - 1.
KODO_SLIDE_API
uint64_t kslide_encoder_window_upper_bound(kslide_encoder_t* encoder);

/// The window represents the symbols which will be included in the next
/// encoding. The window cannot exceed the bounds of the stream.
///
/// Example: If window_lower_bound=4 and window_symbol=3 the following
///          symbol indices will be included 4,5,6
///
/// @param encoder The encoder to configure
/// @param window_lower_bound Sets the index of the oldest symbol in the
///        window.
/// @param window_symbols Sets number of symbols within the window.
KODO_SLIDE_API
void kslide_encoder_set_window(kslide_encoder_t* encoder,
                               uint32_t lower_bound, uint32_t symbols);

/// @param encoder The encoder to query
/// @return The size of the coefficient vector in the current window in
///         bytes. The number of coefficients is equal to the number of
///         symbols in the window. The size in bits of each coefficients
///         depends on the finite field chosen. A custom coding scheme can
///         be implemented by generating the coding vector manually.
///         Alternatively the built-in generator can be used. See
///         kslide_encoder_set_seed(...) and kslide_encoder_generate(...).
KODO_SLIDE_API
uint32_t kslide_encoder_coefficient_vector_size(kslide_encoder_t* encoder);

/// Seed the internal random generator function. If using the same seed
/// on the encoder and decoder the exact same set of coefficients will
/// be generated.
/// @param encoder The encoder to configure
/// @param seed_value A value for the seed.
KODO_SLIDE_API
void kslide_encoder_set_seed(kslide_encoder_t* encoder, uint32_t seed_value);

/// Generate coding coefficients for the symbols in the coding window
/// according to the specified seed (see kslide_encoder_set_seed(...)).
/// @param encoder The encoder to use
/// @param coefficients Buffer where the coding coefficients should be stored.
///        This buffer must be the size of the coefficient vector in bytes.
KODO_SLIDE_API
void kslide_encoder_generate(kslide_encoder_t* encoder, uint8_t* coefficients);

/// Write an encoded symbol according to the coding coefficients.
/// @param encoder The encoder to use
/// @param symbol The buffer where the encoded symbol will be stored.
///        The symbol buffer must be kslide_encoder_symbol_size() large.
/// @param coefficients The coding coefficients. These must have the
///        memory layout required. A compatible format can be created using
///        kslide_encoder_generate(...)
KODO_SLIDE_API
void kslide_encoder_write_symbol(kslide_encoder_t* encoder, uint8_t* symbol,
                                 const uint8_t* coefficients);

/// Write a source symbol to the symbol buffer.
/// @param encoder The encoder to use
/// @param symbol The buffer where the source symbol will be stored. The
///        symbol buffer must be symbol size large.
/// @param index The symbol index which should be written.
KODO_SLIDE_API
void kslide_encoder_write_source_symbol(kslide_encoder_t* encoder,
                                        uint8_t* symbol, uint64_t index);

//------------------------------------------------------------------
// DECODER API
//------------------------------------------------------------------

/// @param decoder The decoder to query
/// @return The size of a symbol in the stream in bytes.
KODO_SLIDE_API
uint64_t kslide_decoder_symbol_size(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The total number of symbols known at the decoder. The number of
///         symbols in the decoding window MUST be less than or equal to
///         this number. The total range of valid symbol indicies is
///
///             for (uint64_t i = 0; i < kslide_decoder_stream_symbols(); ++i)
///             {
///               std::cout << i + kslide_decoder_stream_lower_bound() << "\n";
///             }
///
KODO_SLIDE_API
uint64_t kslide_decoder_stream_symbols(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The index of the oldest symbol known by the decoder. This symbol
///         may not be inside the window but can be included in the window
///         if needed.
KODO_SLIDE_API
uint64_t kslide_decoder_stream_lower_bound(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The upper bound of the stream. The range of valid symbol indices
///         goes from [decoder::stream_lower_bound(),
///         decoder::stream_upper_bound()). Note the stream is a half-open
///         interval. Going from decoder::stream_lower_bound() to
///         decoder::stream_upper_bound() - 1.
KODO_SLIDE_API
uint64_t kslide_decoder_stream_upper_bound(kslide_decoder_t* decoder);

/// @param decoder The decoder to use
/// Adds a new symbol to the front of the decoder. Increments the number of
/// symbols in the stream and increases the decoder::stream_upper_bound().
///
/// @param symbol Pointer to the symbol. Note, the caller must ensure that
///        the memory of the symbol remains valid as long as the symbol is
///        included in the stream. The caller is responsible for freeing the
///        memory if needed. Once the symbol is popped from the stream.
/// @return The stream index of the symbol being added.
KODO_SLIDE_API
uint64_t kslide_decoder_push_front_symbol(kslide_decoder_t* decoder, uint8_t* symbol);

/// @param decoder The decoder to use
/// Remove the "oldest" symbol from the stream. Increments the
/// decoder::stream_lower_bound().
/// @return The index of the symbol being removed
KODO_SLIDE_API
uint64_t kslide_decoder_pop_back_symbol(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The number of symbols currently in the coding window. The
///         window must be within the bounds of the stream.
KODO_SLIDE_API
uint64_t kslide_decoder_window_symbols(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The index of the "oldest" symbol in the coding window.
KODO_SLIDE_API
uint64_t kslide_decoder_window_lower_bound(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The upper bound of the window. The range of valid symbol indices
///         goes from [decoder::window_lower_bound(),
///         decoder::window_upper_bound()). Note the window is a half-open
///         interval. Going from decoder::window_lower_bound() to
///         decoder::window_upper_bound() - 1.
KODO_SLIDE_API
uint64_t kslide_decoder_window_upper_bound(kslide_decoder_t* decoder);

/// The window represents the symbols which will be included in the next
/// decoding. The window cannot exceed the bounds of the stream.
///
/// Example: If window_lower_bound=4 and window_symbol=3 the following
///          symbol indices will be included 4,5,6
///
/// @param decoder The decoder to configure
/// @param window_lower_bound Sets the index of the oldest symbol in the
///        window.
/// @param window_symbols Sets number of symbols within the window.
KODO_SLIDE_API
void kslide_decoder_set_window(kslide_decoder_t* decoder, uint64_t window_offset, uint64_t window_symbols);

/// @param decoder The decoder to query
/// @return The size of the coefficient vector in the current window in
///         bytes. The number of coefficients is equal to the number of
///         symbols in the window. The size in bits of each coefficients
///         depends on the finite field chosen. A custom coding scheme can
///         be implemented by generating the coding vector manually.
///         Alternatively the built-in generator can be used. See
///         decoder::set_seed(...) and decoder::generate(...).
KODO_SLIDE_API
uint64_t kslide_decoder_coefficient_vector_size(kslide_decoder_t* decoder);

/// Seed the internal random generator function. If using the same seed
/// on the decoder and decoder the exact same set of coefficients will
/// be generated.
/// @param decoder The decoder to configure
/// @param seed_value A value for the seed.
KODO_SLIDE_API
void kslide_decoder_set_seed(kslide_decoder_t* decoder, uint64_t seed_value);

/// Generate coding coefficients for the symbols in the coding window
/// according to the specified seed (see decoder::set_seed(...)).
/// @param decoder The decoder to use
/// @param coefficients Buffer where the coding coefficients should be
///        stored. This buffer must be decoder::coefficient_vector_size()
///        large in bytes.
KODO_SLIDE_API
void kslide_decoder_generate(kslide_decoder_t* decoder, uint8_t* coefficients);

/// Decodes a coded symbol according to the coding coefficients.
///
/// Both buffers may be modified during this call. The reason for this
/// is that the decoder will directly operate on the provided memory
/// for performance reasons.
///
/// @param decoder The decoder to use
/// @param symbol Buffer representing a coded symbol.
///
/// @param coefficients The coding coefficients used to
///        create the encoded symbol
KODO_SLIDE_API
void kslide_decoder_read_symbol(kslide_decoder_t* decoder, uint8_t* symbol, uint8_t* coefficients);

/// Add a source symbol at the decoder.
///
/// @param decoder The decoder to use
/// @param symbol Buffer containing the source symbol's data.
/// @param index The index of the source symbol in the stream
KODO_SLIDE_API
void kslide_decoder_read_source_symbol(kslide_decoder_t* decoder, uint8_t* symbol, uint64_t index);

/// The rank of a decoder indicates how many symbols have been
/// partially or fully decoded. This number is also equivalent to the
/// number of pivot elements we have in the stream.
///
/// @param decoder The decoder to query
/// @return The rank of the decoder
KODO_SLIDE_API
uint64_t kslide_decoder_rank(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The number of missing symbols at the decoder
KODO_SLIDE_API
uint64_t kslide_decoder_symbols_missing(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The number of partially decoded symbols at the decoder
KODO_SLIDE_API
uint64_t kslide_decoder_symbols_partially_decoded(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @return The number of decoded symbols at the decoder
KODO_SLIDE_API
uint64_t kslide_decoder_symbols_decoded(kslide_decoder_t* decoder);

/// @param decoder The decoder to query
/// @param index Index of the symbol to check.
///
/// @return True if the symbol is decoded (i.e. it corresponds to a source
///         symbol), and otherwise false.
KODO_SLIDE_API
bool kslide_decoder_is_symbol_decoded(kslide_decoder_t* decoder, uint64_t index);

#ifdef __cplusplus
}
#endif

// Clean up define
#undef KODO_SLIDE_API
