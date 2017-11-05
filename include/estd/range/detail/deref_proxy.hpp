#pragma once

#include <utility>

namespace estd {
namespace detail {
	// Proxy object that can be dereferenced to some value.
	template<typename T>
	class DerefProxy {
		T val;

	public:
		DerefProxy(T const  & val) : val(val) {}
		DerefProxy(T       && val) : val(std::move(val)) {}

		T       & operator* ()       { return val; }
		T const & operator* () const { return val; }
		T       * operator->()       { return &val; }
		T const * operator->() const { return &val; }
	};
}}
