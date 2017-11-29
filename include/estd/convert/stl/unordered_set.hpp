#pragma once
#include "../convert.hpp"

#include <type_traits>
#include <unordered_set>

namespace estd {

/// Convert a unordered_multiset<F> to unordered_multiset<T>.
template<typename F, typename T, typename E>
std::enable_if_t<can_convert<F, T>::value, std::unordered_multiset<T>>
convert(std::unordered_multiset<F> const & from, To<std::unordered_multiset<T>>) {
	std::unordered_multiset<T> result;
	for (F const & elem : from) result.insert(to<T>(elem));
	return result;
}

/// Convert a unordered_multiset<F> to unordered_multiset<T> while moving all elements.
template<typename F, typename T, typename E>
std::enable_if_t<can_convert<F, T>::value, std::unordered_multiset<T>>
convert(std::unordered_multiset<F> && from, To<std::unordered_multiset<T>>) {
	std::unordered_multiset<T> result;
	for (F & elem : from) result.insert(to<T>(std::move(elem)));
	return result;
}

/// Parse a unordered_multiset<F> to unordered_multiset<T>.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::unordered_multiset<T>, E>>
convert(std::unordered_multiset<F> const & from, Parse<std::unordered_multiset<T>, E>) {
	std::unordered_multiset<T> result;
	for (F const & elem : from) {
		estd::result<T, E> converted = parse<T>(elem);
		if (!converted) return converted.error_unchecked();
		auto [i, inserted] = result.insert(std::move(*converted));
	}
	return result;
}

/// Parse a unordered_multiset<F> to unordered_multiset<T> while moving all elements.
template<typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::unordered_multiset<T>, E>>
convert(std::unordered_multiset<F> && from, Parse<std::unordered_multiset<T>, E>) {
	std::unordered_multiset<T> result;
	for (F & elem : from) {
		estd::result<T, E> converted = parse<T>(std::move(elem));
		if (!converted) return converted.error_unchecked();
		auto [i, inserted] = result.insert(std::move(*converted));
	}
	return result;
}

}
