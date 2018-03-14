#pragma once
#include "../../type_name.hpp"
#include <forward_list>

namespace estd::detail {

template<typename T>
struct pretty_type_name<std::forward_list<T>> {
	operator std::string() {
		return "std::forward_list<" + type_name<T>() + ">";
	}
};

}
