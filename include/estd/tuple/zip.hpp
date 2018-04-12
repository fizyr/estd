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
#include "transform.hpp"

#include "../traits/is_tuple.hpp"
#include "../utility/parameter_pack.hpp"

#include <cstddef>
#include <tuple>

namespace estd {

namespace detail {
	template<std::size_t I, typename... Tuples>
	using zip_tuple_at_index_t = std::tuple<std::tuple_element_t<I, std::decay_t<Tuples>>...>;

	template<std::size_t I, typename... Tuples>
	zip_tuple_at_index_t<I, Tuples...> zip_tuple_at_index(Tuples && ...tuples) {
		return {std::get<I>(std::forward<Tuples>(tuples))...};
	}

	template<typename... Tuples, std::size_t... I>
	std::tuple<zip_tuple_at_index_t<I, Tuples...>...> tuple_zip(Tuples && ...tuples, std::index_sequence<I...>) {
		return {zip_tuple_at_index<I>(std::forward<Tuples>(tuples)...)...};
	}

}

/// Zip a number of tuples together into a tuple of tuples.
template<typename... Tuples, typename = std::enable_if_t<(is_tuple<Tuples> && ...)>>
auto zip(Tuples && ...tuples) {
	constexpr std::size_t Size = std::tuple_size_v<std::decay_t<parameter_pack_head<Tuples...>>>;
	static_assert(((std::tuple_size_v<std::decay_t<Tuples>> == Size) && ...), "Tuple size mismatch, can not zip.");
	return detail::tuple_zip<Tuples...>(std::forward<Tuples>(tuples)..., std::make_index_sequence<Size>());
}

/// Zip the given tuples, but as references instead of as the original types.
template<typename... Tuples>
constexpr auto zip_ref(Tuples && ...tuple) {
	return zip(tuple_ref(tuple)...);
}

/// Zip the given tuples, but as const references instead of as the original types.
template<typename... Tuples>
constexpr auto zip_cref(Tuples && ...tuple) {
	return zip(tuple_cref(tuple)...);
}

}
