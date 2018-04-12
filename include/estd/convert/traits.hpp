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
#include <utility>
#include <type_traits>

#include "./convert.hpp"

namespace estd {

namespace convert_detail {
	template<typename F, typename T, typename Tag, typename = void> struct has_possible : std::false_type{};
	template<typename F, typename T, typename Tag> struct has_possible<F, T, Tag, std::void_t<decltype(&::estd::conversion<F, T, Tag>::possible)>> : std::true_type {};

	template<typename F, typename T, typename Tag, typename = void> struct has_perform_impl : std::false_type{};
	template<typename F, typename T, typename Tag> struct has_perform_impl<F, T, Tag, std::void_t<decltype(::estd::conversion<std::decay_t<F>, T, Tag>::perform(std::declval<F>()))>> : std::true_type {};

	template<typename F, typename T, typename Tag>
	constexpr bool has_perform = false
		|| has_perform_impl<F       &, T, Tag>()
		|| has_perform_impl<F const &, T, Tag>()
		|| has_perform_impl<F      &&, T, Tag>();

	// check if a not-explicitly impossible conversion is defined.
	template<typename F, typename T, typename Tag,
		bool = has_possible<F, T, Tag>(),
		bool = has_perform<F, T, Tag>
	> struct can_convert : std::false_type{};

	template<typename F, typename T, typename Tag>
	struct can_convert<F, T, Tag, true, true> : std::integral_constant<bool, conversion<F, T, Tag>::possible == true> {};

	template<typename F, typename T, typename Tag>
	struct can_convert<F, T, Tag, false, true> : std::true_type {};
}

/// Check if a conversion exists for the given types.
template<typename From, typename To, typename Tag = default_conversion>
constexpr bool can_convert = convert_detail::can_convert<std::decay_t<From>, To, Tag>::value;

/// Check if a conversion that might fail exists for the given types.
template<typename From, typename To, typename Error, typename Tag = default_conversion>
constexpr bool can_parse = can_convert<std::decay_t<From>, result<To, Error>, Tag>;

}
