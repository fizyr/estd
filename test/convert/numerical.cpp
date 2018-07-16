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

#include "convert/convert.hpp"
#include "convert/numerical.hpp"

#include "../catch.hpp"

template<typename T, typename E>
struct Catch::StringMaker<estd::result<T, E>> {
	static std::string convert(estd::result<T, E> const & value) {
		if (value) return "Ok(" + StringMaker<T>::convert(*value) + ")";
		else return "Err(" + StringMaker<E>::convert(value.error_unchecked()) + ")";
	}
};

template<>
struct Catch::StringMaker<std::error_code> {
	static std::string convert(std::error_code const & error) {
		return std::string(error.category().name()) + " error " + std::to_string(error.value()) + ": " + error.message();
	}
};

namespace estd {

TEST_CASE("string/numerical conversions work for char", "[convert]") {
	REQUIRE((parse<char, std::error_code>(   "0")) == (char)    0);
	REQUIRE((parse<char, std::error_code>(   "1")) == (char)    1);
	REQUIRE((parse<char, std::error_code>(  "-1")) == (char)   -1);
	REQUIRE((parse<char, std::error_code>( "127")) == (char)  127);
	REQUIRE((parse<char, std::error_code>("-127")) == (char) -127);

	REQUIRE((parse<signed char, std::error_code>(   "0")) == (signed char)    0);
	REQUIRE((parse<signed char, std::error_code>(   "1")) == (signed char)    1);
	REQUIRE((parse<signed char, std::error_code>(  "-1")) == (signed char)   -1);
	REQUIRE((parse<signed char, std::error_code>( "127")) == (signed char)  127);
	REQUIRE((parse<signed char, std::error_code>("-127")) == (signed char) -127);

	REQUIRE((parse<unsigned char, std::error_code>(  "0")) == (unsigned char)   0);
	REQUIRE((parse<unsigned char, std::error_code>(  "1")) == (unsigned char)   1);
	REQUIRE((parse<unsigned char, std::error_code>("127")) == (unsigned char) 127);
	REQUIRE((parse<unsigned char, std::error_code>("255")) == (unsigned char) 255);

	REQUIRE(convert<std::string>((char)  0) ==  "0");
	REQUIRE(convert<std::string>((char)  1) ==  "1");

	REQUIRE(convert<std::string>((signed char) -1) == "-1");
	REQUIRE(convert<std::string>((signed char)  0) ==  "0");
	REQUIRE(convert<std::string>((signed char)  1) ==  "1");

	REQUIRE(convert<std::string>((unsigned char) 0) ==  "0");
	REQUIRE(convert<std::string>((unsigned char) 1) ==  "1");
}

TEST_CASE("string/numerical conversions work for short", "[convert]") {
	REQUIRE((parse<short, std::error_code>(     "0")) == (short)      0);
	REQUIRE((parse<short, std::error_code>(     "1")) == (short)      1);
	REQUIRE((parse<short, std::error_code>(    "-1")) == (short)     -1);
	REQUIRE((parse<short, std::error_code>( "32767")) == (short)  32767);
	REQUIRE((parse<short, std::error_code>("-32767")) == (short) -32767);

	REQUIRE((parse<unsigned short, std::error_code>(    "0")) == (unsigned short)     0);
	REQUIRE((parse<unsigned short, std::error_code>(    "1")) == (unsigned short)     1);
	REQUIRE((parse<unsigned short, std::error_code>("32767")) == (unsigned short) 32767);
	REQUIRE((parse<unsigned short, std::error_code>("65535")) == (unsigned short) 65535);

	REQUIRE(convert<std::string>((short) -1) == "-1");
	REQUIRE(convert<std::string>((short)  0) ==  "0");
	REQUIRE(convert<std::string>((short)  1) ==  "1");

	REQUIRE(convert<std::string>((unsigned short) 0) ==  "0");
	REQUIRE(convert<std::string>((unsigned short) 1) ==  "1");

}

TEST_CASE("string/numerical conversions work for int", "[convert]") {
	REQUIRE((parse<int, std::error_code>(          "0")) == (int)           0);
	REQUIRE((parse<int, std::error_code>(          "1")) == (int)           1);
	REQUIRE((parse<int, std::error_code>(         "-1")) == (int)          -1);
	REQUIRE((parse<int, std::error_code>( "2147483647")) == (int)  2147483647);
	REQUIRE((parse<int, std::error_code>("-2147483647")) == (int) -2147483647);

	REQUIRE((parse<unsigned int, std::error_code>(         "0")) == (unsigned int)          0);
	REQUIRE((parse<unsigned int, std::error_code>(         "1")) == (unsigned int)          1);
	REQUIRE((parse<unsigned int, std::error_code>("2147483647")) == (unsigned int) 2147483647);
	REQUIRE((parse<unsigned int, std::error_code>("4294967295")) == (unsigned int) 4294967295);

	REQUIRE(convert<std::string>((int) -1) == "-1");
	REQUIRE(convert<std::string>((int)  0) ==  "0");
	REQUIRE(convert<std::string>((int)  1) ==  "1");

	REQUIRE(convert<std::string>((unsigned int) 0) ==  "0");
	REQUIRE(convert<std::string>((unsigned int) 1) ==  "1");
}

TEST_CASE("string/numerical conversions work for long", "[convert]") {
	REQUIRE((parse<long, std::error_code>(          "0")) == (long)           0);
	REQUIRE((parse<long, std::error_code>(          "1")) == (long)           1);
	REQUIRE((parse<long, std::error_code>(         "-1")) == (long)          -1);
	REQUIRE((parse<long, std::error_code>( "2147483647")) == (long)  2147483647);
	REQUIRE((parse<long, std::error_code>("-2147483647")) == (long) -2147483647);

	REQUIRE((parse<unsigned long, std::error_code>(         "0")) == (unsigned long)          0);
	REQUIRE((parse<unsigned long, std::error_code>(         "1")) == (unsigned long)          1);
	REQUIRE((parse<unsigned long, std::error_code>("2147483647")) == (unsigned long) 2147483647);
	REQUIRE((parse<unsigned long, std::error_code>("4294967295")) == (unsigned long) 4294967295);

	REQUIRE(convert<std::string>((long) -1) == "-1");
	REQUIRE(convert<std::string>((long)  0) ==  "0");
	REQUIRE(convert<std::string>((long)  1) ==  "1");

	REQUIRE(convert<std::string>((unsigned long) 0) ==  "0");
	REQUIRE(convert<std::string>((unsigned long) 1) ==  "1");
}

TEST_CASE("string/numerical conversions work for long long", "[convert]") {
	REQUIRE((parse<long, std::error_code>(                   "0")) == (long)                    0);
	REQUIRE((parse<long, std::error_code>(                   "1")) == (long)                    1);
	REQUIRE((parse<long, std::error_code>(                  "-1")) == (long)                   -1);
	REQUIRE((parse<long, std::error_code>( "9223372036854775807")) == (long)  9223372036854775807);
	REQUIRE((parse<long, std::error_code>("-9223372036854775807")) == (long) -9223372036854775807);

	REQUIRE((parse<unsigned long long, std::error_code>("0"))                     == (unsigned long long)                    0u);
	REQUIRE((parse<unsigned long long, std::error_code>("1"))                     == (unsigned long long)                    1u);
	REQUIRE((parse<unsigned long long, std::error_code>("9223372036854775807"))   == (unsigned long long)  9223372036854775807u);
	REQUIRE((parse<unsigned long long, std::error_code>("18446744073709551615"))  == (unsigned long long) 18446744073709551615u);

	REQUIRE(convert<std::string>((long long) -1) == "-1");
	REQUIRE(convert<std::string>((long long)  0) ==  "0");
	REQUIRE(convert<std::string>((long long)  1) ==  "1");

	REQUIRE(convert<std::string>((unsigned long long) 0) ==  "0");
	REQUIRE(convert<std::string>((unsigned long long) 1) ==  "1");
}

TEST_CASE("invalid patterns give an error") {
	REQUIRE((parse<int, std::error_code>("aap")) == make_error_code(std::errc::invalid_argument));
	REQUIRE((parse<int, std::error_code>("123aap")) == make_error_code(std::errc::invalid_argument));
	REQUIRE((parse<unsigned, std::error_code>("-1")) == make_error_code(std::errc::invalid_argument));
	REQUIRE((parse<short, std::error_code>("1000000000")) == make_error_code(std::errc::result_out_of_range));
}


}
