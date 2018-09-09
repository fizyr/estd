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
#include <type_traits>

namespace estd {

template<typename T, typename E> class result;

namespace detail {
	template<typename> struct is_result : std::false_type {};

	template<typename T, typename E> struct is_result<result<T, E>> : std::true_type {};

	template<typename> struct result_value_type {};
	template<typename T, typename E> struct result_value_type<result<T, E>> {
		using type = T;
	};

	template<typename> struct result_error_type {};
	template<typename T, typename E> struct result_error_type<result<T, E>> {
		using type = E;
	};
}

/// Compile time test to check if a type is an instantiation of result<T, E>.
template<typename R> constexpr bool is_result = detail::is_result<R>::value;

/// Get the value type of a result<T, E>.
/**
 * Triggers a SFINAE-friendly error if R is not a result<T, E>.
 */
template<typename R> using result_value_type = typename detail::result_value_type<std::decay_t<R>>::type;

/// Get the error type of a result<T, E>.
/**
 * Triggers a SFINAE-friendly error if R is not a result<T, E>.
 */
template<typename R> using result_error_type = typename detail::result_error_type<std::decay_t<R>>::type;

}
