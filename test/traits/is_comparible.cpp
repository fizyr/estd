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

#include "traits/is_comparible.hpp"

#include "../static_assert_same.hpp"

namespace estd {

struct A{};
struct B{};
struct C{};

bool operator== (A const &, B const &);
bool operator== (B const &, A const &);

bool operator== (B const &, C const &);
bool operator== (C const &, B const &);

static_assert(is_comparible<int, int>);
static_assert(is_comparible<int, char>);
static_assert(is_comparible<int, int *> == false);
static_assert(is_comparible<int, char *> == false);

static_assert(is_comparible<A, B>);
static_assert(is_comparible2<A, B>);
static_assert(is_comparible<B, C>);
static_assert(is_comparible2<B, C>);
static_assert(is_comparible<A, C> == false);
static_assert(is_comparible2<A, C> == false);

struct HalfA{};
struct HalfB{};

bool operator==(HalfA const &, HalfB);
static_assert(is_comparible<HalfA, HalfB>);
static_assert(is_comparible<HalfB, HalfA> == false);
static_assert(is_comparible2<HalfA, HalfB> == false);
static_assert(is_comparible2<HalfB, HalfA> == false);

}
