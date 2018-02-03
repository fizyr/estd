#pragma once
#include "../traits.hpp"

#include <unordered_set>
#include <type_traits>

namespace estd {

/// Convert a unordered_multiset<F> to unordered_multiset<T>.
template<typename F, typename T, typename Tag>
struct conversion<std::unordered_multiset<F>, std::unordered_multiset<T>, Tag> {
	using From = std::unordered_multiset<F>;
	using To   = std::unordered_multiset<T>;

	static constexpr bool impossible = !can_convert<F, T, Tag>;

	static To perform(From const & from) {
		static_assert(!impossible, "no conversion available for F and T");
		To result;
		for (F const & elem : from) result.push_back(convert<T, Tag>(elem));
		return result;
	}

	static To perform(From && from) {
		static_assert(!impossible, "no conversion available for F and T");
		To result;
		for (F && elem : from) result.push_back(convert<T, Tag>(std::move(elem)));
		return result;
	}
};

/// Parse a unordered_multiset<F> to ErrorOr<unordered_multiset<T>>.
template<typename F, typename T, typename E, typename Tag>
struct conversion<std::unordered_multiset<F>, result<std::unordered_multiset<T>, E>, Tag> {
	using From   = std::unordered_multiset<F>;
	using Raw    = std::unordered_multiset<T>;
	using To     = result<Raw, E>;

	static constexpr bool impossible = !can_parse<F, T, E, Tag>;

	static To convert(From const & from) {
		static_assert(!impossible, "no conversion available for F and result<T, E>");
		Raw result;
		for (F const & elem : from) {
			estd::result<T, E> converted = parse<T, E>(elem);
			if (!converted) return converted.error_unchecked();
			result.push_back(std::move(*converted));
		}
		return result;
	}

	static To convert(From && from) {
		static_assert(!impossible, "no conversion available for F and result<T, E>");
		Raw result;
		for (F const & elem : from) {
			estd::result<T, E> converted = parse<T, E>(std::move(elem));
			if (!converted) return converted.error_unchecked();
			result.push_back(std::move(*converted));
		}
		return result;
	}
};

}
