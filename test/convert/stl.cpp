#include "convert/convert.hpp"
#include "convert/stl.hpp"

#include "../catch.hpp"


namespace estd {

TEST_CASE("if T is convertible to Y, std::vector<T> is convertible to std::vector<Y>", "[convert]") {
	REQUIRE((convert<std::vector<int>>(std::vector<float>{1.5, 2.2}) == std::vector<int>{1, 2}));
}


}
