#pragma once
#include "../convert.hpp"

#include <forward_list>
#include <type_traits>

namespace estd {

/// Convert a forward_list<F> to forward_list<T>.
template<typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::forward_list<T>>
convert(std::forward_list<F> const & from, To<std::forward_list<T>>) {
	std::forward_list<T> result;
	for (F const & elem : from) result.push_back(to<T>(elem));
	return result;
}

/// Convert a forward_list<F> to forward_list<T> while moving all elements.
template<typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::forward_list<T>>
convert(std::forward_list<F> && from, To<std::forward_list<T>>) {
	std::forward_list<T> result;
	for (F & elem : from) result.push_back(to<T>(std::move(elem)));
	return result;
}

/// Parse a forward_list<F> to forward_list<T>.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::forward_list<T>, E>>
convert(std::forward_list<F> const & from, Parse<std::forward_list<T>, E>) {
	std::forward_list<T> result;
	for (F const & elem : from) {
		estd::result<T, E> converted = parse<T, E>(elem);
		if (!converted) return converted.error_unchecked();
		result.push_back(std::move(*converted));
	}
	return result;
}

/// Parse a forward_list<F> to forward_list<T> while moving all elements.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::forward_list<T>, E>>
convert(std::forward_list<F> && from, Parse<std::forward_list<T>, E>) {
	std::forward_list<T> result;
	for (F & elem : from) {
		estd::result<T, E> converted = parse<T, E>(std::move(elem));
		if (!converted) return converted.error_unchecked();
		result.push_back(std::move(*converted));
	}
	return result;
}

}
