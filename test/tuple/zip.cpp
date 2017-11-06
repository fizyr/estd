#include "tuple/zip.hpp"

#include "../static_assert_same.hpp"

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

namespace estd {

static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<>>()), std::index_sequence<>>);
static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<int>>()), std::index_sequence<0>>);
static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<int, bool, double>>()), std::index_sequence<0, 1, 2>>);

TEST_CASE("tuple zip", "[tuple]") {
	SECTION("zip two tuples") {
		auto zipped = zip(std::tuple(1, 2.0, 3), std::tuple(4.0, 5, 6.0));
		REQUIRE((zipped == std::tuple(std::tuple(1, 4.0), std::tuple(2.0, 5), std::tuple(3, 6.0))));
		static_assert_same<decltype(zipped), std::tuple<std::tuple<int, double>, std::tuple<double, int>, std::tuple<int, double>>>();
	}

	SECTION("zip three tuples") {
		auto zipped = zip(std::tuple(1, 2, 3), std::tuple(4, 5, 6), std::tuple(7, 8, 9));
		REQUIRE((zipped == std::tuple(
			std::tuple(1, 4, 7),
			std::tuple(2, 5, 8),
			std::tuple(3, 6, 9)
		)));
		static_assert_same<decltype(zipped), std::tuple<std::tuple<int, int, int>, std::tuple<int, int, int>, std::tuple<int, int, int>>>();
	}

	SECTION("references are preserved") {
		int a = 1;
		int b = 2;
		REQUIRE(zip(std::tie(a), std::tie(b)) == (std::tuple<std::tuple<int, int>>(std::tuple(1, 2))));
		REQUIRE(zip(std::tie(a, a), std::tie(b, b)) == std::tuple(std::tuple(1, 2), std::tuple(1, 2)));
		static_assert_same<decltype(zip(std::tie(a), std::tie(b))), std::tuple<std::tuple<int &, int &>>>();

		auto [tuple] = zip(std::tie(a), std::tie(b));
		REQUIRE(&std::get<0>(tuple) == &a);
		REQUIRE(&std::get<1>(tuple) == &b);
	}
}

}
