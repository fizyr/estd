#pragma once
#include <limits>
#include <type_traits>
#include <utility>

#include "../traits/is_integer_sequence.hpp"

namespace estd {


namespace {
	/// Concatenate two integer sequences.
	template<typename T, T... first, T... second>
	constexpr auto concat_two(std::integer_sequence<T, first...>, std::integer_sequence<T, second...>) {
		return std::integer_sequence<T, first..., second...>();
	}

	template<typename Head, typename... Tail>
	struct ConcatIntegerSequence {
		constexpr static auto concat(Head head, Tail... tail) {
			return concat_two(head, ConcatIntegerSequence<Tail...>::concat(tail...));
		}
	};

	template<typename Head>
	struct ConcatIntegerSequence<Head> {
		constexpr static auto concat(Head head) {
			return head;
		}
	};
}

/// Concatenate any amount of integer sequences.
template<typename... Sequences> constexpr
auto concat(Sequences... sequences)
-> std::enable_if_t<(is_integer_sequence<Sequences> && ...), decltype(ConcatIntegerSequence<Sequences...>::concat(sequences...))>
{
	return ConcatIntegerSequence<Sequences...>::concat(sequences...);
}

/// Remove N elements from the front of an integer sequence and return the remaining elements.
/**
 * Overload for empty integer sequences.
 */
template<std::size_t N, typename T>
constexpr auto remove_front(std::integer_sequence<T>) {
	static_assert(N == 0, "Tried to remove an element from an empty integer_sequence.");
	return std::integer_sequence<T>{};
}

/// Remove N elements from the front of an integer sequence and return the remaining elements.
template<std::size_t N, typename T, T head, T... tail>
constexpr auto remove_front(std::integer_sequence<T, head, tail...>) {
	static_assert(N <= sizeof...(tail) + 1, "Tried to remove an element from an empty integer_sequence.");
	if constexpr(N == 0) return std::integer_sequence<T, head, tail...>{};
	else return remove_front<N - 1>(std::integer_sequence<T, tail...>{});
}

/// Remove N elements from the back of an integer sequence and return the remaining elements.
/**
 * Overload for empty integer sequences.
 */
template<std::size_t N, typename T>
constexpr auto remove_back(std::integer_sequence<T>) {
	static_assert(N == 0, "Tried to remove an element from an empty integer_sequence.");
	return std::integer_sequence<T>{};
}

/// Remove N elements from the back of an integer sequence and return the remaining elements.
template<std::size_t N, typename T, T head, T... tail>
constexpr auto remove_back(std::integer_sequence<T, head, tail...>) {
	static_assert(N <= sizeof...(tail) + 1, "Tried to remove more elements than present from integer_sequence.");
	if constexpr(N == sizeof...(tail) + 1) {
		return std::integer_sequence<T>{};
	} else {
		return concat(std::integer_sequence<T, head>(), remove_back<N>(std::integer_sequence<T, tail...>()));
	}
}

/// Take a slice from an integer sequuence.
/**
 * \param Start The first index included in the slice.
 * \param End   The first index after Start not included in the slice.
 *
 * If either Start or End is negative, counting starts from the end of the original sequence.
 *
 * If End is std::numeric_limits<long int>::max() (the default), the slice extends to the end of the original sequence.
 *
 * Examples:
 *   slice<0>(sequence);       // Return the entire sequence.
 *   slice<1, 3>(sequence);    // Return elements 1 and 2.
 *   slice<-2>(sequence);      // Return the last two elements.
 *   slice<-2, -1>(sequence);  // Return the second-to-last element.
 *   slice< 0, -1>(sequence);  // Return everything except the last element.
 */
template<long int Start, long int End = std::numeric_limits<long int>::max(), typename T, T... values>
constexpr auto slice(std::integer_sequence<T, values...> sequence) {
	if constexpr(Start < 0) {
		return slice<sizeof...(values) + Start, End>(sequence);
	} else if constexpr(End < 0) {
		return slice<Start, sizeof...(values) + End>(sequence);
	} else if constexpr(End == std::numeric_limits<long int>::max()) {
		return remove_front<Start>(sequence);
	} else {
		static_assert(Start <= sizeof...(values), "Slice start index out of bounds for integer_sequence.");
		static_assert(End   <= sizeof...(values), "Slice end index out of bounds for integer_sequence.");
		static_assert(Start <= End,               "Slice start index is higher than slice end index.");
		return remove_back<sizeof...(values) - End>(remove_front<Start>(sequence));
	}
}

}
