#pragma once
#include <cstddef>
#include <type_traits>

namespace estd {

namespace detail {
	template<std::size_t N, typename Head, typename... Tail>
	struct parameter_pack_element {
		using type = typename parameter_pack_element<N - 1, Tail...>::type;
	};

	template<typename Head, typename... Tail>
	struct parameter_pack_element<0, Head, Tail...> {
		using type = Head;
	};

	template<std::size_t N, typename T, T head, T... tail>
	struct value_pack_element : std::integral_constant<T, value_pack_element<N - 1, T, tail...>::value> {};

	template<typename T, T head, T... tail>
	struct value_pack_element<0, T, head, tail...> : std::integral_constant<T, head> {};
}

/// Get the first type from a parameter pack.
template<typename H, typename...> using parameter_pack_head = H;

/// Get a type from a parameter pack by index.
template<std::size_t N, typename... Types> using parameter_pack_element = typename detail::parameter_pack_element<N, Types...>::type;

/// Get the first value from a parameter pack.
template<typename T, T Head, T...> constexpr T value_pack_head = Head;

/// Get a value from a parameter pack by index.
template<std::size_t N, typename T, T... values> constexpr T value_pack_element = detail::value_pack_element<N, T, values...>::value;


}
