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
#include "range.hpp"

# if __has_include(<catch2/catch_test_macros.hpp>)
#   include <catch2/catch_test_macros.hpp>
# else
#   include <catch2/catch.hpp>
# endif

#include <vector>

namespace estd {

TEST_CASE("a default constructed range is empty", "[range]") {
	REQUIRE(range<int *>().empty());
	REQUIRE(range<std::vector<int>::iterator>().empty());
}

TEST_CASE("a range of two pointers", "[range]") {
	int a[4] = {1, 2, 3, 4};

	range r(a, a + 4);
	SECTION("has two iterators") {
		REQUIRE(r.begin() == a);
		REQUIRE(r.end()   == a + 4);
		REQUIRE(r.distance() == 4);
		REQUIRE(not r.empty());
	}

	SECTION("can be copied") {
		range r2 = r;
		REQUIRE(r2 == r);
		REQUIRE(r2.begin() == r.begin());
		REQUIRE(r2.end()   == r.end());
	}

	SECTION("is empty when begin() == end()") {
		r.begin() = r.end();
		REQUIRE(r.empty());
		REQUIRE(r.distance() == 0);
	}

	SECTION("can be looped") {
		int i = 1;
		for (int elem : r) REQUIRE(elem == i++);
	}
}

TEST_CASE("a range for a vector", "[range]") {
	std::vector<int> container{1, 2, 3};
	range r(container);

	SECTION("has two iterators") {
		REQUIRE(r.begin() == container.begin());
		REQUIRE(r.end()   == container.end());
		REQUIRE(r.distance() == 3);
		REQUIRE(not r.empty());
	}

	SECTION("can be copied") {
		range r2 = r;
		REQUIRE(r2 == r);
		REQUIRE(r2.begin() == r.begin());
		REQUIRE(r2.end()   == r.end());
	}

	SECTION("is empty when begin() == end()") {
		r.begin() = r.end();
		REQUIRE(r.empty());
		REQUIRE(r.distance() == 0);
	}

	SECTION("can be looped") {
		int i = 1;
		for (int elem : r) REQUIRE(elem == i++);
	}
}

}
