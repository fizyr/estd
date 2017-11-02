#pragma once

namespace estd {

struct in_place_valid_t{};
struct in_place_error_t{};

constexpr in_place_valid_t in_place_valid;
constexpr in_place_error_t in_place_error;

}
