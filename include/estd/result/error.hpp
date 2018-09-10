#pragma once
#include "./unspecified_category.hpp"

#include <initializer_list>
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

/// General purpose error.
/**
 * An error combines an std::error_code with a description stack.
 *
 * The description stack can be used to add more details to the raw error code, to be displayed to a human.
 * The description stack is not meant to be inspected programmatically to determine how to process the error.
 * In general, code not meant for logging or displaying the error should only look at the error code.
 */
struct error {
	/// The error code to indicate the type of error.
	std::error_code code;

	/// A stack of descriptions to clarify the error.
	/**
	 * In general, descriptions are pushed to the back.
	 * This means that information closer to the error point is at the front of the vector.
	 */
	std::vector<std::string> description;

	/// Construct an empty error representing success.
	error() {}

	/// Construct an error with a code and description stack.
	error(std::error_code code, std::vector<std::string> description = {}) :
		code{code},
		description(std::move(description)) {}

	/// Construct an error with a code and description stack.
	error(std::error_code code, std::initializer_list<std::string> description) :
		code{code},
		description(std::vector<std::string>{std::move(description)}) {}

	/// Construct an error with a code and description.
	error(std::error_code code, std::string description) :
		code{code},
		description{std::move(description)} {}

	/// Construct an error with a code and description stack.
	template<typename T, typename = std::enable_if_t<can_make_error_code<T>>>
	error(T code, std::vector<std::string> description = {}) : error{make_error_code(code), std::move(description)} {}

	/// Construct an error with a code and description stack.
	template<typename T, typename = std::enable_if_t<can_make_error_code<T>>>
	error(T code, std::initializer_list<std::string> description) : error{make_error_code(code), std::vector<std::string>{std::move(description)}} {}

	/// Construct an error with a code and description.
	template<typename T, typename = std::enable_if_t<can_make_error_code<T>>>
	error(T code, std::string description) : error{make_error_code(code), std::move(description)} {}

	/// Create an unspecified error with a description.
	explicit error(std::string description) : error{unspecified_errc::unspecified, std::move(description)} {};

	///// Create an unspecified error with a description stack.
	explicit error(std::vector<std::string> description) : error{unspecified_errc::unspecified, std::move(description)} {}

	/// Create an unspecified error with a description stack.
	explicit error(std::initializer_list<std::string> description) : error(std::vector<std::string>{std::move(description)}) {};

	/// Check if this error represents an error and not sucess.
	explicit operator bool() const noexcept {
		return !!code;
	}

	/// Check if the error code is the same as another one.
	bool operator== (std::error_code other) const { return code == other; }
	bool operator!= (std::error_code other) const { return code != other; }

	/// Check if the error code is equivalent to an error condition.
	bool operator== (std::error_condition other) const { return code == other; }
	bool operator!= (std::error_condition other) const { return code != other; }

	/// Check if the error code is equivalent to an std::errc.
	bool operator== (std::errc other) const { return code == other; }
	bool operator!= (std::errc other) const { return code != other; }

	/// Create a new error with the same description but a different error code.
	error with_code(std::error_code code) const & { return {code, description}; }
	error with_code(std::error_code code)      && { return {code, std::move(description)}; }

	/// Create a new error with the same code, but with a description pushed to the stack.
	error push_description(std::string action) const & {
		std::vector<std::string> new_trace = description;
		new_trace.push_back(std::move(action));
		return {code, new_trace};
	}

	/// Create a new error with the same code, but with a description pushed to the stack.
	/**
	 * This overload destroys the original error to re-use the description stack.
	 */
	error push_description(std::string action) && {
		std::vector<std::string> new_trace = std::move(description);
		new_trace.push_back(std::move(action));
		return {code, std::move(new_trace)};
	}

	/// Format the error code as a string.
	/**
	 * Returns a string in the format of:
	 *  {category name} error {error number}: {error message}
	 */
	std::string format_code() const {
		// If the error is from the special unspecified category,
		// only format the category name and integer value, not the message.
		if (code.category() == unspecified_error_category()) {
			return std::string{code.category().name()} + " error " + std::to_string(code.value());
		}
		return std::string{code.category().name()} + " error " + std::to_string(code.value()) + ": " + code.message();
	}

	/// Format the description stack without the error code.
	/**
	 * Returns a string in the format of:
	 *   {description N}: {description N-1}: ... {description 1}
	 */
	std::string format_description(std::size_t additional_size = 0) const {
		// Determine the total message size.
		std::size_t size = additional_size;
		for (std::string const & string : description) {
			size += string.size() + 2;
		}
		if (size > 2) size -= 2;

		// Make a string with the right capacity.
		std::string result;
		result.reserve(size);

		// Add all descriptions, back to front.
		for (auto i = description.rbegin(); i != description.rend(); ++i) {
			if (&*i == &description.front()) {
				result += *i;
			} else {
				result += *i + ": ";
			}
		}

		return result;
	}

	/// Format the error code along with the description stack.
	/**
	 * Returns a string in the format of:
	 *   {description N}: {description N-1}: ... {description 1}: {error}
	 *
	 * Where {error} is the output of `format_code()`.
	 */
	std::string format() const {
		// If the error is the special unspecified error and there is a description,
		// show only the description.
		if (code == unspecified_errc::unspecified && !description.empty()) {
			return format_description();
		}

		std::string code_description = format_code();
		if (description.empty()) return code_description;

		std::string result = format_description(code_description.size() + 2);
		result += ": ";
		result += code_description;
		return result;
	}
};

class error_exception : public std::exception {
	estd::error error_;
	std::string formatted_;

public:
	error_exception(estd::error error) : error_{std::move(error)} {
		formatted_ = error_.format();
	}

	char const * what() const noexcept override {
		return formatted_.c_str();
	}

	estd::error const & error() const & { return error_; }
	estd::error       & error()       & { return error_; }
	estd::error      && error()      && { return std::move(error_); }

	std::error_code const & code() const { return error_.code; }
};

inline error_exception make_default_exception(error error) {
	return error_exception{std::move(error)};
}

}
