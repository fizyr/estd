#pragma once
#include "../traits.hpp"

#include <map>
#include <type_traits>

namespace estd {

/// Convert a map<K, F> to map<K, T>.
template<typename K, typename F, typename T, typename Tag>
struct conversion<std::map<K, F>, std::map<K, T>, Tag> {
	using From = std::map<K, F>;
	using To   = std::map<K, T>;

	static constexpr bool possible = can_convert<F, T, Tag>;

	static To perform(From const & from) {
		static_assert(possible, "no conversion available for F and T");
		To result;
		for (std::pair<K, F> const & elem : from) result.insert(std::pair<K, T>{elem.first, convert<T, Tag>(elem.second)});
		return result;
	}

	static To perform(From && from) {
		static_assert(possible, "no conversion available for F and T");
		To result;
		for (std::pair<K, F> & elem : from) result.insert(std::pair<K, T>{std::move(elem.first), convert<T, Tag>(std::move(elem.second))});
		return result;
	}
};

/// Convert a map<K, F> to result<map<K, T>, E>.
template<typename K, typename F, typename T, typename E, typename Tag>
struct conversion<std::map<K, F>, result<std::map<K, T>, E>, Tag> {
	using From = std::map<K, F>;
	using Raw  = std::map<K, T>;
	using To   = result<Raw, E>;

	static constexpr bool possible = can_parse<F, T, Tag>;

	static To perform(From const & from) {
		static_assert(possible, "no conversion available for F and result<T, E>");
		To result;
		for (std::pair<K, F> const & elem : from) {
			estd::result<T, E> converted = parse<T, E, Tag>(elem.second);
			if (!converted) return converted.error_unchecked();
			result.insert(std::pair<K, T>{elem.first, std::move(*converted)});
		}
		return result;
	}

	static To perform(From && from) {
		static_assert(possible, "no conversion available for F and result<T, E>");
		To result;
		for (std::pair<K, F> & elem : from) {
			estd::result<T, E> converted = parse<T, E, Tag>(std::move(elem.second));
			if (!converted) return converted.error_unchecked();
			result.insert(std::pair<K, T>{elem.first, std::move(*converted)});
		}
		return result;
	}
};


/// Convert a multimap<K, F> to multimap<K, T>.
template<typename K, typename F, typename T, typename Tag>
struct conversion<std::multimap<K, F>, std::multimap<K, T>, Tag> {
	using From = std::multimap<K, F>;
	using To   = std::multimap<K, T>;

	static constexpr bool possible = can_convert<F, T, Tag>;

	static To perform(From const & from) {
		static_assert(possible, "no conversion available for F and T");
		To result;
		for (std::pair<K, F> const & elem : from) result.insert(std::pair<K, T>{elem.first, convert<T, Tag>(elem.second)});
		return result;
	}

	static To perform(From && from) {
		static_assert(possible, "no conversion available for F and T");
		To result;
		for (std::pair<K, F> & elem : from) result.insert(std::pair<K, T>{std::move(elem.first), convert<T, Tag>(std::move(elem.second))});
		return result;
	}
};

/// Convert a multimap<K, F> to result<multimap<K, T>, E>.
template<typename K, typename F, typename T, typename E, typename Tag>
struct conversion<std::multimap<K, F>, result<std::multimap<K, T>, E>, Tag> {
	using From = std::multimap<K, F>;
	using Raw  = std::multimap<K, T>;
	using To   = result<Raw, E>;

	static constexpr bool possible = can_parse<F, T, Tag>;

	static To perform(From const & from) {
		static_assert(possible, "no conversion available for F and result<T, E>");
		To result;
		for (std::pair<K, F> const & elem : from) {
			estd::result<T, E> converted = parse<T, E, Tag>(elem.second);
			if (!converted) return converted.error_unchecked();
			result.insert(std::pair<K, T>{elem.first, std::move(*converted)});
		}
		return result;
	}

	static To perform(From && from) {
		static_assert(possible, "no conversion available for F and result<T, E>");
		To result;
		for (std::pair<K, F> & elem : from) {
			estd::result<T, E> converted = parse<T, E, Tag>(std::move(elem.second));
			if (!converted) return converted.error_unchecked();
			result.insert(std::pair<K, T>{elem.first, std::move(*converted)});
		}
		return result;
	}
};

}
