#include "tuple.hpp"
#include "utility/make_ref.hpp"

#include "../static_assert_same.hpp"

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

namespace estd {

static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<>>()), std::index_sequence<>>);
static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<int>>()), std::index_sequence<0>>);
static_assert(std::is_same_v<decltype(tuple_index_sequence<std::tuple<int, bool, double>>()), std::index_sequence<0, 1, 2>>);

TEST_CASE("tuple front/back", "[tuple]") {
	SECTION("correct element is returned") {
		REQUIRE(front(std::tuple(1, 2, 3)) == 1);
		REQUIRE(back(std::tuple(1, 2, 3)) == 3);
	}

	SECTION("correct reference type is returned") {
		std::tuple<int, int, int> tuple(1, 2, 3);
		std::tuple<int, int, int> const const_tuple(1, 2, 3);

		static_assert_same<decltype(front(tuple)), int &>();
		static_assert_same<decltype(front(const_tuple)), int const &>();
		static_assert_same<decltype(back(tuple)), int &>();
		static_assert_same<decltype(back(const_tuple)), int const &>();
		static_assert_same<decltype(front(std::tuple(1, 2, 3))), int &&>();
		static_assert_same<decltype(make_cref(front(std::tuple(1, 2, 3)))), int const &&>();
		static_assert_same<decltype(back(std::tuple(1, 2, 3))), int &&>();
		static_assert_same<decltype(make_cref(back(std::tuple(1, 2, 3)))), int const &&>();
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

	SECTION("Order may be non-incremental") {
		REQUIRE(select(std::tuple(1, 2, 3), std::index_sequence<2, 0, 1>()) == std::tuple(3, 1, 2));
	}

	SECTION("Indices may be repeated") {
		REQUIRE(select(std::tuple(1, 2, 3), std::index_sequence<2, 2, 1>()) == std::tuple(3, 3, 2));
	}

	SECTION("references are preserved") {
		int a = 0;
		int b = 10;
		auto sliced = select(std::tie(a, b), std::index_sequence<0, 1>());
		static_assert_same<decltype(sliced), std::tuple<int &, int &>>();
		REQUIRE(&std::get<0>(sliced) == &a);
		REQUIRE(&std::get<1>(sliced) == &b);
	}

	SECTION("references are not added automatically") {
		std::tuple tuple(1, 2, 3);
		static_assert_same<decltype(select(tuple,            std::index_sequence<0>())), std::tuple<int>>();
		static_assert_same<decltype(select(make_cref(tuple), std::index_sequence<0>())), std::tuple<int>>();
		static_assert_same<decltype(select(std::move(tuple), std::index_sequence<0>())), std::tuple<int>>();
		static_assert_same<decltype(select(std::tuple(1),    std::index_sequence<0>())), std::tuple<int>>();
	}
}

TEST_CASE("tuple select_ref", "[tuple]") {
	SECTION("correct indices are copied") {
		REQUIRE(select_ref(std::tuple(1, 2, 3), std::index_sequence<0, 1, 2>()) == std::tuple(1, 2, 3));
		REQUIRE(select_ref(std::tuple(1, 2, 3), std::index_sequence<0, 1   >()) == std::tuple(1, 2   ));
		REQUIRE(select_ref(std::tuple(1, 2, 3), std::index_sequence<0,    2>()) == std::tuple(1,    3));
		REQUIRE(select_ref(std::tuple(1, 2, 3), std::index_sequence<   1, 2>()) == std::tuple(   2, 3));
		REQUIRE(select_ref(std::tuple(1, 2, 3), std::index_sequence<0      >()) == std::tuple(1      ));
		REQUIRE(select_ref(std::tuple(1, 2, 3), std::index_sequence<   1   >()) == std::tuple(   2   ));
		REQUIRE(select_ref(std::tuple(1, 2, 3), std::index_sequence<      2>()) == std::tuple(      3));
		REQUIRE(select_ref(std::tuple(1, 2, 3), std::index_sequence<       >()) == std::tuple<>());
	}

	SECTION("Order may be non-incremental") {
		REQUIRE(select_ref(std::tuple(1, 2, 3), std::index_sequence<2, 0, 1>()) == std::tuple(3, 1, 2));
	}

	SECTION("Indices may be repeated") {
		REQUIRE(select_ref(std::tuple(1, 2, 3), std::index_sequence<2, 2, 1>()) == std::tuple(3, 3, 2));
	}

	SECTION("returned elements are references") {
		std::tuple tuple(1, 2, 3);
		auto selected = select_ref(tuple, std::index_sequence<0, 1, 2>());
		REQUIRE(&std::get<0>(selected) == &std::get<0>(tuple));
		REQUIRE(&std::get<1>(selected) == &std::get<1>(tuple));
		REQUIRE(&std::get<2>(selected) == &std::get<2>(tuple));

		static_assert_same<decltype(select_ref(tuple,            std::index_sequence<0>())), std::tuple<int       &>>();
		static_assert_same<decltype(select_ref(make_cref(tuple), std::index_sequence<0>())), std::tuple<int const &>>();

		static_assert_same<decltype(select_ref(std::move(tuple), std::index_sequence<0>())), std::tuple<int &&>>();
		static_assert_same<decltype(select_ref(std::tuple(1),    std::index_sequence<0>())), std::tuple<int &&>>();
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

TEST_CASE("tuple slice_ref", "[tuple]") {
	SECTION("correct indices are copied") {
		REQUIRE(slice_ref<0>(std::tuple(1, 2, 3)) == std::tuple(1, 2, 3));
		REQUIRE((slice_ref<0, std::numeric_limits<long int>::max()>(std::tuple(1, 2, 3)) == std::tuple(1, 2, 3)));

		REQUIRE((slice_ref< 0    >(std::tuple(0, 1, 2, 3, 4)) == std::tuple(0, 1, 2, 3, 4)));
		REQUIRE((slice_ref< 0, -1>(std::tuple(0, 1, 2, 3, 4)) == std::tuple(0, 1, 2, 3   )));
		REQUIRE((slice_ref< 1, -1>(std::tuple(0, 1, 2, 3, 4)) == std::tuple(   1, 2, 3   )));
		REQUIRE((slice_ref<-1    >(std::tuple(0, 1, 2, 3, 4)) == std::tuple(            4)));
		REQUIRE((slice_ref<-2, -1>(std::tuple(0, 1, 2, 3, 4)) == std::tuple(         3   )));
		REQUIRE((slice_ref<-1, -1>(std::tuple(0, 1, 2, 3, 4)) == std::tuple<>()));
	}

	SECTION("references are added to everything") {
		std::tuple<int> tuple(1);
		std::tuple<int> const const_tuple(1);
		static_assert_same<decltype(slice_ref<0>(tuple)),                    std::tuple<int &>>();
		static_assert_same<decltype(slice_ref<0>(std::move(tuple))),         std::tuple<int &&>>();
		static_assert_same<decltype(slice_ref<0>(const_tuple)),              std::tuple<int const &>>();
		static_assert_same<decltype(slice_ref<0>(std::tuple(1))),            std::tuple<int &&>>();

		REQUIRE(&std::get<0>(slice_ref<0>(tuple))       == &std::get<0>(tuple));
		REQUIRE(&std::get<0>(slice_ref<0>(const_tuple)) == &std::get<0>(const_tuple));
	}
}

}
