#pragma once
#include <type_traits>
#include <utility>

namespace estd {

namespace detail {
	template<typename T>
	struct is_integer_sequence : std::false_type{};

	template<typename T, T... values>
	struct is_integer_sequence<std::integer_sequence<T, values...>> : std::true_type{};
}

/// Check wheter a type is a std::integer_sequence after decaying.
template<typename T>
constexpr bool is_integer_sequence = detail::is_integer_sequence<std::decay_t<T>>::value;

}
