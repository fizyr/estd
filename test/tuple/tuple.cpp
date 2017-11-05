#include "tuple.hpp"

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

namespace estd {

static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<>>()), std::index_sequence<>>);
static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<int>>()), std::index_sequence<0>>);
static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<int, bool, double>>()), std::index_sequence<0, 1, 2>>);

TEST_CASE("tuple transform", "[tuple]") {
	SECTION("add one to each tuple element") {
		auto add_one = [] (auto a) { return a + 1; };
		REQUIRE(tuple_transform(std::tuple(1, 10),   add_one) == std::tuple(2, 11));
		REQUIRE(tuple_transform(std::tuple(1, 10.0), add_one) == std::tuple(2, 11.0));
	}

	SECTION("modify tuple elements in-place") {
		auto add_one = [] (auto && a) { a += 1; };
		int a = 0;
		int b = 10;
		tuple_transform(std::tie(a, b), add_one);
		REQUIRE(a == 1);
		REQUIRE(b == 11);
	}

	SECTION("tuple_transform decays results") {
		auto identity = [] (auto && a) { return a; };
		static_assert(std::is_same_v<decltype(tuple_transform(std::make_tuple(1, 2), identity)), std::tuple<int , int >>);
	}

	SECTION("tuple_transform_raw preserves references in results") {
		auto identity = [] (auto && a) -> decltype(a) & { return a; };
		static_assert(std::is_same_v<decltype(tuple_transform_raw(std::make_tuple(1, 2), identity)), std::tuple<int &, int &>>);
	}

	SECTION("void results are mapped to empty_t") {
		auto drop_int_add_one = [] (auto a) {
			if constexpr(std::is_same_v<decltype(a), int>) return;
			else return a + 1;
		};

		static_assert(std::is_same_v<decltype(tuple_transform    (std::make_tuple(1, 2.0), drop_int_add_one)), std::tuple<empty_t, double>>);
		static_assert(std::is_same_v<decltype(tuple_transform_raw(std::make_tuple(1, 2.0), drop_int_add_one)), std::tuple<empty_t, double>>);

		REQUIRE(tuple_transform    (std::make_tuple(1, 2.0), drop_int_add_one) == std::tuple(empty, 3.0));
		REQUIRE(tuple_transform_raw(std::make_tuple(3.0, 8), drop_int_add_one) == std::tuple(4.0, empty));
	}

	SECTION("when all elements are mapped to void, transform returns void itself") {
		auto drop = [] (auto) {};
		static_assert(std::is_same_v<decltype(tuple_transform    (std::make_tuple(1, 2.0), drop)), void>);
		static_assert(std::is_same_v<decltype(tuple_transform_raw(std::make_tuple(1, 2.0), drop)), void>);
	}

	SECTION("only transform_raw can dereference pointers to references") {
		auto deref = [] (auto a) -> decltype(*a) & { return *a; };
		int a = 0;
		int b = 1;
		static_assert(std::is_same_v<decltype(tuple_transform    (std::tuple(&a, &b), deref)), std::tuple<int  , int  >>);
		static_assert(std::is_same_v<decltype(tuple_transform_raw(std::tuple(&a, &b), deref)), std::tuple<int &, int &>>);

		auto dereferenced_raw = tuple_transform_raw(std::tuple(&a, &b), deref);
		REQUIRE(&std::get<0>(dereferenced_raw) == &a);
		REQUIRE(&std::get<1>(dereferenced_raw) == &b);

		auto dereferenced = tuple_transform(std::tuple(&a, &b), deref);
		REQUIRE(&std::get<0>(dereferenced) != &a);
		REQUIRE(&std::get<1>(dereferenced) != &b);
	}
}

}
