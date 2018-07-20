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

#include "../catch.hpp"

namespace estd {


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
}

}