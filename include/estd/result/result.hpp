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
#include "in_place.hpp"
#include "detail/result_storage.hpp"

#include <stdexcept>
#include <type_traits>
#include <utility>
#include <system_error>
#include <optional>

namespace estd {

/// Make an exception object for an error.
/**
 * Used as the default by result.value().
 */
template<typename T>
T && make_default_exception (T && in) { return in; }

/// Make a std::system_error for std::error_code.
inline std::system_error make_default_exception (std::error_code error) { return error; }

template<typename T, typename E>
class result {
public:
	using Type  = T;
	using Error = E;
	static_assert(!std::is_rvalue_reference_v<T>, "rvalue references are not supported in result<T, E>");
	static_assert(!std::is_rvalue_reference_v<E>, "rvalue references are not supported in result<T, E>");

protected:
	using decayed_T = std::decay_t<T>;
	using decayed_E = std::decay_t<E>;
	using result_storage = detail::result_storage<T, E>;

	/// Result storage.
	result_storage data_;

	template<typename T2, typename E2>
	friend class result;

	/// True if T and E are not implicitly convertible to each-other.
	constexpr static bool unambiguous_types_ = !std::is_same<T, E>{} && !std::is_convertible<E, T>{} && !std::is_convertible<T, E>{};

	template<typename Arg> constexpr static bool allow_implicit_valid_conversion_ = unambiguous_types_ && std::is_constructible_v<result_storage, in_place_valid_t, Arg>;
	template<typename Arg> constexpr static bool allow_implicit_error_conversion_ = unambiguous_types_ && std::is_constructible_v<result_storage, in_place_error_t, Arg>;

	template<typename T2, typename E2>
	constexpr static bool explicitly_convertible_ = std::is_constructible<detail::result_storage<T, E>, detail::result_storage<T2, E2>>::value;

public:
	/// Construct a valid result in place.
	template<typename... Args>
	result(in_place_valid_t, Args && ... args) : data_{in_place_valid, std::forward<Args>(args)...} {}

	/// Construct an error result in place.
	template<typename... Args>
	result(in_place_error_t, Args && ... args) : data_{in_place_error, std::forward<Args>(args)...} {}

	/// Allow implicit conversion from T and E only if T and E are not implicitly convertible to eachother.
	template<char B = 1, typename = std::enable_if_t<B && allow_implicit_valid_conversion_<decayed_T        &>>> result(decayed_T        & value) : data_{in_place_valid, value} {}
	template<char B = 1, typename = std::enable_if_t<B && allow_implicit_valid_conversion_<decayed_T const  &>>> result(decayed_T const  & value) : data_{in_place_valid, value} {}
	template<char B = 1, typename = std::enable_if_t<B && allow_implicit_valid_conversion_<decayed_T       &&>>> result(decayed_T       && value) : data_{in_place_valid, std::move(value)} {}
	template<char B = 1, typename = std::enable_if_t<B && allow_implicit_valid_conversion_<decayed_T const &&>>> result(decayed_T const && value) : data_{in_place_valid, std::move(value)} {}
	template<bool B = 1, typename = std::enable_if_t<B && allow_implicit_error_conversion_<decayed_E        &>>> result(decayed_E        & error) : data_{in_place_error, error} {}
	template<bool B = 1, typename = std::enable_if_t<B && allow_implicit_error_conversion_<decayed_E const  &>>> result(decayed_E const  & error) : data_{in_place_error, error} {}
	template<bool B = 1, typename = std::enable_if_t<B && allow_implicit_error_conversion_<decayed_E       &&>>> result(decayed_E       && error) : data_{in_place_error, std::move(error)} {}
	template<bool B = 1, typename = std::enable_if_t<B && allow_implicit_error_conversion_<decayed_E const &&>>> result(decayed_E const && error) : data_{in_place_error, std::move(error)} {}

	/// Allow explicit conversion from result<T2, E2>.
	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result(result<T2, E2> && other) : data_{std::move(other.data_)} {}

	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result(result<T2, E2> const & other) : data_{other.data} {}

	/// Check if the result is a valid value.
	bool valid() const { return data_.valid(); }

	/// Check if the result is a valid value.
	explicit operator bool() const { return valid(); }

