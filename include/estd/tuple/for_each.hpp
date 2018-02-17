#pragma once
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace estd {

namespace detail {
	template<std::size_t I, typename F, typename Tuple>
	constexpr std::size_t tuple_for_each_impl(Tuple && tuple, F && f) {
		if constexpr (I == std::tuple_size_v<std::decay_t<Tuple>>) {
			return I;
		} else {
			using std::get;
			using R = decltype(std::invoke(f, get<I>(std::forward<Tuple>(tuple))));
			if constexpr(std::is_same_v<R, void>) {
				std::invoke(f, get<I>(std::forward<Tuple>(tuple)));
			} else {
				if (!std::invoke(f, get<I>(std::forward<Tuple>(tuple)))) return I;
			}
			return tuple_for_each_impl<I + 1>(std::forward<Tuple>(tuple), std::forward<F>(f));
		}
	}
}

/// Execute a functor for each tuple element in order.
/**
 * The functor may optionally return a value to abort the loop before all elements are processed.
 * If the functor returns a value it is tested in boolean context.
 * If the value compares as false, the loop is stopped.
 *
 * Returns the index of the last element that cause the loop to halt,
 * or the size of the tuple if no element caused the loop to stop.
 */
template<typename F, typename Tuple>
constexpr std::size_t for_each(Tuple && tuple, F && f) {
	return detail::tuple_for_each_impl<0>(std::forward<Tuple>(tuple), std::forward<F>(f));
}

}
