#include "utility/move_marker.hpp"

#include "../catch.hpp"

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
