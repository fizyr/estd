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

#include "convert/convert.hpp"
#include "convert/traits.hpp"

# if __has_include(<catch2/catch_test_macros.hpp>)
#   include <catch2/catch_test_macros.hpp>
# else
#   include <catch2/catch.hpp>
# endif

namespace estd {

TEST_CASE("implicitly convertible primitive can be converted using convert<T>(foo)", "[convert]") {

	REQUIRE(convert<int>(5.0)  == 5);
	REQUIRE(convert<int>(6.0f) == 6);
	REQUIRE(convert<int>(6u)   == 6);
	REQUIRE(convert<int>(6ul)  == 6);
	REQUIRE(convert<int>(6l)   == 6);

	REQUIRE(convert<float>(5.5)  == 5.5f);
	REQUIRE(convert<float>(6.2f) == 6.2f);
	REQUIRE(convert<float>(6u)   == 6.f);
	REQUIRE(convert<float>(6ul)  == 6.f);
	REQUIRE(convert<float>(6l)   == 6.f);
}

TEST_CASE("explicitly convertible structs can be converted using convert<T>(foo)", "[convert]") {
	struct Foo {
		int foo;
		bool operator==(Foo const & other) const { return foo == other.foo; }
	};

	struct Bar {
		int bar;
		explicit operator Foo() { return Foo{bar}; }
	};

	REQUIRE(convert<Foo>(Bar{5}) == Foo{5});
	REQUIRE(convert<Foo>(Bar{6}) == Foo{6});
}

}
