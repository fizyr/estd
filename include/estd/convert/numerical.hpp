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
#include "./traits.hpp"
#include "./convert.hpp"

#include "../result/result.hpp"

#include <charconv>
#include <string>
#include <string_view>
#include <system_error>

namespace estd {

namespace impl {
	template<typename T>
	result<T, std::error_code> string_to_numerical(std::string_view input) {
		T value;
		auto [ptr, error] = std::from_chars(&input[0], &input[input.size()], value);
		if (error != std::errc{}) return {estd::in_place_error, make_error_code(error)};
		if (ptr != &input[input.size()]) return {estd::in_place_error, make_error_code(std::errc::invalid_argument)};
		return {estd::in_place_valid, value};
	}

	template<typename T>
	result<std::string, std::error_code> numerical_to_string(T input) {
		// Start with 16 bytes, grow as needed.
		std::string result;
		result.resize(16);
		while (true) {
			auto [ptr, error] = std::to_chars(&result[0], &result[result.size()], input);
			if (error == std::errc::value_too_large) {
				if (result.size() >= 1024) return {in_place_error, make_error_code(std::errc::value_too_large)};
				result.resize(result.size() * 2);
			} else if (error != std::errc{}) {
				return {estd::in_place_error, make_error_code(error)};
			} else {
				// Resize down to the used space.
				result.resize(ptr - &result[0]);
				return {estd::in_place_valid, std::move(result)};
			}
		}
	}
}

}

#define ESTD_DEFINE_NUMERICAL_CONVERSIONS(T) \
template<> struct estd::conversion<std::string_view, estd::result<T, std::error_code>, estd::default_conversion> { \
	static estd::result<T, std::error_code> perform(std::string_view from) {\
		return estd::impl::string_to_numerical<T>(from); \
	} \
};\
template<> struct estd::conversion<char const *, estd::result<T, std::error_code>, estd::default_conversion> { \
	static estd::result<T, std::error_code> perform(char const * from) {\
		return estd::impl::string_to_numerical<T>(from); \
	} \
};\
template<> struct estd::conversion<std::string, estd::result<T, std::error_code>, estd::default_conversion> { \
	static estd::result<T, std::error_code> perform(std::string const & from) {\
		return estd::impl::string_to_numerical<T>(from); \
	} \
};\
template<> struct estd::conversion<T, std::string, estd::default_conversion> { \
	static std::string perform(T from) {\
		auto result = impl::numerical_to_string<T>(from); \
		if (!result) throw std::system_error(result.error(), "unexpected error during numerical-to-string conversion"); \
		return std::move(*result); \
	} \
}

ESTD_DEFINE_NUMERICAL_CONVERSIONS(char);
ESTD_DEFINE_NUMERICAL_CONVERSIONS(signed char);
ESTD_DEFINE_NUMERICAL_CONVERSIONS(unsigned char);

ESTD_DEFINE_NUMERICAL_CONVERSIONS(short);
ESTD_DEFINE_NUMERICAL_CONVERSIONS(unsigned short);

ESTD_DEFINE_NUMERICAL_CONVERSIONS(int);
ESTD_DEFINE_NUMERICAL_CONVERSIONS(unsigned int);

ESTD_DEFINE_NUMERICAL_CONVERSIONS(long);
ESTD_DEFINE_NUMERICAL_CONVERSIONS(unsigned long);

ESTD_DEFINE_NUMERICAL_CONVERSIONS(long long);
ESTD_DEFINE_NUMERICAL_CONVERSIONS(unsigned long long);

//ESTD_DEFINE_NUMERICAL_CONVERSIONS(float);
//ESTD_DEFINE_NUMERICAL_CONVERSIONS(double);
//ESTD_DEFINE_NUMERICAL_CONVERSIONS(long double);
#undef ESTD_DEFINE_NUMERICAL_CONVERSIONS
