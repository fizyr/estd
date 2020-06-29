#pragma once

#include "./result.hpp"
#include "./error.hpp"

#include <catch2/catch.hpp>

/// StringMaker for estd::result<T, E>.
template<typename T, typename E>
struct Catch::StringMaker<estd::result<T, E>> {
	static std::string convert(estd::result<T, E> const & value) {
		if (value) return "Ok(" + StringMaker<T>::convert(*value) + ")";
		else return "Err(" + StringMaker<E>::convert(value.error_unchecked()) + ")";
	}
};

/// StringMaker for estd::result<void, E>.
template<typename E>
struct Catch::StringMaker<estd::result<void, E>> {
	static std::string convert(estd::result<void, E> const & value) {
		if (value) return "Ok(void)";
		else return "Err(" + StringMaker<E>::convert(value.error_unchecked()) + ")";
	}
};

/// StringMaker for std::error_code.
template<>
struct Catch::StringMaker<std::error_code> {
	static std::string convert(std::error_code const & error) {
		return std::string(error.category().name()) + " error " + std::to_string(error.value()) + ": " + error.message();
	}
};

/// StringMaker for estd::error.
template<>
struct Catch::StringMaker<estd::error> {
	static std::string convert(estd::error const & error) {
		return error.format();
	}
};
