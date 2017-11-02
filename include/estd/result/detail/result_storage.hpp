#include "../in_place.hpp"
#include "copyable.hpp"

#include <type_traits>
#include <utility>

namespace estd {
namespace detail {

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
	std::aligned_union_t<1, T, E> data_;
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
		new (&as_expected()) T(std::forward<Args>(args)...);
		valid_ = true;
	}

	template<typename... Args>
	result_storage_base(in_place_error_t, Args && ... args) {
		new (&as_unexpected()) E(std::forward<Args>(args)...);
		valid_ = false;
	}

	result_storage_base(result_storage_base const & other) {
		valid_ = other.valid_;
		if (valid_) new (&as_expected())   T(other.as_expected());
		else        new (&as_unexpected()) E(other.as_unexpected());
	}

	result_storage_base(result_storage_base && other) {
		valid_ = other.valid_;
		if (valid_) new (&as_expected())   T(std::move(other.as_expected()));
		else        new (&as_unexpected()) E(std::move(other.as_unexpected()));
	}

	/// Allow explicit conversion from ErrorOr<T2, E2>.
	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result_storage_base(result_storage_base<T2, E2> && other) {
		valid_ = other.valid_;
		if (!valid_) new (&as_unexpected()) E(std::move(other.as_unexpected()));
		else         new (&as_expected())   T(std::move(other.as_expected()));
	}

	template<typename T2, typename E2, typename C = std::enable_if_t<explicitly_convertible_<T2, E2>>>
	explicit result_storage_base(result_storage_base<T2, E2> const & other) {
		valid_ = other.valid_;
		if (!valid_) new (&as_unexpected()) E(other.as_unexpected());
		else         new (&as_expected())   T(other.as_expected());
	}

	result_storage_base & operator= (result_storage_base const & other) {
		if (valid_ != other.valid_) {
			this->~result_storage_base();
			new (this) result_storage_base(other);
		} else {
			if (valid_) as_expected()   = other.as_expected();
			else        as_unexpected() = other.as_unexpected();
		}
		return *this;
	}

	result_storage_base & operator= (result_storage_base && other) {
		if (valid_ != other.valid_) {
			this->~result_storage_base();
			new (this) result_storage_base(std::move(other));
		} else {
			if (valid_) as_expected()   = std::move(other.as_expected());
			else        as_unexpected() = std::move(other.as_unexpected());
		}
		return *this;
	}

	~result_storage_base() {
		if (valid_) as_expected().~T();
		else        as_unexpected().~E();
	}

	template<typename T2, typename E2>
	bool operator==(result_storage_base<T2, E2> const & other) const {
		if (valid() != other.valid()) return false;
		if (!valid()) return as_unexpected() == other.as_unexpected();
		return as_expected() == other.as_expected();
	}

	template<typename T2, typename E2>
	bool operator!=(result_storage_base<T2, E2> const & other) const {
		return !(*this == other);
	}

	bool valid() const { return valid_; }

	T       & as_expected()       { return reinterpret_cast<T       &>(data_); }
	T const & as_expected() const { return reinterpret_cast<T const &>(data_); }

	E       & as_unexpected()       { return reinterpret_cast<E       &>(data_); }
	E const & as_unexpected() const { return reinterpret_cast<E const &>(data_); }
};


}}
