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

// TEST(test_kodo_slide_c, api)
// {
//     uint32_t symbols = 50;
//     uint32_t symbol_size = 750;
//     auto decoder_factory = kodo_slide_decoder_factory_construct(
//         kodo_slide_binary8, symbols, symbol_size);

//     auto encoder_factory = kodo_slide_encoder_factory_construct(
//         kodo_slide_binary8, symbols, symbol_size);

//     auto decoder = kodo_slide_decoder_factory_build(decoder_factory);
//     auto encoder = kodo_slide_encoder_factory_build(encoder_factory);

//     EXPECT_EQ(
//         kodo_slide_decoder_block_size(decoder),
//         kodo_slide_encoder_block_size(encoder));

//     EXPECT_EQ(symbol_size, kodo_slide_decoder_symbol_size(decoder));
//     EXPECT_EQ(symbols, kodo_slide_decoder_symbols(decoder));

//     EXPECT_EQ(symbol_size, kodo_slide_encoder_symbol_size(encoder));
//     EXPECT_EQ(symbols, kodo_slide_encoder_symbols(encoder));

//     std::vector<uint8_t> data_in(kodo_slide_encoder_block_size(encoder));
//     std::generate(data_in.begin(), data_in.end(), rand);
//     kodo_slide_encoder_set_const_symbols(
//         encoder, data_in.data(), data_in.size());

//     std::vector<uint8_t> data_out(kodo_slide_decoder_block_size(decoder));
//     kodo_slide_decoder_set_mutable_symbols(
//         decoder, data_out.data(), data_out.size());

//     EXPECT_EQ(
//         kodo_slide_decoder_payload_size(decoder),
//         kodo_slide_encoder_payload_size(encoder));

//     std::vector<uint8_t> payload(kodo_slide_encoder_payload_size(encoder));

//     EXPECT_TRUE(kodo_slide_is_systematic_on(encoder));
//     kodo_slide_encoder_set_systematic_off(encoder);
//     EXPECT_FALSE(kodo_slide_is_systematic_on(encoder));
//     kodo_slide_set_systematic_on(encoder);
//     EXPECT_TRUE(kodo_slide_is_systematic_on(encoder));
//     kodo_slide_encoder_set_systematic_off(encoder);

//     EXPECT_EQ(0U, kodo_slide_decoder_rank(decoder));
//     while(!kodo_slide_decoder_is_complete(decoder))
//     {
//         kodo_slide_encoder_write_payload(encoder, payload.data());
//         kodo_slide_decoder_read_payload(decoder, payload.data());
//     }
//     EXPECT_EQ(symbols, kodo_slide_decoder_rank(decoder));

//     EXPECT_EQ(data_in, data_out);

//     kodo_slide_decoder_destruct(decoder);
//     kodo_slide_encoder_destruct(encoder);
//     kodo_slide_decoder_factory_destruct(decoder_factory);
//     kodo_slide_encoder_factory_destruct(encoder_factory);
// }
