#include "result.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

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
}

}
