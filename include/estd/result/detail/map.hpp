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

#include "../in_place.hpp"
#include "../traits.hpp"

#include "../../traits/decay_if.hpp"

#include <type_traits>
#include <functional>

namespace estd {
	template<typename T, typename E> class result;
}

namespace estd::detail {

template<bool Decay, typename R, typename F>
auto map_result_value(R && result, F && func) {
	using T = estd::result_value_type<R>;
	using E = estd::result_error_type<R>;

	if constexpr (std::is_void_v<T>) {
		using value_type  = decay_if<Decay, std::invoke_result_t<F>>;
		using result_type = estd::result<value_type, E>;

		// Invalid result, return error as-is.
		if (!result) {
			if constexpr(std::is_same_v<E, void>) {
				return result_type{in_place_error};
			} else {
				return result_type{in_place_error, std::forward<R>(result).error_unchecked()};
			}
		}

		if constexpr (std::is_void_v<value_type>) {
			std::invoke(std::forward<F>(func));
			return result_type{in_place_valid};
		} else {
			return result_type{in_place_valid, std::invoke(std::forward<F>(func))};
		}

	} else {
		using value_type  = decay_if<Decay, std::invoke_result_t<F, decltype(std::forward<R>(result).value_unchecked())>>;
		using result_type = estd::result<value_type, E>;

		// Invalid result, return error as-is.
		if (!result) {
			if constexpr(std::is_same_v<E, void>) {
				return result_type{in_place_error};
			} else {
				return result_type{in_place_error, std::forward<R>(result).error_unchecked()};
			}
		}

		if constexpr (std::is_same_v<value_type, void>) {
			std::invoke(std::forward<F>(func), std::forward<R>(result).value_unchecked());
			return result_type{in_place_valid};
		} else {
			return result_type{in_place_valid, std::invoke(std::forward<F>(func), std::forward<R>(result).value_unchecked())};
		}

	}
}

template<bool Decay, typename R, typename F>
auto map_result_error(R && result, F && func) {
	using T = estd::result_value_type<R>;
	using E = estd::result_error_type<R>;

	if constexpr (std::is_void_v<E>) {
		using error_type  = decay_if<Decay, std::invoke_result_t<F>>;
		using result_type = estd::result<T, error_type>;

		// Valid result, return value as-is.
		if (result) {
			if constexpr(std::is_same_v<T, void>) {
				return result_type{in_place_valid};
			} else {
				return result_type{in_place_valid, std::forward<R>(result).value_unchecked()};
			}
		}

		if constexpr (std::is_same_v<error_type, void>) {
			std::invoke(std::forward<F>(func));
			return result_type{in_place_error};
		} else {
			return result_type{in_place_error, std::invoke(std::forward<F>(func))};
		}

	} else {
		using error_type  = decay_if<Decay, std::invoke_result_t<F, decltype(std::forward<R>(result).error_unchecked())>>;
		using result_type = estd::result<T, error_type>;

		// Valid result, return value as-is.
		if (result) {
			if constexpr(std::is_same_v<T, void>) {
				return result_type{in_place_valid};
			} else {
				return result_type{in_place_valid, std::forward<R>(result).value_unchecked()};
			}
		}

		if constexpr (std::is_same_v<error_type, void>) {
			std::invoke(std::forward<F>(func), std::forward<R>(result).error_unchecked());
			return result_type{in_place_error};
		} else {
			return result_type{in_place_error, std::invoke(std::forward<F>(func), std::forward<R>(result).error_unchecked())};
		}
	}
}

}
