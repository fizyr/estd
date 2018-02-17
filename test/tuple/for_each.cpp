#include "tuple/for_each.hpp"

#include "../catch.hpp"

namespace estd {

TEST_CASE("tuple for_each", "[tuple]") {
	SECTION("processes each element if functor returns void") {
		int sum = 0;
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each(tuple, [&] (auto v) { sum += v; }) == 4);
		REQUIRE(sum == 10);
	}

	SECTION("processes each element if functor returns true") {
		int sum = 0;
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each(tuple, [&] (auto v) { sum += v; return true; }) == 4);
		REQUIRE(sum == 10);
	}

	SECTION("stops when functor returns false") {
		int sum = 0;
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each(tuple, [&] (auto v) { sum += v; return v != 3; }) == 2);
		REQUIRE(sum == 6);
	}

	SECTION("reports index if stopped") {
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each(tuple, [&] (auto v) { return v != 4; }) == 3);
	}

	SECTION("reports tuple size if not stopped") {
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each(tuple, [&] (auto) { return true; }) == 4);
	}
}

TEST_CASE("tuple for_each_i", "[tuple]") {
	SECTION("processes each element with the right index") {
		int sum = 0;
		auto tuple = std::make_tuple(0, 1, 2, 3);
		std::size_t index = for_each_i(tuple, [&] (int i, auto v) {
			REQUIRE(i == v);
			sum += v;
		});
		REQUIRE(index == 4);
		REQUIRE(sum   == 6);
	}

	SECTION("processes each element if functor returns true") {
		int sum = 0;
		auto tuple = std::make_tuple(0, 1, 2, 3);
		std::size_t index = for_each_i(tuple, [&] (int i, auto v) {
			REQUIRE(i == v);
			sum += v;
			return true;
		});
		REQUIRE(index == 4);
		REQUIRE(sum   == 6);
	}

	SECTION("stops when functor returns false") {
		int sum = 0;
		auto tuple = std::make_tuple(0, 1, 2, 3);
		std::size_t index = for_each_i(tuple, [&] (int i, auto v) {
			REQUIRE(i == v);
			sum += v;
			return i != 2;
		});
		REQUIRE(index == 2);
		REQUIRE(sum   == 3);
	}

	SECTION("reports index if stopped") {
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each_i(tuple, [&] (std::size_t i, auto) { return i != 3; }) == 3);
	}

	SECTION("reports tuple size if not stopped") {
		auto tuple = std::make_tuple(1, 2, 3, 4);
		REQUIRE(for_each_i(tuple, [&] (std::size_t, auto) { return true; }) == 4);
	}
}

}
