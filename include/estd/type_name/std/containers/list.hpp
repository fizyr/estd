#pragma once
#include "../../type_name.hpp"
#include <list>

namespace estd::detail {

template<typename T>
struct pretty_type_name<std::list<T>> {
	operator std::string() {
		return "std::list<" + type_name<T>() + ">";
	}
};

}
