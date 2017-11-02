#pragma once
#include "../result.hpp"

namespace estd {

/// Default conversion tag.
/**
 * Used for conversion functions that can not fail.
 */
template<typename T>
struct To {
	using type = T;
};

/// Unsafe conversion tag.
/**
 * Used for conversion functions that return an result<T, E>.
 */
template<typename T, typename E>
struct Parse {
	using type = result<T, E>;
};

}
