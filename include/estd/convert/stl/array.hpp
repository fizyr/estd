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
#include "../traits.hpp"

#include <array>
#include <type_traits>

namespace estd {

/// Convert a array<F, N> to array<T, N>.
template<typename F, typename T, std::size_t N, typename Tag>
struct conversion<std::array<F, N>, std::array<T, N>, Tag> {
	using From = std::array<F, N>;
	using To   = std::array<T, N>;

	static constexpr bool possible = can_convert<F, T, Tag>;

	static To perform(From const & from) {
		static_assert(possible, "no conversion available for F and T");
		To result;
		for (std::size_t i = 0; i < N; ++i) result[i](convert<T, Tag>(from[i]));
		return result;
	}

	static To perform(From && from) {
		static_assert(possible, "no conversion available for F and T");
		std::array<T, N> result;
		for (std::size_t i = 0; i < N; ++i) result[i](convert<T, Tag>(std::move(from[i])));
		return result;
	}
};

/// Convert a array<F, N> to result<array<T, N>, E>.
template<typename F, typename T, typename E, std::size_t N, typename Tag>
struct conversion<std::array<F, N>, result<std::array<T, N>, E>, Tag> {
	using From = std::array<F, N>;
	using Raw  = std::array<T, N>;
	using To   = result<Raw, E>;

	static constexpr bool possible = can_parse<F, T, E, Tag>;

	static To perform(From const & from) {
		static_assert(possible, "no conversion available for F and result<T, E>");
		To result;
		for (std::size_t i = 0; i < N; ++i) {
			estd::result<T, E> converted = parse<T, E>(from[i]);
			if (!converted) return converted.error_unchecked();
			result[i] = std::move(*converted);
		}
		return result;
	}

	static To perform(From && from) {
		static_assert(possible, "no conversion available for F and result<T, E>");
		To result;
		for (std::size_t i = 0; i < N; ++i) {
			estd::result<T, E> converted = parse<T, E>(std::move(from[i]));
			if (!converted) return converted.error_unchecked();
			result[i] = std::move(*converted);
		}
		return result;
	}
};

}
