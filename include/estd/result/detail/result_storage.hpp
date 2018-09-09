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
#include "../in_place.hpp"
#include "../../traits/type_traits.hpp"
#include "copyable.hpp"

#include <type_traits>
#include <utility>

namespace estd {
namespace detail {

// Wrapper to transparantly store references in result<T, E>
template<typename T>
struct lvalue_wrapper {
	T value_;

	lvalue_wrapper(T const & value) : value_{value} {}
	lvalue_wrapper(T      && value) : value_{std::move(value)} {}

	template<typename ...Args>
	explicit lvalue_wrapper(Args && ...args) : value_(std::forward<Args>(args)...) {}

	T       &  access()       &  { return value_; }
	T const &  access() const &  { return value_; }
	T       && access()       && { return std::move(value_); }
	T const && access() const && { return std::move(value_); }
};

// Wrapper to transparantly store references in result<T, E>
template<typename T>
struct lvalue_ref_wrapper {
	T * value_;
	lvalue_ref_wrapper(T & value) : value_{&value} {}
	T & access() const { return *value_; }
};

// Wrapper to transparantly store rvalue references in result<T, E>
template<typename T>
struct rvalue_ref_wrapper {
	T * value_;
	rvalue_ref_wrapper(T && value) : value_{&value} {}
	T && access() const { return std::move(*value_); }
};

// Determine the storage type in a result<T, E> for some type.
template<typename T> struct result_type_storage_impl       { using type = lvalue_wrapper<T>; };
template<typename T> struct result_type_storage_impl<T  &> { using type = lvalue_ref_wrapper<T>; };
template<typename T> struct result_type_storage_impl<T &&> { using type = rvalue_ref_wrapper<T>; };
template<typename T> using result_type_storage = typename result_type_storage_impl<T>::type;

template<typename T, typename E>
class result_storage_base;

// Create a type derived from result_storage_base that is just as copyable as T and E.
template<typename T, typename E>
struct result_storage_maybe_copyable {
	constexpr static bool copy_constructible = std::is_copy_constructible<T>{} && std::is_copy_constructible<E>{};
	constexpr static bool copy_assignable    = std::is_copy_assignable<T>{}    && std::is_copy_assignable<E>{};
	constexpr static int copyable            = copy_constructible && copy_assignable;

	constexpr static bool move_constructible = std::is_move_constructible<T>{} && std::is_move_constructible<E>{};
	constexpr static bool move_assignable    = std::is_move_assignable<T>{}    && std::is_move_assignable<E>{};
	constexpr static int movable             = move_constructible && move_assignable;

	class type :
		public result_storage_base<T, E>,
		private non_copyable<copy_constructible, copyable>,
		private non_movable<move_constructible, movable>
	{
		using result_storage_base<T, E>::result_storage_base;
	};
};

template<typename T, typename E>
using result_storage = typename result_storage_maybe_copyable<T, E>::type;

template<typename T, typename E>
class result_storage_base {
	using Valid = result_type_storage<T>;
	using Error = result_type_storage<E>;

	union {
		Valid valid_;
		Error error_;
	};

	bool is_valid_;

	template<typename T2, typename E2>
	friend class result_storage_base;

	template<typename T2, typename E2>
	constexpr static bool explicitly_convertible_ = std::is_constructible<T, T2>::value && std::is_constructible<E, E2>::value;

public:
	template<typename... Args>
	result_storage_base(in_place_valid_t, Args && ... args) {
		new (&valid_) Valid(std::forward<Args>(args)...);
		is_valid_ = true;
	}

	template<typename... Args>
	result_storage_base(in_place_error_t, Args && ... args) {
		new (&error_) Error(std::forward<Args>(args)...);
		is_valid_ = false;
	}

	result_storage_base(result_storage_base const & other) {
		is_valid_ = other.is_valid_;
		if (is_valid_) new (&valid_) Valid(other.valid_.access());
		else           new (&error_) Error(other.error_.access());
	}

	result_storage_base(result_storage_base && other) {
		is_valid_ = other.is_valid_;
		if (is_valid_) new (&valid_) Valid(std::move(other.valid_).access());
		else           new (&error_) Error(std::move(other.error_).access());
	}

	/// Allow explicit conversion from ErrorOr<T2, E2>.
	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result_storage_base(result_storage_base<T2, E2> const & other) {
		is_valid_ = other.is_valid_;
		if (is_valid_) new (&valid_) Valid(other.valid_.access());
		else          new (&error_) Error(other.error_.access());
	}

	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result_storage_base(result_storage_base<T2, E2> && other) {
		is_valid_ = other.is_valid_;
		if (is_valid_) new (&valid_) Valid(std::move(other.valid_).access());
		else           new (&error_) Error(std::move(other.error_).access());
	}

	result_storage_base & operator= (result_storage_base const & other) {
		if (is_valid_ != other.is_valid_) {
			this->~result_storage_base();
			new (this) result_storage_base(other);
		} else {
			if (is_valid_) valid_= other.valid_;
			else           error_= other.error_;
		}
		return *this;
	}

	result_storage_base & operator= (result_storage_base && other) {
		if (is_valid_ != other.is_valid_) {
			this->~result_storage_base();
			new (this) result_storage_base(std::move(other));
		} else {
			if (is_valid_) valid_= std::move(other.valid_);
			else           error_= std::move(other.error_);
		}
		return *this;
	}

	~result_storage_base() {
		if (is_valid_) valid_.~Valid();
		else           error_.~Error();
	}

	bool valid() const { return is_valid_; }

	decltype(auto) as_valid()        & { return valid_.access(); }
	decltype(auto) as_valid()       && { return std::move(valid_).access(); }
	decltype(auto) as_valid() const  & { return valid_.access(); }

	decltype(auto) as_error()        & { return error_.access(); }
	decltype(auto) as_error()       && { return std::move(error_).access(); }
	decltype(auto) as_error() const  & { return error_.access(); }
};


}}
