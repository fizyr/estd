#pragma once
#include <utility>
#include <type_traits>

#include "./convert.hpp"

namespace estd {

namespace convert_detail {
	template<typename F, typename T, typename Tag, typename = void> struct has_possible : std::false_type{};
	template<typename F, typename T, typename Tag> struct has_possible<F, T, Tag, std::void_t<decltype(&::estd::conversion<F, T, Tag>::possible)>> : std::true_type {};

	template<typename F, typename T, typename Tag, typename = void> struct has_perform_impl : std::false_type{};
	template<typename F, typename T, typename Tag> struct has_perform_impl<F, T, Tag, std::void_t<decltype(::estd::conversion<std::decay_t<F>, T, Tag>::perform(std::declval<F>()))>> : std::true_type {};

	template<typename F, typename T, typename Tag>
	constexpr bool has_perform = false
		|| has_perform_impl<F       &, T, Tag>()
		|| has_perform_impl<F const &, T, Tag>()
		|| has_perform_impl<F      &&, T, Tag>();

	// check if a not-explicitly impossible conversion is defined.
	template<typename F, typename T, typename Tag,
		bool = has_possible<F, T, Tag>(),
		bool = has_perform<F, T, Tag>
	> struct can_convert : std::false_type{};

	template<typename F, typename T, typename Tag>
	struct can_convert<F, T, Tag, true, true> : std::integral_constant<bool, conversion<F, T, Tag>::possible == true> {};

	template<typename F, typename T, typename Tag>
	struct can_convert<F, T, Tag, false, true> : std::true_type {};
}

/// Check if a conversion exists for the given types.
template<typename From, typename To, typename Tag = default_conversion>
constexpr bool can_convert = convert_detail::can_convert<std::decay_t<From>, To, Tag>::value;

/// Check if a conversion that might fail exists for the given types.
template<typename From, typename To, typename Error, typename Tag = default_conversion>
constexpr bool can_parse = can_convert<std::decay_t<From>, result<To, Error>, Tag>;

}
