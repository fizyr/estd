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

#include "../static_assert_same.hpp"
#include "tuple/fold.hpp"

# if __has_include(<catch2/catch_test_macros.hpp>)
#   include <catch2/catch_test_macros.hpp>
# else
#   include <catch2/catch.hpp>
# endif

namespace estd {

TEST_CASE("tuple left fold", "[tuple]") {
	auto sum = [] (auto a, auto b) { return a + b; };
	auto mul = [] (auto a, auto b) { return a * b; };
	auto drop_right = [] (auto a, auto) { return a; };
	auto drop_left  = [] (auto, auto b) { return b; };

	SECTION("foldl over empty tuple returns initial accumulator") {
		REQUIRE(foldl(std::tuple<>(), 5, sum) == 5);
	}

	SECTION("foldl without accumulator over single element tuple returns first element") {
		REQUIRE(foldl(std::tuple(5), mul) == 5);
	}

	SECTION("foldl without accumulator over non-empty tuple") {
		REQUIRE(foldl(std::tuple(1, 2, 3, 4), sum) == 1 + 2 + 3 + 4);
		REQUIRE(foldl(std::tuple(1, 2, 3, 4), mul) == 1 * 2 * 3 * 4);
	}

	SECTION("foldl with accumulator over non-empty tuple") {
		REQUIRE(foldl(std::tuple(1, 2, 3, 4), 10, sum) == 10 + 1 + 2 + 3 + 4);
		REQUIRE(foldl(std::tuple(1, 2, 3, 4), 10, mul) == 10 * 1 * 2 * 3 * 4);
	}

	SECTION("foldl over heterogenous tuple") {
		REQUIRE(foldl(std::tuple(1, 2.0, 3.f, 4), 10, sum) == 10.0 + 1 + 2 + 3 + 4);
		REQUIRE(foldl(std::tuple(1, 2.0, 3.f, 4), 10, mul) == 10.0 * 1 * 2 * 3 * 4);
		static_assert_same<decltype(foldl(std::tuple(1, 2.0, 3.f, 4), 10, sum)), double>();
		static_assert_same<decltype(foldl(std::tuple(1, 2.0, 3.f, 4), 10, mul)), double>();
	}

	SECTION("foldl is a left fold") {
		REQUIRE(foldl(std::tuple(1.0, 2, 3, 4.0), 10, drop_right) == 10);
		REQUIRE(foldl(std::tuple(1  , 2, 3, 4.0),     drop_right) ==  1);
		static_assert_same<decltype(foldl(std::tuple(1.0, 2, 3, 4.0), 10, drop_right)), int>();
		static_assert_same<decltype(foldl(std::tuple(1  , 2, 3, 4.0),     drop_right)), int>();

		REQUIRE(foldl(std::tuple(1, 2, 3, 4.0), 10, drop_left) == 4.0);
		REQUIRE(foldl(std::tuple(1, 2, 3, 4.0),     drop_left) == 4.0);
		static_assert_same<decltype(foldl(std::tuple(1, 2, 3, 4.0), 10, drop_left)), double>();
		static_assert_same<decltype(foldl(std::tuple(1, 2, 3, 4.0),     drop_left)), double>();
	}
}

TEST_CASE("tuple right fold", "[tuple]") {
	auto sum = [] (auto a, auto b) { return a + b; };
	auto mul = [] (auto a, auto b) { return a * b; };
	auto drop_right = [] (auto a, auto) { return a; };
	auto drop_left  = [] (auto, auto b) { return b; };

	SECTION("foldr over empty tuple returns initial accumulator") {
		REQUIRE(foldr(std::tuple<>(), 5, sum) == 5);
	}

	SECTION("foldr without accumulator over single element tuple returns first element") {
		REQUIRE(foldr(std::tuple(5), mul) == 5);
	}

	SECTION("foldr without accumulator over non-empty tuple") {
		REQUIRE(foldr(std::tuple(1, 2, 3, 4), sum) == 1 + 2 + 3 + 4);
		REQUIRE(foldr(std::tuple(1, 2, 3, 4), mul) == 1 * 2 * 3 * 4);
	}

	SECTION("foldr with accumulator over non-empty tuple") {
		REQUIRE(foldr(std::tuple(1, 2, 3, 4), 10, sum) == 10 + 1 + 2 + 3 + 4);
		REQUIRE(foldr(std::tuple(1, 2, 3, 4), 10, mul) == 10 * 1 * 2 * 3 * 4);
	}

	SECTION("foldr over heterogenous tuple") {
		REQUIRE(foldr(std::tuple(1, 2.0, 3.f, 4), 10, sum) == 10.0 + 1 + 2 + 3 + 4);
		REQUIRE(foldr(std::tuple(1, 2.0, 3.f, 4), 10, mul) == 10.0 * 1 * 2 * 3 * 4);
		static_assert_same<decltype(foldr(std::tuple(1, 2.0, 3.f, 4), 10, sum)), double>();
		static_assert_same<decltype(foldr(std::tuple(1, 2.0, 3.f, 4), 10, mul)), double>();
	}

	SECTION("foldr is a right fold") {
		REQUIRE(foldr(std::tuple(1, 2, 3, 4.0), 10.0, drop_right) == 1);
		REQUIRE(foldr(std::tuple(1, 2, 3, 4.0),       drop_right) == 1);
		static_assert_same<decltype(foldr(std::tuple(1, 2, 3, 4.0), 10.0, drop_right)), int>();
		static_assert_same<decltype(foldr(std::tuple(1, 2, 3, 4.0),       drop_right)), int>();

		REQUIRE(foldr(std::tuple(1, 2, 3, 4  ), 10.0, drop_left) == 10.0);
		REQUIRE(foldr(std::tuple(1, 2, 3, 4.0),       drop_left) ==  4.0);
		static_assert_same<decltype(foldr(std::tuple(1, 2, 3, 4  ), 10.0, drop_left)), double>();
		static_assert_same<decltype(foldr(std::tuple(1, 2, 3, 4.0),       drop_left)), double>();
	}
}

}
