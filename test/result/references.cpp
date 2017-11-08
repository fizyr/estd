#include "result.hpp"
#include "tracker.hpp"

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

namespace estd {

struct Error {
	int code;

	Error() {};
	explicit Error(int code) : code(code) {};
	explicit operator int() { return code; }
};


TEST_CASE("results can hold references", "[result]") {
	int a = 0;
	result<int &, Error> valid{in_place_valid, a};
	result<int &&, Error> k{in_place_valid, 5};

	SECTION("reference refers to the same value") {
		REQUIRE(valid);
		REQUIRE(&valid.value() == &a);
		REQUIRE(&*valid == &a);
	}

	SECTION("copy construct") {
		result<int &, Error> copied_valid{valid};

		REQUIRE(copied_valid);
		REQUIRE(&copied_valid.value() == &a);
		REQUIRE(&*copied_valid == &a);
	}

	SECTION("copy assign") {
		int b = 6;
		result<int &, Error> copied_valid{in_place_valid, b};
		copied_valid = valid;

		REQUIRE(copied_valid);
		REQUIRE(&copied_valid.value() == &a);
		REQUIRE(&*copied_valid == &a);
	}

	SECTION("copy assign to previously invalid") {
		result<int &, Error> copied_valid{in_place_error};
		copied_valid = valid;

		REQUIRE(copied_valid);
		REQUIRE(&copied_valid.value() == &a);
		REQUIRE(&*copied_valid == &a);
	}

	SECTION("move construct") {
		result<int &, Error> moved_valid{std::move(valid)};

		REQUIRE(moved_valid);
		REQUIRE(&moved_valid.value() == &a);
		REQUIRE(&*moved_valid == &a);

	}

	SECTION("move assign") {
		int b = 6;
		result<int &, Error> moved_valid{in_place_valid, b};
		moved_valid = std::move(valid);

		REQUIRE(moved_valid);
		REQUIRE(&moved_valid.value() == &a);
		REQUIRE(&*moved_valid == &a);
	}

	SECTION("move assign to previously invalid") {
		result<int &, Error> moved_valid{in_place_error};
		moved_valid = std::move(valid);

		REQUIRE(moved_valid);
		REQUIRE(&moved_valid.value() == &a);
		REQUIRE(&*moved_valid == &a);
	}
}

TEST_CASE("history of results", "[result]") {
	Tracker ok;
	Tracker error;
	using Result = result<Tracker&, Tracker&>;
	Result valid{in_place_valid, ok};
	Result invalid{in_place_error, error};

	SECTION("default construct a result") {
		REQUIRE(valid);
		REQUIRE(!invalid);

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("copy construct a result") {
		Result copied_valid{valid};
		Result copied_error{invalid};

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("copy assign result to one of the same type (valid/invalid)") {
		Tracker b;
		Result copied_valid{in_place_valid, b}; copied_valid = valid;
		Result copied_error{in_place_error, b}; copied_error = invalid;

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("copy assign result to one of the other type (valid/invalid)") {
		Tracker b;
		Result copied_valid{in_place_error, b}; copied_valid = valid;
		Result copied_error{in_place_valid, b}; copied_error = invalid;

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("move construct a result") {
		Result moved_valid{std::move(valid)};
		Result moved_error{std::move(invalid)};

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("move assign result to one of the same type (valid/invalid)") {
		Tracker b;
		Result moved_valid{in_place_valid, b}; moved_valid = std::move(valid);
		Result moved_error{in_place_error, b}; moved_error = std::move(invalid);

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("move assign result to one of the other type (valid/invalid)") {
		Tracker b;
		Result moved_valid{in_place_error, b}; moved_valid = std::move(valid);
		Result moved_error{in_place_valid, b}; moved_error = std::move(invalid);

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}
}

}
