#include "type_name.hpp"

#include <catch2/catch.hpp>

namespace estd {

TEST_CASE("type_name<T> works for primitive types", "[type_name]") {
	CHECK(type_name<bool>()      == "bool");
	CHECK(type_name<char>()      == "char");
	CHECK(type_name<short>()     == "short");
	CHECK(type_name<int>()       == "int");
	CHECK(type_name<long>()      == "long");
	CHECK(type_name<long long>() == "long long");

	CHECK(type_name<unsigned char>()      == "unsigned char");
	CHECK(type_name<unsigned short>()     == "unsigned short");
	CHECK(type_name<unsigned int>()       == "unsigned int");
	CHECK(type_name<unsigned long>()      == "unsigned long");
	CHECK(type_name<unsigned long long>() == "unsigned long long");

	CHECK(type_name<float>()       == "float");
	CHECK(type_name<double>()      == "double");
	CHECK(type_name<long double>() == "long double");
}

TEST_CASE("full_type_name<T> works for primitive types", "[type_name]") {
	CHECK(full_type_name<bool>()      == "bool");
	CHECK(full_type_name<char>()      == "char");
	CHECK(full_type_name<short>()     == "short");
	CHECK(full_type_name<int>()       == "int");
	CHECK(full_type_name<long>()      == "long");
	CHECK(full_type_name<long long>() == "long long");

	CHECK(full_type_name<unsigned char>()      == "unsigned char");
	CHECK(full_type_name<unsigned short>()     == "unsigned short");
	CHECK(full_type_name<unsigned int>()       == "unsigned int");
	CHECK(full_type_name<unsigned long>()      == "unsigned long");
	CHECK(full_type_name<unsigned long long>() == "unsigned long long");

	CHECK(full_type_name<float>()       == "float");
	CHECK(full_type_name<double>()      == "double");
	CHECK(full_type_name<long double>() == "long double");
}

TEST_CASE("type_name<T> works cv-qualified primitive types", "[type_name]") {
	CHECK(type_name<int>()                  == "int");
	CHECK(type_name<int const>()            == "int const");
	CHECK(type_name<int volatile>()         == "int volatile");
	CHECK(type_name<int const volatile>()   == "int const volatile");
}

TEST_CASE("type_name<T> works cv-qualified primitive reference types", "[type_name]") {
	CHECK(type_name<int &>()                == "int &");
	CHECK(type_name<int const &>()          == "int const &");
	CHECK(type_name<int volatile &>()       == "int volatile &");
	CHECK(type_name<int volatile const &>() == "int const volatile &");

	CHECK(type_name<int &&>()                == "int &&");
	CHECK(type_name<int const &&>()          == "int const &&");
	CHECK(type_name<int volatile &&>()       == "int volatile &&");
	CHECK(type_name<int volatile const &&>() == "int const volatile &&");
}

TEST_CASE("type_name<T> works cv-qualified complex types", "[type_name]") {
	CHECK(type_name<std::string>()                  == "std::string");
	CHECK(type_name<std::string const>()            == "std::string const");
	CHECK(type_name<std::string volatile>()         == "std::string volatile");
	CHECK(type_name<std::string const volatile>()   == "std::string const volatile");

	CHECK(type_name<std::string &>()                  == "std::string &");
	CHECK(type_name<std::string const &>()            == "std::string const &");
	CHECK(type_name<std::string volatile &>()         == "std::string volatile &");
	CHECK(type_name<std::string const volatile &>()   == "std::string const volatile &");

	CHECK(type_name<std::string &&>()                  == "std::string &&");
	CHECK(type_name<std::string const &&>()            == "std::string const &&");
	CHECK(type_name<std::string volatile &&>()         == "std::string volatile &&");
	CHECK(type_name<std::string const volatile &&>()   == "std::string const volatile &&");
}

template<typename T> struct Foo{};

TEST_CASE("type_name<T> works cv-qualified templated types", "[type_name]") {
	CHECK(type_name<estd::Foo<int>>()                   == "estd::Foo<int>");
	CHECK(type_name<estd::Foo<int> const>()             == "estd::Foo<int> const");
	CHECK(type_name<estd::Foo<int> volatile>()          == "estd::Foo<int> volatile");
	CHECK(type_name<estd::Foo<int> const volatile>()    == "estd::Foo<int> const volatile");

	CHECK(type_name<estd::Foo<int> &>()                 == "estd::Foo<int> &");
	CHECK(type_name<estd::Foo<int> const &>()           == "estd::Foo<int> const &");
	CHECK(type_name<estd::Foo<int> volatile &>()        == "estd::Foo<int> volatile &");
	CHECK(type_name<estd::Foo<int> const volatile &>()  == "estd::Foo<int> const volatile &");

	CHECK(type_name<estd::Foo<int> &&>()                == "estd::Foo<int> &&");
	CHECK(type_name<estd::Foo<int> const &&>()          == "estd::Foo<int> const &&");
	CHECK(type_name<estd::Foo<int> volatile &&>()       == "estd::Foo<int> volatile &&");
	CHECK(type_name<estd::Foo<int> const volatile &&>() == "estd::Foo<int> const volatile &&");
}

}
