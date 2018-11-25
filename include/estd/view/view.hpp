/* Copyright 2018-2019 Fizyr B.V. - https://fizyr.com
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

#include "../traits/containers.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>

namespace estd {

/// A non-owning view on a range of elements.
template<typename T>
class view {
public:
	using value_type       = T;
	using reference        = T &;
	using const_reference  = T const &;
	using iterator         = T *;
	using const_iterator   = T const *;
	using difference_type  = std::ptrdiff_t;
	using size_type        = std::size_t;

private:
	/// True if this is a const view, false otherwise.
	constexpr static bool is_const_ = std::is_const_v<T>;

	/// Make a type const if this is a const view.
	template<typename C> using make_const_ = std::conditional<is_const_, C const, C>;

	/// The begin pointer.
	T * begin_;

	/// The end pointer.
	T * end_;

public:
	/// Create a view from a begin and end pointer.
	constexpr view(T * begin, T * end) : begin_{begin}, end_{end} {}

	/// Create a view from a begin pointer and size.
	constexpr view(T * begin, std::size_t count) : begin_{begin}, end_{begin + count} {}

	/// Create a view from a contiguous container.
	template<typename Container, typename = std::enable_if_t<is_contiguous_container<Container>>>
	constexpr view(Container & container) : view(container.data(), container.size()) {}

	/// Create a view from a contiguous container.
	template<typename Container, typename = std::enable_if_t<is_const_ && is_contiguous_container<Container>>>
	constexpr view(Container const & container) : view(container.data(), container.size()) {}

	/// Disallow creation of a view from pointers that are implicitly convertible, if the size of the other type is different.
	template<typename B, std::enable_if_t<sizeof(B) != sizeof(T)>> constexpr view(B * begin, B * end)     = delete;
	template<typename B, std::enable_if_t<sizeof(B) != sizeof(T)>> constexpr view(B * begin, std::size_t) = delete;

	/// Allow implicit conversion of a view<T> to a view<T const>.
	constexpr operator view<T const>() {
		return {cbegin(), cend()};
	}

	/// Get a pointer to the first element.
	constexpr T       *    begin() const { return begin_; }
	constexpr T       * &  begin()       { return begin_; }
	constexpr T const *   cbegin() const { return begin_; }

	/// Get a pointer directly past the last element.
	constexpr T       *    end() const { return end_; }
	constexpr T       * &  end()       { return end_; }
	constexpr T const *   cend() const { return end_; }

	/// Get a pointer to the data.
	constexpr T * data() const { return begin_; }

	/// Get the number of elements in the view.
	constexpr std::size_t size() const {
		return end_ - begin_;
	}

	/// Get the total size of the view in bytes.
	constexpr std::size_t byte_size() const {
		return size() * sizeof(T);
	}

	/// Get a reverse iterator for the first element in the reversed view.
	constexpr std::reverse_iterator<T *> rbegin() const {
		return std::make_reverse_iterator(end());
	}

	/// Get a reverse iterator for the last element in the reversed view.
	constexpr std::reverse_iterator<T *> rend() const {
		return std::make_reverse_iterator(begin());
	}

	/// Get a reverse iterator for the first element in the reversed view.
	constexpr std::reverse_iterator<T const *> crbegin() const {
		return std::make_reverse_iterator(cend());
	}

	/// Get a reverse iterator for the last element in the reversed view.
	constexpr std::reverse_iterator<T const *> crend() const {
		return std::make_reverse_iterator(cbegin());
	}

	/// Get a reference to an element by index, without bounds checking.
	constexpr T & operator[] (std::size_t i) const {
		return begin_[i];
	}

	/// Get a reference to an element by index, with bounds checking.
	/**
	 * \throws std::range_error if the index is out of bounds.
	 */
	T & at(std::size_t i) const {
		if (i >= size()) throw std::range_error("index " + std::to_string(i) + " out of range, view size is " + std::to_string(size()));
		return begin_[i];
	}

	/// Compare two views for equality.
	/**
	 * Two views are considered equal if the range of elements compare equal.
	 */
	bool operator==(view const & other) {
		return size() == other.size() && std::equal(begin(), end(), other.begin());
	}

	/// Compare two views for inequality.
	/**
	 * Two views are considered equal if the range of elements compare equal.
	 */
	bool operator!=(view const & other) {
		return !(*this == other);
	}
};

/// Compare a view<CharT> for equality with a std::basic_string_view<CharT>
template<typename CharT, typename Traits>
bool operator==(view<CharT> a, std::basic_string_view<CharT, Traits> b) {
	return std::basic_string_view<CharT, Traits>{a.data(), a.size()} == b;
}

/// Compare a view<CharT> for equality with a std::basic_string_view<CharT>
template<typename CharT, typename Traits>
bool operator==(std::basic_string_view<CharT, Traits> a, view<CharT> b) {
	return b == a;
}

/// Compare a view<CharT> for inequality with a std::basic_string_view<CharT>
template<typename CharT, typename Traits>
bool operator!=(view<CharT> a, std::basic_string_view<CharT, Traits> b) {
	return !(a == b);
}

/// Compare a view<CharT> for inequality with a std::basic_string_view<CharT>
template<typename CharT, typename Traits>
bool operator!=(std::basic_string_view<CharT, Traits> a, view<CharT> b) {
	return !(a == b);
}

/// Typedef for a view of const bytes.
using byte_view = view<std::uint8_t const>;

/// Typedef for a view of mutable bytes.
using mut_byte_view = view<std::uint8_t>;

}
