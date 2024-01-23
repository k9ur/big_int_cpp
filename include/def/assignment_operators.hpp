#ifndef BIG_INT_ASSIGNMENT_OPERATORS_HPP
#define BIG_INT_ASSIGNMENT_OPERATORS_HPP

#include <utility>

#include "../big_int.hpp"
#include "constructors.hpp"
#include "external_functions.hpp"

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::operator=(const BigIntConstructible auto other)
{
	return *this = make_big_int<base>(other);
}

template<std::uint32_t base>
template<std::uint32_t other_base>
constexpr BigInt<base>& BigInt<base>::operator=(const BigInt<other_base>& other)
{
	return *this = BigInt<base>(other);
}

template<std::uint32_t base>
template<std::uint32_t other_base>
constexpr BigInt<base>& BigInt<base>::operator=(BigInt<other_base>&& other)
{
	return *this = BigInt<base>(std::move(other));
}

#endif // BIG_INT_ASSIGNMENT_OPERATORS_HPP