	/// Get the contained value without checking if it is valid.
	add_lref <T> operator* ()       & { return data_.as_valid(); }
	add_clref<T> operator* () const & { return data_.as_valid(); }
	add_rref <T> operator* ()      && { return std::move(data_.as_valid()); }

	std::remove_reference_t<T> const * operator-> () const { return &data_.as_valid(); }
	std::remove_reference_t<T>       * operator-> ()       { return &data_.as_valid(); }

	/// Compare with another result.
	template<typename T2, typename E2> bool operator==(result<T2, E2> const & other) const { return data_ == other.data_; }
	template<typename T2, typename E2> bool operator!=(result<T2, E2> const & other) const { return data_ != other.data_; }

	/// Get the contained value.
	/**
	 * \throws make_default_exception(error()) if the result is not valid.
	 */
	add_lref <T> value()       & { ensure_value(); return data_.as_valid(); }
	add_clref<T> value() const & { ensure_value(); return data_.as_valid(); }
	add_rref <T> value()      && { return std::move(value()); }

	/// Get the contained value.
	/**
	 * \throws make_exception(error()) if the result is not valid.
	 */
	template<typename F> add_lref<T>  value(F && make_exception)       & { ensure_value(std::forward<F>(make_exception)); return data_.as_valid(); }
	template<typename F> add_clref<T> value(F && make_exception) const & { ensure_value(std::forward<F>(make_exception)); return data_.as_valid(); }
	template<typename F> add_rref<T>  value(F && make_exception)      && { std::move(value(std::forward<F>(make_exception))); }

	/// Get the contained value or a fallback if the result is not valid.
	add_lref<T>  value_or(add_lref<T>  fallback)       & noexcept { if (!*this) return fallback; return data_.as_valid(); }
	add_clref<T> value_or(add_clref<T> fallback) const & noexcept { if (!*this) return fallback; return data_.as_valid(); }
	add_rref<T>  value_or(add_rref<T>  fallback)      && noexcept { std::move(value_or(fallback)); }
	template<int B = 1, typename = std::enable_if_t<B && !std::is_reference_v<E>>>
	add_lref<T>  value_or(add_lref<T>  fallback)      && noexcept { std::move(value_or(fallback)); }

	/// Get the held error without checking if it is valid.
	add_lref<E>   error_unchecked()       & { return data_.as_error(); }
	add_clref<E>  error_unchecked() const & { return data_.as_error(); }
	add_rref<E>   error_unchecked()      && { return std::move(data_.as_error()); }

	/// Get the held error.
	/**
	 * \throws a logic error if the result is valid.
	 */
	add_lref <E> error()       & { ensure_error(); return data_.as_error(); }
	add_clref<E> error() const & { ensure_error(); return data_.as_error(); }
	add_rref <E> error()      && { ensure_error(); return std::move(data_.as_error()); }

	/// Get the held error, or a fallback value if the result is valid.
	add_lref<E>  error_or(add_lref<E>   fallback)       & noexcept { if (*this) return fallback; return data_.as_error(); }
	add_clref<E> error_or(add_clref<E>  fallback) const & noexcept { if (*this) return fallback; return data_.as_error(); }
	add_rref<E>  error_or(add_rref<E>   fallback)      && noexcept { std::move(error_or(fallback)); }
	template<int B = 1, typename = std::enable_if_t<B && !std::is_reference_v<E>>>
	add_lref<E>  error_or(add_lref<E>   fallback)      && noexcept { std::move(error_or(fallback)); }

	/// Get the held error, or a static default constructed error if the result is valid.
	add_clref<E> error_or() const noexcept(noexcept(E{})) { static E default_error; return error_or(default_error); }

private:
	/// \throws make_default_exception(error()) if the result is not valid.
	void ensure_value() const {
		if (!*this) throw make_default_exception(data_.as_error());
	}

	/// \throws make_exception(error()) if the result is not valid.
	template<typename MakeException>
	void ensure_value(MakeException && make_exception) const {
		if (!*this) throw std::forward<MakeException>(make_exception)(data_.as_error());
	}

	/// \throws logic_error if the result is valid.
	void ensure_error() const {
		if (*this) throw std::logic_error("attempted to access error of a valid result");
	}
};

template<typename E>
class result<void, E> {
public:
	using Type  = void;
	using Error = E;

protected:
	/// The error storage.
	std::optional<detail::result_type_storage<E>> error_;

public:
	/// Construct a valid result in-place.
	result(in_place_valid_t) : error_{} {};

