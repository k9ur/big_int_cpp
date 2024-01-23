#ifndef BIG_INT_UNARY_OPERATORS_HPP
#define BIG_INT_UNARY_OPERATORS_HPP

#include "../big_int.hpp"
#include "arithmetic_assignment_operators.hpp"
#include "member_functions.hpp"
#include "static_functions.hpp"
#include "helper_functions.hpp"

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator+() const
{
	auto temp{ clone() };
	temp.negative = false;
	return temp;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator-() const
{
	auto temp{ clone() };
	temp.negative = positive();
	return temp;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator~() const
{
	if constexpr(__int_log_of(true_base(), 2)) {
		auto temp{ clone() };
		return temp.__not_helper();
	} else { // Convert to a binary base, then call, then convert base back: O(N^2)
		BigInt<MAX_BASE> this_bin_base{ clone() }; // May give unexpected results when using with a non-binary base, since it uses 32 bits
		return this_bin_base.__not_helper(); // Implicitly converted back to BigInt<base>
	}
}


template<std::uint32_t base>
constexpr BigInt<base>::operator bool() const noexcept
{
	return !is_zero();
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator++()
{
	return *this += 1;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator--()
{
	return *this -= 1;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator++(int)
{
	const auto old{ clone() };
	operator++();
	return old;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator--(int)
{
	const auto old{ clone() };
	operator--();
	return old;
}

#endif // BIG_INT_UNARY_OPERATORS_HPP


