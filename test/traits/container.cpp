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

static_assert(std::is_same_v<std::input_iterator_tag,         minimal_iterator_tag<std::input_iterator_tag>>);
static_assert(std::is_same_v<std::output_iterator_tag,        minimal_iterator_tag<std::output_iterator_tag>>);
static_assert(std::is_same_v<std::forward_iterator_tag,       minimal_iterator_tag<std::forward_iterator_tag>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_tag<std::bidirectional_iterator_tag>>);
static_assert(std::is_same_v<std::random_access_iterator_tag, minimal_iterator_tag<std::random_access_iterator_tag>>);

static_assert(std::is_same_v<std::input_iterator_tag,         minimal_iterator_tag<std::tuple<std::input_iterator_tag>>>);
static_assert(std::is_same_v<std::output_iterator_tag,        minimal_iterator_tag<std::tuple<std::output_iterator_tag>>>);
static_assert(std::is_same_v<std::forward_iterator_tag,       minimal_iterator_tag<std::tuple<std::forward_iterator_tag>>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_tag<std::tuple<std::bidirectional_iterator_tag>>>);
static_assert(std::is_same_v<std::random_access_iterator_tag, minimal_iterator_tag<std::tuple<std::random_access_iterator_tag>>>);

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

static_assert(std::is_same_v<std::forward_iterator_tag, minimal_iterator_tag<std::tuple<
	std::bidirectional_iterator_tag,
	std::forward_iterator_tag,
	std::random_access_iterator_tag
>>>);

static_assert(std::is_same_v<std::random_access_iterator_tag, minimal_iterator_category<std::vector<int>::iterator>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_category<std::list<int>::iterator>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_category<std::vector<int>::iterator, std::list<int>::iterator>>);
static_assert(std::is_same_v<std::output_iterator_tag,        minimal_iterator_category<std::back_insert_iterator<std::vector<int>>>>);
static_assert(std::is_same_v<std::input_iterator_tag,         minimal_iterator_category<std::istreambuf_iterator<char>>>);

static_assert(std::is_same_v<std::random_access_iterator_tag, minimal_iterator_category<std::tuple<std::vector<int>::iterator>>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_category<std::tuple<std::list<int>::iterator>>>);
static_assert(std::is_same_v<std::bidirectional_iterator_tag, minimal_iterator_category<std::tuple<std::vector<int>::iterator, std::list<int>::iterator>>>);
static_assert(std::is_same_v<std::output_iterator_tag,        minimal_iterator_category<std::tuple<std::back_insert_iterator<std::vector<int>>>>>);
static_assert(std::is_same_v<std::input_iterator_tag,         minimal_iterator_category<std::tuple<std::istreambuf_iterator<char>>>>);

}
