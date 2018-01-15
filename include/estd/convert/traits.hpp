#pragma once
#include "tags.hpp"

#include <utility>
#include <type_traits>

namespace estd {

namespace detail {
	// check if convert(F, To<T>) is a valid expression
	template<typename F, typename T, typename = std::void_t<>>
	struct can_convert : std::false_type{};

	template<typename F, typename T>
	struct can_convert<F, T, std::void_t<decltype(convert(std::declval<F>(), To<T>{}))>> : std::true_type{};

	// check if convert(F, Parse<T, E>) is a valid expression
	template<typename F, typename T, typename E, typename = std::void_t<>>
	struct can_parse : std::false_type{};

	template<typename F, typename T, typename E>
	struct can_parse<F, T, E, std::void_t<decltype(convert(std::declval<F>(), Parse<T, E>{}))>> : std::true_type{};
}

/// Check if an errorless conversion exists for the given types.
template<typename From, typename To>
struct can_convert : detail::can_convert<From, To>{};

/// Check if an errorless conversion exists for the given types.
template<typename From, typename To>
constexpr bool can_convert_v = can_convert<From, To>::value;

/// Check if a conversion that might fail exists for the given types.
template<typename From, typename To, typename Error>
struct can_parse : detail::can_parse<From, To, Error>{};

/// Check if an errorless conversion exists for the given types.
template<typename From, typename To, typename Error>
constexpr bool can_parse_v = can_parse<From, To, Error>::value;

}
