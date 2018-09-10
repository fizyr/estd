#pragma once
#include <cstdint>
#include <system_error>
#include <type_traits>

namespace estd {

enum class unspecified_errc : int {
	unspecified = -1,
};

namespace detail {
	inline class unspecified_error_category : public std::error_category {
		char const * name() const noexcept override { return "unspecified"; }

		std::string message(int code) const override {
			(void) code;
			return "unspecified error";
		}
	} unspecified_error_category_;
}

inline std::error_category & unspecified_error_category() {
	return detail::unspecified_error_category_;
}

inline std::error_code make_error_code(unspecified_errc code) {
	return std::error_code{int(code), unspecified_error_category()};
}

inline std::error_code unspecified_error(int value) {
	return std::error_code(value, unspecified_error_category());
}

}

template<> class std::is_error_code_enum<estd::unspecified_errc> : public std::true_type{};
