#pragma once
#include <string>

#if !defined(ESTD_NO_DEMANGLE)
#define ESTD_NO_DEMANGLE 0
#endif

#if !ESTD_NO_DEMANGLE && __has_include(<cxxabi.h>)
#define ESTD_DEMANGLE_CXXABI
#include <cstdlib>
#include <cxxabi.h>
#endif

namespace estd {

#ifdef ESTD_DEMANGLE_CXXABI
std::string demangle(std::string const & mangled) {
	int status;
	char * demangled = abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status);
	if (status != 0) {
		std::free(demangled);
		return mangled;
	}

	std::string string = demangled;
	std::free(demangled);
	return string;
}
#else
std::string demangle(std::string const & mangled) {
	return mangled;
}
#endif

}
