#include "result.hpp"

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

namespace estd {

struct NonCopyable {
	int code;
	explicit NonCopyable(int code) : code{code} {};

	NonCopyable(NonCopyable const &) = delete;
	NonCopyable(NonCopyable      &&) = default;
	NonCopyable & operator=(NonCopyable const &) = delete;
	NonCopyable & operator=(NonCopyable      &&) = default;
};

struct NonCopyConstructible {
	int code;

	explicit NonCopyConstructible(int code) : code{code} {};

	NonCopyConstructible(NonCopyConstructible const &) = delete;
	NonCopyConstructible(NonCopyConstructible      &&) = default;
	NonCopyConstructible & operator=(NonCopyConstructible const &) = default;
	NonCopyConstructible & operator=(NonCopyConstructible      &&) = default;
};

struct NonCopyAssignable {
	int code;

	explicit NonCopyAssignable(int code) : code{code} {};

	NonCopyAssignable(NonCopyAssignable const &) = default;
	NonCopyAssignable(NonCopyAssignable      &&) = default;
	NonCopyAssignable & operator=(NonCopyAssignable const &) = delete;
	NonCopyAssignable & operator=(NonCopyAssignable      &&) = default;
};

struct NonMovable {
	int code;

	explicit NonMovable(int code) : code{code} {};

	NonMovable(NonMovable const &) = delete;
	NonMovable(NonMovable      &&) = delete;
	NonMovable & operator=(NonMovable const &) = delete;
	NonMovable & operator=(NonMovable      &&) = delete;
};

struct NonMoveConstructible {
	int code;

	explicit NonMoveConstructible(int code) : code{code} {};

	NonMoveConstructible(NonMoveConstructible const &) = delete;
	NonMoveConstructible(NonMoveConstructible      &&) = delete;
	NonMoveConstructible & operator=(NonMoveConstructible const &) = default;
	NonMoveConstructible & operator=(NonMoveConstructible      &&) = default;
};

struct NonMoveAssignable {
	int code;

	explicit NonMoveAssignable(int code) : code{code} {};

	NonMoveAssignable(NonMoveAssignable const &) = default;
	NonMoveAssignable(NonMoveAssignable      &&) = default;
	NonMoveAssignable & operator=(NonMoveAssignable const &) = delete;
	NonMoveAssignable & operator=(NonMoveAssignable      &&) = delete;
};

static_assert(std::is_move_constructible< result<int, NonCopyable>         >{}, "");
static_assert(std::is_move_constructible< result<int, NonCopyConstructible>>{}, "");
static_assert(std::is_move_constructible< result<int, NonCopyAssignable>   >{}, "");
static_assert(std::is_move_assignable<    result<int, NonCopyAssignable>   >{}, "");
static_assert(std::is_move_assignable<    result<int, NonCopyConstructible>>{}, "");
static_assert(std::is_move_assignable<    result<int, NonCopyable>         >{}, "");

static_assert(not std::is_copy_constructible< result<int, NonCopyable>         >{}, "");
static_assert(not std::is_copy_constructible< result<int, NonCopyConstructible>>{}, "");
static_assert(    std::is_copy_constructible< result<int, NonCopyAssignable>   >{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonCopyAssignable>   >{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonCopyConstructible>>{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonCopyable>         >{}, "");

static_assert(not std::is_copy_constructible< result<int, NonMovable>          >{}, "");
static_assert(not std::is_copy_constructible< result<int, NonMoveConstructible>>{}, "");
static_assert(not std::is_copy_constructible< result<int, NonMoveAssignable>   >{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonMoveAssignable>   >{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonMoveConstructible>>{}, "");
static_assert(not std::is_copy_assignable<    result<int, NonMovable>          >{}, "");

static_assert(not std::is_move_constructible< result<int, NonMovable>          >{}, "");
static_assert(not std::is_move_constructible< result<int, NonMoveConstructible>>{}, "");
static_assert(    std::is_move_constructible< result<int, NonMoveAssignable>   >{}, "");
static_assert(not std::is_move_assignable<    result<int, NonMoveAssignable>   >{}, "");
// Inheriting from something not move-constructible also deletes move-assignment.
// TODO: fix?
static_assert(not std::is_move_assignable<    result<int, NonMoveConstructible>>{}, "");
static_assert(not std::is_move_assignable<    result<int, NonMovable>          >{}, "");

TEST_CASE("dummy", "[result]") {
	REQUIRE(true);
}

}
