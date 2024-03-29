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
#include "tuple/zip.hpp"

# if __has_include(<catch2/catch_test_macros.hpp>)
#   include <catch2/catch_test_macros.hpp>
# else
#   include <catch2/catch.hpp>
# endif

namespace estd {

static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<>>()), std::index_sequence<>>);
static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<int>>()), std::index_sequence<0>>);
static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<int, bool, double>>()), std::index_sequence<0, 1, 2>>);

TEST_CASE("tuple zip", "[tuple]") {
	SECTION("zip two tuples") {
		auto zipped = zip(std::tuple(1, 2.0, 3), std::tuple(4.0, 5, 6.0));
		REQUIRE((zipped == std::tuple(std::tuple(1, 4.0), std::tuple(2.0, 5), std::tuple(3, 6.0))));
		static_assert_same<decltype(zipped), std::tuple<std::tuple<int, double>, std::tuple<double, int>, std::tuple<int, double>>>();
	}

	SECTION("zip three tuples") {
		auto zipped = zip(std::tuple(1, 2, 3), std::tuple(4, 5, 6), std::tuple(7, 8, 9));
		REQUIRE((zipped == std::tuple(
			std::tuple(1, 4, 7),
			std::tuple(2, 5, 8),
			std::tuple(3, 6, 9)
		)));
		static_assert_same<decltype(zipped), std::tuple<std::tuple<int, int, int>, std::tuple<int, int, int>, std::tuple<int, int, int>>>();
	}

	SECTION("references are preserved") {
		int a = 1;
		int b = 2;
		REQUIRE(zip(std::tie(a), std::tie(b)) == (std::tuple<std::tuple<int, int>>(std::tuple(1, 2))));
		REQUIRE(zip(std::tie(a, a), std::tie(b, b)) == std::tuple(std::tuple(1, 2), std::tuple(1, 2)));
		static_assert_same<decltype(zip(std::tie(a), std::tie(b))), std::tuple<std::tuple<int &, int &>>>();

		auto [tuple] = zip(std::tie(a), std::tie(b));
		REQUIRE(&std::get<0>(tuple) == &a);
		REQUIRE(&std::get<1>(tuple) == &b);
	}
}

}
