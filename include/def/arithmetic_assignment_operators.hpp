#ifndef BIG_INT_ARITHMETIC_ASSIGNMENT_OPERATORS_HPP
#define BIG_INT_ARITHMETIC_ASSIGNMENT_OPERATORS_HPP

#include <stdexcept>

#include "../big_int.hpp"
#include "assignment_operators.hpp"
#include "comparison_operators.hpp"
#include "member_functions.hpp"
#include "static_functions.hpp"
#include "helper_functions.hpp"
#include "external_functions.hpp"

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator+=(const BigInt<base>& other)
{
	if(other.is_zero())
		return *this;
	else if(positive() != other.positive()) {
		if(is_zero()) {
			*this = other;
			negative = positive();
			return *this;
		}
		return __sub_eq_signless(other);
	} else {
		if(is_zero())
			return *this = other;
		return __add_eq_signless(other);
	}
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator-=(const BigInt<base>& other)
{
	if(other.is_zero())
		return *this;
	else if(positive() != other.positive()) {
		if(is_zero())
			return *this = other;
		return __add_eq_signless(other);
	} else {
		if(is_zero()) {
			*this = other;
			negative = positive();
			return *this;
		}
		return __sub_eq_signless(other);
	}
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator*=(const BigInt<base>& other)
{
	if(is_zero() || other == 1)
		return *this;
	else if(other == -1) {
		negative = positive();
		return *this;
	} else if(other.is_zero() || *this == 1)
		return *this = other;
	else if(*this == -1) {
		*this = other;
		negative = positive();
		return *this;
	}

	negative ^= other.negative;
	return __mul_eq_signless(other);
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator/=(const BigInt<base>& other)
{
	if(other.is_zero())
		throw std::domain_error(__THROW_DIVIDE_ZERO_DOMAIN);
	if(is_zero() || other == 1)
		return *this;
	else if(other == -1) {
		negative = positive();
		return *this;
	} else if(__compare_signless(other) == __LESS_SO)
		return *this = 0; // size() >= other.size() guaranteed

	negative ^= other.negative;
	return __div_mod_eq_signless<false>(other);
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator%=(const BigInt<base>& other)
{
	if(other.is_zero())
		throw std::domain_error(__THROW_DIVIDE_ZERO_DOMAIN);
	if(is_zero() || __compare_signless(other) == __LESS_SO)
		return *this; // size() >= other.size() guaranteed
	else if(other == 1 || other == -1)
		return *this = 0;

	return __div_mod_eq_signless<true>(other);
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator&=(const BigInt<base>& other)
{
	const auto neg = !positive() || !other.positive();
	if(is_zero()) {
		negative = neg;
		return *this;
	} else if(other.is_zero()) {
		*this = other;
		negative = neg;
		return *this;
	}

	negative = neg;
	return __bitwise_signless<BitwiseType::AND>(other);
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator|=(const BigInt<base>& other)
{
	const auto neg = !positive() && !other.positive();
	if(other.is_zero()) {
		negative = neg;
		return *this;
	} else if(is_zero()) {
		*this = other;
		negative = neg;
		return *this;
	}

	negative = neg;
	return __bitwise_signless<BitwiseType::OR>(other);
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator^=(const BigInt<base>& other)
{
	const auto neg = positive() == other.positive();
	if(other.is_zero()) {
		negative = neg;
		return *this;
	} else if(is_zero()) {
		*this = other;
		negative = neg;
		return *this;
	}

	negative = neg;
	return __bitwise_signless<BitwiseType::XOR>(other);
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator<<=(const BigInt<base>& other)
{
	return __shl_shr_eq_helper<false>(other);
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator>>=(const BigInt<base>& other)
{
	return __shl_shr_eq_helper<true>(other);
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator+=(const std::int64_t other)
{
	if(other == 0)
		return *this;
	else if(positive() != (other >= 0)) {
		if(is_zero()) {
			*this = other;
			negative = positive();
			return *this;
		}
		return __sub_eq_signless(__constexpr_abs(other)); // Specialisation
	} else {
		if(is_zero())
			return *this = other;
		return __add_eq_signless(__constexpr_abs(other)); // Specialisation
	}
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator-=(const std::int64_t other)
{
	if(other == 0)
		return *this;
	else if(positive() != (other >= 0)) {
		if(is_zero())
			return *this = other;
		return __add_eq_signless(__constexpr_abs(other)); // Specialisation
	} else {
		if(is_zero()) {
			*this = other;
			negative = positive();
			return *this;
		}
		return __sub_eq_signless(__constexpr_abs(other)); // Specialisation
	}
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator*=(const std::int64_t other)
{
	if(is_zero() || other == 1)
		return *this;
	else if(other == -1) {
		negative = positive();
		return *this;
	} else if(other == 0 || *this == 1)
		return *this = other;
	else if(*this == -1) {
		*this = other;
		negative = positive();
		return *this;
	}

	negative ^= other < 0;
	return __mul_eq_signless(__constexpr_abs(other)); // Specialisation
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator/=(const std::int64_t other)
{
	if(other == 0)
		throw std::domain_error(__THROW_DIVIDE_ZERO_DOMAIN);
	if(is_zero() || other == 1)
		return *this;
	else if(other == -1) {
		negative = positive();
		return *this;
	}
	const auto other_abs = __constexpr_abs(other);
	if(__compare_signless(other_abs) == __LESS_SO)
		return *this = 0; // size() >= "other.size()" guaranteed

	negative ^= other < 0;
	return __div_mod_eq_signless<false>(other_abs); // Specialisation
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator%=(const std::int64_t other)
{
	if(other == 0)
		throw std::domain_error(__THROW_DIVIDE_ZERO_DOMAIN);
	const auto other_abs = __constexpr_abs(other);
	if(is_zero() || __compare_signless(other_abs) == __LESS_SO)
		return *this; // size() >= "other.size()" guaranteed
	else if(other == 1 || other == -1)
		return *this = 0;

	return __div_mod_eq_signless<true>(other_abs); // Specialisation
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator&=(const std::int64_t other)
{
	const auto neg = !positive() || (other < 0);
	if(is_zero()) {
		negative = neg;
		return *this;
	} else if(other == 0) {
		*this = other;
		negative = neg;
		return *this;
	}

	negative = neg;
	return __bitwise_signless<BitwiseType::AND>(__constexpr_abs(other)); // Specialisation
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator|=(const std::int64_t other)
{
	const auto neg = !positive() && (other < 0);
	if(other == 0) {
		negative = neg;
		return *this;
	} else if(is_zero()) {
		*this = other;
		negative = neg;
		return *this;
	}

	negative = neg;
	return __bitwise_signless<BitwiseType::OR>(__constexpr_abs(other)); // Specialisation
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator^=(const std::int64_t other)
{
	const auto neg = positive() == (other >= 0);
	if(other == 0) {
		negative = neg;
		return *this;
	} else if(is_zero()) {
		*this = other;
		negative = neg;
		return *this;
	}

	negative = neg;
	return __bitwise_signless<BitwiseType::XOR>(__constexpr_abs(other)); // Specialisation
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator<<=(const std::int64_t other)
{
	return __shl_shr_eq_helper<false>(other); // Specialisation
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator>>=(const std::int64_t other)
{
	return __shl_shr_eq_helper<true>(other); // Specialisation
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::operator+=(const T other)
{
	if constexpr(INT64Constructible<T>)
		return *this += static_cast<std::int64_t>(other);
	else
		return *this += make_big_int<base>(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::operator-=(const T other)
{
	if constexpr(INT64Constructible<T>)
		return *this -= static_cast<std::int64_t>(other);
	else
		return *this -= make_big_int<base>(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::operator*=(const T other)
{
	if constexpr(INT64Constructible<T>)
		return *this *= static_cast<std::int64_t>(other);
	else
		return *this *= make_big_int<base>(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::operator/=(const T other)
{
	if constexpr(INT64Constructible<T>)
		return *this /= static_cast<std::int64_t>(other);
	else
		return *this /= make_big_int<base>(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::operator%=(const T other)
{
	if constexpr(INT64Constructible<T>)
		return *this %= static_cast<std::int64_t>(other);
	else
		return *this %= make_big_int<base>(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::operator&=(const T other)
{
	if constexpr(INT64Constructible<T>)
		return *this &= static_cast<std::int64_t>(other);
	else
		return *this &= make_big_int<base>(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::operator|=(const T other)
{
	if constexpr(INT64Constructible<T>)
		return *this |= static_cast<std::int64_t>(other);
	else
		return *this |= make_big_int<base>(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::operator^=(const T other)
{
	if constexpr(INT64Constructible<T>)
		return *this ^= static_cast<std::int64_t>(other);
	else
		return *this ^= make_big_int<base>(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::operator<<=(const T other)
{
	if constexpr(INT64Constructible<T>)
		return *this <<= static_cast<std::int64_t>(other);
	else
		return *this <<= make_big_int<base>(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::operator>>=(const T other)
{
	if constexpr(INT64Constructible<T>)
		return *this >>= static_cast<std::int64_t>(other);
	else
		return *this >>= make_big_int<base>(other);
}

#endif // BIG_INT_ARITHMETIC_ASSIGNMENT_OPERATORS_HPP

