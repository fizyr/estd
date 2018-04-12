/* Copyright 2017-2018 Fizyr B.V. - https://fizyr.com
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
