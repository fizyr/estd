#include "scope_guard.hpp"

#include "../catch.hpp"

#include <type_traits>

namespace estd {


TEST_CASE("scope_guards can be moved but not copied", "[scope_guard]") {
	REQUIRE(std::is_copy_constructible_v <scope_guard<void(*)()>> == false);
	REQUIRE(std::is_copy_assignable_v    <scope_guard<void(*)()>> == false);
	REQUIRE(std::is_move_constructible_v <scope_guard<void(*)()>> == true );
	REQUIRE(std::is_move_assignable_v    <scope_guard<void(*)()>> == true );
}

TEST_CASE("scope_guard triggers when running out of scope", "[scope_guard]") {
	int triggered = 0;

	{
		REQUIRE(triggered == 0);
		auto guard = scope_guard([&] { ++triggered; });
		REQUIRE(triggered == 0);
	}

	REQUIRE(triggered == 1);
}

TEST_CASE("resetting a scope_guard disables the trigger", "[scope_guard]") {
	int triggered = 0;

	{
		auto guard = scope_guard([&] { ++triggered; });
		guard.reset();
	}

	REQUIRE(triggered == 0);
}

TEST_CASE("moving a scope_guard triggers only once", "[scope_guard]") {
	int triggered = 0;

	{
		auto guard = scope_guard([&] { ++triggered; });
		auto other_guard = std::move(guard);
	}

	REQUIRE(triggered == 1);
}

TEST_CASE("resetting a moved scope_guard doesn't trigger", "[scope_guard]") {
	int triggered = 0;

	{
		auto guard = scope_guard([&] { ++triggered; });
		auto other_guard = std::move(guard);
		other_guard.reset();
	}

	REQUIRE(triggered == 0);
}

}
