#pragma once
#include <type_traits>

namespace estd {

template<typename From, typename To, typename Type>
using replace_type = std::conditional_t<std::is_same_v<Type, From>, To, Type>;

}
