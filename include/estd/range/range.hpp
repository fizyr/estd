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

}
