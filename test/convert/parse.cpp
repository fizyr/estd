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

#include "convert.hpp"
#include "result/error.hpp"

#include <catch2/catch.hpp>

namespace estd {

struct A {
	int value;
};

template<>
struct conversion<A, result<int, error>> {
	static result<int, error> perform(A const & a) {
		if (a.value >= 0) return a.value;
		return error{std::errc::invalid_argument, "value is negative"};
	}
};

template<>
struct conversion<A, result<int, int>> {
	static result<int, int> perform(A const & a) {
		if (a.value >= 0) return {in_place_valid, a.value};
		return {in_place_error, a.value};
	}
};

template<> struct define_default_parse_error<A, int> { using type = error; };


TEST_CASE("default_parse_error allows shorter parse<T>(...) invocation.", "[convert]") {
	REQUIRE(parse<int>(A{1}) == 1);
	REQUIRE(parse<int>(A{-1}) == std::errc::invalid_argument);

	SECTION("but the long form still works") {
		REQUIRE((parse<int, error>)(A{ 1}) == 1);
		REQUIRE((parse<int, error>)(A{-1}) == std::errc::invalid_argument);
		REQUIRE((parse<int, int>)(A{ 1}) == (result<int, int>(in_place_valid,  1)));
		REQUIRE((parse<int, int>)(A{-1}) == (result<int, int>(in_place_error, -1)));

	}
}

}
