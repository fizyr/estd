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

#include "range/enumerate.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

#include <vector>

namespace estd {

TEST_CASE("enumerating a vector", "[enumerate]") {
	std::vector<int> vec{3, 2, 1};

	SECTION("counter increments and values are looped") {
		int i = 0;
		for (auto [count, elem] : enumerate(vec)) {
			REQUIRE(i++ == count);
			REQUIRE(elem == 3 - count);
		}
	}

	SECTION("identity of elements is same") {
		REQUIRE(&vec.front() == &std::get<1>(*enumerate(vec).begin()));
		for (auto const [count, elem] : enumerate(vec)) {
			REQUIRE(&vec[count] == &elem);
		}
	}

	SECTION("type of looped elements is reference") {
		static_assert(std::is_same_v<decltype(*enumerate(vec).begin()), std::tuple<std::ptrdiff_t, int &>>);
	}
}

TEST_CASE("enumerating an enumeration of a vector", "[enumerate]") {
	std::vector<int> vec{3, 2, 1};
	int i = 0;
	for (auto [count1, elem] : enumerate(enumerate(vec))) {
		auto [count2, value] = elem;
		REQUIRE(i++ == count1);
		REQUIRE(count2 == count1);
		REQUIRE(value == 3 - count1);
	}
}

TEST_CASE("elements can be modified while enumerating a vector", "[enumerate]") {
	std::vector<int> vec{3, 2, 1};
	int i = 0;

	for (auto [count, elem] : enumerate(vec)) {
		elem = count;
	}

	REQUIRE(vec == (std::vector{0, 1, 2}));
}

TEST_CASE("elements can not be modified while enumerating a const vector", "[enumerate]") {
	std::vector<int> const vec{3, 2, 1};
	static_assert(std::is_same_v<decltype(*enumerate(vec).begin()), std::tuple<std::ptrdiff_t, int const &>>);
}

}
