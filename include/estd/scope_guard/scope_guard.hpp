#pragma once
#include <utility>

namespace estd {

template<typename F>
class scope_guard {
private:
	/// The function to execute when the scope guard gets deleted.
	F f;

	/// State of the scope guard.
	bool valid = true;

public:
	scope_guard(F f) : f(std::move(f)) {}

	scope_guard(scope_guard && other): f{std::move(other.f)} {
		other.reset();
	}

	scope_guard & operator= (scope_guard && other) {
		f = std::move(other.f);
		other.reset();
	}

	~scope_guard() {
		// Run the function to notify we've run out of scope.
		if (valid) f();
		valid = false;
	}

	/// Disables the scope guard.
	void reset() {
		valid = false;
	}
};

}
