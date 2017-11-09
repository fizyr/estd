#include "range/enumerate.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

#include <vector>

namespace estd {

TEST_CASE("enumerating a vector", "[enumerate]") {
	std::vector<int> vec{3, 2, 1};

	SECTION("counter increments and values are looped") {
		int i = 0;
		for (auto [count, elem] : enumerate(vec)) {
			REQUIRE(i++ == count);
			REQUIRE(elem == 3 - count);
		}
	}

	SECTION("identity of elements is same") {
		REQUIRE(&vec.front() == &std::get<1>(*enumerate(vec).begin()));
		for (auto const [count, elem] : enumerate(vec)) {
			REQUIRE(&vec[count] == &elem);
		}
	}

	SECTION("type of looped elements is reference") {
		static_assert(std::is_same_v<decltype(*enumerate(vec).begin()), std::tuple<std::ptrdiff_t, int &>>);
	}
}

TEST_CASE("enumerating an enumeration of a vector", "[enumerate]") {
	std::vector<int> vec{3, 2, 1};
	int i = 0;
	for (auto [count1, elem] : enumerate(enumerate(vec))) {
		auto [count2, value] = elem;
		REQUIRE(i++ == count1);
		REQUIRE(count2 == count1);
		REQUIRE(value == 3 - count1);
	}
}

TEST_CASE("elements can be modified while enumerating a vector", "[enumerate]") {
	std::vector<int> vec{3, 2, 1};
	int i = 0;

	for (auto [count, elem] : enumerate(vec)) {
		elem = count;
	}

	REQUIRE(vec == (std::vector{0, 1, 2}));
}

TEST_CASE("elements can not be modified while enumerating a const vector", "[enumerate]") {
	std::vector<int> const vec{3, 2, 1};
	static_assert(std::is_same_v<decltype(*enumerate(vec).begin()), std::tuple<std::ptrdiff_t, int const &>>);
}

}
