#pragma once
#include "tuple.hpp"

#include "../traits/replace_type.hpp"
#include "../utility/make_ref.hpp"

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace estd {

/// An empty value.
constexpr struct empty_t {} empty;

constexpr bool operator==(empty_t, empty_t) { return true;  }
constexpr bool operator!=(empty_t, empty_t) { return false; }

namespace detail {
	/// Invoke a functor, replacing void return values with an empty_t value.
	template<typename F, typename ...Args>
	constexpr replace_type<void, empty_t, std::invoke_result_t<F &&, Args && ...>>
	invoke_map_void(F && f, Args && ...args) {
		using R = std::invoke_result_t<F &&, Args && ...>;
		if constexpr(std::is_same_v<R, void>) {
			std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
			return empty;
		} else {
			return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
		}
	}

	/// Apply a functor to each tuple alement and return a tuple of the results.
	/**
	 * The result types are decayed with std::decay_t.
	 *
	 * If the functor returns void for all tuple elements, this function also returns void.
	 * Otherwise, all void return values are mapped to empty_t values.
	 */
	template<typename F, typename Tuple, std::size_t ...I>
	constexpr auto tuple_transform_impl(Tuple && tuple, std::index_sequence<I...>, F && f) {
		using std::get;
		constexpr bool all_void = (std::is_same_v<decltype(std::invoke(f, get<I>(std::forward<Tuple>(tuple)))), void> && ...);
		if constexpr(all_void) {
			(std::invoke(f, get<I>(std::forward<Tuple>(tuple))), ...);
		} else {
			return std::make_tuple(invoke_map_void(f, get<I>(std::forward<Tuple>(tuple)))...);
		}
	}

	/// Apply a functor to each tuple alement and return a tuple of the results without decaying the result types.
	/*
	 * This function can return tuples with references in them.
	 * You should make sure none of those references are dangling.
	 *
	 * If the functor returns void for all tuple elements, this function also returns void.
	 * Otherwise, all void return values are mapped to empty_t values.
	 */
	template<typename F, typename Tuple, std::size_t ...I>
	constexpr auto tuple_transform_raw_impl(Tuple && tuple, std::index_sequence<I...>, F && f) {
		using std::get;
		constexpr bool all_void = (std::is_same_v<decltype(std::invoke(f, get<I>(std::forward<Tuple>(tuple)))), void> && ...);
		if constexpr(all_void) {
			(std::invoke(f, get<I>(std::forward<Tuple>(tuple))), ...);
		} else {
			return std::tuple<decltype(invoke_map_void(f, get<I>(std::forward<Tuple>(tuple))))...>(invoke_map_void(f, get<I>(std::forward<Tuple>(tuple)))...);
		}
	}
}

/// Create a tuple of values by applying a function to each value, and passing the results to std::make_tuple (thus decaying the value types).
template<typename Tuple, typename F>
constexpr auto tuple_transform(Tuple && tuple, F && f) {
	return detail::tuple_transform_impl(std::forward<Tuple>(tuple), tuple_index_sequence<Tuple>(), std::forward<F>(f));
}

/// Create a tuple of values by applying a function to each value, without decaying the result types.
template<typename Tuple, typename F>
constexpr auto tuple_transform_raw(Tuple && tuple, F && f) {
	return detail::tuple_transform_raw_impl(std::forward<Tuple>(tuple), tuple_index_sequence<Tuple>(), std::forward<F>(f));
}

/// Make a tuple of references to the elements of a tuple.
template<typename Tuple>
constexpr auto tuple_ref(Tuple && tuple) {
	return tuple_transform_raw(std::forward<Tuple>(tuple), make_ref);
}

/// Make a tuple of const references to the elements of a tuple.
template<typename Tuple>
constexpr auto tuple_cref(Tuple const & tuple) {
	return tuple_transform_raw(tuple, make_ref);
}

}
