#pragma once
#include "range.hpp"

namespace estd {
namespace detail {
	// Proxy object that can be dereferenced to some value.
	template<typename T>
	class DerefProxy {
		T val;

	public:
		DerefProxy(T const  & val) : val(val) {}
		DerefProxy(T       && val) : val(std::move(val)) {}

		T       & operator* ()       { return val; }
		T const & operator* () const { return val; }
		T       * operator->()       { return &val; }
		T const * operator->() const { return &val; }
	};

	/// Base implementation for enumerate_iterator for InputIterators.
	template<
		typename Iterator,
		typename Counter = typename std::iterator_traits<Iterator>::difference_type,
		typename Tag     = typename std::iterator_traits<Iterator>::iterator_category
	>
	class enumerate_iterator {
	public:
		using wrapped_iterator  = Iterator;
		using value_type        = std::tuple<Counter, decltype(*std::declval<Iterator const>())>;
		using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
		using reference         = value_type const &;
		using pointer           = value_type const *;
		using iterator_category = Tag;

	protected:
		Counter counter_;
		Iterator iterator_;

	public:
		template<typename = std::enable_if_t<std::is_default_constructible_v<Iterator>>>
		enumerate_iterator() : counter_{}, iterator_{} {}

		enumerate_iterator(Iterator iterator) : counter_{}, iterator_{iterator} {}

		bool operator==(enumerate_iterator const & other) const { return iterator_ == other.iterator_; }
		bool operator!=(enumerate_iterator const & other) const { return iterator_ != other.iterator_; }

		value_type operator* () const { return {counter_, *iterator_}; }
		DerefProxy<value_type> operator->() const { return {**this}; }

		enumerate_iterator & operator++() {
			++iterator_;
			++counter_;
			return *this;
		}

		DerefProxy<value_type> operator++(int) {
			DerefProxy<value_type> result{*this};
			++*this;
			return result;
		}

		void swap(enumerate_iterator & other) {
			using std::swap;
			swap(counter_, other.counter);
			swap(iterator_, other.iterator_);
		}

		friend void swap(enumerate_iterator & a, enumerate_iterator & b) { a.swap(b); }
	};

	/// Overrides for ForwardIterators.
	template<typename Iterator, typename Counter>
	class enumerate_iterator<Iterator, Counter, std::forward_iterator_tag> : public enumerate_iterator<Iterator, Counter, std::input_iterator_tag> {
	private:
		using Parent = enumerate_iterator<Iterator, Counter, std::input_iterator_tag>;

	public:
		using iterator_category = std::forward_iterator_tag;

		template<typename = std::enable_if_t<std::is_default_constructible_v<Parent>>> enumerate_iterator() : Parent{} {}
		enumerate_iterator(Iterator iterator) : Parent{iterator} {}

		enumerate_iterator & operator++() { Parent::operator++(); return *this; }

		enumerate_iterator operator++(int) {
			enumerate_iterator old = *this;
			++*this;
			return old;
		}
	};

	/// Additions for BidirectionalIterators.
	template<typename Iterator, typename Counter>
	class enumerate_iterator<Iterator, Counter, std::bidirectional_iterator_tag> : public enumerate_iterator<Iterator, Counter, std::forward_iterator_tag> {
	private:
		using Parent = enumerate_iterator<Iterator, Counter, std::forward_iterator_tag>;

	public:
		using iterator_category = std::bidirectional_iterator_tag;

		template<typename = std::enable_if_t<std::is_default_constructible_v<Parent>>> enumerate_iterator() : Parent{} {}
		enumerate_iterator(Iterator iterator) : Parent{iterator} {}

		enumerate_iterator & operator--() {
			--this->iterator_;
			--this->counter_;
			return *this;
		}

		enumerate_iterator operator--(int) {
			enumerate_iterator old = *this;
			--*this;
			return old;
		}
	};

	/// Additions for RandomAccessIterators.
	template<typename Iterator, typename Counter>
	class enumerate_iterator<Iterator, Counter, std::random_access_iterator_tag> : public enumerate_iterator<Iterator, Counter, std::bidirectional_iterator_tag> {
	private:
		using Parent = enumerate_iterator<Iterator, Counter, std::bidirectional_iterator_tag>;

	public:
		using value_type        = typename Parent::value_type;
		using difference_type   = typename Parent::difference_type;
		using iterator_category = std::random_access_iterator_tag;

		template<typename = std::enable_if_t<std::is_default_constructible_v<Parent>>> enumerate_iterator() : Parent{} {}
		enumerate_iterator(Iterator iterator) : Parent{iterator} {}

		enumerate_iterator & operator+=(difference_type n) {
			Parent::iterator_ += n;
			Parent::counter_  += n;
			return *this;
		}

		enumerate_iterator & operator-=(difference_type n) {
			Parent::iterator_ -= n;
			Parent::counter_  -= n;
			return *this;
		}

		difference_type operator-(enumerate_iterator other) const { return Parent::iterator_ - other.iterator_; }

		value_type operator[](difference_type n) const { return *(*this + n); }

		bool operator< (enumerate_iterator other) const { return Parent::iterator_ <  other.iterator_; }
		bool operator> (enumerate_iterator other) const { return Parent::iterator_ >  other.iterator_; }
		bool operator<=(enumerate_iterator other) const { return Parent::iterator_ <= other.iterator_; }
		bool operator>=(enumerate_iterator other) const { return Parent::iterator_ >= other.iterator_; }

		friend enumerate_iterator operator+(enumerate_iterator const & it, difference_type n) { return enumerate_iterator{it} += n; }
		friend enumerate_iterator operator+(difference_type n, enumerate_iterator const & it) { return enumerate_iterator{it} += n; }
		friend enumerate_iterator operator-(enumerate_iterator const & it, difference_type n) { return enumerate_iterator{it} -= n; }
		friend enumerate_iterator operator-(difference_type n, enumerate_iterator const & it) { return enumerate_iterator{it} -= n; }
	};
}

template<typename Iterator>
range<detail::enumerate_iterator<Iterator>> enumerate(Iterator begin, Iterator end) {
	return {begin, end};
}

template<typename Container>
range<detail::enumerate_iterator<iterator_type<Container &>>> enumerate(Container & container) {
	return {container.begin(), container.end()};
}

template<typename Container>
range<detail::enumerate_iterator<iterator_type<Container const &>>> enumerate(Container const & container) {
	return {container.begin(), container.end()};
}

}
