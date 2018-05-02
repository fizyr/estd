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
#include "../traits/containers.hpp"
#include "../convert.hpp"

#include <iterator>

namespace estd {

template<typename ForwardIterator, typename Tag = typename std::iterator_traits<ForwardIterator>::iterator_category>
class range {
public:
	using iterator        = ForwardIterator;
	using difference_type = typename std::iterator_traits<iterator>::difference_type;
	using value_type      = typename std::iterator_traits<iterator>::value_type;
	using pointer         = typename std::iterator_traits<iterator>::pointer;
	using reference       = typename std::iterator_traits<iterator>::pointer;

private:
	ForwardIterator begin_;
	ForwardIterator end_;

public:
	range(ForwardIterator begin, ForwardIterator end) : begin_(std::move(begin)), end_(std::move(end)) {}

	/// Default constructible if ForwardIterator is.
	template<typename = std::enable_if_t<std::is_default_constructible_v<ForwardIterator>>> range() : begin_(), end_() {}

	template<typename Container, typename = std::enable_if_t<is_iterable<Container>>>
	range(Container & container) : begin_(container.begin()), end_(container.end()) {}

	ForwardIterator       & begin()       { return begin_; }
	ForwardIterator const & begin() const { return begin_; }

	ForwardIterator       & end()       { return end_; }
	ForwardIterator const & end() const { return end_; }

	bool empty() const {
		return begin() == end();
	}

	difference_type distance() const {
		return std::distance(begin_, end_);
	}

	bool operator==(range const & other) const { return begin() == other.begin() && end() == other.end(); }
	bool operator!=(range const & other) const { return !(*this == other); }
};

// Deduction guide for constructing from containers.
template<typename Container, typename = std::enable_if_t<is_iterable<Container>>>
range(Container & container) -> range<iterator_type<Container>>;

// Create a reversed range for a container.
template<typename Container>
std::enable_if_t<is_reverse_iterable<Container>, range<reverse_iterator_type<Container>>>
reverse(Container & container) {
	return range{container.rbegin(), container.rend()};
}

}
