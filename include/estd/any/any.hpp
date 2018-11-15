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

#include <memory>

namespace estd {

namespace detail {
	/// Base interface for the storage used by any.
	struct any_storage_base {
		/// Virtual destructor so deleting through pointers to the base type works properly.
		virtual ~any_storage_base() = default;
	};

	/// Implementation of any_storage for specific types.
	template<typename T>
	struct any_storage : any_storage_base {
		T data;

		any_storage(T const & data) : data(data) {}
		any_storage(T      && data) : data(std::move(data)) {}
	};
}

/// A typed-erased unique pointer.
/**
 * Data is guaranteed to be stored on the heap.
 * As such, moving an instance of any does not move the underlying data.
 */
class any {
private:
	/// The heap storage to hold the wrapped value.
	std::unique_ptr<detail::any_storage_base> storage_;

public:
	/// Create an empty any instance that doesn't hold a value.
	any() = default;

	/// Construct an any instance.
	/**
	 * The given value will be copied into storage on the heap.
	 */
	template<typename T>
	explicit any(T const & value) :
		storage_{std::make_unique<detail::any_storage<T>>(value)}
	{}

	/// Construct an any instance.
	/**
	 * The given value will be moved into storage on the heap.
	 */
	template<typename T>
	explicit any(T && value) :
		storage_{std::make_unique<detail::any_storage<T>>(std::move(value))}
	{}

	/// Swap the values of two any instances.
	void swap(any & other) noexcept {
		storage_.swap(other.storage_);
	}

	/// Get a pointer to the held data if it is of type T, a nullptr otherwise.
	template<typename T>
	T * get() noexcept {
		auto typed = dynamic_cast<detail::any_storage<T> *>(storage_.get());
		if (!typed) return nullptr;
		return &typed->data;
	}

	/// Get a pointer to the held data if it is of type T, a nullptr otherwise.
	template<typename T>
	T const * get() const noexcept {
		auto typed = dynamic_cast<detail::any_storage<T> *>(storage_.get());
		if (!typed) return nullptr;
		return &typed->data;
	}

	/// Get a reference to the held data as type T, without checking the type.
	template<typename T>
	T & get_static() noexcept {
		auto typed = static_cast<detail::any_storage<T> *>(storage_.get());
		return typed->data;
	}

	/// Get a reference to the held data as type T, without checking the type.
	template<typename T>
	T const & get_static() const noexcept {
		auto typed = static_cast<detail::any_storage<T> *>(storage_.get());
		return typed->data;
	}

	/// Check if the any object has a value.
	bool has_value() const noexcept {
		return !!storage_;
	}

	/// Check if the any object has a value of the specified type.
	template<typename T>
	bool contains_type() const noexcept {
		return get<T>() != nullptr;
	}
};


}
