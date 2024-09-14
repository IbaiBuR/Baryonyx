#pragma once

#include <cstddef>
#include <cstdint>
#include <stdfloat>

using u8   = std::uint8_t;
using u16  = std::uint16_t;
using u32  = std::uint32_t;
using u64  = std::uint64_t;
using u128 = __uint128_t;

using usize = std::size_t;

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;

using f16 = std::float16_t;
using f32 = std::float32_t;
using f64 = std::float64_t;

using score       = i32;
using zobrist_key = u64;
using tt_key      = u16;