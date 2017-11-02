#pragma once
#include "../convert.hpp"

#include <list>
#include <type_traits>

namespace estd {

/// Convert a list<F> to list<T>.
template<typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::list<T>>
convert(std::list<F> const & from, To<std::list<T>>) {
	std::list<T> result;
	for (F const & elem : from) result.push_back(to<T>(elem));
	return result;
}

/// Convert a list<F> to list<T> while moving all elements.
template<typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::list<T>>
convert(std::list<F> && from, To<std::list<T>>) {
	std::list<T> result;
	for (F && elem : from) result.push_back(to<T>(std::move(elem)));
	return result;
}

/// Parse a list<F> to list<T>.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::list<T>, E>>
convert(std::list<F> const & from, Parse<std::list<T>, E>) {
	std::list<T> result;
	for (F const & elem : from) {
		estd::result<T, E> converted = parse<T, E>(elem);
		if (!converted) return converted.error_unchecked();
		result.push_back(std::move(*converted));
	}
	return result;
}

/// Parse a list<F> to list<T> while moving all elements.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::list<T>, E>>
convert(std::list<F> && from, Parse<std::list<T>, E>) {
	std::list<T> result;
	for (F && elem : from) {
		estd::result<T, E> converted = parse<T, E>(std::move(elem));
		if (!converted) return converted.error_unchecked();
		result.push_back(std::move(*converted));
	}
	return result;
}

}
