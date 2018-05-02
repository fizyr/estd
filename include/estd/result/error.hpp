#pragma once
#include <string>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>

namespace estd {

namespace detail {
	template<typename T, typename = void>
	struct can_make_error_code : std::false_type {};

	template<typename T>
	struct can_make_error_code<T, std::void_t<decltype(make_error_code(T{}))>> : std::true_type {};
}

template<typename T>
constexpr bool can_make_error_code = detail::can_make_error_code<T>::value;

struct error {
	/// The error code to indicate the type of error.
	std::error_code code;

	/// A stack of descriptions to clarify the error.
	std::vector<std::string> description;

	/// Construct an empty error representing success.
	error() {}

	/// Construct an error with a code and description.
	error(std::error_code code, std::vector<std::string> description = {}) :
		code{code},
		description(std::move(description)) {}

	/// Construct an error with a code and description.
	error(std::error_code code, std::string description) :
		code{code},
		description{std::move(description)} {}

	/// Construct an error with a code and description.
	template<typename T, typename = std::enable_if_t<can_make_error_code<T>>>
	error(T code, std::vector<std::string> description = {}) :
		error{make_error_code(code), std::move(description)} {}

	/// Check if this error represents an error and not sucess.
	explicit operator bool() const noexcept {
		return !!code;
	}

	// Create a new error with the same description but a different error code.
	error with_code(std::error_code code) const & { return {code, description}; }
	error with_code(std::error_code code)      && { return {code, std::move(description)}; }

	// Create a new error with the same code, but with a description pushed to the stack.
	error push_trace(std::string action) const & {
		std::vector<std::string> new_trace = description;
		new_trace.push_back(std::move(action));
		return {code, new_trace};
	}

	error push_trace(std::string action) && {
		std::vector<std::string> new_trace = std::move(description);
		new_trace.push_back(std::move(action));
		return {code, std::move(new_trace)};
	}

	std::string format_code() const {
		return std::string{code.category().name()} + " error " + std::to_string(code.value()) + ": " + code.message();
	}

	std::string format() const {
		std::size_t size = 0;
		for (std::string const & string : description) size += string.size() + 2;
		std::string result;
		result.reserve(size + 64);

		for (auto i = description.rbegin(); i != description.rend(); ++i) result += *i + ": ";
		result += format_code();
		return result;
	}
};

}
