#include "utility/make_ref.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

namespace estd {

TEST_CASE("make ref", "[utility]") {
	int a = 5;
	int const b = 6;
	REQUIRE(&make_ref(a) == &a);
	REQUIRE(&make_ref(b) == &b);
	REQUIRE( make_ref(1) == 1);
	static_assert_same<decltype(make_ref(a)), int &>();
	static_assert_same<decltype(make_ref(b)), int const &>();
	static_assert_same<decltype(make_ref(1)), int &&>();
	static_assert_same<decltype(make_ref(static_cast<int const &&>(1))), int const &&>();
}

TEST_CASE("make cref", "[utility]") {
	int a = 5;
	int const b = 6;
	REQUIRE(&make_cref(a) == &a);
	REQUIRE(&make_cref(b) == &b);
	REQUIRE( make_cref(1) == 1);
	static_assert_same<decltype(make_cref(a)), int const &>();
	static_assert_same<decltype(make_cref(b)), int const &>();
	static_assert_same<decltype(make_cref(1)), int const &&>();
	static_assert_same<decltype(make_cref(static_cast<int const>(1))), int const &&>();
}

}
