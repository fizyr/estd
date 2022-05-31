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
#include "tuple/transform.hpp"

# if __has_include(<catch2/catch_test_macros.hpp>)
#   include <catch2/catch_test_macros.hpp>
# else
#   include <catch2/catch.hpp>
# endif

namespace estd {

TEST_CASE("tuple transform", "[tuple]") {
	SECTION("add one to each tuple element") {
		auto add_one = [] (auto a) { return a + 1; };
		REQUIRE(tuple_transform_decay(std::tuple(1, 10),   add_one) == std::tuple(2, 11));
		REQUIRE(tuple_transform_decay(std::tuple(1, 10.0), add_one) == std::tuple(2, 11.0));
	}

	SECTION("modify tuple elements in-place") {
		auto add_one = [] (auto && a) { a += 1; };
		int a = 0;
		int b = 10;
		tuple_transform_decay(std::tie(a, b), add_one);
		REQUIRE(a == 1);
		REQUIRE(b == 11);
	}

	SECTION("tuple_transform_decay decays results") {
		auto identity = [] (auto && a) { return a; };
		static_assert(std::is_same_v<decltype(tuple_transform_decay(std::make_tuple(1, 2), identity)), std::tuple<int , int >>);
	}

	SECTION("tuple_transform_raw preserves references in results") {
		auto identity = [] (auto && a) -> decltype(a) & { return a; };
		static_assert(std::is_same_v<decltype(tuple_transform_raw(std::make_tuple(1, 2), identity)), std::tuple<int &, int &>>);
	}

	SECTION("void results are mapped to empty_t") {
		auto drop_int_add_one = [] (auto a) {
			if constexpr(std::is_same_v<decltype(a), int>) return;
			else return a + 1;
		};

		static_assert(std::is_same_v<decltype(tuple_transform_decay(std::make_tuple(1, 2.0), drop_int_add_one)), std::tuple<empty_t, double>>);
		static_assert(std::is_same_v<decltype(tuple_transform_raw  (std::make_tuple(1, 2.0), drop_int_add_one)), std::tuple<empty_t, double>>);

		REQUIRE(tuple_transform_decay(std::make_tuple(1, 2.0), drop_int_add_one) == std::tuple(empty, 3.0));
		REQUIRE(tuple_transform_raw  (std::make_tuple(3.0, 8), drop_int_add_one) == std::tuple(4.0, empty));
	}

	SECTION("when all elements are mapped to void, transform returns void itself") {
		auto drop = [] (auto) {};
		static_assert(std::is_same_v<decltype(tuple_transform_decay(std::make_tuple(1, 2.0), drop)), void>);
		static_assert(std::is_same_v<decltype(tuple_transform_raw  (std::make_tuple(1, 2.0), drop)), void>);
	}

	SECTION("only transform_raw can dereference pointers to references") {
		auto deref = [] (auto a) -> decltype(*a) & { return *a; };
		int a = 0;
		int b = 1;
		static_assert(std::is_same_v<decltype(tuple_transform_decay(std::tuple(&a, &b), deref)), std::tuple<int  , int  >>);
		static_assert(std::is_same_v<decltype(tuple_transform_raw  (std::tuple(&a, &b), deref)), std::tuple<int &, int &>>);

		auto dereferenced_raw = tuple_transform_raw(std::tuple(&a, &b), deref);
		REQUIRE(&std::get<0>(dereferenced_raw) == &a);
		REQUIRE(&std::get<1>(dereferenced_raw) == &b);

		auto dereferenced = tuple_transform_decay(std::tuple(&a, &b), deref);
		REQUIRE(&std::get<0>(dereferenced) != &a);
		REQUIRE(&std::get<1>(dereferenced) != &b);
	}
}

TEST_CASE("tuple ref", "[tuple]") {
	std::tuple<int, int> a{1, 2};
	auto b = tuple_ref(a);
	static_assert_same<decltype(b), std::tuple<int &, int &>>();
	static_assert_same<decltype(tuple_ref(std::tuple(5, 6))), std::tuple<int &&, int &&>>();
	REQUIRE(&std::get<0>(a) == &std::get<0>(b));
	REQUIRE(&std::get<1>(a) == &std::get<1>(b));
}

TEST_CASE("tuple cref", "[tuple]") {
	std::tuple<int, int> a{1, 2};
	auto b = tuple_cref(a);
	static_assert_same<decltype(b), std::tuple<int const &, int const &>>();
	static_assert_same<decltype(tuple_cref(std::tuple(5, 6))), std::tuple<int const &, int const &>>();
	REQUIRE(&std::get<0>(a) == &std::get<0>(b));
	REQUIRE(&std::get<1>(a) == &std::get<1>(b));
}

}
