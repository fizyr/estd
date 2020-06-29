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
#include "detail/map.hpp"
#include "../traits/is_comparible.hpp"

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
class [[nodiscard]] result {
public:
	using Type  = T;
	using Error = E;
	static_assert(!std::is_rvalue_reference_v<T>, "rvalue references are not supported in result<T, E>");
	static_assert(!std::is_rvalue_reference_v<E>, "rvalue references are not supported in result<T, E>");

protected:
	using DecayedT = std::decay_t<T>;
	using DecayedE = std::decay_t<E>;
	using result_storage = detail::result_storage<T, E>;

	using NonAbstractDecayedT = std::conditional_t<std::is_abstract_v<DecayedT>, int, DecayedT>;

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
	template<char B = 1, typename = std::enable_if_t<B && allow_implicit_valid_conversion_<DecayedT        &>>> result(DecayedT        & value) : data_{in_place_valid, value} {}
	template<char B = 1, typename = std::enable_if_t<B && allow_implicit_valid_conversion_<DecayedT const  &>>> result(DecayedT const  & value) : data_{in_place_valid, value} {}
	template<char B = 1, typename = std::enable_if_t<B && allow_implicit_valid_conversion_<DecayedT       &&>>> result(DecayedT       && value) : data_{in_place_valid, std::move(value)} {}
	template<char B = 1, typename = std::enable_if_t<B && allow_implicit_valid_conversion_<DecayedT const &&>>> result(DecayedT const && value) : data_{in_place_valid, std::move(value)} {}
	template<bool B = 1, typename = std::enable_if_t<B && allow_implicit_error_conversion_<DecayedE        &>>> result(DecayedE        & error) : data_{in_place_error, error} {}
	template<bool B = 1, typename = std::enable_if_t<B && allow_implicit_error_conversion_<DecayedE const  &>>> result(DecayedE const  & error) : data_{in_place_error, error} {}
	template<bool B = 1, typename = std::enable_if_t<B && allow_implicit_error_conversion_<DecayedE       &&>>> result(DecayedE       && error) : data_{in_place_error, std::move(error)} {}
	template<bool B = 1, typename = std::enable_if_t<B && allow_implicit_error_conversion_<DecayedE const &&>>> result(DecayedE const && error) : data_{in_place_error, std::move(error)} {}

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
	decltype(auto) operator* ()       & { return data_.as_valid(); }
	decltype(auto) operator* () const & { return data_.as_valid(); }
	decltype(auto) operator* ()      && { return std::move(data_).as_valid(); }

	std::remove_reference_t<T> const * operator-> () const { return &data_.as_valid(); }
	std::remove_reference_t<T>       * operator-> ()       { return &data_.as_valid(); }

	/// Get the contained value without checking if it is valid.
	decltype(auto) value_unchecked()       & { return data_.as_valid(); }
	decltype(auto) value_unchecked() const & { return data_.as_valid(); }
	decltype(auto) value_unchecked()      && { return std::move(data_).as_valid(); }

	/// Get the contained value.
	/**
	 * \throws make_default_exception(error()) if the result is not valid.
	 */
	decltype(auto) value()       & { ensure_value(); return data_.as_valid(); }
	decltype(auto) value() const & { ensure_value(); return data_.as_valid(); }
	decltype(auto) value()      && { ensure_value(); return std::move(data_).as_valid(); }

	/// Get the contained value.
	/**
	 * \throws make_exception(error()) if the result is not valid.
	 */
	template<typename F> decltype(auto) value(F && make_exception)       & { ensure_value(std::forward<F>(make_exception)); return data_.as_valid(); }
	template<typename F> decltype(auto) value(F && make_exception) const & { ensure_value(std::forward<F>(make_exception)); return data_.as_valid(); }
	template<typename F> decltype(auto) value(F && make_exception)      && { ensure_value(std::forward<F>(make_exception)); return std::move(data_).as_valid(); }

	/// Get the contained value or a fallback if the result is not valid.
	template<bool B = 1, typename = std::enable_if_t<B && !std::is_abstract_v<DecayedT>>>
	NonAbstractDecayedT value_or(NonAbstractDecayedT fallback) const {
		if (!*this) return std::move(fallback);
		return data_.as_valid();
	}

