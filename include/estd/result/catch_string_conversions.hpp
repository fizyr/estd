#pragma once

#include "./result.hpp"
#include "./error.hpp"

// This header adds Catch2 string conversions for estd::result<T, E> and estd::error,
// for pretty messages with the Catch2 unit testing framework.

#if !defined(CATCH_VERSION_MAJOR) && !defined(TWOBLUECUBES_CATCH_HPP_INCLUDED)
// You must already have included the Catch2 header, since we can not know how to include it properly.
// Some projects embed `catch.hpp`, other use a system include.
#error "You must include this header after including the Catch2 header (usually called catch.hpp)."
#else
template<typename T, typename E>
struct Catch::StringMaker<estd::result<T, E>> {
	static std::string convert(estd::result<T, E> const & value) {
		if (value) return "Ok(" + StringMaker<T>::convert(*value) + ")";
		else return "Err(" + StringMaker<E>::convert(value.error_unchecked()) + ")";
	}
};

template<typename E>
struct Catch::StringMaker<estd::result<void, E>> {
	static std::string convert(estd::result<void, E> const & value) {
		if (value) return "Ok(void)";
		else return "Err(" + StringMaker<E>::convert(value.error_unchecked()) + ")";
	}
};

template<>
struct Catch::StringMaker<std::error_code> {
	static std::string convert(std::error_code const & error) {
		return std::string(error.category().name()) + " error " + std::to_string(error.value()) + ": " + error.message();
	}
};

template<>
struct Catch::StringMaker<estd::error> {
	static std::string convert(estd::error const & error) {
		return error.format();
	}
};
#endif
