#ifndef BIG_INT_UNSAFE_UTILITY_FUNCTIONS_HPP
#define BIG_INT_UNSAFE_UTILITY_FUNCTIONS_HPP

#include <algorithm>

#include "../big_int.hpp"
#include "member_functions.hpp"
#include "static_functions.hpp"

template<std::uint32_t base>
constexpr bool BigInt<base>::valid() const
{
	return size() && (back() || size() == 1) && (base == 0 || std::ranges::all_of(bigits, [](const auto bigit) { // Check each bigit for validity: O(N)
		return bigit < true_base();
	}));
}

template<std::uint32_t base>
constexpr void BigInt<base>::reserve(const std::size_t new_cap)
{
	return bigits.reserve(new_cap);
}

template<std::uint32_t base>
constexpr void BigInt<base>::resize(const std::size_t count)
{
	return bigits.resize(count);
}

template<std::uint32_t base>
constexpr void BigInt<base>::resize(const std::size_t count, const std::uint32_t value)
{
	return bigits.resize(count, value);
}

template<std::uint32_t base>
constexpr void BigInt<base>::assign(const std::size_t count, const std::uint32_t value)
{
	return bigits.assign(count, value);
}

template<std::uint32_t base>
constexpr std::uint32_t& BigInt<base>::front()
{
	return bigits.front();
}

template<std::uint32_t base>
constexpr const std::uint32_t& BigInt<base>::front() const
{
	return bigits.front();
}

template<std::uint32_t base>
constexpr std::uint32_t& BigInt<base>::back()
{
	return bigits.back();
}

template<std::uint32_t base>
constexpr const std::uint32_t& BigInt<base>::back() const
{
	return bigits.back();
}

#endif // BIG_INT_UNSAFE_UTILITY_FUNCTIONS_HPP

