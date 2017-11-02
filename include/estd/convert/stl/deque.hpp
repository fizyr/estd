#pragma once
#include "../convert.hpp"

#include <deque>
#include <type_traits>

namespace estd {

/// Convert a deque<F> to deque<T>.
template<typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::deque<T>>
convert(std::deque<F> const & from, To<std::deque<T>>) {
	std::deque<T> result;
	for (F const & elem : from) result.push_back(to<T>(elem));
	return result;
}

/// Convert a deque<F> to deque<T> while moving all elements.
template<typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::deque<T>>
convert(std::deque<F> && from, To<std::deque<T>>) {
	std::deque<T> result;
	for (F && elem : from) result.push_back(to<T>(std::move(elem)));
	return result;
}

/// Parse a deque<F> to deque<T>.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::deque<T>, E>>
convert(std::deque<F> const & from, Parse<std::deque<T>, E>) {
	std::deque<T> result;
	for (F const & elem : from) {
		estd::result<T, E> converted = parse<T, E>(elem);
		if (!converted) return converted.error_unchecked();
		result.push_back(std::move(*converted));
	}
	return result;
}

/// Parse a deque<F> to deque<T> while moving all elements.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::deque<T>, E>>
convert(std::deque<F> && from, Parse<std::deque<T>, E>) {
	std::deque<T> result;
	for (F && elem : from) {
		estd::result<T, E> converted = parse<T, E>(std::move(elem));
		if (!converted) return converted.error_unchecked();
		result.push_back(std::move(*converted));
	}
	return result;
}

}
