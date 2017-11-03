#include "result.hpp"

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include <type_traits>

namespace estd {

struct Error {
	int code;

	explicit Error(int code) : code(code) {};
	explicit operator int() { return code; }

	bool operator==(Error const & other) const {
		return code == other.code;
	}
};

struct Foo {};

struct DefaultConstructible { DefaultConstructible() {} };

struct ConvertibleToInt {
	int code;

	explicit ConvertibleToInt(int code) : code(code) {};
	operator int() { return code; }
};

struct ConvertibleFromInt {
	int code;

	ConvertibleFromInt(int code) : code(code) {};
	explicit operator int() { return code; }
};

struct ConvertibleToFromInt {
	int code;

	ConvertibleToFromInt(int code) : code(code) {};
	operator int() { return code; }
};

struct Unrelated{};

// Test implicit conversions when E is not convertible to T.
static_assert(std::is_convertible<int,   result<int,  Error>>{}, "");
static_assert(std::is_convertible<Error, result<int,  Error>>{}, "");

// Test implicit conversions when E is convertible to T.
static_assert(!std::is_convertible<int,                  result<int, ConvertibleToInt>    >{}, "");
static_assert(!std::is_convertible<ConvertibleToInt,     result<int, ConvertibleToInt>    >{}, "");
static_assert(!std::is_convertible<int,                  result<int, ConvertibleFromInt>  >{}, "");
static_assert(!std::is_convertible<ConvertibleFromInt,   result<int, ConvertibleFromInt>  >{}, "");
static_assert(!std::is_convertible<int,                  result<int, ConvertibleToFromInt>>{}, "");
static_assert(!std::is_convertible<ConvertibleToFromInt, result<int, ConvertibleToFromInt>>{}, "");

// Conversion from unrelated tytpe shouldn't work at all.
static_assert(!std::is_convertible<Unrelated, result<int, Error>>{}, "");

// Assert that result<T, E> can not be implicitly converted to T or E.
static_assert(!std::is_convertible<result<int, Error>, int  >{}, "");
static_assert(!std::is_convertible<result<int, Error>, Error>{}, "");


// Assert that E can be implicitly converted to result<void, E>.
static_assert(std::is_convertible<Error,                result<void, Error>               >{}, "");
static_assert(std::is_convertible<ConvertibleToInt,     result<void, ConvertibleToInt>    >{}, "");
static_assert(std::is_convertible<ConvertibleFromInt,   result<void, ConvertibleFromInt>  >{}, "");
static_assert(std::is_convertible<ConvertibleToFromInt, result<void, ConvertibleToFromInt>>{}, "");

// Assert that in_place_[error]_t can be inplicitly converted to result<T, E> if T and E are default constructible.
static_assert(std::is_convertible<in_place_valid_t,       result<DefaultConstructible, Error>>{}, "");
static_assert(std::is_convertible<in_place_error_t, result<void, DefaultConstructible>>{}, "");

// Assert that in_place_t can be inplicitly converted to result<void, E>.
static_assert(std::is_convertible<in_place_valid_t, result<void, Error>>{}, "");

// Assert that in_place_[error]_t can be inplicitly converted to result<T, E> if T and E are default constructible.
static_assert(std::is_constructible<result<Error, Foo>, result<int, Foo>>{}, "");
static_assert(std::is_constructible<result<Error, Error>, result<int, int>>{}, "");
static_assert(std::is_constructible<result<Foo, ConvertibleToInt>, result<Foo, int>>{}, "");
static_assert(std::is_constructible<result<int, int>, result<short, int>>{}, "");
static_assert(std::is_constructible<result<int, int>, result<short, short>>{}, "");
static_assert(not std::is_constructible<result<Foo, int>, result<int, int>>{}, "");
static_assert(not std::is_constructible<result<int, Foo>, result<int, int>>{}, "");

TEST_CASE("convert a result<A, B> to result<C, D> if compatible", "[result]") {
	REQUIRE((result<Error, Foo>{result<int, Foo>{in_place_valid, 10}}.value()) == Error{10});
	REQUIRE((result<Foo, Error>{result<Foo, int>{in_place_error, 11}}.error()) == Error{11});
}

}
