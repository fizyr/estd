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
#include "result.hpp"
#include "result/catch_string_conversions.hpp"

# if __has_include(<catch2/catch_test_macros.hpp>)
#   include <catch2/catch_test_macros.hpp>
# else
#   include <catch2/catch.hpp>
# endif

namespace estd {

struct A {
	int data;
	explicit A(int data) : data{data} {};
};

struct B {
	int data;
	explicit B(int data) : data{data} {};
};

bool operator==(A const & lhs, A const & rhs) { return lhs.data == rhs.data; }
bool operator==(B const & lhs, B const & rhs) { return lhs.data == rhs.data; }
bool operator==(A const & lhs, B const & rhs) { return lhs.data == rhs.data; }
bool operator==(B const & lhs, A const & rhs) { return lhs.data == rhs.data; }
bool operator!=(A const & lhs, A const & rhs) { return !(lhs == rhs); }
bool operator!=(B const & lhs, B const & rhs) { return !(lhs == rhs); }
bool operator!=(A const & lhs, B const & rhs) { return !(lhs == rhs); }
bool operator!=(B const & lhs, A const & rhs) { return !(lhs == rhs); }

TEST_CASE("compare results for equality", "[result]") {
	SECTION("valid equal results") {
		REQUIRE((result<A, A>(in_place_valid, 5)) == (result<A, A>(in_place_valid, 5)));
		REQUIRE((result<A, A>(in_place_valid, 5)) == (result<A, B>(in_place_valid, 5)));
		REQUIRE((result<A, A>(in_place_valid, 5)) == (result<B, A>(in_place_valid, 5)));
		REQUIRE((result<A, A>(in_place_valid, 5)) == (result<B, B>(in_place_valid, 5)));
	}

	SECTION("valid inequal results") {
		REQUIRE((result<A, A>(in_place_valid, 5)) != (result<A, A>(in_place_valid, 6)));
		REQUIRE((result<A, A>(in_place_valid, 5)) != (result<A, B>(in_place_valid, 6)));
		REQUIRE((result<A, A>(in_place_valid, 5)) != (result<B, A>(in_place_valid, 6)));
		REQUIRE((result<A, A>(in_place_valid, 5)) != (result<B, B>(in_place_valid, 6)));
	}

	SECTION("invalid equal results") {
		REQUIRE((result<A, A>(in_place_error, 5)) == (result<A, A>(in_place_error, 5)));
		REQUIRE((result<A, A>(in_place_error, 5)) == (result<A, B>(in_place_error, 5)));
		REQUIRE((result<A, A>(in_place_error, 5)) == (result<B, A>(in_place_error, 5)));
		REQUIRE((result<A, A>(in_place_error, 5)) == (result<B, B>(in_place_error, 5)));
	}

	SECTION("invalid inequal results") {
		REQUIRE((result<A, A>(in_place_error, 5)) != (result<A, A>(in_place_error, 6)));
		REQUIRE((result<A, A>(in_place_error, 5)) != (result<A, B>(in_place_error, 6)));
		REQUIRE((result<A, A>(in_place_error, 5)) != (result<B, A>(in_place_error, 6)));
		REQUIRE((result<A, A>(in_place_error, 5)) != (result<B, B>(in_place_error, 6)));
	}

	SECTION("valid and invalid results are not equal") {
		REQUIRE((result<A, A>(in_place_valid, 5)) != (result<A, A>(in_place_error, 5)));
		REQUIRE((result<A, A>(in_place_valid, 5)) != (result<A, B>(in_place_error, 5)));
		REQUIRE((result<A, A>(in_place_valid, 5)) != (result<B, A>(in_place_error, 5)));
		REQUIRE((result<A, A>(in_place_valid, 5)) != (result<B, B>(in_place_error, 5)));
	}

	SECTION("compare with raw values") {
		REQUIRE((result<int, A>(in_place_valid, 5)) == 5);
		REQUIRE((result<int, A>(in_place_valid, 5)) != 6);
		REQUIRE((result<int, A>(in_place_error, 5)) != 5);

		REQUIRE((result<A, int>(in_place_error, 5)) == 5);
		REQUIRE((result<A, int>(in_place_error, 5)) != 6);
		REQUIRE((result<A, int>(in_place_valid, 5)) != 5);
	}

	SECTION("compare with raw comparible") {
		REQUIRE((result<char, A>(in_place_valid, 5)) == 5);
		REQUIRE((result<char, A>(in_place_valid, 5)) != 6);
		REQUIRE((result<char, A>(in_place_error, 5)) != 5);

		REQUIRE((result<A, char>(in_place_error, 5)) == 5);
		REQUIRE((result<A, char>(in_place_error, 5)) != 6);
		REQUIRE((result<A, char>(in_place_valid, 5)) != 5);
	}
}

}
