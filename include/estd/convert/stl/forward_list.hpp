#pragma once
#include "../traits.hpp"

#include <forward_list>
#include <type_traits>

namespace estd {

/// Convert a forward_list<F> to forward_list<T>.
template<typename F, typename T, typename Tag>
struct conversion<std::forward_list<F>, std::forward_list<T>, Tag> {
	using From = std::forward_list<F>;
	using To   = std::forward_list<T>;

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
		for (F & elem : from) result.push_back(convert<T, Tag>(std::move(elem)));
		return result;
	}
};

/// Parse a forward_list<F> to ErrorOr<forward_list<T>>.
template<typename F, typename T, typename E, typename Tag>
struct conversion<std::forward_list<F>, result<std::forward_list<T>, E>, Tag> {
	using From   = std::forward_list<F>;
	using Raw    = std::forward_list<T>;
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
		for (F & elem : from) {
			estd::result<T, E> converted = parse<T, E>(std::move(elem));
			if (!converted) return converted.error_unchecked();
			result.push_back(std::move(*converted));
		}
		return result;
	}
};

}
