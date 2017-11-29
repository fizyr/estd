#pragma once
#include "../convert.hpp"

#include <type_traits>
#include <vector>

namespace estd {

/// Convert a vector<F> to vector<T>.
template<typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::vector<T>>
convert(std::vector<F> const & from, To<std::vector<T>>) {
	std::vector<T> result;
	result.reserve(from.size());
	for (F const & elem : from) result.push_back(to<T>(elem));
	return result;
}

/// Convert a vector<F> to vector<T> while moving all elements.
template<typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::vector<T>>
convert(std::vector<F> && from, To<std::vector<T>>) {
	std::vector<T> result;
	result.reserve(from.size());
	for (F & elem : from) result.push_back(to<T>(std::move(elem)));
	return result;
}

/// Parse a vector<F> to vector<T>.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::vector<T>, E>>
convert(std::vector<F> const & from, Parse<std::vector<T>, E>) {
	std::vector<T> result;
	result.reserve(from.size());
	for (F const & elem : from) {
		estd::result<T, E> converted = parse<T, E>(elem);
		if (!converted) return converted.error_unchecked();
		result.push_back(std::move(*converted));
	}
	return result;
}

/// Parse a vector<F> to vector<T> while moving all elements.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::vector<T>, E>>
convert(std::vector<F> && from, Parse<std::vector<T>, E>) {
	std::vector<T> result;
	result.reserve(from.size());
	for (F & elem : from) {
		estd::result<T, E> converted = parse<T, E>(std::move(elem));
		if (!converted) return converted.error_unchecked();
		result.push_back(std::move(*converted));
	}
	return result;
}

}
