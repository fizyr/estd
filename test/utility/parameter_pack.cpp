#include "utility/parameter_pack.hpp"

#include "../static_assert_same.hpp"

#include <type_traits>

namespace estd {

void test() {

static_assert_same<parameter_pack_head<bool>, bool>();
static_assert_same<parameter_pack_head<double, bool>, double>();
static_assert_same<parameter_pack_head<int, double, bool>, int>();

static_assert_same<parameter_pack_element<0, bool>, bool>();
static_assert_same<parameter_pack_element<0, int, double, bool>, int>();
static_assert_same<parameter_pack_element<1, int, double, bool>, double>();
static_assert_same<parameter_pack_element<2, int, double, bool>, bool>();

static_assert(value_pack_head<int, 3> == 3);
static_assert(value_pack_head<int, 2, 3> == 2);
static_assert(value_pack_head<int, 1, 2, 3> == 1);

static_assert(value_pack_element<0, int, 3> == 3);
static_assert(value_pack_element<0, int, 1, 2, 3> == 1);
static_assert(value_pack_element<1, int, 1, 2, 3> == 2);
static_assert(value_pack_element<2, int, 1, 2, 3> == 3);

}

}
