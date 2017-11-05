#include "traits/is_tuple.hpp"

#include <tuple>

namespace estd {
	static_assert(not is_tuple<int>);
	static_assert(is_tuple<std::tuple<>>);
	static_assert(is_tuple<std::tuple<int>>);
	static_assert(is_tuple<std::tuple<bool>>);
	static_assert(is_tuple<std::tuple<int, bool>>);
	static_assert(is_tuple<std::tuple<int, bool, double>>);
}
