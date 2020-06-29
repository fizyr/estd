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

#include "result/error.hpp"
#include "traits/is_comparible.hpp"
#include "result/catch_string_conversions.hpp"

#include <catch2/catch.hpp>

namespace estd {

class test_category_ : public std::error_category {
	char const * name() const noexcept override {
		return "test";
	}

	std::string message(int code) const override {
		switch (code) {
			case 1: return "one";
			case 2: return "two";
			case 3: return "three";
		}
		return "unknown error";
	}
} test_category_;

std::error_category const & test_category() {
	return test_category_;
}


TEST_CASE("error compares to std::error_code", "[result]") {
	static_assert(is_comparible<error, std::error_code>);
	REQUIRE(error{std::errc::invalid_argument} == make_error_code(std::errc::invalid_argument));
	REQUIRE(error{std::errc::invalid_argument} != make_error_code(std::errc::address_in_use));
}

TEST_CASE("error compares to std::error_condition", "[result]") {
	static_assert(is_comparible<error, std::errc>);
	REQUIRE(error{std::errc::invalid_argument} == make_error_condition(std::errc::invalid_argument));
	REQUIRE(error{std::errc::invalid_argument} != make_error_condition(std::errc::address_in_use));
}

TEST_CASE("error compares to std::errc", "[result]") {
	static_assert(is_comparible<error, std::error_condition>);
	REQUIRE(error{std::errc::invalid_argument} == std::errc::invalid_argument);
	REQUIRE(error{std::errc::invalid_argument} != std::errc::address_in_use);
}

TEST_CASE("error is not comparible with itself", "[result]") {
	static_assert(is_comparible<error, error> == false);
	CHECK((is_comparible<error, error>) == false);
}

TEST_CASE("unspecified errors have the right category and value", "[result]") {
	CHECK(error("aap").code == unspecified_errc::unspecified);
	CHECK(error({"aap", "noot"}).code == unspecified_errc::unspecified);
}

TEST_CASE("error.format_code() formats correctly", "[result]") {
	std::error_code test_code = {1, test_category()};

	CHECK(error(test_code).format_code() == "test error 1: one");
	SECTION("error description does not influence format_code()") {
		CHECK(error(test_code, "aap").format_code() == "test error 1: one");
	}

	SECTION("errors of unspecified category only print the category name and value") {
		CHECK(error(unspecified_errc::unspecified).format_code() == "unspecified error -1");
		CHECK(error({1, unspecified_error_category()}).format_code() == "unspecified error 1");
	}
}

TEST_CASE("error.format_description() formats correctly", "[result]") {
	CHECK(error(std::errc::invalid_argument).format_description() == "");
	CHECK(error(std::errc::invalid_argument, "aap").format_description() == "aap");
	CHECK(error(std::errc::invalid_argument, {"aap", "noot"}).format_description() == "noot: aap");
}

TEST_CASE("error.format() formats correctly", "[result]") {
	std::error_code test_code = {1, test_category()};

	CHECK(error(test_code).format() == "test error 1: one");
	CHECK(error(test_code, "aap").format() == "aap: test error 1: one");
	CHECK(error(test_code, {"aap", "noot"}).format() == "noot: aap: test error 1: one");

	SECTION("unspecified errors don't include the code") {
		CHECK(error("aap").format() == "aap");
		CHECK(error({"aap", "noot"}).format() == "noot: aap");
		SECTION("unless they have no description") {
			CHECK(error(unspecified_errc::unspecified).format() == "unspecified error -1");
		}
	}
}

}
