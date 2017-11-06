#include "traits/replace_type.hpp"

#include "../static_assert_same.hpp"

namespace estd {
void test() {

static_assert_same<int, replace_type<bool, double, int>>();
static_assert_same<int, replace_type<bool, int, bool>>();
static_assert_same<int, replace_type<bool, int, int>>();

}}
