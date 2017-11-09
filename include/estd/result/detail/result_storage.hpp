#include "../in_place.hpp"
#include "copyable.hpp"

#include <type_traits>
#include <utility>

namespace estd {
namespace detail {

// Wrapper to transparantly store references in result<T, E>
template<typename T>
struct lvalue_ref_wrapper {
	T * value_;

	lvalue_ref_wrapper(T & value) : value_{&value} {}

	operator T & () {
		return *value_;
	}

	operator T const & () const {
		return *value_;
	}

};

// Wrapper to transparantly store rvalue references in result<T, E>
template<typename T>
struct rvalue_ref_wrapper {
	T * value_;

	rvalue_ref_wrapper(T && value) : value_{&value} {}

	operator T && () {
		return *value_;
	}

	operator T const & () const {
		return *value_;
	}
};

// Determine the storage type in a result<T, E> for some type.
template<typename T> struct result_type_storage_impl       { using type = T; };
template<typename T> struct result_type_storage_impl<T &>  { using type = lvalue_ref_wrapper<T>; };
template<typename T> struct result_type_storage_impl<T &&> { using type = rvalue_ref_wrapper<T>; };
template<typename T> using result_type_storage = typename result_type_storage_impl<T>::type;

template<typename T, typename E>
class result_storage_base;

// Create a type derived from result_storage_base that is just as copyable as T and E.
template<typename T, typename E>
struct result_storage_maybe_copyable {
	constexpr static bool copy_constructible = std::is_copy_constructible<T>{} && std::is_copy_constructible<E>{};
	constexpr static bool copy_assignable    = std::is_copy_assignable<T>{}    && std::is_copy_assignable<E>{};
	constexpr static int copyable            = copy_constructible && copy_assignable;

	constexpr static bool move_constructible = std::is_move_constructible<T>{} && std::is_move_constructible<E>{};
	constexpr static bool move_assignable    = std::is_move_assignable<T>{}    && std::is_move_assignable<E>{};
	constexpr static int movable             = move_constructible && move_assignable;

	class type :
		public result_storage_base<T, E>,
		private non_copyable<copy_constructible, copyable>,
		private non_movable<move_constructible, movable>
	{
		using result_storage_base<T, E>::result_storage_base;
	};
};

template<typename T, typename E>
using result_storage = typename result_storage_maybe_copyable<T, E>::type;

template<typename T, typename E>
class result_storage_base {
	using ok_storage    = result_type_storage<T>;
	using error_storage = result_type_storage<E>;

	std::aligned_union_t<1, ok_storage, error_storage> data_;
	bool valid_;

	template<typename T2, typename E2>
	friend class result_storage_base;

	template<typename T2, typename E2>
	constexpr static bool implicitly_convertible_() {
		return std::is_convertible<T2, T>{} && std::is_convertible<E2, E>{};
	}

	template<typename T2, typename E2>
	constexpr static bool explicitly_convertible_ = std::is_constructible<T, T2>::value && std::is_constructible<E, E2>::value;

public:
	template<typename... Args, typename = std::enable_if_t<std::is_constructible_v<ok_storage, Args...>>>
	result_storage_base(in_place_valid_t, Args && ... args) {
		new (&as_ok_storage()) ok_storage(std::forward<Args>(args)...);
		valid_ = true;
	}

	template<typename... Args, typename = std::enable_if_t<std::is_constructible_v<error_storage, Args...>>>
	result_storage_base(in_place_error_t, Args && ... args) {
		new (&as_error_storage()) error_storage(std::forward<Args>(args)...);
		valid_ = false;
	}

	result_storage_base(result_storage_base const & other) {
		valid_ = other.valid_;
		if (valid_) new (&as_ok_storage())       ok_storage(other.as_ok_storage());
		else        new (&as_error_storage()) error_storage(other.as_error_storage());
	}

	result_storage_base(result_storage_base && other) {
		valid_ = other.valid_;
		if (valid_) new (&as_ok_storage())       ok_storage(std::move(other.as_ok_storage()));
		else        new (&as_error_storage()) error_storage(std::move(other.as_error_storage()));
	}

	/// Allow explicit conversion from ErrorOr<T2, E2>.
	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result_storage_base(result_storage_base<T2, E2> const & other) {
		valid_ = other.valid_;
		if (valid_) new (&as_ok_storage())       ok_storage(other.as_ok_storage());
		else        new (&as_error_storage()) error_storage(other.as_error_storage());
	}

	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result_storage_base(result_storage_base<T2, E2> && other) {
		valid_ = other.valid_;
		if (valid_) new (&as_ok_storage())       ok_storage(std::move(other.as_ok_storage()));
		else        new (&as_error_storage()) error_storage(std::move(other.as_error_storage()));
	}

	result_storage_base & operator= (result_storage_base const & other) {
		if (valid_ != other.valid_) {
			this->~result_storage_base();
			new (this) result_storage_base(other);
		} else {
			if (valid_) as_ok_storage()    = other.as_ok_storage();
			else        as_error_storage() = other.as_error_storage();
		}
		return *this;
	}

	result_storage_base & operator= (result_storage_base && other) {
		if (valid_ != other.valid_) {
			this->~result_storage_base();
			new (this) result_storage_base(std::move(other));
		} else {
			if (valid_) as_ok_storage()    = std::move(other.as_ok_storage());
			else        as_error_storage() = std::move(other.as_error_storage());
		}
		return *this;
	}

	~result_storage_base() {
		if (valid_)    as_ok_storage().~ok_storage();
		else        as_error_storage().~error_storage();
	}

	template<typename T2, typename E2>
	bool operator==(result_storage_base<T2, E2> const & other) const {
		if (valid() != other.valid()) return false;
		if (!valid()) return as_error() == other.as_error();
		return as_ok() == other.as_ok();
	}

	template<typename T2, typename E2>
	bool operator!=(result_storage_base<T2, E2> const & other) const {
		return !(*this == other);
	}

	bool valid() const { return valid_; }

	std::remove_reference_t<T>       & as_ok()       { return as_ok_storage();}
	std::remove_reference_t<T> const & as_ok() const { return as_ok_storage(); }

	std::remove_reference_t<E>       & as_error()       { return as_error_storage(); }
	std::remove_reference_t<E> const & as_error() const { return as_error_storage(); }

private:
	ok_storage          & as_ok_storage()          { return reinterpret_cast<ok_storage          &>(data_); }
	ok_storage    const & as_ok_storage()    const { return reinterpret_cast<ok_storage    const &>(data_); }
	error_storage       & as_error_storage()       { return reinterpret_cast<error_storage       &>(data_); }
	error_storage const & as_error_storage() const { return reinterpret_cast<error_storage const &>(data_); }
};


}}
