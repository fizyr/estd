#pragma once
#include <tuple>
#include <type_traits>

namespace estd {

namespace detail {
	template<typename T>
	struct is_tuple_impl : std::false_type{};

	template<typename... T>
	struct is_tuple_impl<std::tuple<T...>> : std::true_type{};
}

/// Check wheter a type is a std::tuple after decaying.
template<typename T>
constexpr bool is_tuple = detail::is_tuple_impl<std::decay_t<T>>::value;

}
