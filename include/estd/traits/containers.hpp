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

template<typename T> using iterator_type               = std::decay_t<decltype(begin(std::declval<T &>()))>;
template<typename T> using const_iterator_type         = std::decay_t<decltype(begin(std::declval<T const &>()))>;
template<typename T> using reverse_iterator_type       = std::decay_t<decltype(std::declval<T &>().rbegin())>;
template<typename T> using const_reverse_iterator_type = std::decay_t<decltype(std::declval<T const &>().rend())>;

namespace detail {
	using std::declval;

	template<typename T, typename = void> struct has_begin : std::false_type {};
	template<typename T> struct has_begin<T, std::void_t<decltype(declval<T>().begin())>> : std::true_type {};

	template<typename T, typename = void> struct has_end : std::false_type {};
	template<typename T> struct has_end<T, std::void_t<decltype(declval<T>().end())>> : std::true_type {};

	template<typename T, typename = void> struct has_rbegin : std::false_type {};
	template<typename T> struct has_rbegin<T, std::void_t<decltype(declval<T>().rbegin())>> : std::true_type {};

	template<typename T, typename = void> struct has_rend : std::false_type {};
	template<typename T> struct has_rend<T, std::void_t<decltype(declval<T>().rend())>> : std::true_type {};

	template<typename T, typename = void> struct has_size : std::false_type {};
	template<typename T> struct has_size<T, std::void_t<decltype(declval<T>().size())>> : std::true_type {};

	template<typename T, typename = void> struct has_reserve : std::false_type {};
	template<typename T> struct has_reserve<T, std::void_t<decltype(declval<T>().reserve(30))>> : std::true_type {};

	template<typename T, typename = void> struct has_erase : std::false_type {};
	template<typename T> struct has_erase<T, std::void_t<decltype(declval<T>().erase(declval<const_iterator_type<T>>(), declval<const_iterator_type<T>>()))>> : std::true_type {};
}

/// Test if a type T is iterable (has a begin() and end() member)
template<typename T> constexpr bool is_iterable = detail::has_begin<T>() && detail::has_end<T>();

/// Test if a type T is reverse iterable (has a rbegin() and rend() member)
template<typename T> constexpr bool is_reverse_iterable = detail::has_rbegin<T>() && detail::has_rend<T>();

/// Test if a type T has a size() member.
template<typename T> constexpr bool has_size = detail::has_size<T>();

/// Test if a type T has a reserve() member.
template<typename T> constexpr bool has_reserve = detail::has_reserve<T>();

/// Test if a type T has a erase() member.
template<typename T> constexpr bool has_erase = detail::has_erase<T>();

/// Test if a type T is a container.
/**
 * Currently this is a relaxed check. It only tests if the members begin(), end() and size() exist.
 */
template<typename T> constexpr bool is_container = is_iterable<T> && has_size<T>;

}
