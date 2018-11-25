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
#include "../view/view.hpp"

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace estd {

/// A simple owning array type allocated on the heap.
template<typename T>
class heap_array {
public:
	using value_type       = T;
	using reference        = T &;
	using const_reference  = T const &;
	using iterator         = T *;
	using const_iterator   = T const *;
	using difference_type  = std::ptrdiff_t;
	using size_type        = std::size_t;

private:
	/// The data.
	std::unique_ptr<T[]> data_;

	/// The size of the array in elements.
	std::size_t size_;

public:
	/// Create an empty heap array (does not perform any heap allocation).
	constexpr heap_array() : data_{nullptr}, size_{0} {}

	/// Create a heap array from a raw std::unique_ptr<T[]> and a size.
	constexpr heap_array(std::unique_ptr<T[]> data, std::size_t size) : data_{std::move(data)}, size_{size} {}

	/// Create a heap array from list of values.
	/**
	 * Note that the values will be copied into the array,
	 * since initializer lists only give const access to their contents.
	 */
	constexpr heap_array(std::initializer_list<T> data) : heap_array(allocate(data.size())) {
		std::size_t i = 0;
		for (auto & entry : data) {
			data_[i++] = std::move(entry);
		}
	}

	/// Create a value-initialized array with a given size.
	static heap_array allocate(std::size_t size) {
		if (!size) return heap_array{};
		return {std::make_unique<T[]>(size), size};
	}

	/// Create a default-initialized array with a given size.
	static heap_array unitialized(std::size_t size) {
		if (!size) return heap_array{};
		// TODO: Should we do default initialization or really uninitialized?
		return {std::unique_ptr<T[]>(new T[size]), size};
	}

	/// Get a pointer to the first element.
	constexpr T       * begin()        noexcept { return data_.get(); }
	constexpr T const * begin()  const noexcept { return data_.get(); }
	constexpr T const * cbegin() const noexcept { return data_.get(); }

	/// Get a pointer directly past the last element.
	constexpr T       * end()        noexcept { return begin() + size(); }
	constexpr T const * end()  const noexcept { return begin() + size(); }
	constexpr T const * cend() const noexcept { return begin() + size(); }

	/// Get a pointer to the data.
	constexpr T       * data()        noexcept { return data_.get(); }
	constexpr T const * data()  const noexcept { return data_.get(); }

	/// Get the number of elements in the array.
	constexpr std::size_t size() const noexcept {
		return size_;
	}

	/// Check if the array is empty (has size 0).
	constexpr bool empty() const noexcept {
		return size_ == 0;
	}

	/// Get the total size of the array in bytes.
	constexpr std::size_t byte_size() const noexcept {
		return size() * sizeof(T);
	}

	/// Get a reverse iterator for the first element in the reversed array.
	constexpr std::reverse_iterator<T       *> rbegin()        noexcept { return std::make_reverse_iterator(end()); }
	constexpr std::reverse_iterator<T const *> rbegin()  const noexcept { return std::make_reverse_iterator(end()); }
	constexpr std::reverse_iterator<T const *> crbegin() const noexcept { return std::make_reverse_iterator(cend()); }

	/// Get a reverse iterator for the last element in the reversed array.
	constexpr std::reverse_iterator<T       *> rend()        noexcept { return std::make_reverse_iterator(begin()); }
	constexpr std::reverse_iterator<T const *> rend()  const noexcept { return std::make_reverse_iterator(begin()); }
	constexpr std::reverse_iterator<T const *> crend() const noexcept { return std::make_reverse_iterator(cbegin()); }

	/// Get a reference to an element by index, without bounds checking.
	constexpr T       & operator[] (std::size_t i)       noexcept { return begin()[i]; }
	constexpr T const & operator[] (std::size_t i) const noexcept { return begin()[i]; }

	/// Get a reference to an element by index, with bounds checking.
	/**
	 * \throws std::range_error if the index is out of bounds.
	 */
	T const & at(std::size_t i) const {
		if (i >= size()) throw std::range_error("index " + std::to_string(i) + " out of range, array size is " + std::to_string(size()));
		return begin()[i];
	}

	/// Get a reference to an element by index, with bounds checking.
	/**
	 * \throws std::range_error if the index is out of bounds.
	 */
	T & at(std::size_t i) {
		if (i >= size()) throw std::range_error("index " + std::to_string(i) + " out of range, array size is " + std::to_string(size()));
		return begin()[i];
	}

	/// Allow implicitly conversion to a non-owning view of the same data.
	constexpr operator view<T>() noexcept {
		return view<T>{data(), size()};
	}

	/// Allow implicitly conversion to a non-owning view of the same data.
	constexpr operator view<T const>() const noexcept {
		return view<T const>{data(), size()};
	}

	/// Compare two heap arrays for equality.
	/**
	 * Two heap arrays are equal if their ranges of elements are equal.
	 */
	bool operator==(heap_array const & other) {
		return size() == other.size() && std::equal(begin(), end(), other.begin());
	}

	/// Compare two heap arrays for inequality.
	/**
	 * Two heap arrays are equal if their ranges of elements are equal.
	 */
	bool operator!=(heap_array const & other) {
		return !(*this == other);
	}
};

/// Typedef for heap arrays of bytes.
using byte_heap_array = heap_array<std::uint8_t>;

}
