#include "range.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

#include <vector>

namespace estd {

TEST_CASE("a default constructed range is empty", "[range]") {
	REQUIRE(range<int *>().empty());
	REQUIRE(range<std::vector<int>::iterator>().empty());
}

TEST_CASE("a range of two pointers", "[range]") {
	int a[4] = {1, 2, 3, 4};

	range r(a, a + 4);
	SECTION("has two iterators") {
		REQUIRE(r.begin() == a);
		REQUIRE(r.end()   == a + 4);
		REQUIRE(r.distance() == 4);
		REQUIRE(not r.empty());
	}

	SECTION("can be copied") {
		range r2 = r;
		REQUIRE(r2 == r);
		REQUIRE(r2.begin() == r.begin());
		REQUIRE(r2.end()   == r.end());
	}

	SECTION("is empty when begin() == end()") {
		r.begin() = r.end();
		REQUIRE(r.empty());
		REQUIRE(r.distance() == 0);
	}

	SECTION("can be looped") {
		int i = 1;
		for (int elem : r) REQUIRE(elem == i++);
	}
}

TEST_CASE("a range for a vector", "[range]") {
	std::vector<int> container{1, 2, 3};
	range r(container);

	SECTION("has two iterators") {
		REQUIRE(r.begin() == container.begin());
		REQUIRE(r.end()   == container.end());
		REQUIRE(r.distance() == 3);
		REQUIRE(not r.empty());
	}

	SECTION("can be copied") {
		range r2 = r;
		REQUIRE(r2 == r);
		REQUIRE(r2.begin() == r.begin());
		REQUIRE(r2.end()   == r.end());
	}

	SECTION("is empty when begin() == end()") {
		r.begin() = r.end();
		REQUIRE(r.empty());
		REQUIRE(r.distance() == 0);
	}

	SECTION("can be looped") {
		int i = 1;
		for (int elem : r) REQUIRE(elem == i++);
	}
}

}
