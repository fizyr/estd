#include "util/integer_sequence.hpp"

#include "../static_assert_same.hpp"

#include <type_traits>

namespace estd {

void tests() {

static_assert_same<decltype(concat(std::integer_sequence<int, 1, 2, 3>{}, std::integer_sequence<int, 4>{})), std::integer_sequence<int, 1, 2, 3, 4>>();
static_assert_same<decltype(concat(std::integer_sequence<int, 1, 2, 3>{}, std::integer_sequence<int>{})),    std::integer_sequence<int, 1, 2, 3>>();

static_assert_same<decltype(concat(
		std::integer_sequence<int, 1>{},
		std::integer_sequence<int, 2>{},
		std::integer_sequence<int, 3>{}
)), std::integer_sequence<int, 1, 2, 3>>();

static_assert_same<decltype(concat(
		std::integer_sequence<int, 1, 2>{},
		std::integer_sequence<int>{},
		std::integer_sequence<int, 3, 4>{},
		std::integer_sequence<int, 5>{},
		std::integer_sequence<int>{}
)), std::integer_sequence<int, 1, 2, 3, 4, 5>>();

static_assert_same<decltype(remove_front<0>(std::integer_sequence<int, 1, 2, 3>{})), std::integer_sequence<int, 1, 2, 3>>();
static_assert_same<decltype(remove_front<1>(std::integer_sequence<int, 1, 2, 3>{})), std::integer_sequence<int,    2, 3>>();
static_assert_same<decltype(remove_front<2>(std::integer_sequence<int, 1, 2, 3>{})), std::integer_sequence<int,       3>>();
static_assert_same<decltype(remove_front<3>(std::integer_sequence<int, 1, 2, 3>{})), std::integer_sequence<int         >>();

static_assert_same<decltype(remove_back<0>(std::integer_sequence<int, 1, 2, 3>{})), std::integer_sequence<int, 1, 2, 3>>();
static_assert_same<decltype(remove_back<1>(std::integer_sequence<int, 1, 2, 3>{})), std::integer_sequence<int, 1, 2   >>();
static_assert_same<decltype(remove_back<2>(std::integer_sequence<int, 1, 2, 3>{})), std::integer_sequence<int, 1      >>();
static_assert_same<decltype(remove_back<3>(std::integer_sequence<int, 1, 2, 3>{})), std::integer_sequence<int         >>();

static_assert_same<decltype(slice< 0    >(std::integer_sequence<int, 0, 1, 2, 3, 4>{})), std::integer_sequence<int, 0, 1, 2, 3, 4>>();
static_assert_same<decltype(slice< 0,  5>(std::integer_sequence<int, 0, 1, 2, 3, 4>{})), std::integer_sequence<int, 0, 1, 2, 3, 4>>();
static_assert_same<decltype(slice< 1, -1>(std::integer_sequence<int, 0, 1, 2, 3, 4>{})), std::integer_sequence<int,    1, 2, 3   >>();
static_assert_same<decltype(slice<-1    >(std::integer_sequence<int, 0, 1, 2, 3, 4>{})), std::integer_sequence<int,             4>>();
static_assert_same<decltype(slice<-1, -1>(std::integer_sequence<int, 0, 1, 2, 3, 4>{})), std::integer_sequence<int               >>();
static_assert_same<decltype(slice<-2, -1>(std::integer_sequence<int, 0, 1, 2, 3, 4>{})), std::integer_sequence<int,          3   >>();
static_assert_same<decltype(slice< 2    >(std::integer_sequence<int, 0, 1, 2, 3, 4>{})), std::integer_sequence<int,       2, 3, 4>>();
static_assert_same<decltype(slice< 3    >(std::integer_sequence<int, 0, 1, 2, 3, 4>{})), std::integer_sequence<int,          3, 4>>();
static_assert_same<decltype(slice< 5    >(std::integer_sequence<int, 0, 1, 2, 3, 4>{})), std::integer_sequence<int               >>();
static_assert_same<decltype(slice< 5,  5>(std::integer_sequence<int, 0, 1, 2, 3, 4>{})), std::integer_sequence<int               >>();

}

}
