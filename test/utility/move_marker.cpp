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

#include "utility/move_marker.hpp"

# if __has_include(<catch2/catch_test_macros.hpp>)
#   include <catch2/catch_test_macros.hpp>
# else
#   include <catch2/catch.hpp>
# endif

namespace estd {

TEST_CASE("newly constructed move_marker reports false", "[utility]") {
	move_marker marker;
	REQUIRE(bool(marker) == false);
}

TEST_CASE("a moved from move_marker reports true", "[utility]") {
	SECTION("when using the move constructor") {
		move_marker original;
		move_marker moved_to{std::move(original)};

		REQUIRE(bool(original) == true);
		REQUIRE(bool(moved_to) == false);
	}
	SECTION("when using move assignment") {
		move_marker original;
		move_marker moved_to = std::move(original);

		REQUIRE(bool(original) == true);
		REQUIRE(bool(moved_to) == false);
	}
}

TEST_CASE("a move_marker that is moved back in to inherits the value again", "[utility]") {
	SECTION("when moving in a non-moved marker") {
		move_marker original;
		move_marker moved_to{std::move(original)};
		original = std::move(moved_to);

		REQUIRE(bool(original) == false);
		REQUIRE(bool(moved_to) == true);
	}
	SECTION("when moving in a moved marker") {
		move_marker original;
		move_marker moved_to{std::move(original)};
		move_marker moved_again{std::move(original)};
		original = std::move(moved_again);

		REQUIRE(bool(original)    == true);
		REQUIRE(bool(moved_to)    == false);
		REQUIRE(bool(moved_again) == true);
	}
}
TEST_CASE("moving a marker into itself does nothing", "[utility]") {
	move_marker original;
	move_marker & same = original;
	original = std::move(same);

	REQUIRE(bool(original) == false);
	REQUIRE(bool(same)     == false);
}

TEST_CASE("a copied from move_marker inherits the value", "[utility]") {
	move_marker moved;
	move_marker not_moved                  = std::move(moved);
	move_marker copy_constructed_moved     {moved};
	move_marker copy_constructed_not_moved {not_moved};
	move_marker copy_assigned_moved        = moved;
	move_marker copy_assigned_not_moved    = not_moved;

	REQUIRE(bool(moved)                      == true);
	REQUIRE(bool(copy_constructed_moved)     == true);
	REQUIRE(bool(copy_assigned_moved)        == true);
	REQUIRE(bool(not_moved)                  == false);
	REQUIRE(bool(copy_constructed_not_moved) == false);
	REQUIRE(bool(copy_assigned_not_moved)    == false);
}

}
