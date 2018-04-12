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
#include <cstddef>
#include <type_traits>

namespace estd {

namespace detail {
	template<std::size_t N, typename Head, typename... Tail>
	struct parameter_pack_element {
		using type = typename parameter_pack_element<N - 1, Tail...>::type;
	};

	template<typename Head, typename... Tail>
	struct parameter_pack_element<0, Head, Tail...> {
		using type = Head;
	};

	template<std::size_t N, typename T, T head, T... tail>
	struct value_pack_element : std::integral_constant<T, value_pack_element<N - 1, T, tail...>::value> {};

	template<typename T, T head, T... tail>
	struct value_pack_element<0, T, head, tail...> : std::integral_constant<T, head> {};
}

/// Get a type from a parameter pack by index.
template<std::size_t N, typename... Types> using parameter_pack_element = typename detail::parameter_pack_element<N, Types...>::type;

/// Get the first type from a parameter pack.
template<typename... Types> using parameter_pack_head = parameter_pack_element<0, Types...>;

/// Get the first value from a parameter pack.
template<typename T, T Head, T...> constexpr T value_pack_head = Head;

/// Get a value from a parameter pack by index.
template<std::size_t N, typename T, T... values> constexpr T value_pack_element = detail::value_pack_element<N, T, values...>::value;


}
