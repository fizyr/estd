#include <type_traits>

namespace estd {

/// Get a template parameter dependent true value.
template<typename...> constexpr bool always_true  = true;

/// Get a template parameter dependent false value.
template<typename...> constexpr bool always_false = false;

template<int...> using void_v = void;

namespace detail {
	template<typename T> struct make_ref_to_const_impl { static_assert(always_false<T>, "T is not a reference type"); };
	template<typename T> struct make_ref_to_const_impl<T        &> { using type = T const  &; };
	template<typename T> struct make_ref_to_const_impl<T       &&> { using type = T const &&; };
	template<typename T> struct make_ref_to_const_impl<T const &&> { using type = T const &&; };
}

/// Given a reference type, make a reference-to-const.
template<typename T> using make_ref_to_const = typename detail::make_ref_to_const_impl<T>::type;

// Equivalent to std::add_lvalue_reference_t<T>.
template<typename T> using add_lref  = std::add_lvalue_reference_t<T>;
// Equivalent to std::add_rvalue_reference_t<T>.
template<typename T> using add_rref  = std::add_rvalue_reference_t<T>;
// Equivalent to make_ref_to_const<add_lref<T>>.
template<typename T> using add_clref = make_ref_to_const<add_lref<T>>;
// Equivalent to make_ref_to_const<add_rref<T>>.
template<typename T> using add_crref = make_ref_to_const<add_rref<T>>;

}
