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

#include "traits/iterator.hpp"

#include <vector>
#include <list>
#include <iterator>

namespace estd {

// minimal_iterator_tag
static_assert(std::is_same_v<std::input_iterator_tag,         minimal_iterator_tag<std::input_iterator_tag>>);
static_assert(std::is_same_v<std::output_iterator_tag,        minimal_iterator_tag<std::output_iterator_tag>>);
static_assert(std::is_same_v<std::forward_iterator_tag,       minimal_iterator_tag<std::forward_iterator_tag>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_tag<std::bidirectional_iterator_tag>>);
static_assert(std::is_same_v<std::random_access_iterator_tag, minimal_iterator_tag<std::random_access_iterator_tag>>);

static_assert(std::is_same_v<std::input_iterator_tag, minimal_iterator_tag<
	std::random_access_iterator_tag,
	std::bidirectional_iterator_tag,
	std::input_iterator_tag
>>);

static_assert(std::is_same_v<std::input_iterator_tag, minimal_iterator_tag<
	std::input_iterator_tag,
	std::forward_iterator_tag
>>);

static_assert(std::is_same_v<std::output_iterator_tag, minimal_iterator_tag<
	std::output_iterator_tag,
	std::forward_iterator_tag
>>);

static_assert(std::is_same_v<std::forward_iterator_tag, minimal_iterator_tag<
	std::forward_iterator_tag,
	std::bidirectional_iterator_tag
>>);

static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_tag<
	std::bidirectional_iterator_tag,
	std::random_access_iterator_tag
>>);

static_assert(std::is_same_v<std::output_iterator_tag, minimal_iterator_tag<
	std::random_access_iterator_tag,
	std::forward_iterator_tag,
	std::output_iterator_tag
>>);

static_assert(std::is_same_v<std::forward_iterator_tag, minimal_iterator_tag<
	std::bidirectional_iterator_tag,
	std::forward_iterator_tag,
	std::random_access_iterator_tag
>>);

static_assert(std::is_same_v<std::random_access_iterator_tag, minimal_iterator_category<std::vector<int>::iterator>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_category<std::list<int>::iterator>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_category<std::vector<int>::iterator, std::list<int>::iterator>>);
static_assert(std::is_same_v<std::output_iterator_tag,        minimal_iterator_category<std::back_insert_iterator<std::vector<int>>>>);
static_assert(std::is_same_v<std::input_iterator_tag,         minimal_iterator_category<std::istreambuf_iterator<char>>>);

/// minimun_iterator_tag for tuples of iterators
static_assert(std::is_same_v<std::input_iterator_tag,         minimal_iterator_tag<std::tuple<std::input_iterator_tag>>>);
static_assert(std::is_same_v<std::output_iterator_tag,        minimal_iterator_tag<std::tuple<std::output_iterator_tag>>>);
static_assert(std::is_same_v<std::forward_iterator_tag,       minimal_iterator_tag<std::tuple<std::forward_iterator_tag>>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_tag<std::tuple<std::bidirectional_iterator_tag>>>);
static_assert(std::is_same_v<std::random_access_iterator_tag, minimal_iterator_tag<std::tuple<std::random_access_iterator_tag>>>);

static_assert(std::is_same_v<std::forward_iterator_tag, minimal_iterator_tag<std::tuple<
	std::bidirectional_iterator_tag,
	std::forward_iterator_tag,
	std::random_access_iterator_tag
>>>);

static_assert(std::is_same_v<std::random_access_iterator_tag, minimal_iterator_category<std::tuple<std::vector<int>::iterator>>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_category<std::tuple<std::list<int>::iterator>>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_category<std::tuple<std::vector<int>::iterator, std::list<int>::iterator>>>);
static_assert(std::is_same_v<std::output_iterator_tag,        minimal_iterator_category<std::tuple<std::back_insert_iterator<std::vector<int>>>>>);
static_assert(std::is_same_v<std::input_iterator_tag,         minimal_iterator_category<std::tuple<std::istreambuf_iterator<char>>>>);

// is_iterator_tag_atleast
static_assert(is_iterator_tag_atleast<std::input_iterator_tag,         std::input_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::output_iterator_tag,        std::input_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::forward_iterator_tag,       std::input_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::bidirectional_iterator_tag, std::input_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::random_access_iterator_tag, std::input_iterator_tag>);

