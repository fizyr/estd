/* Copyright 2017-2019 Fizyr B.V. - https://fizyr.com
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
#include "heap_array/heap_array.hpp"

#include <catch2/catch.hpp>

#include <vector>

namespace estd {

TEST_CASE("constructors work", "[heap_array]") {
	SECTION("heap_array() gives an empty array", "[heap_array]") {
		auto default_constructed = heap_array<int>();
		REQUIRE(default_constructed.size() == 0);
		REQUIRE(default_constructed.empty() == true);
		REQUIRE(default_constructed.data() == nullptr);
	}

	SECTION("heap_array::allocate() gives value-initialized data", "[heap_array]") {
		auto allocated = heap_array<int>::allocate(5);
		REQUIRE(allocated.size() == 5);
		REQUIRE(allocated.empty() == false);
		REQUIRE(allocated.data() != nullptr);

		REQUIRE(allocated[0] == 0);
		REQUIRE(allocated[1] == 0);
		REQUIRE(allocated[2] == 0);
		REQUIRE(allocated[3] == 0);
		REQUIRE(allocated[4] == 0);
	}

	SECTION("heap_array::allocate() can allocate empty arrays", "[heap_array]") {
		auto allocated_empty = heap_array<int>::allocate(0);
		REQUIRE(allocated_empty.size() == 0);
		REQUIRE(allocated_empty.empty() == true);
		REQUIRE(allocated_empty.data() == nullptr);
	}

	SECTION("heap_array::unitialized() allocates the requested size", "[heap_array]") {
		auto unitialized = heap_array<int>::unitialized(5);
		REQUIRE(unitialized.size() == 5);
		REQUIRE(unitialized.empty() == false);
		REQUIRE(unitialized.data() != nullptr);
	}

	SECTION("heap_array::unitialized() can allocate empty arrays", "[heap_array]") {
		auto uninitialized_empty = heap_array<int>::unitialized(0);
		REQUIRE(uninitialized_empty.size() == 0);
		REQUIRE(uninitialized_empty.empty() == true);
		REQUIRE(uninitialized_empty.data() == nullptr);
	}


	SECTION("heap_array{val, val, val, ...} initialized a new array with the given values", "[heap_array]") {
		auto allocated = heap_array<int>{1, 2, 3, 4, 5};
		REQUIRE(allocated.size() == 5);
		REQUIRE(allocated.empty() == false);
		REQUIRE(allocated.data() != nullptr);

		REQUIRE(allocated[0] == 1);
		REQUIRE(allocated[1] == 2);
		REQUIRE(allocated[2] == 3);
		REQUIRE(allocated[3] == 4);
		REQUIRE(allocated[4] == 5);
	}
}

TEST_CASE("basic observers work", "[heap_array]") {
	heap_array<int> array{1, 2, 3, 4, 5};

	SECTION("size and iterators are consistent") {
		REQUIRE(array.size() == 5);
		REQUIRE(array.end()  - array.begin()  == 5);
		REQUIRE(array.cend() - array.cbegin() == 5);
	}

	SECTION("regular iterators work") {
		auto it = array.begin();
		REQUIRE(*it++ == 1);
		REQUIRE(*it++ == 2);
		REQUIRE(*it++ == 3);
		REQUIRE(*it++ == 4);
		REQUIRE(*it++ == 5);
		REQUIRE(it == array.end());
	}

	SECTION("reverse iterators work") {
		auto it = array.rbegin();
		REQUIRE(*it++ == 5);
		REQUIRE(*it++ == 4);
		REQUIRE(*it++ == 3);
		REQUIRE(*it++ == 2);
		REQUIRE(*it++ == 1);
		REQUIRE(it == array.rend());
	}

	SECTION("mutating values works") {
		REQUIRE(array[0] == 1);
		array[0] = -1;
		REQUIRE(array[0] == -1);
		array.at(0) = 1;
		REQUIRE(array[0] == 1);
	}

}

TEST_CASE("byte size works", "[heap_array]") {
	REQUIRE(heap_array<std::uint32_t>::allocate(5).byte_size() == 20);
}

TEST_CASE("equality comparison works", "[view]") {
	heap_array<int> a = {1, 2, 3, 4, 5};
	heap_array<int> b = {1, 2, 3, 4, 5};
	heap_array<int> c = {1, 2, 3, 4, 5, 6};
	heap_array<int> d = {1, 2, 3, 4};

	REQUIRE((a == a) == true);
	REQUIRE((a == b) == true);
	REQUIRE((a == c) == false);
	REQUIRE((a == d) == false);

	REQUIRE((a != a) == false);
	REQUIRE((a != b) == false);
	REQUIRE((a != c) == true);
	REQUIRE((a != d) == true);
}

}
