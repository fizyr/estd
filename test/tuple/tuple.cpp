#include "tuple.hpp"

#include "../static_assert_same.hpp"

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

TEST_CASE("tuple select", "[tuple]") {
	SECTION("correct indices are copied") {
		REQUIRE(select(std::tuple(1, 2, 3), std::index_sequence<0, 1, 2>()) == std::tuple(1, 2, 3));
		REQUIRE(select(std::tuple(1, 2, 3), std::index_sequence<0, 1   >()) == std::tuple(1, 2   ));
		REQUIRE(select(std::tuple(1, 2, 3), std::index_sequence<0,    2>()) == std::tuple(1,    3));
		REQUIRE(select(std::tuple(1, 2, 3), std::index_sequence<   1, 2>()) == std::tuple(   2, 3));
		REQUIRE(select(std::tuple(1, 2, 3), std::index_sequence<0      >()) == std::tuple(1      ));
		REQUIRE(select(std::tuple(1, 2, 3), std::index_sequence<   1   >()) == std::tuple(   2   ));
		REQUIRE(select(std::tuple(1, 2, 3), std::index_sequence<      2>()) == std::tuple(      3));
		REQUIRE(select(std::tuple(1, 2, 3), std::index_sequence<       >()) == std::tuple<>());
	}

	SECTION("references are preserved") {
		int a = 0;
		int b = 10;
		auto sliced = select(std::tie(a, b), std::index_sequence<0, 1>());
		static_assert_same<decltype(sliced), std::tuple<int &, int &>>();
		REQUIRE(&std::get<0>(sliced) == &a);
		REQUIRE(&std::get<1>(sliced) == &b);
	}
}

TEST_CASE("tuple remove front", "[tuple]") {
	SECTION("correct indices are copied") {
		REQUIRE(remove_front<0>(std::tuple(1, 2, 3)) == std::tuple(1, 2, 3));
		REQUIRE(remove_front<1>(std::tuple(1, 2, 3)) == std::tuple(   2, 3));
		REQUIRE(remove_front<2>(std::tuple(1, 2, 3)) == std::tuple(      3));
	}

	SECTION("references are preserved") {
		int a = 0;
		int b = 10;
		auto sliced = remove_front<0>(std::tie(a, b));
		static_assert_same<decltype(sliced), std::tuple<int &, int &>>();
		REQUIRE(&std::get<0>(sliced) == &a);
		REQUIRE(&std::get<1>(sliced) == &b);
	}
}

TEST_CASE("tuple remove back", "[tuple]") {
	SECTION("correct indices are copied") {
		REQUIRE(remove_back<0>(std::tuple(1, 2, 3)) == std::tuple(1, 2, 3));
		REQUIRE(remove_back<1>(std::tuple(1, 2, 3)) == std::tuple(1, 2   ));
		REQUIRE(remove_back<2>(std::tuple(1, 2, 3)) == std::tuple(1      ));
	}

	SECTION("references are preserved") {
		int a = 0;
		int b = 10;
		auto sliced = remove_back<0>(std::tie(a, b));
		static_assert_same<decltype(sliced), std::tuple<int &, int &>>();
		REQUIRE(&std::get<0>(sliced) == &a);
		REQUIRE(&std::get<1>(sliced) == &b);
	}
}

TEST_CASE("tuple slice", "[tuple]") {
	SECTION("correct indices are copied") {
		REQUIRE(slice<0>(std::tuple(1, 2, 3)) == std::tuple(1, 2, 3));
		REQUIRE((slice<0, std::numeric_limits<long int>::max()>(std::tuple(1, 2, 3)) == std::tuple(1, 2, 3)));

		REQUIRE((slice< 0    >(std::tuple(0, 1, 2, 3, 4)) == std::tuple(0, 1, 2, 3, 4)));
		REQUIRE((slice< 0, -1>(std::tuple(0, 1, 2, 3, 4)) == std::tuple(0, 1, 2, 3   )));
		REQUIRE((slice< 1, -1>(std::tuple(0, 1, 2, 3, 4)) == std::tuple(   1, 2, 3   )));
		REQUIRE((slice<-1    >(std::tuple(0, 1, 2, 3, 4)) == std::tuple(            4)));
		REQUIRE((slice<-2, -1>(std::tuple(0, 1, 2, 3, 4)) == std::tuple(         3   )));
		REQUIRE((slice<-1, -1>(std::tuple(0, 1, 2, 3, 4)) == std::tuple<>()));
	}

	SECTION("references are preserved") {
		int a = 0;
		int b = 10;
		auto sliced = slice<0>(std::tie(a, b));
		static_assert_same<decltype(sliced), std::tuple<int &, int &>>();
		REQUIRE(&std::get<0>(sliced) == &a);
		REQUIRE(&std::get<1>(sliced) == &b);
	}
}

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

	SECTION("foldl is a left foldl") {
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

	SECTION("foldr is a right foldr") {
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
