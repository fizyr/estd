#pragma once

namespace estd {

namespace detail {
	struct make_ref_t {
		template<typename T> constexpr T       &   operator() (T       &  a) const { return a; }
		template<typename T> constexpr T       &&  operator() (T       && a) const { return static_cast<T &&>(a); }
		template<typename T> constexpr T const &   operator() (T const &  a) const { return a; }
		template<typename T> constexpr T const &&  operator() (T const && a) const { return static_cast<T const &&>(a); }
	};

	struct make_cref_t {
		template<typename T> constexpr T const &  operator() (T       &  a) const { return a; }
		template<typename T> constexpr T const && operator() (T       && a) const { return static_cast<T const &&>(a); }
		template<typename T> constexpr T const &  operator() (T const &  a) const { return a; }
		template<typename T> constexpr T const && operator() (T const && a) const { return static_cast<T const &&>(a); }
	};
}

/// Functor that takes a single value and returns a reference to it.
/**
 * The mapping of input arguments to return type is as follows:
 *   T &        -> T &
 *   T &&       -> T &&
 *   T const &  -> T const &
 *   T const && -> T const &&
 */
constexpr detail::make_ref_t  make_ref;

/// Functor that takes a single value and returns a constant reference to it.
/**
 * The mapping of input arguments to return type is as follows:
 *   T &        -> T const &
 *   T &&       -> T const &
 *   T const &  -> T const &
 *   T const && -> T const &
 */
constexpr detail::make_cref_t make_cref;

}
