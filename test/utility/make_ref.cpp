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

#include "utility/make_ref.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

namespace estd {

TEST_CASE("make ref", "[utility]") {
	int a = 5;
	int const b = 6;
	REQUIRE(&make_ref(a) == &a);
	REQUIRE(&make_ref(b) == &b);
	REQUIRE( make_ref(1) == 1);
	static_assert_same<decltype(make_ref(a)), int &>();
	static_assert_same<decltype(make_ref(b)), int const &>();
	static_assert_same<decltype(make_ref(1)), int &&>();
	static_assert_same<decltype(make_ref(static_cast<int const &&>(1))), int const &&>();
}

TEST_CASE("make cref", "[utility]") {
	int a = 5;
	int const b = 6;
	REQUIRE(&make_cref(a) == &a);
	REQUIRE(&make_cref(b) == &b);
	REQUIRE( make_cref(1) == 1);
	static_assert_same<decltype(make_cref(a)), int const &>();
	static_assert_same<decltype(make_cref(b)), int const &>();
	static_assert_same<decltype(make_cref(1)), int const &&>();
	static_assert_same<decltype(make_cref(static_cast<int const &&>(1))), int const &&>();
}

}
