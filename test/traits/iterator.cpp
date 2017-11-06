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
