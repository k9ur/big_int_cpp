#ifndef BIG_INT_BINARY_ARITHMETIC_OPERATORS_HPP
#define BIG_INT_BINARY_ARITHMETIC_OPERATORS_HPP

#include "../big_int.hpp"
#include "arithmetic_assignment_operators.hpp"
#include "member_functions.hpp"

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator+(const BigInt<base>& other) const
{
	return clone() += other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator-(const BigInt<base>& other) const
{
	return clone() -= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator*(const BigInt<base>& other) const
{
	return clone() *= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator/(const BigInt<base>& other) const
{
	return clone() /= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator%(const BigInt<base>& other) const
{
	return clone() %= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator&(const BigInt<base>& other) const
{
	return clone() &= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator|(const BigInt<base>& other) const
{
	return clone() |= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator^(const BigInt<base>& other) const
{
	return clone() ^= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator<<(const BigInt<base>& other) const
{
	return clone() <<= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator>>(const BigInt<base>& other) const
{
	return clone() >>= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator+(const BigIntConstructible auto other) const
{
	return clone() += other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator-(const BigIntConstructible auto other) const
{
	return clone() -= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator*(const BigIntConstructible auto other) const
{
	return clone() *= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator/(const BigIntConstructible auto other) const
{
	return clone() /= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator%(const BigIntConstructible auto other) const
{
	return clone() %= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator&(const BigIntConstructible auto other) const
{
	return clone() &= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator|(const BigIntConstructible auto other) const
{
	return clone() |= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator^(const BigIntConstructible auto other) const
{
	return clone() ^= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator<<(const BigIntConstructible auto other) const
{
	return clone() <<= other;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::operator>>(const BigIntConstructible auto other) const
{
	return clone() >>= other;
}

#endif // BIG_INT_BINARY_ARITHMETIC_OPERATORS_HPP

