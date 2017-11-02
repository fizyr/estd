#pragma once
#include "../convert.hpp"

#include <array>
#include <type_traits>

namespace estd {

/// Convert a array<F, N> to array<T, N>.
template<typename F, typename T, std::size_t N>
std::enable_if_t<can_convert<F, T>::value, std::array<T, N>>
convert(std::array<F, N> const & from, To<std::array<T, N>>) {
	std::array<T, N> result;
	for (std::size_t i = 0; i < N; ++i) result[i](to<T>(from[i]));
	return result;
}

/// Convert a array<F, N> to array<T, N> while moving all elements.
template<typename F, typename T, std::size_t N>
std::enable_if_t<can_convert<F, T>::value, std::array<T, N>>
convert(std::array<F, N> && from, To<std::array<T, N>>) {
	std::array<T, N> result;
	for (std::size_t i = 0; i < N; ++i) result[i](to<T>(std::move(from[i])));
	return result;
}

/// Parse a array<F, N> to array<T, N>.
template<typename F, typename T, typename E, std::size_t N>
std::enable_if_t<can_parse<F, T, E>::value, result<std::array<T, N>, E>>
convert(std::array<F, N> const & from, Parse<std::array<T, N>, E>) {
	std::array<T, N> result;
	for (std::size_t i = 0; i < N; ++i) {
		estd::result<T, E> converted = parse<T, E>(from[i]);
		if (!converted) return converted.error_unchecked();
		result[i] = std::move(*converted);
	}
	return result;
}

/// Parse a array<F, N> to array<T, N> while moving all elements.
template<typename F, typename T, typename E, std::size_t N>
std::enable_if_t<can_parse<F, T, E>::value, result<std::array<T, N>, E>>
convert(std::array<F, N> && from, Parse<std::array<T, N>, E>) {
	std::array<T, N> result;
	for (std::size_t i = 0; i < N; ++i) {
		estd::result<T, E> converted = parse<T, E>(std::move(from[i]));
		if (!converted) return converted.error_unchecked();
		result[i] = std::move(*converted);
	}
	return result;
}

}

