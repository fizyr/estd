#include "tuple.hpp"
#include "../traits/is_tuple.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

namespace estd {

namespace detail {
	template<typename Tuple, typename Predicate, std::size_t ...I>
	constexpr auto tuple_all_impl(Tuple && tuple, Predicate && pred, std::index_sequence<I...>) {
		return (pred(std::get<I>(tuple)) && ...);
	}

	template<typename Tuple, typename Predicate, std::size_t ...I>
	constexpr auto tuple_any_impl(Tuple && tuple, Predicate && pred, std::index_sequence<I...>) {
		return (pred(std::get<I>(tuple)) || ...);
	}

	template<typename Tuple, typename Predicate, std::size_t ...I>
	constexpr auto tuple_none_impl(Tuple && tuple, Predicate && pred, std::index_sequence<I...>) {
		return (!pred(std::get<I>(tuple)) && ...);
	}

	template<typename Tuple, typename Predicate, std::size_t ...I>
	constexpr auto tuple_count_impl(Tuple && tuple, Predicate && pred, std::index_sequence<I...>) {
		return (bool(pred(std::get<I>(tuple))) + ...);
	}

	template<typename Tuple, typename Predicate, std::size_t ...I>
	constexpr auto tuple_count_not_impl(Tuple && tuple, Predicate && pred, std::index_sequence<I...>) {
		return (!pred(std::get<I>(tuple)) + ...);
	}
}

/// Check if all elements of a tuple satisfy a given predicate.
template<typename Tuple, typename Predicate, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto all_of(Tuple && tuple, Predicate && pred) {
	return detail::tuple_all_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(pred), tuple_index_sequence<Tuple>);
}

/// Check if any elements of a tuple satisfy a given predicate.
template<typename Tuple, typename Predicate, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto any_of(Tuple && tuple, Predicate && pred) {
	return detail::tuple_any_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(pred), tuple_index_sequence<Tuple>);
}

/// Check if none elements of a tuple satisfy a given predicate.
template<typename Tuple, typename Predicate, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto none_of(Tuple && tuple, Predicate && pred) {
	return detail::tuple_none_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(pred), tuple_index_sequence<Tuple>);
}

/// Count the number tuple elements that satisfy a given predicate.
template<typename Tuple, typename Predicate, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto count_if(Tuple && tuple, Predicate && pred) {
	return detail::tuple_count_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(pred), tuple_index_sequence<Tuple>);
}

/// Count the number tuple elements that don't satisfy a given predicate.
template<typename Tuple, typename Predicate, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto count_if_not(Tuple && tuple, Predicate && pred) {
	return detail::tuple_count_not_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(pred), tuple_index_sequence<Tuple>);
}

/// Perform a left fold over the values of a tuple with an initial accumulator.
/**
 * If the tuple is empty, the initial accumulator value is returned unchanged.
 */
template<typename F, typename Tuple, typename Accumulator, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldl(Tuple && tuple, Accumulator && accumulator, F && f) {
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 0) {
		return accumulator;
	} else {
		return foldl(slice_ref<1>(std::forward<Tuple>(tuple)), f(accumulator, front(std::forward<Tuple>(tuple))), f);
	}
}

/// Perform a left fold over the values of a tuple without an initial accumulator.
/**
 * If the tuple contains exactly one element, that element is returned unchanged.
 */
template<typename F, typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldl(Tuple && tuple, F && f) {
	static_assert(std::tuple_size_v<Tuple> > 0, "Can not perform a fold over an empty tuple without an initial accumulator value.");
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 1) {
		return get<0>(std::forward<Tuple>(tuple));
	} else {
		return foldl(slice_ref<1>(std::forward<Tuple>(tuple)), front(std::forward<Tuple>(tuple)), f);
	}
}

/// Perform a right fold over the values of a tuple with an initial accumulator.
/**
 * If the tuple is empty, the initial accumulator value is returned unchanged.
 */
template<typename F, typename Tuple, typename Accumulator, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldr(Tuple && tuple, Accumulator && accumulator, F && f) {
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 0) {
		return accumulator;
	} else {
		return foldr(slice_ref<0, -1>(std::forward<Tuple>(tuple)), f(back(std::forward<Tuple>(tuple)), accumulator), f);
	}
}

/// Perform a right fold over the values of a tuple without an initial accumulator.
/**
 * If the tuple contains exactly one element, that element is returned unchanged.
 */
template<typename F, typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
constexpr auto foldr(Tuple && tuple, F && f) {
	static_assert(std::tuple_size_v<Tuple> > 0, "Can not perform a fold over an empty tuple without an initial accumulator value.");
	using std::get;
	if constexpr(std::tuple_size_v<Tuple> == 1) {
		return get<0>(std::forward<Tuple>(tuple));
	} else {
		return foldr(slice_ref<0, -1>(std::forward<Tuple>(tuple)), back(std::forward<Tuple>(tuple)), f);
	}
}

}
