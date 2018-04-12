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

#pragma once

namespace estd {

namespace detail {
	struct make_ref_t {
		template<typename T> constexpr T       &   operator() (T       &  a) const { return a; }
		template<typename T> constexpr T       &&  operator() (T       && a) const { return static_cast<T &&>(a); }
		template<typename T> constexpr T const &   operator() (T const &  a) const { return a; }
		template<typename T> constexpr T const &&  operator() (T const && a) const { return static_cast<T const &&>(a); }
	};

	struct make_cref_t {
		template<typename T> constexpr T const &  operator() (T       &  a) const { return a; }
		template<typename T> constexpr T const && operator() (T       && a) const { return static_cast<T const &&>(a); }
		template<typename T> constexpr T const &  operator() (T const &  a) const { return a; }
		template<typename T> constexpr T const && operator() (T const && a) const { return static_cast<T const &&>(a); }
	};
}

/// Functor that takes a single value and returns a reference to it.
/**
 * The mapping of input arguments to return type is as follows:
 *   T &        -> T &
 *   T &&       -> T &&
 *   T const &  -> T const &
 *   T const && -> T const &&
 */
constexpr detail::make_ref_t  make_ref;

/// Functor that takes a single value and returns a constant reference to it.
/**
 * The mapping of input arguments to return type is as follows:
 *   T &        -> T const &
 *   T &&       -> T const &
 *   T const &  -> T const &
 *   T const && -> T const &
 */
constexpr detail::make_cref_t make_cref;

}
