#pragma once
#include "../../type_name.hpp"
#include <set>

namespace estd::detail {

template<typename T>
struct pretty_type_name<std::set<T>> {
	operator std::string() {
		return "std::set<" + type_name<T>() + ">";
	}
};

template<typename T>
struct pretty_type_name<std::multiset<T>> {
	operator std::string() {
		return "std::multiset<" + type_name<T>() + ">";
	}
};

}
