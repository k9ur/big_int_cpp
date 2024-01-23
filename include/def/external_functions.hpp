#ifndef BIG_INT_EXTERNAL_FUNCTIONS_HPP
#define BIG_INT_EXTERNAL_FUNCTIONS_HPP

#include "../big_int.hpp"
#include "constructors.hpp"
#include "member_functions.hpp"

template<std::uint32_t base>
constexpr BigInt<base> make_big_int(const BigIntConstructible auto arg)
{
	return BigInt<base>(arg);
}

constexpr BigInt<> operator""_bi(std::uint64_t num)
{
	return make_big_int(num);
}

constexpr BigInt<> operator""_big_int(std::uint64_t num)
{
	return make_big_int(num);
}

constexpr BigInt<> operator""_bi(const char* str, std::size_t size)
{
	return make_big_int(std::string_view(str, size));
}

constexpr BigInt<> operator""_big_int(const char* str, std::size_t size)
{
	return make_big_int(std::string_view(str, size));
}

#endif // BIG_INT_EXTERNAL_FUNCTIONS_HPP

