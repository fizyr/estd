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
#include "tuple.hpp"

#include "../traits/replace_type.hpp"
#include "../utility/make_ref.hpp"

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace estd {

/// An empty value.
constexpr struct empty_t {} empty;

constexpr bool operator==(empty_t, empty_t) { return true;  }
constexpr bool operator!=(empty_t, empty_t) { return false; }

namespace detail {
	/// Invoke a functor, replacing void return values with an empty_t value.
	template<typename F, typename ...Args>
	constexpr replace_type<void, empty_t, std::invoke_result_t<F &&, Args && ...>>
	invoke_map_void(F && f, Args && ...args) {
		using R = std::invoke_result_t<F &&, Args && ...>;
		if constexpr(std::is_same_v<R, void>) {
			std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
			return empty;
		} else {
			return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
		}
	}

	/// Apply a functor to each tuple alement and return a tuple of the results.
	/**
	 * The result types are decayed with std::decay_t.
	 *
	 * If the functor returns void for all tuple elements, this function also returns void.
	 * Otherwise, all void return values are mapped to empty_t values.
	 */
	template<typename F, typename Tuple, std::size_t ...I>
	constexpr auto tuple_transform_decay_impl(Tuple && tuple, std::index_sequence<I...>, F && f) {
		using std::get;
		constexpr bool all_void = (std::is_same_v<decltype(std::invoke(f, get<I>(std::forward<Tuple>(tuple)))), void> && ...);
		if constexpr(all_void) {
			(std::invoke(f, get<I>(std::forward<Tuple>(tuple))), ...);
		} else {
			return std::make_tuple(invoke_map_void(f, get<I>(std::forward<Tuple>(tuple)))...);
		}
	}

	/// Apply a functor to each tuple alement and return a tuple of the results without decaying the result types.
	/*
	 * This function can return tuples with references in them.
	 * You should make sure none of those references are dangling.
	 *
	 * If the functor returns void for all tuple elements, this function also returns void.
	 * Otherwise, all void return values are mapped to empty_t values.
	 */
	template<typename F, typename Tuple, std::size_t ...I>
	constexpr auto tuple_transform_raw_impl(Tuple && tuple, std::index_sequence<I...>, F && f) {
		using std::get;
		constexpr bool all_void = (std::is_same_v<decltype(std::invoke(f, get<I>(std::forward<Tuple>(tuple)))), void> && ...);
		if constexpr(all_void) {
			(std::invoke(f, get<I>(std::forward<Tuple>(tuple))), ...);
		} else {
			return std::tuple<decltype(invoke_map_void(f, get<I>(std::forward<Tuple>(tuple))))...>(invoke_map_void(f, get<I>(std::forward<Tuple>(tuple)))...);
		}
	}
}

/// Create a tuple of values by applying a function to each value, and passing the results to std::make_tuple (thus decaying the value types).
template<typename Tuple, typename F>
constexpr auto tuple_transform_decay(Tuple && tuple, F && f) {
	return detail::tuple_transform_decay_impl(std::forward<Tuple>(tuple), tuple_index_sequence<Tuple>(), std::forward<F>(f));
}

/// Create a tuple of values by applying a function to each value, without decaying the result types.
template<typename Tuple, typename F>
constexpr auto tuple_transform_raw(Tuple && tuple, F && f) {
	return detail::tuple_transform_raw_impl(std::forward<Tuple>(tuple), tuple_index_sequence<Tuple>(), std::forward<F>(f));
}

/// Make a tuple of references to the elements of a tuple.
template<typename Tuple>
constexpr auto tuple_ref(Tuple && tuple) {
	return tuple_transform_raw(std::forward<Tuple>(tuple), make_ref);
}

/// Make a tuple of const references to the elements of a tuple.
template<typename Tuple>
constexpr auto tuple_cref(Tuple const & tuple) {
	return tuple_transform_raw(tuple, make_ref);
}

}
