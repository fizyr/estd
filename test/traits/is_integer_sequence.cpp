#include "traits/is_integer_sequence.hpp"

#include <utility>

namespace estd {
	static_assert(not is_integer_sequence<int>);
	static_assert(is_integer_sequence<std::integer_sequence<int>>);
	static_assert(is_integer_sequence<std::integer_sequence<int, 1>>);
	static_assert(is_integer_sequence<std::integer_sequence<int, 1, 2>>);
	static_assert(is_integer_sequence<std::index_sequence<>>);
	static_assert(is_integer_sequence<std::index_sequence<1>>);
	static_assert(is_integer_sequence<std::index_sequence<1, 2>>);
}
