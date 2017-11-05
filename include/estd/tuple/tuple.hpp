#pragma once
#include <tuple>
#include <type_traits>
#include <utility>
#include <functional>

namespace estd {

/// An empty value.
constexpr struct empty_t {} empty;

constexpr bool operator==(empty_t, empty_t) { return true;  }
constexpr bool operator!=(empty_t, empty_t) { return false; }

namespace detail {
	template<typename From, typename To, typename Type>
	using replace_type = std::conditional_t<std::is_same_v<Type, From>, To, Type>;

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
	constexpr auto tuple_transform(Tuple && tuple, std::index_sequence<I...>, F && f) {
		constexpr bool all_void = (std::is_same_v<decltype(std::invoke(f, std::get<I>(std::forward<Tuple>(tuple)))), void> && ...);
		if constexpr(all_void) {
			(std::invoke(f, std::get<I>(std::forward<Tuple>(tuple))), ...);
		} else {
			return std::make_tuple(invoke_map_void(f, std::get<I>(std::forward<Tuple>(tuple)))...);
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
	constexpr auto tuple_transform_raw(Tuple && tuple, std::index_sequence<I...>, F && f) {
		constexpr bool all_void = (std::is_same_v<decltype(std::invoke(f, std::get<I>(std::forward<Tuple>(tuple)))), void> && ...);
		if constexpr(all_void) {
			(std::invoke(f, std::get<I>(std::forward<Tuple>(tuple))), ...);
		} else {
			return std::tuple<decltype(invoke_map_void(f, std::get<I>(std::forward<Tuple>(tuple))))...>(invoke_map_void(f, std::get<I>(std::forward<Tuple>(tuple)))...);
		}
	}
}

/// Make an index sequence for a tuple.
template<typename Tuple>
constexpr auto tuple_index_sequence() {
	return std::make_index_sequence<std::tuple_size_v<Tuple>>();
}

/// Make an index sequence for a tuple.
template<typename Tuple>
constexpr auto tuple_index_sequence(Tuple const &) {
	return tuple_index_sequence<Tuple>();
}

/// Create a tuple of values by applying a function to each value, and passing the results to std::make_tuple (thus decaying the value types).
template<typename Tuple, typename F>
constexpr auto tuple_transform(Tuple && tuple, F && f) {
	return detail::tuple_transform(std::forward<Tuple>(tuple), tuple_index_sequence<Tuple>(), std::forward<F>(f));
}

/// Create a tuple of values by applying a function to each value, without decaying the result types.
template<typename Tuple, typename F>
constexpr auto tuple_transform_raw(Tuple && tuple, F && f) {
	return detail::tuple_transform_raw(std::forward<Tuple>(tuple), tuple_index_sequence<Tuple>(), std::forward<F>(f));
}

}
