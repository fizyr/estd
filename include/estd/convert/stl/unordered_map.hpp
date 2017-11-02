#pragma once
#include "../convert.hpp"

#include <type_traits>
#include <unordered_map>

namespace estd {

/// Convert a unordered_map<K, F> to unordered_map<K, T>.
template<typename K, typename T, typename F>
std::enable_if_t<can_convert<F, T>::value, std::unordered_map<K, T>>
convert(std::unordered_map<K, F> const & from, To<std::unordered_map<K, T>>) {
	std::unordered_map<K, T> result;
	for (F const & elem : from) {
		result.insert({elem.first, to<T>(elem.second)});
	}
	return result;
}

/// Convert a unordered_map<K, F> to unordered_map<K, T> while moving all elements.
template<typename K, typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::unordered_map<K, T>>
convert(std::unordered_map<K, F> && from, To<std::unordered_map<K, T>>) {
	std::unordered_map<K, T> result;
	for (F && elem : from) {
		result.insert({elem.first, to<T>(std::move(elem.second))});
	}
	return result;
}

/// Parse a unordered_map<K, F> to unordered_map<K, T>.
template<typename K, typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::unordered_map<K, T>, E>>
convert(std::unordered_map<K, F> const & from, Parse<std::unordered_map<K, T>, E>) {
	std::unordered_map<K, T> result;
	for (F const & elem : from) {
		estd::result<T, E> converted = parse<T>(elem.second);
		if (!converted) return converted.error_unchecked();
		result.insert({elem.first, std::move(*converted)});
	}
	return result;
}

/// Parse a unordered_map<K, F> to unordered_map<K, T> while moving all elements.
template<typename K, typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::unordered_map<K, T>, E>>
convert(std::unordered_map<K, F> && from, Parse<std::unordered_map<K, T>, E>) {
	std::unordered_map<K, T> result;
	for (F && elem : from) {
		estd::result<T, E> converted = parse<T>(std::move(elem.second));
		if (!converted) return converted.error_unchecked();
		result.insert({elem.first, std::move(*converted)});
	}
	return result;
}

/// Convert a unordered_multimap<K, F> to unordered_multimap<K, T>.
template<typename K, typename T, typename F>
std::enable_if_t<can_convert<F, T>::value, std::unordered_multimap<K, T>>
convert(std::unordered_multimap<K, F> const & from, To<std::unordered_multimap<K, T>>) {
	std::unordered_multimap<K, T> result;
	for (F const & elem : from) {
		result.insert({elem.first, to<T>(elem.second)});
	}
	return result;
}

/// Convert a unordered_multimap<K, F> to unordered_multimap<K, T> while moving all elements.
template<typename K, typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::unordered_multimap<K, T>>
convert(std::unordered_multimap<K, F> && from, To<std::unordered_multimap<K, T>>) {
	std::unordered_multimap<K, T> result;
	for (F && elem : from) {
		result.insert({elem.first, to<T>(std::move(elem.second))});
	}
	return result;
}

/// Parse a unordered_multimap<K, F> to unordered_multimap<K, T>.
template<typename K, typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::unordered_multimap<K, T>, E>>
convert(std::unordered_multimap<K, F> const & from, Parse<std::unordered_multimap<K, T>, E>) {
	std::unordered_multimap<K, T> result;
	for (F const & elem : from) {
		estd::result<T, E> converted = parse<T>(elem.second);
		if (!converted) return converted.error_unchecked();
		result.insert({elem.first, std::move(*converted)});
	}
	return result;
}

/// Parse a unordered_multimap<K, F> to unordered_multimap<K, T> while moving all elements.
template<typename K, typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::unordered_multimap<K, T>, E>>
convert(std::unordered_multimap<K, F> && from, Parse<std::unordered_multimap<K, T>, E>) {
	std::unordered_multimap<K, T> result;
	for (F && elem : from) {
		estd::result<T, E> converted = parse<T>(std::move(elem.second));
		if (!converted) return converted.error_unchecked();
		result.insert({elem.first, std::move(*converted)});
	}
	return result;
}

}
