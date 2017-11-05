#pragma once
#include <iterator>
#include <type_traits>
#include <utility>

namespace estd {

template<typename T> using iterator_type       = std::decay_t<decltype(begin(std::declval<T &>()))>;
template<typename T> using const_iterator_type = std::decay_t<decltype(begin(std::declval<T const &>()))>;

namespace detail {
	using std::declval;

	template<typename T, typename = void> struct has_begin : std::false_type {};
	template<typename T> struct has_begin<T, std::void_t<decltype(declval<T>().begin())>> : std::true_type {};

	template<typename T, typename = void> struct has_end : std::false_type {};
	template<typename T> struct has_end<T, std::void_t<decltype(declval<T>().end())>> : std::true_type {};

	template<typename T, typename = void> struct has_size : std::false_type {};
	template<typename T> struct has_size<T, std::void_t<decltype(declval<T>().size())>> : std::true_type {};

	template<typename T, typename = void> struct has_reserve : std::false_type {};
	template<typename T> struct has_reserve<T, std::void_t<decltype(declval<T>().reserve(30))>> : std::true_type {};

	template<typename T, typename = void> struct has_erase : std::false_type {};
	template<typename T> struct has_erase<T, std::void_t<decltype(declval<T>().erase(declval<const_iterator_type<T>>(), declval<const_iterator_type<T>>()))>> : std::true_type {};

	template<typename T> struct iterator_tag_value;
	template<> struct iterator_tag_value<std::input_iterator_tag>         : std::integral_constant<int, 0> {};
	template<> struct iterator_tag_value<std::output_iterator_tag>        : std::integral_constant<int, 0> {};
	template<> struct iterator_tag_value<std::forward_iterator_tag>       : std::integral_constant<int, 1> {};
	template<> struct iterator_tag_value<std::bidirectional_iterator_tag> : std::integral_constant<int, 2> {};
	template<> struct iterator_tag_value<std::random_access_iterator_tag> : std::integral_constant<int, 3> {};

	template<typename ...T> struct minimal_iterator_tag;
	template<typename ...T> struct minimal_iterator_category;

	template<typename ...T>
	struct minimal_iterator_tag<std::tuple<T...>> {
		using type = typename minimal_iterator_tag<T...>::type;
	};

	template<typename T>
	struct minimal_iterator_tag<T> {
		using type = T;
	};

	template<typename T, typename Y>
	struct minimal_iterator_tag<T, Y> {
		using type = std::conditional_t<iterator_tag_value<T>() <= iterator_tag_value<Y>(), T, Y>;
	};

	template<typename T, typename Y, typename ...R>
	struct minimal_iterator_tag<T, Y, R...> {
		using type = typename minimal_iterator_tag<typename minimal_iterator_tag<T, Y>::type, R...>::type;
	};

	template<typename ...T>
	struct minimal_iterator_category {
		using type = typename minimal_iterator_tag<typename std::iterator_traits<T>::iterator_category...>::type;
	};

	template<typename ...T>
	struct minimal_iterator_category<std::tuple<T...>> {
		using type = typename minimal_iterator_category<T...>::type;
	};

}

/// Test if a type T is iterable (has a begin() and end() member)
template<typename T> constexpr bool is_iterable = detail::has_begin<T>() && detail::has_end<T>();

/// Get the minimal iterator tag of a list of tags.
/**
 * The tags may be specified directly as template arguments,
 * or as a single tuple of tags.
 *
 * For the purpose of this check, input and ouput iterators are considered equally low.
 * If both an input and an output iterator are present in the list,
 * the result is either std::input_iterator_tag or std::output_iterator_tag.
 */
template<typename ...Tags> using minimal_iterator_tag = typename detail::minimal_iterator_tag<Tags...>::type;

/// Get the minimal iterator category of a list of iterators.
/**
 * The iterators may be specified directly as template arguments,
 * or as a single tuple of iterators.
 *
 * For the purpose of this check, input and ouput iterators are considered equally low.
 * If both an input and an output iterator are present in the list,
 * the result is either std::input_iterator_tag or std::output_iterator_tag.
 */
template<typename ...Iterators> using minimal_iterator_category = typename detail::minimal_iterator_category<Iterators...>::type;

/// Test if a type T has a size() member.
template<typename T> constexpr bool has_size = detail::has_size<T>();

/// Test if a type T has a reserve() member.
template<typename T> constexpr bool has_reserve = detail::has_reserve<T>();

/// Test if a type T has a erase() member.
template<typename T> constexpr bool has_erase = detail::has_erase<T>();

/// Test if a type T is a container.
/**
 * Currently this is a relaxed check. It only tests if the members begin(), end() and size() exist.
 */
template<typename T> constexpr bool is_container = is_iterable<T> && has_size<T>;

}
