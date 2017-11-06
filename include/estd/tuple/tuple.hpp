#pragma once
#include "../traits/is_tuple.hpp"
#include "../utility/integer_sequence.hpp"
#include "../utility/parameter_pack.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

namespace estd {

/// Make an index sequence for a tuple.
template<typename Tuple>
constexpr auto tuple_index_sequence() {
	return std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>();
}

/// Make an index sequence for a tuple.
template<typename Tuple>
constexpr auto tuple_index_sequence(Tuple const &) {
	return tuple_index_sequence<Tuple>();
}

/// Get the first element of a tuple.
template<typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
auto front(Tuple && tuple) -> decltype(std::get<0>(std::forward<Tuple>(tuple))) {
	return std::get<0>(std::forward<Tuple>(tuple));
}

/// Get the last element of a tuple.
template<typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
auto back(Tuple && tuple) -> decltype(std::get<std::tuple_size_v<std::decay_t<Tuple>> -1>(std::forward<Tuple>(tuple))) {
	return std::get<std::tuple_size_v<std::decay_t<Tuple>> -1>(std::forward<Tuple>(tuple));
}

/// Select elements from a tuple with an index sequence.
/**
 * Each tuple element will have the exact same type as the original tuple element.
 * If it was a reference, it will still be a reference.
 */
template<typename Tuple, std::size_t... I, typename = std::enable_if_t<is_tuple<Tuple>>>
auto select(Tuple && tuple, std::index_sequence<I...>) {
	using std::get;
	return std::tuple<std::tuple_element_t<I, Tuple>...>{get<I>(std::forward<Tuple>(tuple))...};
}

/// Select elements by reference from a tuple with an index sequence.
/**
 * Each tuple element be a reference to the original tuple element,
 * or simply the reference from the original tuple.
 */
template<typename Tuple, std::size_t... I, typename = std::enable_if_t<is_tuple<Tuple>>>
auto select_ref(Tuple && tuple, std::index_sequence<I...>) {
	using std::get;
	return std::tuple<decltype(get<I>(std::forward<Tuple>(tuple)))...>{get<I>(std::forward<Tuple>(tuple))...};
}

/// Take a slice of a tuple.
/**
 * \param Start The first index included in the slice.
 * \param End   The first index after Start not included in the slice.
 *
 * The types of each element in the returned slice is exactly the same as
 * the type of the element in the original tuple.
 * If an element was a reference, it will still be a reference in the slice.
 * If it was not a reference, the element is copied or moved into the slice.
 *
 * If either Start or End is negative, it is counted from the end of the original tuple.
 *
 * If End is std::numeric_limits<long int>::max() (the default),
 * the slice extends to the end of the original tuple.
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

/// Take a slice of a tuple by reference.
/**
 * \param Start The first index included in the slice.
 * \param End   The first index after Start not included in the slice.
 *
 * Each element in the slice will be a reference to the matching element
 * in the original tuple, unless it was already a reference.
 * In that case, the reference is simply copied.
 *
 * If either Start or End is negative, it is counted from the end of the original tuple.
 *
 * If End is std::numeric_limits<long int>::max() (the default),
 * the slice extends to the end of the original tuple.
 *
 * Examples:
 *   slice<0>(tuple);       // Return a copy of the entire tuple.
 *   slice<1, 3>(tuple);    // Return a tuple with elements 1 and 2.
 *   slice<-2>(tuple);      // Return a tuple with the last two elements.
 *   slice<-2, -1>(tuple);  // Return a tuple with the second-to-last element.
 *   slice<0, -1>(tuple);   // Return a tuple with everything except the last element.
 */
template<long int Start, long int End = std::numeric_limits<long int>::max(), typename Tuple, typename = std::enable_if_t<is_tuple<Tuple>>>
auto slice_ref(Tuple && tuple) {
	return select_ref(std::forward<Tuple>(tuple), slice<Start, End>(tuple_index_sequence<Tuple>()));
}

}
