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
#include <iterator>
#include <type_traits>
#include <utility>

namespace estd {

namespace detail {
	template<typename T> struct iterator_tag_value;
	template<> struct iterator_tag_value<std::input_iterator_tag>         : std::integral_constant<int, 0> {};
	template<> struct iterator_tag_value<std::output_iterator_tag>        : std::integral_constant<int, 0> {};
	template<> struct iterator_tag_value<std::forward_iterator_tag>       : std::integral_constant<int, 1> {};
	template<> struct iterator_tag_value<std::bidirectional_iterator_tag> : std::integral_constant<int, 2> {};
	template<> struct iterator_tag_value<std::random_access_iterator_tag> : std::integral_constant<int, 3> {};

	template<typename ...T> struct minimal_iterator_tag;
	template<typename ...T> struct minimal_iterator_category;

	template<typename ...T>
	struct minimal_iterator_tag<std::tuple<T...>> {
		using type = typename minimal_iterator_tag<T...>::type;
	};

	template<typename T>
	struct minimal_iterator_tag<T> {
		using type = T;
	};

	template<typename T, typename Y>
	struct minimal_iterator_tag<T, Y> {
		using type = std::conditional_t<iterator_tag_value<T>() <= iterator_tag_value<Y>(), T, Y>;
	};

	template<typename T, typename Y, typename ...R>
	struct minimal_iterator_tag<T, Y, R...> {
		using type = typename minimal_iterator_tag<typename minimal_iterator_tag<T, Y>::type, R...>::type;
	};

	template<typename ...T>
	struct minimal_iterator_category {
		using type = typename minimal_iterator_tag<typename std::iterator_traits<T>::iterator_category...>::type;
	};

	template<typename ...T>
	struct minimal_iterator_category<std::tuple<T...>> {
		using type = typename minimal_iterator_category<T...>::type;
	};

}

/// Get the minimal iterator tag of a list of tags.
/**
 * The tags may be specified directly as template arguments,
 * or as a single tuple of tags.
 *
 * For the purpose of this check, input and ouput iterators are considered equally low.
 * If both an input and an output iterator are present in the list,
 * the result is either std::input_iterator_tag or std::output_iterator_tag.
 */
template<typename ...Tags> using minimal_iterator_tag = typename detail::minimal_iterator_tag<Tags...>::type;

/// Get the minimal iterator category of a list of iterators.
/**
 * The iterators may be specified directly as template arguments,
 * or as a single tuple of iterators.
 *
 * For the purpose of this check, input and ouput iterators are considered equally low.
 * If both an input and an output iterator are present in the list,
 * the result is either std::input_iterator_tag or std::output_iterator_tag.
 */
template<typename ...Iterators> using minimal_iterator_category = typename detail::minimal_iterator_category<Iterators...>::type;

/// Check if an iterator tag represents at minimum the same category as a given tag.
/**
 * For the purpose of this check, input and output iterators represent the same category.
 */
template<typename Tag, typename MinTag>
constexpr bool is_iterator_tag_atleast = detail::iterator_tag_value<Tag>() >= detail::iterator_tag_value<MinTag>();

/// Check if the category of an iterator is at minimum the same as a given tag.
/**
 * For the purpose of this check, input and output iterators represent the same category.
 */
template<typename Iterator, typename MinTag>
constexpr bool is_iterator_atleast = is_iterator_tag_atleast<typename std::iterator_traits<Iterator>::iterator_category, MinTag>;

}
