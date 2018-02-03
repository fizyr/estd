#include "convert/traits.hpp"

#include <type_traits>

// Implicitly convertible primitives are convertible.
template<typename T, typename Y>
constexpr void assert_convertible_to_integral() {
	static_assert(estd::can_convert<T, std::make_signed_t<Y>>,   "");
	static_assert(estd::can_convert<T, std::make_unsigned_t<Y>>, "");
}

template<typename T>
constexpr void assert_convertible_to_floating() {
	static_assert(estd::can_convert<T, float>,       "");
	static_assert(estd::can_convert<T, double>,      "");
	static_assert(estd::can_convert<T, long double>, "");
}

template<typename T>
constexpr void assert_convertible_to_numeric_types() {
	static_assert(estd::can_convert<T, bool>,      "");
	assert_convertible_to_integral<T, char>();
	assert_convertible_to_integral<T, short>();
	assert_convertible_to_integral<T, int>();
	assert_convertible_to_integral<T, long>();
	assert_convertible_to_integral<T, long long>();
	assert_convertible_to_floating<T>();
}

template<typename T>
constexpr void assert_integral_convertible_to_numeric_types() {
	assert_convertible_to_numeric_types<std::make_signed_t<T>>();
	assert_convertible_to_numeric_types<std::make_unsigned_t<T>>();
}

constexpr void assert_numericals_convertible_to_numeric_types() {
	assert_convertible_to_numeric_types<bool>();
	assert_integral_convertible_to_numeric_types<short>();
	assert_integral_convertible_to_numeric_types<int>();
	assert_integral_convertible_to_numeric_types<long>();
	assert_integral_convertible_to_numeric_types<long long>();
	assert_convertible_to_numeric_types<float>();
	assert_convertible_to_numeric_types<double>();
	assert_convertible_to_numeric_types<long double>();
}

struct Foo {};
struct ExplicitFromFoo { explicit ExplicitFromFoo(Foo const &); };
struct ImplicitFromFoo { ImplicitFromFoo(Foo const &); };
struct Unrelated {};
struct UserDefined {};
struct ExplicitlyImpossible {};
struct EmptyConversion {};

namespace estd {
	template<> struct conversion<Foo, UserDefined> {
		static UserDefined perform(Foo const &);
	};

	template<> struct conversion<Foo, ExplicitlyImpossible> {
		constexpr static bool impossible = true;
		static ExplicitlyImpossible perform(Foo const &);
	};

	template<> struct conversion<Foo, EmptyConversion> {};
}

static_assert(estd::can_convert<Foo, ExplicitFromFoo>, "");
static_assert(estd::can_convert<Foo, ImplicitFromFoo>, "");
static_assert(estd::can_convert<Foo, UserDefined>, "");
static_assert(estd::can_convert<Foo, Unrelated>            == false, "");
static_assert(estd::can_convert<Foo, ExplicitlyImpossible> == false, "");
static_assert(estd::can_convert<Foo, EmptyConversion>      == false, "");
