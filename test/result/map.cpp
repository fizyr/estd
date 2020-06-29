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

#include "result.hpp"
#include "tracker.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

#include "result/catch_string_conversions.hpp"

namespace estd {

struct Error {
	int code;

	Error() : code{0} {};
	explicit Error(int code) : code(code) {};
	explicit operator int() { return code; }

	bool operator== (Error const & other) const { return code == other.code; }
};

struct Foo {
	int code;

	Foo() : code{0} {};
	explicit Foo(int code) : code(code) {};
	explicit operator int() { return code; }

	bool operator== (Foo const & other) const { return code == other.code; }
};

TEST_CASE("map works as expected for non-reference results", "[result]") {
	result<int, Error> valid{in_place_valid, 1};
	result<int, Error> error{in_place_error, 3};

	auto add_one   = [] (int a)   { return a + 1; };
	auto add_error = [] (Error a) { return Error{int(a) + 1}; };

	SECTION("result.map() works") {
		static_assert_same<decltype(valid.map(add_one)),         estd::result<int, Error>>();
		static_assert_same<decltype(valid.map_error(add_error)), estd::result<int, Error>>();

		CHECK(valid.map(add_one) == 2);
		CHECK(error.map(add_one) == Error{3});

		CHECK(valid.map_error(add_error) == 1);
		CHECK(error.map_error(add_error) == Error{4});
	}

	SECTION("result.map_no_decay() works") {
		static_assert_same<decltype(valid.map_no_decay(add_one)),         estd::result<int, Error>>();
		static_assert_same<decltype(valid.map_error_no_decay(add_error)), estd::result<int, Error>>();

		CHECK(valid.map_no_decay(add_one) == 2);
		CHECK(error.map_no_decay(add_one) == Error{3});

		CHECK(valid.map_error_no_decay(add_error) == 1);
		CHECK(error.map_error_no_decay(add_error) == Error{4});
	}

	SECTION("returning void from a map also works") {
		auto make_void = [] (auto ...) {};
		static_assert_same<decltype(valid.map(make_void)), estd::result<void, Error>>();
		CHECK(valid.map(make_void));
	}
}

TEST_CASE("map works as expected for result<void, E>", "[result]") {
	result<void, Error> valid{in_place_valid};
	result<void, Error> error{in_place_error, 3};

	auto make_one  = [] ()   { return 1; };
	auto add_error = [] (Error a) { return Error{int(a) + 1}; };

	SECTION("result<void, E>.map() works") {
		static_assert_same<decltype(valid.map(make_one)),        estd::result<int, Error>>();
		static_assert_same<decltype(valid.map_error(add_error)), estd::result<void, Error>>();

		CHECK(valid.map(make_one) == 1);
		CHECK(error.map(make_one) == Error{3});

		CHECK(valid.map_error(add_error));
		CHECK(error.map_error(add_error) == Error{4});
	}

	SECTION("result<void, E>.map_no_decay() works") {
		static_assert_same<decltype(valid.map_no_decay(make_one)),        estd::result<int, Error>>();
		static_assert_same<decltype(valid.map_error_no_decay(add_error)), estd::result<void, Error>>();

		CHECK(valid.map_no_decay(make_one) == 1);
		CHECK(error.map_no_decay(make_one) == Error{3});

		CHECK(valid.map_error_no_decay(add_error));
		CHECK(error.map_error_no_decay(add_error) == Error{4});
	}

	SECTION("returning void from a map also works") {
		auto make_void = [] (auto ...) {};

		static_assert_same<decltype(valid.map(make_void)),       estd::result<void, Error>>();
		static_assert_same<decltype(valid.map_error(add_error)), estd::result<void, Error>>();

		CHECK(valid.map(make_void));
		CHECK(error.map_no_decay(make_one) == Error{3});

		CHECK(valid.map_error_no_decay(add_error));
		CHECK(error.map_error_no_decay(add_error) == Error{4});
	}
}

TEST_CASE("the mapping functions decay the result by default", "[result]") {
	result<Foo, Error> valid{in_place_valid, 1};
	result<Foo, Error> error{in_place_error, 3};

	auto code  = [] (auto & thing) -> auto & { return thing.code; };

	SECTION("result.map() decays the value") {
		result<int, Error> mapped = valid.map(code);
		REQUIRE(mapped);
		CHECK(mapped.value() == 1);
		CHECK(&mapped.value() != &valid->code);
	}

	SECTION("result.map_no_decay() does not decay the value") {
		result<int &, Error> mapped = valid.map_no_decay(code);
		REQUIRE(mapped);
		CHECK(mapped.value() == 1);
		CHECK(&mapped.value() == &valid->code);
	}

	SECTION("result.map_error() decays the error") {
		result<Foo, int> mapped = error.map_error(code);
		REQUIRE(!mapped);
		CHECK(mapped.error() == 3);
		CHECK(&mapped.error() != &error->code);
	}

	SECTION("result.map_error_no_decay() does not decay the error") {
		result<Foo, int &> mapped = error.map_error_no_decay(code);
		REQUIRE(!mapped);
		CHECK(mapped.error() == 3);
		CHECK(&mapped.error() == &error->code);
	}
}

}
