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

#include "tuple/for_each.hpp"

#include "../catch.hpp"

namespace estd {

TEST_CASE("tuple for_each", "[tuple]") {
	SECTION("processes each element if functor returns void") {
		int sum = 0;
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each(tuple, [&] (auto v) { sum += v; }) == 4);
		REQUIRE(sum == 10);
	}

	SECTION("processes each element if functor returns true") {
		int sum = 0;
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each(tuple, [&] (auto v) { sum += v; return true; }) == 4);
		REQUIRE(sum == 10);
	}

	SECTION("stops when functor returns false") {
		int sum = 0;
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each(tuple, [&] (auto v) { sum += v; return v != 3; }) == 2);
		REQUIRE(sum == 6);
	}

	SECTION("reports index if stopped") {
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each(tuple, [&] (auto v) { return v != 4; }) == 3);
	}

	SECTION("reports tuple size if not stopped") {
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each(tuple, [&] (auto) { return true; }) == 4);
	}
}

TEST_CASE("tuple for_each_i", "[tuple]") {
	SECTION("processes each element with the right index") {
		int sum = 0;
		auto tuple = std::make_tuple(0, 1, 2, 3);
		std::size_t index = for_each_i(tuple, [&] (int i, auto v) {
			REQUIRE(i == v);
			sum += v;
		});
		REQUIRE(index == 4);
		REQUIRE(sum   == 6);
	}

	SECTION("processes each element if functor returns true") {
		int sum = 0;
		auto tuple = std::make_tuple(0, 1, 2, 3);
		std::size_t index = for_each_i(tuple, [&] (int i, auto v) {
			REQUIRE(i == v);
			sum += v;
			return true;
		});
		REQUIRE(index == 4);
		REQUIRE(sum   == 6);
	}

	SECTION("stops when functor returns false") {
		int sum = 0;
		auto tuple = std::make_tuple(0, 1, 2, 3);
		std::size_t index = for_each_i(tuple, [&] (int i, auto v) {
			REQUIRE(i == v);
			sum += v;
			return i != 2;
		});
		REQUIRE(index == 2);
		REQUIRE(sum   == 3);
	}

	SECTION("reports index if stopped") {
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each_i(tuple, [&] (std::size_t i, auto) { return i != 3; }) == 3);
	}

	SECTION("reports tuple size if not stopped") {
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each_i(tuple, [&] (std::size_t, auto) { return true; }) == 4);
	}
}

}
