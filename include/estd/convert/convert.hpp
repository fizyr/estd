#pragma once
#include "tags.hpp"
#include "traits.hpp"

#include "../result.hpp"

#include <system_error>
#include <type_traits>

namespace estd {

/// Convert a value using a guaranteed errorless conversion.
template<typename T, typename F>
T to(F && from) {
	return convert(std::forward<F>(from), To<T>{});
}

/// Convert a value using a conversion that might fail.
template<typename T, typename E, typename F>
result<T, E> parse(F && from) {
	return convert(std::forward<F>(from), Parse<T, E>{});
}

/// Implement To<T> for all Y if T is nothrow-constructible from Y.
template<typename F, typename T>
std::enable_if_t<std::is_nothrow_constructible<T, F>::value, T>
convert(F && from, To<T>) {
	return T(std::forward<F>(from));
}

/// Implement Parse<T> in terms of To<T>, if available.
template<typename F, typename T, typename E>
std::enable_if_t<can_convert_v<F, T>, result<T, E>>
convert(F && from, Parse<T, E>) {
	return {in_place_valid, convert(std::forward<F>(from), To<T>{})};
}

}
