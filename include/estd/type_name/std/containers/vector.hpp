#pragma once
#include "../../type_name.hpp"
#include <vector>

namespace estd::detail {

template<typename T>
struct pretty_type_name<std::vector<T>> {
	operator std::string() {
		return "std::vector<" + type_name<T>() + ">";
	}
};

}
