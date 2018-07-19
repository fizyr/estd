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
#include "./conversion.hpp"

#include "./traits.hpp"
#include "../result/result.hpp"
#include "../result/error.hpp"

#include <type_traits>
#include <utility>

namespace estd {

/// Specializable struct to define the default error type for parsing a T from an F.
template<typename F, typename T, typename Tag = default_conversion>
struct define_default_parse_error {
	using type = error;
};

/// Get the default error type for parsing a T from an F.
template<typename F, typename T, typename Tag = default_conversion>
using default_parse_error = typename define_default_parse_error<F, T, Tag>::type;

/// Convert a value to type T.
template<typename T, typename Tag = default_conversion, typename F>
T convert(F && from) {
	return conversion<std::decay_t<F>, T, Tag>::perform(std::forward<F>(from));
}

/// Convert a value to a result<T, E>.
/**
 * Shorthand for estd::convert<estd::result<T, E>, Tag>(from)
 */
template<typename T, typename E, typename Tag = default_conversion, typename F>
result<T, E> parse(F && from) {
	if constexpr (!estd::can_parse<F, T, E, Tag> && estd::can_convert<F, T, Tag>) {
		return {estd::in_place_valid, convert<T, Tag>(std::forward<F>(from))};
	} else {
		return convert<result<T, E>, Tag>(std::forward<F>(from));
	}
}

/// Convert a value to a result<T>.
/**
 * Shorthand for estd::convert<estd::result<T, E>, Tag>(from)
 */
template<typename T, typename F>
result<T, default_parse_error<F, T>> parse(F && from) {
	using E   = default_parse_error<F, T>;
	using Tag = default_conversion;
	return parse<T, E, Tag>(std::forward<F>(from));
}

}
