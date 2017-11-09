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

protected:
	using no_ref_T  = std::remove_reference_t<T>;
	using no_ref_E  = std::remove_reference_t<E>;
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
	template<typename... Args, typename = std::enable_if_t<std::is_constructible_v<result_storage, in_place_valid_t, Args &&...>>>
	result(in_place_valid_t, Args && ... args) : data_{in_place_valid, std::forward<Args>(args)...} {}

	/// Construct an error result in place.
	template<typename... Args, typename = std::enable_if_t<std::is_constructible_v<result_storage, in_place_error_t, Args &&...>>>
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
	no_ref_T const & operator* () const & { return data_.as_ok(); }
	no_ref_T       & operator* ()       & { return data_.as_ok(); }
	no_ref_T      && operator* ()      && { return std::move(data_.as_ok()); }

	no_ref_T const * operator-> () const { return &data_.as_ok(); }
	no_ref_T       * operator-> ()       { return &data_.as_ok(); }

	/// Compare with another result.
	template<typename T2, typename E2>
	bool operator==(result<T2, E2> const & other) const {
		return data_ == other.data_;
	}

	template<typename T2, typename E2>
	bool operator!=(result<T2, E2> const & other) const {
		return !(*this == other);
	}

	/// Get the contained value.
	/**
	 * \throws make_default_exception(error()) if the result is not valid.
	 */
	no_ref_T const & value() const { ensure_value(); return data_.as_ok(); }
	no_ref_T       & value()       { ensure_value(); return data_.as_ok(); }

	/// Get the contained value.
	/**
	 * \throws make_exception(error()) if the result is not valid.
	 */
	template<typename MakeException>
	no_ref_T const & value(MakeException && make_exception) const {
		ensure_value(std::forward<MakeException>(make_exception));
		return data_.as_ok();
	}

	/// Get the contained value.
	/**
	 * \throws make_exception(error()) if the result is not valid.
	 */
	template<typename MakeException>
	no_ref_T & value(MakeException make_exception) {
		ensure_value(std::forward<MakeException>(make_exception));
		return data_.as_ok();
	}

	/// Get the contained value or a fallback if the result is not valid.
	no_ref_T const & value_or(no_ref_T const & fallback) const noexcept { if (!*this) return fallback; return data_.as_ok(); }
	no_ref_T       & value_or(no_ref_T       & fallback)       noexcept { if (!*this) return fallback; return data_.as_ok(); }

	/// Get the held error without checking if it is valid.
	no_ref_E const & error_unchecked() const { return data_.as_error(); }
	no_ref_E       & error_unchecked()       { return data_.as_error(); }

	/// Get the held error.
	/**
	 * \throws a logic error if the result is valid.
	 */
	no_ref_E const & error() const { ensure_error(); return data_.as_error(); }
	no_ref_E       & error()       { ensure_error(); return data_.as_error(); }

	/// Get the held error, or a fallback value if the result is valid.
	no_ref_E const & error_or(no_ref_E const  & fallback) const noexcept { if (*this) return fallback; return data_.as_error(); }
	no_ref_E       & error_or(no_ref_E        & fallback)       noexcept { if (*this) return fallback; return data_.as_error(); }
	no_ref_E         error_or(no_ref_E       && fallback)       noexcept { return error_or(fallback); }

	/// Get the held error, or a static default constructed error if the result is valid.
	no_ref_E const & error_or() const noexcept(noexcept(E{})) { static E default_error; return error_or(default_error); }

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
	std::optional<E> error_;

public:
	/// Construct a valid result in-place.
	result(in_place_valid_t) : error_{} {};

	/// Construct an error result in-place.
	template<typename... Args>
	result(in_place_error_t, Args && ... args) : error_{std::forward<Args>(args)...} {}


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
	E const & error_unchecked() const { return *error_; }
	E       & error_unchecked()       { return *error_; }

	/// Get the held error.
	/**
	 * \throws a logic error if the result is valid.
	 */
	E const & error() const { ensure_error(); return *error_; }
	E       & error()       { ensure_error(); return *error_; }

	/// Get the held error, a fallback value if the result doesn't hold an error.
	E const & error_or(E const  & fallback) const { if (*this) return fallback; return *error_; }
	E       & error_or(E        & fallback)       { if (*this) return fallback; return *error_; }
	E         error_or(E       && fallback) const { return error_or(fallback); }

	/// Get the held error, or a static default constructed error.
	E error_or() const noexcept(noexcept(E{})) { static E default_error; return error_or(default_error); }

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
