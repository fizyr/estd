#pragma once
namespace estd {
namespace detail {

template<bool constructible = false, bool assignable = false>
struct non_copyable;

template<bool constructible = false, bool assignable = false>
struct non_movable;

template<>
struct non_copyable<false,false> {
	non_copyable()                                  = default;
	non_copyable(non_copyable const  &)             = delete;
	non_copyable(non_copyable       &&)             = default;
	non_copyable & operator=(non_copyable const  &) = delete;
	non_copyable & operator=(non_copyable       &&) = default;
};

template<>
struct non_copyable<false, true> {
	non_copyable()                                  = default;
	non_copyable(non_copyable const  &)             = delete;
	non_copyable(non_copyable       &&)             = default;
	non_copyable & operator=(non_copyable const  &) = default;
	non_copyable & operator=(non_copyable       &&) = default;
};

template<>
struct non_copyable<true, false> {
	non_copyable()                                  = default;
	non_copyable(non_copyable const  &)             = default;
	non_copyable(non_copyable       &&)             = default;
	non_copyable & operator=(non_copyable const  &) = delete;
	non_copyable & operator=(non_copyable       &&) = default;
};

template<>
struct non_copyable<true, true> {
	non_copyable() = default;
};

template<>
struct non_movable<false,false> {
	non_movable()                           = default;
	non_movable(non_movable &&)             = delete;
	non_movable & operator=(non_movable &&) = delete;
};

template<>
struct non_movable<false, true> {
	non_movable()                           = default;
	non_movable(non_movable &&)             = delete;
	non_movable & operator=(non_movable &&) = default;
};

template<>
struct non_movable<true, false> {
	non_movable()                           = default;
	non_movable(non_movable &&)             = default;
	non_movable & operator=(non_movable &&) = delete;
};

template<>
struct non_movable<true, true> {
	non_movable() = default;
};


}}
