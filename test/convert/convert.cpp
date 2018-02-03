#include "convert/convert.hpp"
#include "convert/traits.hpp"

#include "../catch.hpp"

namespace estd {

TEST_CASE("implicitly convertible primitive can be converted using convert<T>(foo)", "[convert]") {

	REQUIRE(convert<int>(5.0)  == 5);
	REQUIRE(convert<int>(6.0f) == 6);
	REQUIRE(convert<int>(6u)   == 6);
	REQUIRE(convert<int>(6ul)  == 6);
	REQUIRE(convert<int>(6l)   == 6);

	REQUIRE(convert<float>(5.5)  == 5.5f);
	REQUIRE(convert<float>(6.2f) == 6.2f);
	REQUIRE(convert<float>(6u)   == 6.f);
	REQUIRE(convert<float>(6ul)  == 6.f);
	REQUIRE(convert<float>(6l)   == 6.f);
}

TEST_CASE("explicitly convertible structs can be converted using convert<T>(foo)", "[convert]") {
	struct Foo {
		int foo;
		bool operator==(Foo const & other) const { return foo == other.foo; }
	};

	struct Bar {
		int bar;
		explicit operator Foo() { return Foo{bar}; }
	};

	REQUIRE(convert<Foo>(Bar{5}) == Foo{5});
	REQUIRE(convert<Foo>(Bar{6}) == Foo{6});
}

}
