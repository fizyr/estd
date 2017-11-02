#include "result.hpp"
#include "tracker.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

namespace estd {

struct Error {
	int code;

	Error() {};
	explicit Error(int code) : code(code) {};
	explicit operator int() { return code; }
};


TEST_CASE("construct results", "[result]") {
	result<int, Error> valid{in_place_valid, 1};
	result<int, Error> error{in_place_error, 3};

	SECTION("a valid result evaluates as true and stores the right value") {
		REQUIRE(valid);
		REQUIRE(valid.value() == 1);
	}

	SECTION("an invalid value evaluates as false and stores the right error") {
		REQUIRE(!error);
		REQUIRE(error.error().code == 3);
	}

	SECTION("copy construct a result") {
		result<int, Error> copied_valid{valid};
		result<int, Error> copied_error{error};

		REQUIRE(copied_valid);
		REQUIRE(!copied_error);
		REQUIRE(copied_valid.value() == 1);
		REQUIRE(copied_error.error().code == 3);
	}

	SECTION("copy assign result to one of the same type (valid/invalid)") {
		result<int, Error> copied_valid{in_place_valid}; copied_valid = valid;
		result<int, Error> copied_error{in_place_error}; copied_error = error;

		REQUIRE(copied_valid);
		REQUIRE(!copied_error);
		REQUIRE(copied_valid.value() == 1);
		REQUIRE(copied_error.error().code == 3);
	}

	SECTION("copy assign result to one of the other type (valid/invalid)") {
		result<int, Error> copied_valid{in_place_error}; copied_valid = valid;
		result<int, Error> copied_error{in_place_valid}; copied_error = error;

		REQUIRE(copied_valid);
		REQUIRE(!copied_error);
		REQUIRE(copied_valid.value() == 1);
		REQUIRE(copied_error.error().code == 3);
	}

	SECTION("move construct a result") {
		result<int, Error> moved_valid{std::move(valid)};
		result<int, Error> moved_error{std::move(error)};

		REQUIRE(moved_valid);
		REQUIRE(!moved_error);
		REQUIRE(moved_valid.value() == 1);
		REQUIRE(moved_error.error().code == 3);
	}

	SECTION("move assign result to one of the same type (valid/invalid)") {
		result<int, Error> moved_valid{in_place_valid}; moved_valid = std::move(valid);
		result<int, Error> moved_error{in_place_error}; moved_error = std::move(error);

		REQUIRE(moved_valid);
		REQUIRE(!moved_error);
		REQUIRE(moved_valid.value() == 1);
		REQUIRE(moved_error.error().code == 3);
	}

	SECTION("move assign result to one of the other type (valid/invalid)") {
		result<int, Error> moved_valid{in_place_error}; moved_valid = std::move(valid);
		result<int, Error> moved_error{in_place_valid}; moved_error = std::move(error);

		REQUIRE(moved_valid);
		REQUIRE(!moved_error);
		REQUIRE(moved_valid.value() == 1);
		REQUIRE(moved_error.error().code == 3);
	}
}

TEST_CASE("history of results", "[result]") {
	using Result = result<Tracker, Tracker>;
	Result valid{in_place_valid};
	Result error{in_place_error};

	SECTION("default construct a result") {
		REQUIRE(valid);
		REQUIRE(!error);

		REQUIRE(valid.value().history == History{Event::default_constructed});
		REQUIRE(error.error().history == History{Event::default_constructed});
	}

	SECTION("copy construct a result") {
		Result copied_valid{valid};
		Result copied_error{error};

		REQUIRE(valid.value().history == (History{Event::default_constructed}));
		REQUIRE(error.error().history == (History{Event::default_constructed}));

		REQUIRE(copied_valid);
		REQUIRE(!copied_error);
		REQUIRE(copied_valid.value().history == (History{Event::copy_constructed}));
		REQUIRE(copied_error.error().history == (History{Event::copy_constructed}));
	}

	SECTION("copy assign result to one of the same type (valid/invalid)") {
		Result copied_valid{in_place_valid}; copied_valid = valid;
		Result copied_error{in_place_error}; copied_error = error;

		REQUIRE(valid.value().history == (History{Event::default_constructed}));
		REQUIRE(error.error().history == (History{Event::default_constructed}));

		REQUIRE(copied_valid);
		REQUIRE(!copied_error);
		REQUIRE(copied_valid.value().history == (History{Event::default_constructed, Event::copy_assigned_to}));
		REQUIRE(copied_error.error().history == (History{Event::default_constructed, Event::copy_assigned_to}));
	}

	SECTION("copy assign result to one of the other type (valid/invalid)") {
		Result copied_valid{in_place_error}; copied_valid = valid;
		Result copied_error{in_place_valid}; copied_error = error;

		REQUIRE(valid.value().history == (History{Event::default_constructed}));
		REQUIRE(error.error().history == (History{Event::default_constructed}));

		// When assinging an invalid to a valid result (or vice-versa), we can't directly assign but must destroy and construct.
		REQUIRE(copied_valid);
		REQUIRE(!copied_error);
		REQUIRE(copied_valid.value().history == (History{Event::copy_constructed}));
		REQUIRE(copied_error.error().history == (History{Event::copy_constructed}));
	}

	SECTION("move construct a result") {
		Result moved_valid{std::move(valid)};
		Result moved_error{std::move(error)};

		REQUIRE(valid.value().history == (History{Event::default_constructed, Event::moved_from}));
		REQUIRE(error.error().history == (History{Event::default_constructed, Event::moved_from}));

		REQUIRE(moved_valid);
		REQUIRE(!moved_error);
		REQUIRE(moved_valid.value().history == (History{Event::move_constructed}));
		REQUIRE(moved_error.error().history == (History{Event::move_constructed}));
	}

	SECTION("move assign result to one of the same type (valid/invalid)") {
		Result moved_valid{in_place_valid}; moved_valid = std::move(valid);
		Result moved_error{in_place_error}; moved_error = std::move(error);

		REQUIRE(valid.value().history == (History{Event::default_constructed, Event::moved_from}));
		REQUIRE(error.error().history == (History{Event::default_constructed, Event::moved_from}));

		REQUIRE(moved_valid);
		REQUIRE(!moved_error);
		REQUIRE(moved_valid.value().history == (History{Event::default_constructed, Event::move_assigned_to}));
		REQUIRE(moved_error.error().history == (History{Event::default_constructed, Event::move_assigned_to}));
	}

	SECTION("move assign result to one of the other type (valid/invalid)") {
		Result moved_valid{in_place_error}; moved_valid = std::move(valid);
		Result moved_error{in_place_valid}; moved_error = std::move(error);

		REQUIRE(valid.value().history == (History{Event::default_constructed, Event::moved_from}));
		REQUIRE(error.error().history == (History{Event::default_constructed, Event::moved_from}));

		// When assinging an invalid to a valid result (or vice-versa), we can't directly assign but must destroy and construct.
		REQUIRE(moved_valid);
		REQUIRE(!moved_error);
		REQUIRE(moved_valid.value().history == (History{Event::move_constructed}));
		REQUIRE(moved_error.error().history == (History{Event::move_constructed}));
	}
}

}
