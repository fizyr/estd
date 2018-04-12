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

#include "traits/containers.hpp"

#include <vector>
#include <string>
#include <map>
#include <list>
#include <iterator>

namespace estd {

static_assert(is_iterable  <int> == false);
static_assert(has_size     <int> == false);
static_assert(has_reserve  <int> == false);
static_assert(has_erase    <int> == false);
static_assert(is_container <int> == false);

static_assert(is_iterable  <std::vector<int>> == true);
static_assert(has_size     <std::vector<int>> == true);
static_assert(has_reserve  <std::vector<int>> == true);
static_assert(has_erase    <std::vector<int>> == true);
static_assert(is_container <std::vector<int>> == true);
static_assert(std::is_same_v<      iterator_type<std::vector<int>>, std::vector<int>::iterator>);
static_assert(std::is_same_v<const_iterator_type<std::vector<int>>, std::vector<int>::const_iterator>);

static_assert(is_iterable  <std::string> == true);
static_assert(has_size     <std::string> == true);
static_assert(has_reserve  <std::string> == true);
static_assert(has_erase    <std::string> == true);
static_assert(is_container <std::string> == true);
static_assert(std::is_same_v<      iterator_type<std::string>, std::string::iterator>);
static_assert(std::is_same_v<const_iterator_type<std::string>, std::string::const_iterator>);

static_assert(is_iterable  <std::map<int, int>> == true);
static_assert(has_size     <std::map<int, int>> == true);
static_assert(has_reserve  <std::map<int, int>> == false);
static_assert(has_erase    <std::map<int, int>> == true);
static_assert(is_container <std::map<int, int>> == true);
static_assert(std::is_same_v<      iterator_type<std::map<int, int>>, std::map<int, int>::iterator>);
static_assert(std::is_same_v<const_iterator_type<std::map<int, int>>, std::map<int, int>::const_iterator>);

}
