#pragma once
#include <type_traits>

namespace estd {

template<typename T1, typename T2>
constexpr void static_assert_same() {
	static_assert(std::is_same_v<T1, T2>);
}

}
