#pragma once
#include <type_traits>
#include <utility>

namespace estd {

template<typename T> using iterator_type       = std::decay_t<decltype(std::declval<T>().begin())>;
template<typename T> using const_iterator_type = std::decay_t<decltype(std::declval<T const>().begin())>;

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
}

/// Test if a type T is iterable (has a begin() and end() member)
template<typename T> constexpr bool is_iterable = detail::has_begin<T>() && detail::has_end<T>();

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
