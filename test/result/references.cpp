/* Copyright 2017-2018 Fizyr B.V. - https://fizyr.com
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "result.hpp"
#include "tracker.hpp"

#include "../catch.hpp"
#include "../static_assert_same.hpp"

namespace estd {

struct Error {
	int code;

	Error() {};
	explicit Error(int code) : code(code) {};
	explicit operator int() { return code; }
};

struct Foo {};

template<typename R, typename T, typename const_T, typename rvalue_T>
void test_valid_observers() {
	static_assert_same<decltype(*std::declval<R       &>()),        T>();
	static_assert_same<decltype(*std::declval<R const &>()),  const_T>();
	static_assert_same<decltype(*std::declval<R      &&>()), rvalue_T>();

	static_assert_same<decltype(std::declval<R       &>().value()),        T>();
	static_assert_same<decltype(std::declval<R const &>().value()),  const_T>();
	static_assert_same<decltype(std::declval<R      &&>().value()), rvalue_T>();

	static_assert_same<decltype(std::declval<R       &>().value_or(std::declval<       T>())), std::decay_t<T>>();
	static_assert_same<decltype(std::declval<R       &>().value_or(std::declval< const_T>())), std::decay_t<T>>();
	static_assert_same<decltype(std::declval<R const &>().value_or(std::declval< const_T>())), std::decay_t<T>>();
	static_assert_same<decltype(std::declval<R      &&>().value_or(std::declval<rvalue_T>())), std::decay_t<T>>();
	static_assert_same<decltype(std::declval<R      &&>().value_or(std::declval<       T>())), std::decay_t<T>>();
	static_assert_same<decltype(std::declval<R      &&>().value_or(std::declval< const_T>())), std::decay_t<T>>();
}

template<typename R, typename E, typename const_E, typename rvalue_E>
void test_error_observers() {
	static_assert_same<decltype(std::declval<R       &>().error_unchecked()),        E>();
	static_assert_same<decltype(std::declval<R const &>().error_unchecked()),  const_E>();
	static_assert_same<decltype(std::declval<R      &&>().error_unchecked()), rvalue_E>();

	static_assert_same<decltype(std::declval<R       &>().error()),        E>();
	static_assert_same<decltype(std::declval<R const &>().error()),  const_E>();
	static_assert_same<decltype(std::declval<R      &&>().error()), rvalue_E>();

	static_assert_same<decltype(std::declval<R       &>().error_or(std::declval<       E>())), std::decay_t<E>>();
	static_assert_same<decltype(std::declval<R       &>().error_or(std::declval< const_E>())), std::decay_t<E>>();
	static_assert_same<decltype(std::declval<R const &>().error_or(std::declval< const_E>())), std::decay_t<E>>();
	static_assert_same<decltype(std::declval<R      &&>().error_or(std::declval<rvalue_E>())), std::decay_t<E>>();
	static_assert_same<decltype(std::declval<R      &&>().error_or(std::declval<       E>())), std::decay_t<E>>();
	static_assert_same<decltype(std::declval<R      &&>().error_or(std::declval< const_E>())), std::decay_t<E>>();
}

void test() {
	/// Test implicit conversion to valid results of reference.
	static_assert(std::is_convertible_v<int        &, result<int &, Error>>);
	//static_assert(std::is_convertible_v<int const  &, result<int &, Error>> == false);
	//static_assert(std::is_convertible_v<int       &&, result<int &, Error>> == false);
	//static_assert(std::is_convertible_v<int const &&, result<int &, Error>> == false);

	static_assert(std::is_convertible_v<int        &, result<int const &, Error>>);
	static_assert(std::is_convertible_v<int const  &, result<int const &, Error>>);
	static_assert(std::is_convertible_v<int       &&, result<int const &, Error>>);
	static_assert(std::is_convertible_v<int const &&, result<int const &, Error>>);

	/// Test implicit conversion to valid error of reference.
	static_assert(std::is_convertible_v<int        &, result<Foo, int &>>);
	//static_assert(std::is_convertible_v<int const  &, result<Foo, int &>> == false);
	//static_assert(std::is_convertible_v<int       &&, result<Foo, int &>> == false);
	//static_assert(std::is_convertible_v<int const &&, result<Foo, int &>> == false);

	static_assert(std::is_convertible_v<int        &, result<Foo, int const &>>);
	static_assert(std::is_convertible_v<int const  &, result<Foo, int const &>>);
	static_assert(std::is_convertible_v<int       &&, result<Foo, int const &>>);
	static_assert(std::is_convertible_v<int const &&, result<Foo, int const &>>);

	/// Test return type of observers.
	test_valid_observers<result<int       &, Foo>, int       &, int const &, int       &>();
	test_valid_observers<result<int const &, Foo>, int const &, int const &, int const &>();
	test_error_observers<result<Foo, int       &>, int       &, int const &, int       &>();
	test_error_observers<result<Foo, int const &>, int const &, int const &, int const &>();

	test_valid_observers<result<int      , Foo>, int       &, int const &, int       &&>();
	test_valid_observers<result<int const, Foo>, int const &, int const &, int const &&>();
	test_error_observers<result<Foo, int      >, int       &, int const &, int       &&>();
	test_error_observers<result<Foo, int const>, int const &, int const &, int const &&>();

}

TEST_CASE("results can hold references", "[result]") {
	int a = 0;
	result<int &, Error> valid{in_place_valid, a};

	SECTION("reference refers to the same value") {
		REQUIRE(valid);
		REQUIRE(&valid.value() == &a);
		REQUIRE(&*valid == &a);

	}

	SECTION("copy construct") {
		result<int &, Error> copied_valid{valid};

		REQUIRE(copied_valid);
		REQUIRE(&copied_valid.value() == &a);
		REQUIRE(&*copied_valid == &a);
	}

	SECTION("copy assign") {
		int b = 6;
		result<int &, Error> copied_valid{in_place_valid, b};
		copied_valid = valid;

		REQUIRE(copied_valid);
		REQUIRE(&copied_valid.value() == &a);
		REQUIRE(&*copied_valid == &a);
	}

	SECTION("copy assign to previously invalid") {
		result<int &, Error> copied_valid{in_place_error};
		copied_valid = valid;

		REQUIRE(copied_valid);
		REQUIRE(&copied_valid.value() == &a);
		REQUIRE(&*copied_valid == &a);
	}

	SECTION("move construct") {
		result<int &, Error> moved_valid{std::move(valid)};

		REQUIRE(moved_valid);
		REQUIRE(&moved_valid.value() == &a);
		REQUIRE(&*moved_valid == &a);
	}

	SECTION("move assign") {
		int b = 6;
		result<int &, Error> moved_valid{in_place_valid, b};
		moved_valid = std::move(valid);

		REQUIRE(moved_valid);
		REQUIRE(&moved_valid.value() == &a);
		REQUIRE(&*moved_valid == &a);
	}

	SECTION("move assign to previously invalid") {
		result<int &, Error> moved_valid{in_place_error};
		moved_valid = std::move(valid);

		REQUIRE(moved_valid);
		REQUIRE(&moved_valid.value() == &a);
		REQUIRE(&*moved_valid == &a);
	}
}

TEST_CASE("history of results", "[result]") {
	Tracker ok;
	Tracker error;
	using Result = result<Tracker&, Tracker&>;
	Result valid{in_place_valid, ok};
	Result invalid{in_place_error, error};

	SECTION("default construct a result") {
		REQUIRE(valid);
		REQUIRE(!invalid);

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("copy construct a result") {
		Result copied_valid{valid};
		Result copied_error{invalid};

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("copy assign result to one of the same type (valid/invalid)") {
		Tracker b;
		Result copied_valid{in_place_valid, b}; copied_valid = valid;
		Result copied_error{in_place_error, b}; copied_error = invalid;

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("copy assign result to one of the other type (valid/invalid)") {
		Tracker b;
		Result copied_valid{in_place_error, b}; copied_valid = valid;
		Result copied_error{in_place_valid, b}; copied_error = invalid;

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("move construct a result") {
		Result moved_valid{std::move(valid)};
		Result moved_error{std::move(invalid)};

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("move assign result to one of the same type (valid/invalid)") {
		Tracker b;
		Result moved_valid{in_place_valid, b}; moved_valid = std::move(valid);
		Result moved_error{in_place_error, b}; moved_error = std::move(invalid);

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}

	SECTION("move assign result to one of the other type (valid/invalid)") {
		Tracker b;
		Result moved_valid{in_place_error, b}; moved_valid = std::move(valid);
		Result moved_error{in_place_valid, b}; moved_error = std::move(invalid);

		REQUIRE(ok.history == History{Event::default_constructed});
		REQUIRE(error.history == History{Event::default_constructed});
	}
}

}
