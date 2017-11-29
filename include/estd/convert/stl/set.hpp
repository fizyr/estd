#pragma once
#include "../convert.hpp"

#include <set>
#include <type_traits>

namespace estd {

/// Convert a multiset<F> to multiset<T>.
template<typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::multiset<T>>
convert(std::multiset<F> const & from, To<std::multiset<T>>) {
	std::multiset<T> result;
	for (F const & elem : from) result.insert(to<T>(elem));
	return result;
}

/// Convert a multiset<F> to multiset<T> while moving all elements.
template<typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::multiset<T>>
convert(std::multiset<F> && from, To<std::multiset<T>>) {
	std::multiset<T> result;
	for (F & elem : from) result.insert(to<T>(std::move(elem)));
	return result;
}

/// Parse a multiset<F> to multiset<T>.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::multiset<T>, E>>
convert(std::multiset<F> const & from, Parse<std::multiset<T>, E>) {
	std::multiset<T> result;
	for (F const & elem : from) {
		estd::result<T, E> converted = parse<T, E>(elem);
		if (!converted) return converted.error_unchecked();
		auto [i, inserted] = result.insert(std::move(*converted));
	}
	return result;
}

/// Parse a multiset<F> to multiset<T> while moving all elements.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::multiset<T>, E>>
convert(std::multiset<F> && from, Parse<std::multiset<T>, E>) {
	std::multiset<T> result;
	for (F & elem : from) {
		estd::result<T, E> converted = parse<T, E>(std::move(elem));
		if (!converted) return converted.error_unchecked();
		auto [i, inserted] = result.insert(std::move(*converted));
	}
	return result;
}

}