	/// Construct an error result in-place.
	template<typename... Args>
	result(in_place_error_t, Args && ... args) : error_{std::in_place, std::forward<Args>(args)...} {}


	/// Allow implicit conversion from an error value.
	result(E const  & error_value) : result{in_place_error, error_value} {}
	result(E       && error_value) : result{in_place_error, std::move(error_value)} {}

	/// Check if the result is valid.
	explicit operator bool() const { return !error_; }

	/// \throws error() if the result is not valid.
	void operator* () const { return value(); }

	/// \throws make_default_exception(error()) if the result is not valid.
	void value() const { ensure_value(); }

	/// \throws make_exception(error()) if the result is not valid.
	template<typename MakeException>
	void value(MakeException && make_exception) const { ensure_value(std::forward<MakeException>(make_exception)); }

	/// Get the held error without checking if it is valid.
	add_lref <E> error_unchecked()       & { return *error_; }
	add_clref<E> error_unchecked() const & { return *error_; }
	add_rref <E> error_unchecked()      && { return std::move(error_unchecked()); }

	/// Get the held error.
	/**
	 * \throws a logic error if the result is valid.
	 */
	add_lref <E> error()       & { ensure_error(); return *error_; }
	add_clref<E> error() const & { ensure_error(); return *error_; }
	add_rref <E> error()      && { std::move(error()); }

	/// Get the held error, a fallback value if the result doesn't hold an error.
	add_lref <E> error_or(add_lref <E> fallback)       & { if (*this) return fallback; return *error_; }
	add_clref<E> error_or(add_clref<E> fallback) const & { if (*this) return fallback; return *error_; }
	add_rref <E> error_or(add_rref <E> fallback)      && { std::move(error_or(fallback)); }

	/// Get the held error, or a static default constructed error.
	add_clref<E> error_or() const noexcept(noexcept(E{})) { static E default_error; return error_or(default_error); }

private:
	/// \throws make_default_exception(error()) if the result is not valid.
	void ensure_value() const {
		if (!*this) throw make_default_exception(*error_);
	}

	/// \throws make_exception(error()) if the result is not valid.
	template<typename MakeException>
	void ensure_value(MakeException && make_exception) const {
		if (!*this) throw std::forward<MakeException>(make_exception)(*error_);
	}

	/// \throws logic_error if the result is valid.
	void ensure_error() const {
		if (*this) throw std::logic_error("attempted to access error of a valid result");
	}
};

/// Allow direct comparison with T if T and E aren't the same types.
template<typename T, typename E, typename = std::enable_if_t<!std::is_same<T, E>{}>>
bool operator==(result<T, E> const & result, T const & raw) {
	return !!result && *result == raw;
}
template<typename T, typename E, typename = std::enable_if_t<!std::is_same<T, E>{}>>
bool operator==(T const & raw, result<T, E> const & result) {
	return result == raw;
}
template<typename T, typename E, typename = std::enable_if_t<!std::is_same<T, E>{}>>
bool operator!=(result<T, E> const & result, T const & raw) {
	return !(result == raw);
}
template<typename T, typename E, typename = std::enable_if_t<!std::is_same<T, E>{}>>
bool operator!=(T const & raw, result<T, E> const & result) {
	return !(result == raw);
}

/// Allow direct comparison with E if T and E aren't the same types.
template<typename T, typename E, typename = std::enable_if_t<!std::is_same<T, E>{}>>
bool operator==(result<T, E> const & result, E const & raw) {
	return !result && result.error_unchecked() == raw;
}
template<typename T, typename E, typename = std::enable_if_t<!std::is_same<T, E>{}>>
bool operator==(E const & raw, result<T, E> const & result) {
	return result == raw;
}
template<typename T, typename E, typename = std::enable_if_t<!std::is_same<T, E>{}>>
bool operator!=(result<T, E> const & result, E const & raw) {
	return !(result == raw);
}
template<typename T, typename E, typename = std::enable_if_t<!std::is_same<T, E>{}>>
bool operator!=(E const & raw, result<T, E> const & result) {
	return !(result == raw);
}
}
