#include "result.hpp"
#include "tracker.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

namespace estd {

struct Error {
	int code;

	Error() : code{0} {};
	explicit Error(int code) : code(code) {};
	explicit operator int() { return code; }

	bool operator== (Error const & other) const { return code == other.code; }
};

TEST_CASE("valid result observers", "[result]") {
	result<int, Error> valid{in_place_valid, 1};
	auto make_exception = [] (Error const &) { return 5; };

	SECTION("operator bool") {
		REQUIRE(valid);
	}

	SECTION("value observers") {
		REQUIRE(valid.value() == 1);
		REQUIRE(*valid == 1);
		REQUIRE(valid.value(make_exception) == 1);
		REQUIRE(valid.value_or(4) == 1);

		valid.value() = 2;
		REQUIRE(valid.value() == 2);
	}

	SECTION("error observers") {
		REQUIRE_THROWS_AS(valid.error(), std::logic_error);
		REQUIRE_NOTHROW(valid.error_unchecked());
		REQUIRE(valid.error_or(Error{7}) == Error{7});
		REQUIRE(valid.error_or() == Error{0});
	}

	SECTION("operator ->") {
		struct Foo {
			int bar() const  { return 5; }
		};
		result<Foo, Error> valid{in_place_valid};

		REQUIRE(valid->bar() == 5);
	}
}

TEST_CASE("invalid result observers", "[result]") {
	result<int, Error> valid{in_place_error, 1};
	auto make_exception = [] (Error const &) { return 5; };

	SECTION("operator bool") {
		REQUIRE(!valid);
	}

	SECTION("value observers") {
		REQUIRE_THROWS_AS(valid.value(), Error);
		REQUIRE_NOTHROW(*valid);
		REQUIRE_THROWS_AS(valid.value(make_exception), int);
		REQUIRE(valid.value_or(4) == 4);
	}

	SECTION("error observers") {
		REQUIRE(valid.error() == Error{1});
		REQUIRE(valid.error_or(Error{7}) == Error{1});
		REQUIRE(valid.error_or() == Error{1});

		valid.error() = Error{2};
		REQUIRE(valid.error() == Error{2});
	}
}

TEST_CASE("valid result<void> observers", "[result]") {
	result<void, Error> valid{in_place_valid};
	auto make_exception = [] (Error const &) { return 5; };

	SECTION("operator bool") {
		REQUIRE(valid);
	}

	SECTION("value observers") {
		REQUIRE_NOTHROW(valid.value());
		REQUIRE_NOTHROW(*valid);
		REQUIRE_NOTHROW(valid.value(make_exception));
	}

	SECTION("error observers") {
		REQUIRE_THROWS_AS(valid.error(), std::logic_error);
		REQUIRE(valid.error_or(Error{7}) == Error{7});
		REQUIRE(valid.error_or() == Error{0});
	}
}

TEST_CASE("invalid result<void> observers", "[result]") {
	result<void, Error> valid{in_place_error, 1};
	auto make_exception = [] (Error const &) { return 5; };

	SECTION("operator bool") {
		REQUIRE(!valid);
	}

	SECTION("value observers") {
		REQUIRE_THROWS_AS(valid.value(),               Error);
		REQUIRE_THROWS_AS(*valid,                      Error);
		REQUIRE_THROWS_AS(valid.value(make_exception), int);
	}

	SECTION("error observers") {
		REQUIRE(valid.error() == Error{1});
		REQUIRE(valid.error_or(Error{7}) == Error{1});
		REQUIRE(valid.error_or() == Error{1});
		valid.error() = Error{2};
		REQUIRE(valid.error() == Error{2});
	}
}

}
