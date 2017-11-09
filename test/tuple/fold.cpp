#include "tuple/fold.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

namespace estd {

TEST_CASE("tuple left fold", "[tuple]") {
	auto sum = [] (auto a, auto b) { return a + b; };
	auto mul = [] (auto a, auto b) { return a * b; };
	auto drop_right = [] (auto a, auto) { return a; };
	auto drop_left  = [] (auto, auto b) { return b; };

	SECTION("foldl over empty tuple returns initial accumulator") {
		REQUIRE(foldl(std::tuple<>(), 5, sum) == 5);
	}

	SECTION("foldl without accumulator over single element tuple returns first element") {
		REQUIRE(foldl(std::tuple(5), mul) == 5);
	}

	SECTION("foldl without accumulator over non-empty tuple") {
		REQUIRE(foldl(std::tuple(1, 2, 3, 4), sum) == 1 + 2 + 3 + 4);
		REQUIRE(foldl(std::tuple(1, 2, 3, 4), mul) == 1 * 2 * 3 * 4);
	}

	SECTION("foldl with accumulator over non-empty tuple") {
		REQUIRE(foldl(std::tuple(1, 2, 3, 4), 10, sum) == 10 + 1 + 2 + 3 + 4);
		REQUIRE(foldl(std::tuple(1, 2, 3, 4), 10, mul) == 10 * 1 * 2 * 3 * 4);
	}

	SECTION("foldl over heterogenous tuple") {
		REQUIRE(foldl(std::tuple(1, 2.0, 3.f, 4), 10, sum) == 10.0 + 1 + 2 + 3 + 4);
		REQUIRE(foldl(std::tuple(1, 2.0, 3.f, 4), 10, mul) == 10.0 * 1 * 2 * 3 * 4);
		static_assert_same<decltype(foldl(std::tuple(1, 2.0, 3.f, 4), 10, sum)), double>();
		static_assert_same<decltype(foldl(std::tuple(1, 2.0, 3.f, 4), 10, mul)), double>();
	}

	SECTION("foldl is a left fold") {
		REQUIRE(foldl(std::tuple(1.0, 2, 3, 4.0), 10, drop_right) == 10);
		REQUIRE(foldl(std::tuple(1  , 2, 3, 4.0),     drop_right) ==  1);
		static_assert_same<decltype(foldl(std::tuple(1.0, 2, 3, 4.0), 10, drop_right)), int>();
		static_assert_same<decltype(foldl(std::tuple(1  , 2, 3, 4.0),     drop_right)), int>();

		REQUIRE(foldl(std::tuple(1, 2, 3, 4.0), 10, drop_left) == 4.0);
		REQUIRE(foldl(std::tuple(1, 2, 3, 4.0),     drop_left) == 4.0);
		static_assert_same<decltype(foldl(std::tuple(1, 2, 3, 4.0), 10, drop_left)), double>();
		static_assert_same<decltype(foldl(std::tuple(1, 2, 3, 4.0),     drop_left)), double>();
	}
}

TEST_CASE("tuple right fold", "[tuple]") {
	auto sum = [] (auto a, auto b) { return a + b; };
	auto mul = [] (auto a, auto b) { return a * b; };
	auto drop_right = [] (auto a, auto) { return a; };
	auto drop_left  = [] (auto, auto b) { return b; };

	SECTION("foldr over empty tuple returns initial accumulator") {
		REQUIRE(foldr(std::tuple<>(), 5, sum) == 5);
	}

	SECTION("foldr without accumulator over single element tuple returns first element") {
		REQUIRE(foldr(std::tuple(5), mul) == 5);
	}

	SECTION("foldr without accumulator over non-empty tuple") {
		REQUIRE(foldr(std::tuple(1, 2, 3, 4), sum) == 1 + 2 + 3 + 4);
		REQUIRE(foldr(std::tuple(1, 2, 3, 4), mul) == 1 * 2 * 3 * 4);
	}

	SECTION("foldr with accumulator over non-empty tuple") {
		REQUIRE(foldr(std::tuple(1, 2, 3, 4), 10, sum) == 10 + 1 + 2 + 3 + 4);
		REQUIRE(foldr(std::tuple(1, 2, 3, 4), 10, mul) == 10 * 1 * 2 * 3 * 4);
	}

	SECTION("foldr over heterogenous tuple") {
		REQUIRE(foldr(std::tuple(1, 2.0, 3.f, 4), 10, sum) == 10.0 + 1 + 2 + 3 + 4);
		REQUIRE(foldr(std::tuple(1, 2.0, 3.f, 4), 10, mul) == 10.0 * 1 * 2 * 3 * 4);
		static_assert_same<decltype(foldr(std::tuple(1, 2.0, 3.f, 4), 10, sum)), double>();
		static_assert_same<decltype(foldr(std::tuple(1, 2.0, 3.f, 4), 10, mul)), double>();
	}

	SECTION("foldr is a right fold") {
		REQUIRE(foldr(std::tuple(1, 2, 3, 4.0), 10.0, drop_right) == 1);
		REQUIRE(foldr(std::tuple(1, 2, 3, 4.0),       drop_right) == 1);
		static_assert_same<decltype(foldr(std::tuple(1, 2, 3, 4.0), 10.0, drop_right)), int>();
		static_assert_same<decltype(foldr(std::tuple(1, 2, 3, 4.0),       drop_right)), int>();

		REQUIRE(foldr(std::tuple(1, 2, 3, 4  ), 10.0, drop_left) == 10.0);
		REQUIRE(foldr(std::tuple(1, 2, 3, 4.0),       drop_left) ==  4.0);
		static_assert_same<decltype(foldr(std::tuple(1, 2, 3, 4  ), 10.0, drop_left)), double>();
		static_assert_same<decltype(foldr(std::tuple(1, 2, 3, 4.0),       drop_left)), double>();
	}
}

}
