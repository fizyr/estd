#include "tuple.hpp"

#include "../static_assert_same.hpp"

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

namespace estd {

static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<>>()), std::index_sequence<>>);
static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<int>>()), std::index_sequence<0>>);
static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<int, bool, double>>()), std::index_sequence<0, 1, 2>>);

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

}
