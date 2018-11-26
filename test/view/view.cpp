/* Copyright 2018-2019 Fizyr B.V. - https://fizyr.com
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

#include "view/view.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

#include <vector>

namespace estd {

TEST_CASE("basic observers work", "[view]") {
	std::vector<int> vec{1, 2, 3, 4, 5};

	SECTION("for const views") {
		view<int const> const_view{vec.data(), vec.size()};

		CHECK(const_view.size()  == 5);
		CHECK(const_view.data()  == vec.data());
		CHECK(const_view.begin() == vec.data());
		CHECK(const_view.end()   == vec.data() + 5);

		CHECK(const_view.at(0) == 1);
		CHECK(const_view.at(4) == 5);
		CHECK_THROWS_AS(const_view.at(-1), std::range_error const &);
		CHECK_THROWS_AS(const_view.at(5), std::range_error const &);
	}

	SECTION("for mutable views") {
		view<int> mut_view{vec.data(), vec.size()};

		CHECK(mut_view.size()  == 5);
		CHECK(mut_view.data()  == vec.data());
		CHECK(mut_view.begin() == vec.data());
		CHECK(mut_view.end()   == vec.data() + 5);
		CHECK(mut_view.at(0) == 1);
		CHECK(mut_view.at(4) == 5);
		CHECK_THROWS_AS(mut_view.at(-1), std::range_error const &);
		CHECK_THROWS_AS(mut_view.at(5), std::range_error const &);
	}
}

TEST_CASE("byte size works", "[view]") {
	std::vector<int> vec{1, 2, 3, 4, 5};
	REQUIRE(view<int>(vec).byte_size() == 20);
	REQUIRE(view<int const>(vec).byte_size() == 20);
}

TEST_CASE("reverse iterators work", "[view]") {
	std::vector<int> vec{1, 2, 3, 4, 5};
	view<int> mut_view{vec.data(), vec.size()};

	SECTION("in the case of rbegin()") {
		auto it = mut_view.rbegin();
		REQUIRE(it != mut_view.rend());
		REQUIRE( *it++ == 5);
		REQUIRE(it != mut_view.rend());
		REQUIRE( *it++ == 4);
		REQUIRE(it != mut_view.rend());
		REQUIRE( *it++ == 3);
		REQUIRE(it != mut_view.rend());
		REQUIRE( *it++ == 2);
		REQUIRE(it != mut_view.rend());
		REQUIRE( *it++ == 1);
		REQUIRE(it == mut_view.rend());
	}

	SECTION("in the case of crbegin()") {
		auto it = mut_view.crbegin();
		REQUIRE(it != mut_view.crend());
		REQUIRE( *it++ == 5);
		REQUIRE(it != mut_view.crend());
		REQUIRE( *it++ == 4);
		REQUIRE(it != mut_view.crend());
		REQUIRE( *it++ == 3);
		REQUIRE(it != mut_view.crend());
		REQUIRE( *it++ == 2);
		REQUIRE(it != mut_view.crend());
		REQUIRE( *it++ == 1);
		REQUIRE(it == mut_view.crend());
	}
}

TEST_CASE("Equality comparison works", "[view]") {
	std::vector<int> vec_a{1, 2, 3, 4, 5};
	std::vector<int> vec_b{1, 2, 3, 4, 5};
	std::vector<int> vec_c{1, 2, 3, 4, 5, 6};
	std::vector<int> vec_d{1, 2, 3};

	view<int> view_a{vec_a};
	view<int> view_b{vec_b};
	view<int> view_c{vec_c};
	view<int> view_d{vec_d};

	REQUIRE((view_a == view_a) == true);
	REQUIRE((view_a == view_b) == true);
	REQUIRE((view_a == view_c) == false);
	REQUIRE((view_a == view_d) == false);

	REQUIRE((view_a != view_a) == false);
	REQUIRE((view_a != view_b) == false);
	REQUIRE((view_a != view_c) == true);
	REQUIRE((view_a != view_d) == true);

	SECTION("Comparison with contiguous container works") {
		REQUIRE((view_a == vec_a) == true);
		REQUIRE((view_a == vec_b) == true);
		REQUIRE((view_a == vec_c) == false);
		REQUIRE((view_a == vec_d) == false);

		REQUIRE((view_a != vec_a) == false);
		REQUIRE((view_a != vec_b) == false);
		REQUIRE((view_a != vec_c) == true);
		REQUIRE((view_a != vec_d) == true);
	}
}


TEST_CASE("mutable views can mutate data", "[view]") {
	std::vector<int> vec{1, 2, 3, 4, 5};
	view<int> mut_view{vec.data(), vec.size()};

	mut_view[0] = 10;
	CHECK(vec[0] == 10);

	SECTION("but const views can not") {
		view<int const> const_view{vec.data(), vec.size()};
		static_assert_same<int const &, decltype(const_view[0])>();
	}
}

/// Verify that construction from dangerous implicitly convertible pointer types is not allowed.
void check_construction_from_convertible_pointers() {
	/// Base class for this test.
	struct Base {
		int foo;
	};

	/// Derived class with the same size as the base class.
	struct SameSize : Base {};

	/// Derived class that is bigger than the base class.
	struct Bigger : Base {
		int bar;
	};

	// Test that a view can be constructed from pointers to the base type, or pointers to derived types with the same size.
	static_assert(std::is_constructible<view<Base>, Base     *, Base     *>() == true);
	static_assert(std::is_constructible<view<Base>, SameSize *, SameSize *>() == true);

	// But not from pointers to derived types that are a bigger.
	static_assert(std::is_constructible<view<Base>, Bigger *, Bigger *>() == false);
}


}
