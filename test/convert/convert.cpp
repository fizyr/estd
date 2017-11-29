#include "convert/convert.hpp"

#include "../catch.hpp"

namespace estd {

TEST_CASE("implicitly convertible primitive can be converted using to<T>(foo)", "[convert]") {
	REQUIRE(to<int>(5.0)  == 5);
	REQUIRE(to<int>(6.0f) == 6);
	REQUIRE(to<int>(6u)   == 6);
	REQUIRE(to<int>(6ul)  == 6);
	REQUIRE(to<int>(6l)   == 6);

	REQUIRE(to<float>(5.5)  == 5.5f);
	REQUIRE(to<float>(6.2f) == 6.2f);
	REQUIRE(to<float>(6u)   == 6.f);
	REQUIRE(to<float>(6ul)  == 6.f);
	REQUIRE(to<float>(6l)   == 6.f);
}

TEST_CASE("no-except implicitly convertible structs can be converted using to<T>(foo)", "[convert]") {
	struct Foo {
		int foo;
		bool operator==(Foo const & other) const { return foo == other.foo; }
	};

	struct Bar {
		int bar;
		operator Foo() noexcept { return Foo{bar}; }
	};

	REQUIRE(to<Foo>(Bar{5})  == Foo{5});
	REQUIRE(to<Foo>(Bar{6})  == Foo{6});
}

}
