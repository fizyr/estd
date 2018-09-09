#include <type_traits>

namespace estd {

/// Decay a type depending on a boolean condition.
template<bool Condition, typename T>
using decay_if = std::conditional_t<Condition, std::decay_t<T>, T>;

}
