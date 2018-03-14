#pragma once
#include "../../type_name.hpp"
#include <map>

namespace estd::detail {

template<typename Key, typename Value>
struct pretty_type_name<std::map<Key, Value>> {
	operator std::string() {
		return "std::map<" + type_name<Key>() + ", " + type_name<Value>() + ">";
	}
};

template<typename Key, typename Value>
struct pretty_type_name<std::multimap<Key, Value>> {
	operator std::string() {
		return "std::multimap<" + type_name<Key>() + ", " + type_name<Value>() + ">";
	}
};

}