static_assert(is_iterator_tag_atleast<std::input_iterator_tag,         std::output_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::output_iterator_tag,        std::output_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::forward_iterator_tag,       std::output_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::bidirectional_iterator_tag, std::output_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::random_access_iterator_tag, std::output_iterator_tag>);

static_assert(is_iterator_tag_atleast<std::input_iterator_tag,         std::forward_iterator_tag> == false);
static_assert(is_iterator_tag_atleast<std::output_iterator_tag,        std::forward_iterator_tag> == false);
static_assert(is_iterator_tag_atleast<std::forward_iterator_tag,       std::forward_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::bidirectional_iterator_tag, std::forward_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::random_access_iterator_tag, std::forward_iterator_tag>);

static_assert(is_iterator_tag_atleast<std::input_iterator_tag,         std::bidirectional_iterator_tag> == false);
static_assert(is_iterator_tag_atleast<std::output_iterator_tag,        std::bidirectional_iterator_tag> == false);
static_assert(is_iterator_tag_atleast<std::forward_iterator_tag,       std::bidirectional_iterator_tag> == false);
static_assert(is_iterator_tag_atleast<std::bidirectional_iterator_tag, std::bidirectional_iterator_tag>);
static_assert(is_iterator_tag_atleast<std::random_access_iterator_tag, std::bidirectional_iterator_tag>);

static_assert(is_iterator_tag_atleast<std::input_iterator_tag,         std::random_access_iterator_tag> == false);
static_assert(is_iterator_tag_atleast<std::output_iterator_tag,        std::random_access_iterator_tag> == false);
static_assert(is_iterator_tag_atleast<std::forward_iterator_tag,       std::random_access_iterator_tag> == false);
static_assert(is_iterator_tag_atleast<std::bidirectional_iterator_tag, std::random_access_iterator_tag> == false);
static_assert(is_iterator_tag_atleast<std::random_access_iterator_tag, std::random_access_iterator_tag>);

// is_iterator_atleast
static_assert(is_iterator_atleast<std::vector<int>::iterator, std::input_iterator_tag>);
static_assert(is_iterator_atleast<std::vector<int>::iterator, std::output_iterator_tag>);
static_assert(is_iterator_atleast<std::vector<int>::iterator, std::forward_iterator_tag>);
static_assert(is_iterator_atleast<std::vector<int>::iterator, std::bidirectional_iterator_tag>);
static_assert(is_iterator_atleast<std::vector<int>::iterator, std::random_access_iterator_tag>);

static_assert(is_iterator_atleast<std::list<int>::iterator, std::input_iterator_tag>);
static_assert(is_iterator_atleast<std::list<int>::iterator, std::output_iterator_tag>);
static_assert(is_iterator_atleast<std::list<int>::iterator, std::forward_iterator_tag>);
static_assert(is_iterator_atleast<std::list<int>::iterator, std::bidirectional_iterator_tag>);
static_assert(is_iterator_atleast<std::list<int>::iterator, std::random_access_iterator_tag> == false);

static_assert(is_iterator_atleast<std::back_insert_iterator<std::vector<int>>, std::input_iterator_tag>);
static_assert(is_iterator_atleast<std::back_insert_iterator<std::vector<int>>, std::output_iterator_tag>);
static_assert(is_iterator_atleast<std::back_insert_iterator<std::vector<int>>, std::forward_iterator_tag>       == false);
static_assert(is_iterator_atleast<std::back_insert_iterator<std::vector<int>>, std::bidirectional_iterator_tag> == false);
static_assert(is_iterator_atleast<std::back_insert_iterator<std::vector<int>>, std::random_access_iterator_tag> == false);

static_assert(is_iterator_atleast<std::istreambuf_iterator<char>, std::input_iterator_tag>);
static_assert(is_iterator_atleast<std::istreambuf_iterator<char>, std::output_iterator_tag>);
static_assert(is_iterator_atleast<std::istreambuf_iterator<char>, std::forward_iterator_tag>       == false);
static_assert(is_iterator_atleast<std::istreambuf_iterator<char>, std::bidirectional_iterator_tag> == false);
static_assert(is_iterator_atleast<std::istreambuf_iterator<char>, std::random_access_iterator_tag> == false);

}
