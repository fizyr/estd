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

#pragma once
namespace estd {
namespace detail {

template<bool constructible = false, bool assignable = false>
struct non_copyable;

template<bool constructible = false, bool assignable = false>
struct non_movable;

template<>
struct non_copyable<false,false> {
	non_copyable()                                  = default;
	non_copyable(non_copyable const  &)             = delete;
	non_copyable(non_copyable       &&)             = default;
	non_copyable & operator=(non_copyable const  &) = delete;
	non_copyable & operator=(non_copyable       &&) = default;
};

template<>
struct non_copyable<false, true> {
	non_copyable()                                  = default;
	non_copyable(non_copyable const  &)             = delete;
	non_copyable(non_copyable       &&)             = default;
	non_copyable & operator=(non_copyable const  &) = default;
	non_copyable & operator=(non_copyable       &&) = default;
};

template<>
struct non_copyable<true, false> {
	non_copyable()                                  = default;
	non_copyable(non_copyable const  &)             = default;
	non_copyable(non_copyable       &&)             = default;
	non_copyable & operator=(non_copyable const  &) = delete;
	non_copyable & operator=(non_copyable       &&) = default;
};

template<> struct non_copyable<true, true> {};

template<>
struct non_movable<false,false> {
	non_movable()                           = default;
	non_movable(non_movable &&)             = delete;
	non_movable & operator=(non_movable &&) = delete;
};

template<>
struct non_movable<false, true> {
	non_movable()                           = default;
	non_movable(non_movable &&)             = delete;
	non_movable & operator=(non_movable &&) = default;
};

template<>
struct non_movable<true, false> {
	non_movable()                           = default;
	non_movable(non_movable &&)             = default;
	non_movable & operator=(non_movable &&) = delete;
};

template<> struct non_movable<true, true> {};

}}
