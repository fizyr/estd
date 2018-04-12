/* Copyright 2017-2018 Fizyr B.V. - https://fizyr.com
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include "../traits/is_tuple.hpp"

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace estd {

namespace detail {
	template<std::size_t I, typename F, typename Tuple>
	constexpr std::size_t tuple_for_each_impl(Tuple && tuple, F && f) {
		if constexpr (I == std::tuple_size_v<std::decay_t<Tuple>>) {
			return I;
		} else {
			using std::get;
			using R = decltype(std::invoke(f, get<I>(std::forward<Tuple>(tuple))));
			if constexpr(std::is_same_v<R, void>) {
				std::invoke(f, get<I>(std::forward<Tuple>(tuple)));
			} else {
				if (!std::invoke(f, get<I>(std::forward<Tuple>(tuple)))) return I;
			}
			return tuple_for_each_impl<I + 1>(std::forward<Tuple>(tuple), std::forward<F>(f));
		}
	}

	template<std::size_t I, typename F, typename Tuple>
	constexpr std::size_t tuple_for_each_i_impl(Tuple && tuple, F && f) {
		if constexpr (I == std::tuple_size_v<std::decay_t<Tuple>>) {
			return I;
		} else {
			using std::get;
			using R = decltype(std::invoke(f, I, get<I>(std::forward<Tuple>(tuple))));
			if constexpr(std::is_same_v<R, void>) {
				std::invoke(f, I, get<I>(std::forward<Tuple>(tuple)));
			} else {
				if (!std::invoke(f, I, get<I>(std::forward<Tuple>(tuple)))) return I;
			}
			return tuple_for_each_i_impl<I + 1>(std::forward<Tuple>(tuple), std::forward<F>(f));
		}
	}
}

/// Execute a functor for each tuple element in order.
/**
 * The functor willed be called as func(elem) where elem is the value of the element.
 *
 * The functor may optionally return a value to abort the loop before all elements are processed.
 * If the functor returns a value it is tested in boolean context.
 * If the value compares as false, the loop is stopped.
 *
 * Returns the index of the element that caused the loop to halt,
 * or the size of the tuple if no element caused the loop to stop.
 */
template<typename F, typename Tuple, typename = std::enable_if_t<estd::is_tuple<Tuple>>>
constexpr std::size_t for_each(Tuple && tuple, F && func) {
	return detail::tuple_for_each_impl<0>(std::forward<Tuple>(tuple), std::forward<F>(func));
}

/// Execute a functor for each tuple element in order.
/**
 * The functor willed be called as func(i, elem) where i is the index in the tuple,
 * and elem is the value of the element.
 *
 * The functor may optionally return a value to abort the loop before all elements are processed.
 * If the functor returns a value it is tested in boolean context.
 * If the value compares as false, the loop is stopped.
 *
 * Returns the index of the element that caused the loop to halt,
 * or the size of the tuple if no element caused the loop to stop.
 */
template<typename F, typename Tuple, typename = std::enable_if_t<estd::is_tuple<Tuple>>>
constexpr std::size_t for_each_i(Tuple && tuple, F && func) {
	return detail::tuple_for_each_i_impl<0>(std::forward<Tuple>(tuple), std::forward<F>(func));
}

}
