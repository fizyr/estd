#include "tracker.hpp"

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include <utility>

namespace estd {

TEST_CASE("tracker show construction/assignment history", "[tracker]") {
	Tracker a;

	SECTION("default constructed tracker") {
		REQUIRE(a.history == (History{Event::default_constructed}));
	}

	SECTION("copy constructed tracker") {
		Tracker b{a};
		REQUIRE(a.history == (History{Event::default_constructed}));
		REQUIRE(b.history == (History{Event::copy_constructed}));
	}

	SECTION("copy assigned tracker") {
		Tracker b; b = a;
		REQUIRE(a.history == (History{Event::default_constructed}));
		REQUIRE(b.history == (History{Event::default_constructed, Event::copy_assigned_to}));
	}

	SECTION("move constructed tracker") {
		Tracker b{std::move(a)};
		REQUIRE(a.history == (History{Event::default_constructed, Event::moved_from}));
		REQUIRE(b.history == (History{Event::move_constructed}));
	}

	SECTION("move assigned tracker") {
		Tracker b; b = std::move(a);
		REQUIRE(a.history == (History{Event::default_constructed, Event::moved_from}));
		REQUIRE(b.history == (History{Event::default_constructed, Event::move_assigned_to}));
	}
}

}
