#pragma once
#include "../../type_name.hpp"

namespace estd::detail {

template<>
struct pretty_type_name<std::string> {
	operator std::string() {
		return "std::string";
	}
};

template<>
struct pretty_type_name<std::wstring> {
	operator std::string() {
		return "std::wstring";
	}
};

}
