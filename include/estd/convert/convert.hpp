#pragma once
#include "../result.hpp"

#include <type_traits>
#include <utility>

namespace estd {

struct default_conversion {};

/// Definition of a conversion.
/**
 * You may define your own conversions by specializing this struct.
 *
 * To define special conversions for your own use, you may specialize on a different Tag type.
 * These special conversions can be invoked by passing the same tag to `estd::convert`.
 *
 * It is also possible to mark a conversion as impossible explicitly.
 * A conversion is considered impossible when any of these conditions is true:
 *   - The specialization has a member `static constexpr bool possible = false`.
 *   - The specialization has no member named `perform`.
 *
 * The actual conversion function must be implemented as:
 *   static To perform(From ... from)
 *
 * The `from` argument may be passed by value, reference or rvalue reference.
 * It is also allowed to define multiple overloads, or to implement the `perform` member
 * as a function template.
 */
template<typename From, typename To, typename Tag = default_conversion>
struct conversion {
	static constexpr bool possible = std::is_constructible_v<To, From>;

	static To perform(From const & from) {
		static_assert(std::is_constructible_v<To, From>, "cannot convert From to To");
		return To(from);
	}
	static To perform(From && from) {
		static_assert(std::is_constructible_v<To, From>, "cannot convert From to To");
		return To(from);
	}
};

/// Convert a value to type T.
template<typename T, typename Tag = default_conversion, typename F>
T convert(F && from) {
	return conversion<std::decay_t<F>, T, Tag>::perform(std::forward<F>(from));
}

/// Convert a value to a result<T, E>.
/**
 * Shorthand for estd::convert<estd::result<T, E>, Tag>(from)
 */
template<typename T, typename E, typename Tag = default_conversion, typename F>
result<T, E> parse(F && from) {
	return convert<result<T, E>, Tag>(std::forward<F>(from));
}

}
