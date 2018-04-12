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

#include "convert/traits.hpp"

#include <type_traits>

// Implicitly convertible primitives are convertible.
template<typename T, typename Y>
constexpr void assert_convertible_to_integral() {
	static_assert(estd::can_convert<T, std::make_signed_t<Y>>,   "");
	static_assert(estd::can_convert<T, std::make_unsigned_t<Y>>, "");
}

template<typename T>
constexpr void assert_convertible_to_floating() {
	static_assert(estd::can_convert<T, float>,       "");
	static_assert(estd::can_convert<T, double>,      "");
	static_assert(estd::can_convert<T, long double>, "");
}

template<typename T>
constexpr void assert_convertible_to_numeric_types() {
	static_assert(estd::can_convert<T, bool>,      "");
	assert_convertible_to_integral<T, char>();
	assert_convertible_to_integral<T, short>();
	assert_convertible_to_integral<T, int>();
	assert_convertible_to_integral<T, long>();
	assert_convertible_to_integral<T, long long>();
	assert_convertible_to_floating<T>();
}

template<typename T>
constexpr void assert_integral_convertible_to_numeric_types() {
	assert_convertible_to_numeric_types<std::make_signed_t<T>>();
	assert_convertible_to_numeric_types<std::make_unsigned_t<T>>();
}

constexpr void assert_numericals_convertible_to_numeric_types() {
	assert_convertible_to_numeric_types<bool>();
	assert_integral_convertible_to_numeric_types<short>();
	assert_integral_convertible_to_numeric_types<int>();
	assert_integral_convertible_to_numeric_types<long>();
	assert_integral_convertible_to_numeric_types<long long>();
	assert_convertible_to_numeric_types<float>();
	assert_convertible_to_numeric_types<double>();
	assert_convertible_to_numeric_types<long double>();
}

struct Foo {};
struct ExplicitFromFoo { explicit ExplicitFromFoo(Foo const &); };
struct ImplicitFromFoo { ImplicitFromFoo(Foo const &); };
struct Unrelated {};
struct UserDefined {};
struct ExplicitlyImpossible {};
struct ExplicitlyPossible {};
struct EmptyConversion {};

namespace estd {
	template<> struct conversion<Foo, UserDefined> {
		static UserDefined perform(Foo const &);
	};

	template<> struct conversion<Foo, ExplicitlyImpossible> {
		constexpr static bool possible = false;
		static ExplicitlyImpossible perform(Foo const &);
	};

	template<> struct conversion<Foo, ExplicitlyPossible> {
		constexpr static bool possible = true;
		static ExplicitlyPossible perform(Foo const &);
	};

	template<> struct conversion<Foo, EmptyConversion> {};
}

static_assert(estd::can_convert<Foo, ExplicitFromFoo>, "");
static_assert(estd::can_convert<Foo, ImplicitFromFoo>, "");
static_assert(estd::can_convert<Foo, UserDefined>, "");
static_assert(estd::can_convert<Foo, Unrelated>            == false, "");
static_assert(estd::can_convert<Foo, ExplicitlyImpossible> == false, "");
static_assert(estd::can_convert<Foo, ExplicitlyPossible>   == true,  "");
static_assert(estd::can_convert<Foo, EmptyConversion>      == false, "");
