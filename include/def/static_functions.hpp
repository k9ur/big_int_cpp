#ifndef BIG_INT_STATIC_FUNCTIONS_HPP
#define BIG_INT_STATIC_FUNCTIONS_HPP

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "../big_int.hpp"

template<std::uint32_t base>
constexpr std::uint64_t BigInt<base>::true_base() noexcept
{
	return base ? base : BASE_ZERO_TRUE_VALUE;
}

template<std::uint32_t base>
constexpr std::size_t BigInt<base>::__int_size(const std::int64_t num) noexcept
{
	std::uint64_t num_copy = __constexpr_abs(num);
	std::size_t bi_size = 1;
	while(num_copy >= true_base()) {
		++bi_size;
		num_copy /= true_base();
	}
	assert(bi_size);
	return bi_size;
}

template<std::uint32_t base>
constexpr std::uint8_t BigInt<base>::__int_log_of(std::uint64_t eq, std::uint64_t base_val) noexcept
{ // Solves x in base_val^x = eq. If eq isn't a power of base, returns 0
	if(base_val == 0)
		base_val = BASE_ZERO_TRUE_VALUE;
	std::uint8_t expo = 0;
	while(eq % base_val == 0) {
		eq /= base_val;
		++expo;
	}
	return eq == 1 ? expo : 0;
}

template<std::uint32_t base>
constexpr bool BigInt<base>::__valid_char(const char c, const char sv_base)
{
	if(sv_base < 2 || 36 < sv_base)
		throw std::domain_error(__THROW_SV_BASE_DOMAIN);
	return ('0' <= c && c <= std::min(static_cast<char>(sv_base - 1  + '0'), '9')) || (sv_base > 10 && (
		   ('a' <= c && c <= std::min(static_cast<char>(sv_base - 11 + 'a'), 'z'))
		|| ('A' <= c && c <= std::min(static_cast<char>(sv_base - 11 + 'A'), 'Z'))
	));
}

template<std::uint32_t base>
constexpr char BigInt<base>::__char_value(const char c, const char sv_base)
{
	if(sv_base < 2 || 36 < sv_base)
		throw std::domain_error(__THROW_SV_BASE_DOMAIN);
	else if(!__valid_char(c, sv_base))
		throw std::invalid_argument(__THROW_CHAR_INVALID);
	return '0' <= c && c <= '9' ? c - '0'
		: 'a' <= c && c <= 'z' ? c - 'a' + 10
		: c - 'A' + 10;
}

template<std::uint32_t base>
constexpr char BigInt<base>::__value_to_char(const char value, const char sv_base)
{
	if(sv_base < 2 || 36 < sv_base)
		throw std::domain_error(__THROW_SV_BASE_DOMAIN);
	else if(value >= sv_base)
		throw std::invalid_argument(__THROW_VALUE_TO_CHAR_INVALID);
	return value < 10 ? value + '0' : value - 10 + 'a';
}

template<std::uint32_t base>
constexpr std::string BigInt<base>::__constexpr_to_string(std::int64_t num, const char sv_base)
{
	if(sv_base < 2 || 36 < sv_base)
		throw std::domain_error(__THROW_SV_BASE_DOMAIN);

	std::uint64_t num_copy_abs = __constexpr_abs(num);
	std::size_t digits = 0;
	const auto neg = num < 0;
	do
		++digits;
	while(num /= sv_base);

	std::string s;
	s.resize(digits + neg);
	for(auto& c : s | __REV_V) {
		c = __value_to_char(num_copy_abs % sv_base, sv_base);
		num_copy_abs /= sv_base;
	}
	if(neg)
		s[0] = '-';
	assert(s.size());
	return s;
}

template<std::uint32_t base>
constexpr std::uint64_t BigInt<base>::__constexpr_abs(const std::int64_t num) noexcept
{
	constexpr auto min_i64 = std::numeric_limits<std::int64_t>::min();
	if(num == min_i64)
		return static_cast<std::uint64_t>(-(min_i64 + 1)) + 1;
	return num < 0 ? -num : num;
}

#endif // BIG_INT_STATIC_FUNCTIONS_HPP

