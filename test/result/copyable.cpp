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

#include "../catch.hpp"
#include "../static_assert_same.hpp"

#include "result/catch_string_conversions.hpp"

namespace estd {

struct NonCopyable {
	int code;
	explicit NonCopyable(int code) : code{code} {};

	NonCopyable(NonCopyable const &) = delete;
	NonCopyable(NonCopyable      &&) = default;
	NonCopyable & operator=(NonCopyable const &) = delete;
	NonCopyable & operator=(NonCopyable      &&) = default;
};

struct NonCopyConstructible {
	int code;

	explicit NonCopyConstructible(int code) : code{code} {};

	NonCopyConstructible(NonCopyConstructible const &) = delete;
	NonCopyConstructible(NonCopyConstructible      &&) = default;
	NonCopyConstructible & operator=(NonCopyConstructible const &) = default;
	NonCopyConstructible & operator=(NonCopyConstructible      &&) = default;
};

struct NonCopyAssignable {
	int code;

	explicit NonCopyAssignable(int code) : code{code} {};

	NonCopyAssignable(NonCopyAssignable const &) = default;
	NonCopyAssignable(NonCopyAssignable      &&) = default;
	NonCopyAssignable & operator=(NonCopyAssignable const &) = delete;
	NonCopyAssignable & operator=(NonCopyAssignable      &&) = default;
};

struct NonMovable {
	int code;

	explicit NonMovable(int code) : code{code} {};

	NonMovable(NonMovable const &) = delete;
	NonMovable(NonMovable      &&) = delete;
	NonMovable & operator=(NonMovable const &) = delete;
	NonMovable & operator=(NonMovable      &&) = delete;
};

struct NonMoveConstructible {
	int code;

	explicit NonMoveConstructible(int code) : code{code} {};

	NonMoveConstructible(NonMoveConstructible const &) = delete;
	NonMoveConstructible(NonMoveConstructible      &&) = delete;
	NonMoveConstructible & operator=(NonMoveConstructible const &) = default;
	NonMoveConstructible & operator=(NonMoveConstructible      &&) = default;
};

struct NonMoveAssignable {
	int code;

	explicit NonMoveAssignable(int code) : code{code} {};

	NonMoveAssignable(NonMoveAssignable const &) = default;
	NonMoveAssignable(NonMoveAssignable      &&) = default;
	NonMoveAssignable & operator=(NonMoveAssignable const &) = delete;
	NonMoveAssignable & operator=(NonMoveAssignable      &&) = delete;
};

static_assert(std::is_move_constructible< result<int, NonCopyable>         >{}, "");
static_assert(std::is_move_constructible< result<int, NonCopyConstructible>>{}, "");
static_assert(std::is_move_constructible< result<int, NonCopyAssignable>   >{}, "");
static_assert(std::is_move_assignable<    result<int, NonCopyAssignable>   >{}, "");
static_assert(std::is_move_assignable<    result<int, NonCopyConstructible>>{}, "");
static_assert(std::is_move_assignable<    result<int, NonCopyable>         >{}, "");

static_assert(not std::is_copy_constructible< result<int, NonCopyable>         >{}, "");
static_assert(not std::is_copy_constructible< result<int, NonCopyConstructible>>{}, "");
static_assert(    std::is_copy_constructible< result<int, NonCopyAssignable>   >{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonCopyAssignable>   >{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonCopyConstructible>>{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonCopyable>         >{}, "");

static_assert(not std::is_copy_constructible< result<int, NonMovable>          >{}, "");
static_assert(not std::is_copy_constructible< result<int, NonMoveConstructible>>{}, "");
static_assert(not std::is_copy_constructible< result<int, NonMoveAssignable>   >{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonMoveAssignable>   >{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonMoveConstructible>>{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonMovable>          >{}, "");

static_assert(not std::is_move_constructible< result<int, NonMovable>          >{}, "");
static_assert(not std::is_move_constructible< result<int, NonMoveConstructible>>{}, "");
static_assert(    std::is_move_constructible< result<int, NonMoveAssignable>   >{}, "");
static_assert(not std::is_move_assignable<    result<int, NonMoveAssignable>   >{}, "");
// Inheriting from something not move-constructible also deletes move-assignment.
// TODO: fix?
static_assert(not std::is_move_assignable<    result<int, NonMoveConstructible>>{}, "");
static_assert(not std::is_move_assignable<    result<int, NonMovable>          >{}, "");

TEST_CASE("dummy", "[result]") {
	REQUIRE(true);
}

}
