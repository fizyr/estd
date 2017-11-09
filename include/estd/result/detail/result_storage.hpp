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
	operator T        & ()        & { return *value_; }
	operator T const  & () const  & { return *value_; }
	operator T       && ()       && { return std::move(*value_); }
};

// Wrapper to transparantly store rvalue references in result<T, E>
template<typename T>
struct rvalue_ref_wrapper {
	T * value_;
	rvalue_ref_wrapper(T && value) : value_{&value} {}
	operator T       && ()       { return *value_; }
	operator T const && () const { return *value_; }
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
	using Valid = result_type_storage<T>;
	using Error = result_type_storage<E>;

	union {
		Valid valid_;
		Error error_;
	};

	bool is_valid_;

	template<typename T2, typename E2>
	friend class result_storage_base;

	template<typename T2, typename E2>
	constexpr static bool explicitly_convertible_ = std::is_constructible<T, T2>::value && std::is_constructible<E, E2>::value;

public:
	template<typename... Args, typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
	result_storage_base(in_place_valid_t, Args && ... args) {
		new (&valid_) Valid(std::forward<Args>(args)...);
		is_valid_ = true;
	}

	template<typename... Args, typename = std::enable_if_t<std::is_constructible_v<E, Args...>>>
	result_storage_base(in_place_error_t, Args && ... args) {
		new (&error_) Error(std::forward<Args>(args)...);
		is_valid_ = false;
	}

	result_storage_base(result_storage_base const & other) {
		is_valid_ = other.is_valid_;
		if (is_valid_) new (&valid_) Valid(other.valid_);
		else           new (&error_) Error(other.error_);
	}

	result_storage_base(result_storage_base && other) {
		is_valid_ = other.is_valid_;
		if (is_valid_) new (&valid_) Valid(std::move(other.valid_));
		else           new (&error_) Error(std::move(other.error_));
	}

	/// Allow explicit conversion from ErrorOr<T2, E2>.
	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result_storage_base(result_storage_base<T2, E2> const & other) {
		is_valid_ = other.is_valid_;
		if (is_valid_) new (&valid_) Valid(other.valid_);
		else          new (&error_) Error(other.error_);
	}

	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result_storage_base(result_storage_base<T2, E2> && other) {
		is_valid_ = other.is_valid_;
		if (is_valid_) new (&valid_) Valid(std::move(other.valid_));
		else           new (&error_) Error(std::move(other.error_));
	}

	result_storage_base & operator= (result_storage_base const & other) {
		if (is_valid_ != other.is_valid_) {
			this->~result_storage_base();
			new (this) result_storage_base(other);
		} else {
			if (is_valid_) valid_= other.valid_;
			else           error_= other.error_;
		}
		return *this;
	}

	result_storage_base & operator= (result_storage_base && other) {
		if (is_valid_ != other.is_valid_) {
			this->~result_storage_base();
			new (this) result_storage_base(std::move(other));
		} else {
			if (is_valid_) valid_= std::move(other.valid_);
			else           error_= std::move(other.error_);
		}
		return *this;
	}

	~result_storage_base() {
		if (is_valid_) valid_.~Valid();
		else           error_.~Error();
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

	bool valid() const { return is_valid_; }

	T        & as_ok()        & { return valid_; }
	T const  & as_ok() const  & { return valid_; }
	T       && as_ok() const && { return valid_; }

	E        & as_error()        & { return error_; }
	E const  & as_error() const  & { return error_; }
	E       && as_error() const && { return error_; }
};


}}
