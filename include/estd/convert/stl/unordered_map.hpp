#pragma once
#include "../traits.hpp"

#include <unordered_map>
#include <type_traits>

namespace estd {

/// Convert a unordered_map<K, F> to unordered_map<K, T>.
template<typename K, typename F, typename T, typename Tag>
struct conversion<std::unordered_map<K, F>, std::unordered_map<K, T>, Tag> {
	using From = std::unordered_map<K, F>;
	using To   = std::unordered_map<K, T>;

	static constexpr bool impossible = !can_convert<F, T, Tag>;

	static To perform(From const & from) {
		static_assert(!impossible, "no conversion available for F and T");
		To result;
		for (std::pair<K, F> const & elem : from) result.insert(std::pair<K, T>{elem.first, convert<T, Tag>(elem.second)});
		return result;
	}

	static To perform(From && from) {
		static_assert(!impossible, "no conversion available for F and T");
		To result;
		for (std::pair<K, F> & elem : from) result.insert(std::pair<K, T>{std::move(elem.first), convert<T, Tag>(std::move(elem.second))});
		return result;
	}
};

/// Convert a unordered_map<K, F> to resutl<unordered_map<K, T>, E>.
template<typename K, typename F, typename T, typename E, typename Tag>
struct conversion<std::unordered_map<K, F>, result<std::unordered_map<K, T>, E>, Tag> {
	using From = std::unordered_map<K, F>;
	using Raw  = std::unordered_map<K, T>;
	using To   = result<Raw, E>;

	static constexpr bool impossible = !can_parse<F, T, Tag>;

	static To perform(From const & from) {
		static_assert(!impossible, "no conversion available for F and result<T, E>");
		To result;
		for (std::pair<K, F> const & elem : from) {
			estd::result<T, E> converted = parse<T, E, Tag>(elem.second);
			if (!converted) return converted.error_unchecked();
			result.insert(std::pair<K, T>{elem.first, std::move(*converted)});
		}
		return result;
	}

	static To perform(From && from) {
		static_assert(!impossible, "no conversion available for F and result<T, E>");
		To result;
		for (std::pair<K, F> & elem : from) {
			estd::result<T, E> converted = parse<T, E, Tag>(std::move(elem.second));
			if (!converted) return converted.error_unchecked();
			result.insert(std::pair<K, T>{elem.first, std::move(*converted)});
		}
		return result;
	}
};


/// Convert a unordered_multimap<K, F> to unordered_multimap<K, T>.
template<typename K, typename F, typename T, typename Tag>
struct conversion<std::unordered_multimap<K, F>, std::unordered_multimap<K, T>, Tag> {
	using From = std::unordered_multimap<K, F>;
	using To   = std::unordered_multimap<K, T>;

	static constexpr bool impossible = !can_convert<F, T, Tag>;

	static To perform(From const & from) {
		static_assert(!impossible, "no conversion available for F and T");
		To result;
		for (std::pair<K, F> const & elem : from) result.insert(std::pair<K, T>{elem.first, convert<T, Tag>(elem.second)});
		return result;
	}

	static To perform(From && from) {
		static_assert(!impossible, "no conversion available for F and T");
		To result;
		for (std::pair<K, F> & elem : from) result.insert(std::pair<K, T>{std::move(elem.first), convert<T, Tag>(std::move(elem.second))});
		return result;
	}
};

/// Convert a unordered_multimap<K, F> to resutl<unordered_multimap<K, T>, E>.
template<typename K, typename F, typename T, typename E, typename Tag>
struct conversion<std::unordered_multimap<K, F>, result<std::unordered_multimap<K, T>, E>, Tag> {
	using From = std::unordered_multimap<K, F>;
	using Raw  = std::unordered_multimap<K, T>;
	using To   = result<Raw, E>;

	static constexpr bool impossible = !can_parse<F, T, Tag>;

	static To perform(From const & from) {
		static_assert(!impossible, "no conversion available for F and result<T, E>");
		To result;
		for (std::pair<K, F> const & elem : from) {
			estd::result<T, E> converted = parse<T, E, Tag>(elem.second);
			if (!converted) return converted.error_unchecked();
			result.insert(std::pair<K, T>{elem.first, std::move(*converted)});
		}
		return result;
	}

	static To perform(From && from) {
		static_assert(!impossible, "no conversion available for F and result<T, E>");
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
