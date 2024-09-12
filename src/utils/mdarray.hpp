#pragma once

#include <array>
#include <cassert>

#include "../types.hpp"

namespace utils {

namespace internal {

/// @note Based on the implementation at https://github.com/Ciekce/Stormphrax/blob/main/src/util/multi_array.h
template <typename T, usize N, usize... Ns>
struct mdarray_impl;

/// @brief Specialization for the last dimension (1D array) of a multidimensional array
/// @tparam T Type of the elements in the array
/// @tparam N Size of the array in the current dimension
template <typename T, usize N>
struct mdarray_impl<T, N> {
        std::array<T, N> data{};

        constexpr T& operator[](const usize i) noexcept {
            assert(i < N);
            return data[i];
        }

        constexpr const T& operator[](const usize i) const noexcept {
            assert(i < N);
            return data[i];
        }
};

/// @brief Recursive template for handling multidimensional arrays
/// @tparam T Type of the elements in the array
/// @tparam N Size of the array in the current dimension
/// @tparam Ns Sizes of the remaining dimensions
template <typename T, usize N, usize... Ns>
struct mdarray_impl {
        std::array<mdarray_impl<T, Ns...>, N> data{};

        template <typename... Idx>
        constexpr T& operator[](const usize i, const Idx... idxs) noexcept {
            assert(i < N);
            return data[i][idxs...];
        }

        template <typename... Idx>
        constexpr const T& operator[](const usize i, const Idx... idxs) const noexcept {
            assert(i < N);
            return data[i][idxs...];
        }
};

} // namespace internal

template <typename T, usize... Ns>
using mdarray = internal::mdarray_impl<T, Ns...>;

} // namespace utils
