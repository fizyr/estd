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

#include "tuple.hpp"
#include "../traits/is_tuple.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

namespace estd {

namespace detail {
	template<typename Tuple, typename Predicate, std::size_t ...I>
	constexpr auto tuple_all_impl(Tuple && tuple, Predicate && pred, std::index_sequence<I...>) {
		return (pred(std::get<I>(tuple)) && ...);
	}

	template<typename Tuple, typename Predicate, std::size_t ...I>
	constexpr auto tuple_any_impl(Tuple && tuple, Predicate && pred, std::index_sequence<I...>) {
		return (pred(std::get<I>(tuple)) || ...);
	}

	template<typename Tuple, typename Predicate, std::size_t ...I>
	constexpr auto tuple_none_impl(Tuple && tuple, Predicate && pred, std::index_sequence<I...>) {
		return (!pred(std::get<I>(tuple)) && ...);
	}

	template<typename Tuple, typename Predicate, std::size_t ...I>
	constexpr auto tuple_count_impl(Tuple && tuple, Predicate && pred, std::index_sequence<I...>) {
		return (bool(pred(std::get<I>(tuple))) + ...);
	}

	template<typename Tuple, typename Predicate, std::size_t ...I>
	constexpr auto tuple_count_not_impl(Tuple && tuple, Predicate && pred, std::index_sequence<I...>) {
		return (!pred(std::get<I>(tuple)) + ...);
	}
}

/// Check if all elements of a tuple satisfy a given predicate.
template<typename Tuple, typename Predicate, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto all_of(Tuple && tuple, Predicate && pred) {
	return detail::tuple_all_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(pred), tuple_index_sequence<Tuple>);
}

/// Check if any elements of a tuple satisfy a given predicate.
template<typename Tuple, typename Predicate, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto any_of(Tuple && tuple, Predicate && pred) {
	return detail::tuple_any_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(pred), tuple_index_sequence<Tuple>);
}

/// Check if none elements of a tuple satisfy a given predicate.
template<typename Tuple, typename Predicate, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto none_of(Tuple && tuple, Predicate && pred) {
	return detail::tuple_none_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(pred), tuple_index_sequence<Tuple>);
}

/// Count the number tuple elements that satisfy a given predicate.
template<typename Tuple, typename Predicate, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto count_if(Tuple && tuple, Predicate && pred) {
	return detail::tuple_count_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(pred), tuple_index_sequence<Tuple>);
}

/// Count the number tuple elements that don't satisfy a given predicate.
template<typename Tuple, typename Predicate, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto count_if_not(Tuple && tuple, Predicate && pred) {
	return detail::tuple_count_not_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(pred), tuple_index_sequence<Tuple>);
}

/// Perform a left fold over the values of a tuple with an initial accumulator.
/**
 * If the tuple is empty, the initial accumulator value is returned unchanged.
 */
template<typename F, typename Tuple, typename Accumulator, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldl(Tuple && tuple, Accumulator && accumulator, F && f) {
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 0) {
		return accumulator;
	} else {
		return foldl(slice_ref<1>(std::forward<Tuple>(tuple)), f(accumulator, front(std::forward<Tuple>(tuple))), f);
	}
}

/// Perform a left fold over the values of a tuple without an initial accumulator.
/**
 * If the tuple contains exactly one element, that element is returned unchanged.
 */
template<typename F, typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldl(Tuple && tuple, F && f) {
	static_assert(std::tuple_size_v<Tuple> > 0, "Can not perform a fold over an empty tuple without an initial accumulator value.");
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 1) {
		return get<0>(std::forward<Tuple>(tuple));
	} else {
		return foldl(slice_ref<1>(std::forward<Tuple>(tuple)), front(std::forward<Tuple>(tuple)), f);
	}
}

/// Perform a right fold over the values of a tuple with an initial accumulator.
/**
 * If the tuple is empty, the initial accumulator value is returned unchanged.
 */
template<typename F, typename Tuple, typename Accumulator, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldr(Tuple && tuple, Accumulator && accumulator, F && f) {
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 0) {
		return accumulator;
	} else {
		return foldr(slice_ref<0, -1>(std::forward<Tuple>(tuple)), f(back(std::forward<Tuple>(tuple)), accumulator), f);
	}
}

/// Perform a right fold over the values of a tuple without an initial accumulator.
/**
 * If the tuple contains exactly one element, that element is returned unchanged.
 */
template<typename F, typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldr(Tuple && tuple, F && f) {
	static_assert(std::tuple_size_v<Tuple> > 0, "Can not perform a fold over an empty tuple without an initial accumulator value.");
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 1) {
		return get<0>(std::forward<Tuple>(tuple));
	} else {
		return foldr(slice_ref<0, -1>(std::forward<Tuple>(tuple)), back(std::forward<Tuple>(tuple)), f);
	}
}

}
