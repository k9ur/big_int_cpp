#ifndef BIG_INT_COMPARISON_OPERATORS_HPP
#define BIG_INT_COMPARISON_OPERATORS_HPP

#include "../big_int.hpp"
#include "member_functions.hpp"
#include "static_functions.hpp"
#include "helper_functions.hpp"
#include "external_functions.hpp"

template<std::uint32_t base>
constexpr bool BigInt<base>::operator==(const BigInt<base>& other) const noexcept
{
	if(positive() != other.positive())
		return is_zero() && other.is_zero();
	if(bigits != other.bigits)
		return false;
	return true;
}

template<std::uint32_t base>
constexpr bool BigInt<base>::operator!=(const BigInt<base>& other) const noexcept
{
	return !(*this == other);
}

template<std::uint32_t base>
constexpr std::strong_ordering BigInt<base>::operator<=>(const BigInt<base>& other) const noexcept
{
	if(positive() != other.positive()) {
		if(is_zero() && other.is_zero())
			return __EQUAL_SO;
		return positive() <=> other.positive();
	}
	return __compare_signless(other, positive());
}

template<std::uint32_t base>
constexpr bool BigInt<base>::operator==(std::int64_t other) const noexcept
{
	if(positive() != (other >= 0))
		return is_zero() && other == 0;
	other = __constexpr_abs(other);
	if(size() != __int_size(other))
		return false;
	for(auto& bigit : bigits) {
		if(other % true_base() != bigit)
			return false;
		other /= true_base();
	}
	return true;
}

template<std::uint32_t base>
constexpr std::strong_ordering BigInt<base>::operator<=>(std::int64_t other) const noexcept
{
	const auto other_positive = other >= 0;
	if(positive() != other_positive) {
		if(is_zero() && other == 0)
			return __EQUAL_SO;
		return positive() <=> other_positive;
	}
	return __compare_signless(__constexpr_abs(other), positive()); // Specialisation
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr bool BigInt<base>::operator==(const T other) const noexcept(INT64Constructible<T>)
{
	if constexpr(INT64Constructible<T>)
		return *this == static_cast<std::int64_t>(other);
	else
		return *this == make_big_int<base>(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr bool BigInt<base>::operator!=(const T other) const noexcept(INT64Constructible<T>)
{
	return !(*this == other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr std::strong_ordering BigInt<base>::operator<=>(const T other) const noexcept(INT64Constructible<T>)
{
	if constexpr(INT64Constructible<T>)
		return *this <=> static_cast<std::int64_t>(other);
	else
		return *this <=> make_big_int<base>(other);
}

#endif // BIG_INT_COMPARISON_OPERATORS_HPP