	/// Get the held error without checking if it is valid.
	decltype(auto) error_unchecked()       & { return data_.as_error(); }
	decltype(auto) error_unchecked() const & { return data_.as_error(); }
	decltype(auto) error_unchecked()      && { return std::move(data_).as_error(); }

	/// Get the held error.
	/**
	 * \throws a logic error if the result is valid.
	 */
	decltype(auto) error()       & { ensure_error(); return data_.as_error(); }
	decltype(auto) error() const & { ensure_error(); return data_.as_error(); }
	decltype(auto) error()      && { ensure_error(); return std::move(data_).as_error(); }

	/// Get the held error, or a fallback value if the result is valid.
	DecayedE error_or(DecayedE fallback) const {
		if (*this) return fallback;
		return data_.as_error();
	}

	/// Get the held error, or a default constructed error if the result is valid.
	std::remove_reference_t<E> error_or() const noexcept(noexcept(E{})) {
		if (*this) return std::remove_reference_t<E>{};
		return data_.as_error();
	}

	/// Apply a function to the value, or return the error unmodified.
	template<typename F> decltype(auto) map(F && func) const &  { return detail::map_result_value<true>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map(F && func)       &  { return detail::map_result_value<true>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map(F && func)       && { return detail::map_result_value<true>(std::move(*this), std::forward<F>(func)); }

	/// Apply a function to the value, or return the error unmodified.
	template<typename F> decltype(auto) map_no_decay(F && func) const &  { return detail::map_result_value<false>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_no_decay(F && func)       &  { return detail::map_result_value<false>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_no_decay(F && func)       && { return detail::map_result_value<false>(std::move(*this), std::forward<F>(func)); }

	/// Apply a function to the value, or return the error unmodified.
	template<typename F> decltype(auto) map_error(F && func) const &  { return detail::map_result_error<true>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_error(F && func)       &  { return detail::map_result_error<true>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_error(F && func)       && { return detail::map_result_error<true>(std::move(*this), std::forward<F>(func)); }

	/// Apply a function to the value, or return the error unmodified.
	template<typename F> decltype(auto) map_error_no_decay(F && func) const &  { return detail::map_result_error<false>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_error_no_decay(F && func)       &  { return detail::map_result_error<false>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_error_no_decay(F && func)       && { return detail::map_result_error<false>(std::move(*this), std::forward<F>(func)); }

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
class [[nodiscard]] result<void, E> {
public:
	using Type  = void;
	using Error = E;

protected:
	using DecayedE = std::decay_t<E>;

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
	bool valid() const { return !error_; }

	/// Check if the result is valid.
	explicit operator bool() const { return valid(); }

	/// \throws error() if the result is not valid.
	void operator* () const { return value(); }

	/// \throws make_default_exception(error()) if the result is not valid.
	void value() const { ensure_value(); }

	/// No-op for compatibility with result<T, E>.
	void value_unchecked() const {};

	/// \throws make_exception(error()) if the result is not valid.
	template<typename MakeException>
	void value(MakeException && make_exception) const { ensure_value(std::forward<MakeException>(make_exception)); }

	/// Get the held error without checking if it is valid.
	decltype(auto) error_unchecked()       & { return error_->access(); }
	decltype(auto) error_unchecked() const & { return error_->access(); }
	decltype(auto) error_unchecked()      && { return std::move(*error_).access(); }

	/// Get the held error.
	/**
	 * \throws a logic error if the result is valid.
	 */
	decltype(auto) error()       & { ensure_error(); return error_->access(); }
	decltype(auto) error() const & { ensure_error(); return error_->access(); }
	decltype(auto) error()      && { ensure_error(); return std::move(*error_).access(); }

	/// Get the held error, or a fallback value if the result doesn't hold an error.
	auto error_or(DecayedE fallback) const & { if (*this) return fallback; return error_->access(); }

	/// Get the held error, or a default constructed error if the result is valid.
	auto error_or() const noexcept(noexcept(E{})) {
		if (*this) return std::remove_reference_t<E>{};
		return error_->access();
	}

	/// Apply a function to the value, or return the error unmodified.
	template<typename F> decltype(auto) map(F && func) const &  { return detail::map_result_value<true>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map(F && func)       &  { return detail::map_result_value<true>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map(F && func)       && { return detail::map_result_value<true>(std::move(*this), std::forward<F>(func)); }

	/// Apply a function to the value, or return the error unmodified.
	template<typename F> decltype(auto) map_no_decay(F && func) const &  { return detail::map_result_value<false>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_no_decay(F && func)       &  { return detail::map_result_value<false>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_no_decay(F && func)       && { return detail::map_result_value<false>(std::move(*this), std::forward<F>(func)); }

	/// Apply a function to the value, or return the error unmodified.
	template<typename F> decltype(auto) map_error(F && func) const &  { return detail::map_result_error<true>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_error(F && func)       &  { return detail::map_result_error<true>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_error(F && func)       && { return detail::map_result_error<true>(std::move(*this), std::forward<F>(func)); }

	/// Apply a function to the value, or return the error unmodified.
	template<typename F> decltype(auto) map_error_no_decay(F && func) const &  { return detail::map_result_error<false>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_error_no_decay(F && func)       &  { return detail::map_result_error<false>(*this,            std::forward<F>(func)); }
	template<typename F> decltype(auto) map_error_no_decay(F && func)       && { return detail::map_result_error<false>(std::move(*this), std::forward<F>(func)); }

private:
	/// \throws make_default_exception(error()) if the result is not valid.
	void ensure_value() const {
		if (!*this) throw make_default_exception(error_->access());
	}

	/// \throws make_exception(error()) if the result is not valid.
	template<typename MakeException>
	void ensure_value(MakeException && make_exception) const {
		if (!*this) throw std::forward<MakeException>(make_exception)(error_->access());
	}

	/// \throws logic_error if the result is valid.
	void ensure_error() const {
		if (*this) throw std::logic_error("attempted to access error of a valid result");
	}
};

/// Compare two results with comparible different types.
template<typename T1, typename E1, typename T2, typename E2, typename = std::enable_if_t<is_comparible<T1, T2> && is_comparible<E1, E2>>>
bool operator==(result<T1, E1> const & a, result<T2, E2> const & b) {
	if (a.valid() && b.valid()) {
		return *a == *b;
	} else if (!a.valid() && !b.valid()) {
		return a.error_unchecked() == b.error_unchecked();
	} else {
		return false;
	}
}

template<typename T1, typename E1, typename T2, typename E2, typename = std::enable_if_t<is_comparible<T1, T2> && is_comparible<E1, E2>>>
bool operator!=(result<T1, E1> const & a, result<T2, E2> const & b) {
	return !(a == b);
}

/// Allow comparing with Raw if Raw is comparible with either T or E, but not both.
template<typename T, typename E, typename Raw, typename = std::enable_if_t<is_comparible<T, Raw> != is_comparible<E, Raw>>>
bool operator==(result<T, E> const & a, Raw const & b) {
	static_assert(is_comparible<T, Raw> || is_comparible<E, Raw>, "Raw is not comparible to either T or E");
	if constexpr(is_comparible<T, Raw> && !is_comparible<E, Raw> && !is_comparible<Raw, E>) {
		return a.valid() && *a == b;
	} else if constexpr (is_comparible<E, Raw> && !is_comparible<T, Raw> && !is_comparible<Raw, T>) {
		return !a.valid() && a.error_unchecked() == b;
	} else {
		static_assert(always_false<T, E, Raw>, "comparison between Raw and T or E would be ambiguous");
	}
}

template<typename T, typename E, typename Raw, typename = std::enable_if_t<is_comparible<T, Raw> != is_comparible<E, Raw>>>
bool operator==(Raw const & a, result<T, E> const & b) {
	return b == a;
}

/// Allow comparing with T2 if T2 is comparible with T but not with E.
template<typename T, typename E, typename Raw, typename = std::enable_if_t<is_comparible<T, Raw> != is_comparible<E, Raw>>>
bool operator!=(result<T, E> const & a, Raw const & b) { return !(a == b); }

template<typename T, typename E, typename Raw, typename = std::enable_if_t<is_comparible<T, Raw> != is_comparible<E, Raw>>>
bool operator!=(Raw const & a, result<T, E> const & b) {
	return !(b == a);
}

}
