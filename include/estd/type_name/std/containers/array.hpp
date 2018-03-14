#pragma once
#include "../../type_name.hpp"
#include <array>

namespace estd::detail {

template<typename T, std::size_t N>
struct pretty_type_name<std::array<T, N>> {
	operator std::string() {
		return "std::array<" + type_name<T>() + ", " + std::to_string(N) + ">";
	}
};

}
