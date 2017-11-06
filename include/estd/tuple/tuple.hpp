#pragma once
#include "../traits/is_tuple.hpp"
#include "../utility/integer_sequence.hpp"
#include "../utility/parameter_pack.hpp"

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
	using std::get;
	return std::tuple<std::tuple_element_t<I, Tuple>...>{get<I>(std::forward<Tuple>(tuple))...};
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

namespace detail {
	template<std::size_t I, typename... Tuples>
	using zip_tuple_at_index_t = std::tuple<std::tuple_element_t<I, std::decay_t<Tuples>>...>;

	template<std::size_t I, typename... Tuples>
	zip_tuple_at_index_t<I, Tuples...> zip_tuple_at_index(Tuples && ...tuples) {
		return {std::get<I>(std::forward<Tuples>(tuples))...};
	}

	template<typename... Tuples, std::size_t... I>
	std::tuple<zip_tuple_at_index_t<I, Tuples...>...> tuple_zip(Tuples && ...tuples, std::index_sequence<I...>) {
		return {zip_tuple_at_index<I>(std::forward<Tuples>(tuples)...)...};
	}

}

/// Zip a number of tuples together into a tuple of tuples.
template<typename... Tuples, typename = std::enable_if_t<(is_tuple<Tuples> && ...)>>
auto zip(Tuples && ...tuples) {
	constexpr std::size_t Size = std::tuple_size_v<std::decay_t<parameter_pack_head<Tuples...>>>;
	static_assert(((std::tuple_size_v<std::decay_t<Tuples>> == Size) && ...), "Tuple size mismatch, can not zip.");
	return detail::tuple_zip<Tuples...>(std::forward<Tuples>(tuples)..., std::make_index_sequence<Size>());
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

/// Perform a foldl over the values of a tuple with an initial accumulator.
/**
 * If the tuple is empty, the initial accumulator value is returned unchanged.
 */
template<typename F, typename Tuple, typename Accumulator, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldl(Tuple && tuple, Accumulator && accumulator, F && f) {
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 0) {
		return accumulator;
	} else if constexpr(std::tuple_size_v<Tuple> == 1) {
		return f(accumulator, get<0>(std::forward<Tuple>(tuple)));
	} else {
		return foldl(remove_front<1>(std::forward<Tuple>(tuple)), f(accumulator, get<0>(std::forward<Tuple>(tuple))), f);
	}
}

/// Perform a foldl over the values of a tuple without an initial accumulator.
/**
 * If the tuple contains exactly one element, that element is returned unchanged.
 */
template<typename F, typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldl(Tuple && tuple, F && f) {
	static_assert(std::tuple_size_v<Tuple> > 0, "Can not foldl over an empty tuple without an initial accumulator value.");
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 1) {
		return get<0>(std::forward<Tuple>(tuple));
	} else {
		return foldl(remove_front<1>(std::forward<Tuple>(tuple)), get<0>(std::forward<Tuple>(tuple)), f);
	}
}

/// Perform a foldl over the values of a tuple with an initial accumulator.
/**
 * If the tuple is empty, the initial accumulator value is returned unchanged.
 */
template<typename F, typename Tuple, typename Accumulator, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldr(Tuple && tuple, Accumulator && accumulator, F && f) {
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 0) {
		return accumulator;
	} else if constexpr(std::tuple_size_v<Tuple> == 1) {
		return f(get<0>(std::forward<Tuple>(tuple)), accumulator);
	} else {
		return foldr(remove_back<1>(std::forward<Tuple>(tuple)), f(get<std::tuple_size_v<Tuple> - 1>(std::forward<Tuple>(tuple)), accumulator), f);
	}
}

/// Perform a foldl over the values of a tuple without an initial accumulator.
/**
 * If the tuple contains exactly one element, that element is returned unchanged.
 */
template<typename F, typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldr(Tuple && tuple, F && f) {
	static_assert(std::tuple_size_v<Tuple> > 0, "Can not foldl over an empty tuple without an initial accumulator value.");
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 1) {
		return get<0>(std::forward<Tuple>(tuple));
	} else {
		return foldr(remove_back<1>(std::forward<Tuple>(tuple)), get<std::tuple_size_v<Tuple> - 1>(std::forward<Tuple>(tuple)), f);
	}
}

}
