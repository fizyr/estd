#include "../in_place.hpp"
#include "copyable.hpp"

#include <type_traits>
#include <utility>

namespace estd {
namespace detail {

// Storage traits for non-reference types.
template<typename T>
struct result_storage_traits {
	using decayed      = std::decay_t<T>;
	using storage_type = T;

	template<typename ...Args>
	constexpr static void construct(storage_type & storage, Args && ...args) { new (&storage) T(std::forward<Args>(args)...); }
	constexpr static void destroy(storage_type & storage) { storage.~T(); }

	template<typename Y>
	constexpr static void copy(storage_type & storage, Y && other) { new (&storage) T(std::forward<Y>(other)); }

	constexpr static std::decay_t<T>       & ref(std::decay_t<T>       & storage) { return storage; }
	constexpr static std::decay_t<T> const & ref(std::decay_t<T> const & storage) { return storage; }
};

// Storage traits for reference types.
template<typename T>
struct result_storage_traits<T &> {
	using decayed      = std::decay_t<T>;
	using storage_type = T *;

	constexpr static void construct(storage_type & storage, T  & value) { storage = &value; }
	constexpr static void destroy(storage_type &) {}

	constexpr static void copy(storage_type & storage, storage_type const & other) { storage = other; }

	constexpr static T       & ref(std::decay_t<storage_type>       & storage) { return *storage; }
	constexpr static T const & ref(std::decay_t<storage_type> const & storage) { return *storage; }
};

// Storage traits for r-value reference types.
template<typename T>
struct result_storage_traits<T &&> {
	using decayed      = std::decay_t<T>;
	using storage_type = T *;

	constexpr static void construct(storage_type & storage, T && value) { storage = &value; }
	constexpr static void destroy(storage_type &) {}

	constexpr static void copy(storage_type & storage, storage_type const & other) { storage = other; }

	constexpr static T       && ref(std::decay_t<storage_type>       && storage) { return *storage; }
	constexpr static T const && ref(std::decay_t<storage_type> const && storage) { return *storage; }
};

template<typename T, typename E>
class result_storage_base;

template<typename T, typename E>
struct result_storage_type {
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
using result_storage = typename result_storage_type<T, E>::type;

template<typename T, typename E>
class result_storage_base {
	using ok_storage    = typename result_storage_traits<T>::storage_type;
	using error_storage = typename result_storage_traits<E>::storage_type;

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
	template<typename... Args>
	result_storage_base(in_place_valid_t, Args && ... args) {
		result_storage_traits<T>::construct(as_ok_storage(), std::forward<Args>(args)...);
		valid_ = true;
	}

	template<typename... Args>
	result_storage_base(in_place_error_t, Args && ... args) {
		result_storage_traits<E>::construct(as_error_storage(), std::forward<Args>(args)...);
		valid_ = false;
	}

	result_storage_base(result_storage_base const & other) {
		valid_ = other.valid_;
		if (valid_) result_storage_traits<T>::copy(as_ok_storage(),    other.as_ok_storage());
		else        result_storage_traits<E>::copy(as_error_storage(), other.as_error_storage());
	}

	result_storage_base(result_storage_base && other) {
		valid_ = other.valid_;
		if (valid_) result_storage_traits<T>::copy(as_ok_storage(),    std::move(other.as_ok_storage()));
		else        result_storage_traits<E>::copy(as_error_storage(), std::move(other.as_error_storage()));
	}

	/// Allow explicit conversion from ErrorOr<T2, E2>.
	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result_storage_base(result_storage_base<T2, E2> const & other) {
		valid_ = other.valid_;
		if (valid_) result_storage_traits<T>::copy(as_ok_storage(),    other.as_ok_storage());
		else        result_storage_traits<E>::copy(as_error_storage(), other.as_error_storage());
	}

	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result_storage_base(result_storage_base<T2, E2> && other) {
		valid_ = other.valid_;
		if (valid_) result_storage_traits<T>::copy(as_ok_storage(),    std::move(other.as_ok_storage()));
		else        result_storage_traits<E>::copy(as_error_storage(), std::move(other.as_error_storage()));
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
		if (valid_) result_storage_traits<T>::destroy(as_ok_storage());
		else        result_storage_traits<E>::destroy(as_error_storage());
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

	std::remove_reference_t<T>       & as_ok()       { return result_storage_traits<T>::ref(as_ok_storage());}
	std::remove_reference_t<T> const & as_ok() const { return result_storage_traits<T>::ref(as_ok_storage()); }

	std::remove_reference_t<E>       & as_error()       { return result_storage_traits<E>::ref(as_error_storage()); }
	std::remove_reference_t<E> const & as_error() const { return result_storage_traits<E>::ref(as_error_storage()); }

private:
	ok_storage          & as_ok_storage()          { return reinterpret_cast<ok_storage          &>(data_); }
	ok_storage    const & as_ok_storage()    const { return reinterpret_cast<ok_storage    const &>(data_); }
	error_storage       & as_error_storage()       { return reinterpret_cast<error_storage       &>(data_); }
	error_storage const & as_error_storage() const { return reinterpret_cast<error_storage const &>(data_); }
};


}}
