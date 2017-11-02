#pragma once
#include "../convert.hpp"

#include <map>
#include <type_traits>

namespace estd {

/// Convert a map<K, F> to map<K, T>.
template<typename K, typename T, typename F>
std::enable_if_t<can_convert<F, T>::value, std::map<K, T>>
convert(std::map<K, F> const & from, To<std::map<K, T>>) {
	std::map<K, T> result;
	for (F const & elem : from) {
		result.insert({elem.first, to<T>(elem.second)});
	}
	return result;
}

/// Convert a map<K, F> to map<K, T> while moving all elements.
template<typename K, typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::map<K, T>>
convert(std::map<K, F> && from, To<std::map<K, T>>) {
	std::map<K, T> result;
	for (F && elem : from) {
		result.insert({elem.first, to<T>(std::move(elem.second))});
	}
	return result;
}

/// Parse a map<K, F> to map<K, T>.
template<typename K, typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::map<K, T>, E>>
convert(std::map<K, F> const & from, Parse<std::map<K, T>, E>) {
	std::map<K, T> result;
	for (F const & elem : from) {
		estd::result<T, E> converted = parse<T>(elem.second);
		if (!converted) return converted.error_unchecked();
		result.insert({elem.first, std::move(*converted)});
	}
	return result;
}

/// Parse a map<K, F> to map<K, T> while moving all elements.
template<typename K, typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::map<K, T>, E>>
convert(std::map<K, F> && from, Parse<std::map<K, T>, E>) {
	std::map<K, T> result;
	for (F && elem : from) {
		estd::result<T, E> converted = parse<T>(std::move(elem.second));
		if (!converted) return converted.error_unchecked();
		result.insert({elem.first, std::move(*converted)});
	}
	return result;
}

/// Convert a multimap<K, F> to multimap<K, T>.
template<typename K, typename T, typename F>
std::enable_if_t<can_convert<F, T>::value, std::multimap<K, T>>
convert(std::multimap<K, F> const & from, To<std::multimap<K, T>>) {
	std::multimap<K, T> result;
	for (F const & elem : from) {
		result.insert({elem.first, to<T>(elem.second)});
	}
	return result;
}

/// Convert a multimap<K, F> to multimap<K, T> while moving all elements.
template<typename K, typename F, typename T>
std::enable_if_t<can_convert<F, T>::value, std::multimap<K, T>>
convert(std::multimap<K, F> && from, To<std::multimap<K, T>>) {
	std::multimap<K, T> result;
	for (F && elem : from) {
		result.insert({elem.first, to<T>(std::move(elem.second))});
	}
	return result;
}

/// Parse a multimap<K, F> to multimap<K, T>.
template<typename K, typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::multimap<K, T>, E>>
convert(std::multimap<K, F> const & from, Parse<std::multimap<K, T>, E>) {
	std::multimap<K, T> result;
	for (F const & elem : from) {
		estd::result<T, E> converted = parse<T>(elem.second);
		if (!converted) return converted.error_unchecked();
		result.insert({elem.first, std::move(*converted)});
	}
	return result;
}

/// Parse a multimap<K, F> to multimap<K, T> while moving all elements.
template<typename K, typename F, typename T, typename E>
std::enable_if_t<can_parse<F, T, E>::value, result<std::multimap<K, T>, E>>
convert(std::multimap<K, F> && from, Parse<std::multimap<K, T>, E>) {
	std::multimap<K, T> result;
	for (F && elem : from) {
		estd::result<T, E> converted = parse<T>(std::move(elem.second));
		if (!converted) return converted.error_unchecked();
		result.insert({elem.first, std::move(*converted)});
	}
	return result;
}

}
