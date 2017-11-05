#pragma once
#include "../traits/is_tuple.hpp"
#include "../util/integer_sequence.hpp"

#include <tuple>
#include <type_traits>
#include <utility>
#include <functional>

namespace estd {

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

/// Select elements from a tuple with an index sequence.
template<typename Tuple, std::size_t... I, typename = std::enable_if_t<is_tuple<Tuple>>>
auto select(Tuple && tuple, std::index_sequence<I...>) {
	return std::tuple<std::tuple_element_t<I, Tuple>...>{std::get<I>(std::forward<Tuple>(tuple))...};
}

/// Remove N elements from the front of a tuple.
template<std::size_t N, typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
auto remove_front(Tuple && tuple) {
	return select(std::forward<Tuple>(tuple), remove_front<N>(tuple_index_sequence<Tuple>()));
}

/// Remove N elements from the back of a tuple.
template<std::size_t N, typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
auto remove_back(Tuple && tuple) {
	return select(std::forward<Tuple>(tuple), remove_back<N>(tuple_index_sequence<Tuple>()));
}

/// Take a slice of a tuple.
/**
 * \param Start The first index included in the slice.
 * \param End   The first index after Start not included in the slice.
 *
 * If either Start or End is negative, counting starts from the end of the original tuple.
 *
 * If End is std::numeric_limits<long int>::max() (the default), the slice extends to the end of the original tuple.
 *
 * Examples:
 *   slice<0>(tuple);       // Return a copy of the entire tuple.
 *   slice<1, 3>(tuple);    // Return a tuple with elements 1 and 2.
 *   slice<-2>(tuple);      // Return a tuple with the last two elements.
 *   slice<-2, -1>(tuple);  // Return a tuple with the second-to-last element.
 *   slice<0, -1>(tuple);   // Return a tuple with everything except the last element.
 */
template<long int Start, long int End = std::numeric_limits<long int>::max(), typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
auto slice(Tuple && tuple) {
	return select(std::forward<Tuple>(tuple), slice<Start, End>(tuple_index_sequence<Tuple>()));
}

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
