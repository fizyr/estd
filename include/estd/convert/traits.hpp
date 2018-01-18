#pragma once
#include "tags.hpp"

#include <utility>
#include <type_traits>

namespace estd {

namespace detail {
	// check if convert(F, Tag) is a valid expression
	template<typename F, typename Tag, typename = std::void_t<>>
	struct can_convert_tagged : std::false_type{};

	template<typename F, typename Tag>
	struct can_convert_tagged<F, Tag, std::void_t<decltype(convert(std::declval<F>(), Tag{}))>> : std::true_type{};
}

/// Check if an errorless conversion exists for the given types.
template<typename From, typename Tag>
struct can_convert_tagged : detail::can_convert_tagged<From, Tag>{};

template<typename From, typename Tag>
constexpr bool can_convert_tagged_v = can_convert_tagged<From, Tag>::value;

/// Check if an errorless conversion exists for the given types.
template<typename From, typename Target>
struct can_convert : detail::can_convert_tagged<From, To<Target>>{};

/// Check if an errorless conversion exists for the given types.
template<typename From, typename Target>
constexpr bool can_convert_v = can_convert<From, Target>::value;

/// Check if a conversion that might fail exists for the given types.
template<typename From, typename Target, typename Error>
struct can_parse : can_convert_tagged<From, Parse<Target, Error>>{};

/// Check if an errorless conversion exists for the given types.
template<typename From, typename Target, typename Error>
constexpr bool can_parse_v = can_parse<From, Target, Error>::value;

}
