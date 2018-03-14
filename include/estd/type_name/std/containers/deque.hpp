#pragma once
#include "../../type_name.hpp"
#include <deque>

namespace estd::detail {

template<typename T>
struct pretty_type_name<std::deque<T>> {
	operator std::string() {
		return "std::deque<" + type_name<T>() + ">";
	}
};

}
