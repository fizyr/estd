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
#include "any.hpp"

# if __has_include(<catch2/catch_test_macros.hpp>)
#   include <catch2/catch_test_macros.hpp>
# else
#   include <catch2/catch.hpp>
# endif

#include <vector>

namespace estd {

void static_checks() {
	any         mut_any{5};
	any const const_any{5};

	// Check that get() preserves constness.
	static_assert_same<decltype(  mut_any.get<int>()), int       *>();
	static_assert_same<decltype(const_any.get<int>()), int const *>();

	// Check that get_static() preserves constness.
	static_assert_same<decltype(  mut_any.get_static<int>()), int       &>();
	static_assert_same<decltype(const_any.get_static<int>()), int const &>();

	// Check that any can be moved but not copied.
	static_assert(std::is_copy_constructible_v <any> == false);
	static_assert(std::is_copy_assignable_v    <any> == false);
	static_assert(std::is_move_constructible_v <any> == true);
	static_assert(std::is_move_assignable_v    <any> == true);
}

TEST_CASE("any can hold an int", "[any]") {
	any a = any{5};

	REQUIRE(any{5}.contains_type<int>() == true);
	REQUIRE(any{5}.contains_type<float>() == false);

	REQUIRE(a.get<int>() != nullptr);
	REQUIRE(*a.get<int>() == 5);
}

TEST_CASE("any.get() gives a nullptr if the wrong type is requested", "[any]") {
	REQUIRE(any{5}.get<double>() == nullptr);

	SECTION("but get_static() ignores safety") {
		REQUIRE(reinterpret_cast<int &>(any{5}.get_static<double>()) == 5);
	}
}

struct MoveInt {
	int value;

	MoveInt(MoveInt const &)        = delete;
	MoveInt(MoveInt &&)             = default;
	MoveInt & operator=(MoveInt &&) = default;
};

static_assert(std::is_copy_constructible_v <MoveInt> == false);
static_assert(std::is_copy_assignable_v    <MoveInt> == false);
static_assert(std::is_move_constructible_v <MoveInt> == true);
static_assert(std::is_move_assignable_v    <MoveInt> == true);

TEST_CASE("any can hold a move-only type and be move constructed", "[any]") {
	any a = any{MoveInt{5}};
	REQUIRE(a.has_value());
	REQUIRE(a.get<MoveInt>()->value == 5);

	any b = std::move(a);
	REQUIRE(a.has_value() == false);
	REQUIRE(b.has_value() == true);
	REQUIRE(b.get<MoveInt>()->value == 5);

	SECTION("it can also be move assigned") {
		a = std::move(b);
		REQUIRE(a.has_value() == true);
		REQUIRE(b.has_value() == false);
		REQUIRE(a.get<MoveInt>()->value == 5);
	}
}

}
