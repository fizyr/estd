#pragma once
#include "demangle.hpp"

#include <typeinfo>
#include <string>

namespace estd {

/// Get the full name of type T.
template<typename T>
std::string full_type_name() {
	return demangle(typeid(T).name());
}

namespace detail {
	/// Specializable definition for the pretty name of a type.
	/**
	 * The specialized struct must have an operator std::string().
	 *
	 * Never call this directly. Always call type_name() instead,
	 * since that may also be specialized when partial specialization is not needed.
	 */
	template<T> struct pretty_type_name {
		operator std::string() {
			return full_type_name<T>();
		}
	};
}

/// Get the pretty name of type T.
/**
 * If you need to implement a partial specialization,
 * specialize struct detail::pretty_type_name instead.
 */
template<typename T>
std::string type_name() {
	return detail::pretty_type_name<T>();
}

}
