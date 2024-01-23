#ifndef BIG_INT_MEMBER_FUNCTIONS_HPP
#define BIG_INT_MEMBER_FUNCTIONS_HPP

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "../big_int.hpp"
#include "constructors.hpp"
#include "assignment_operators.hpp"
#include "arithmetic_assignment_operators.hpp"
#include "unary_operators.hpp"
#include "binary_arithmetic_operators.hpp"
#include "comparison_operators.hpp"
#include "static_functions.hpp"
#include "helper_functions.hpp"
#include "utility_functions.hpp"
#include "external_functions.hpp"

template<std::uint32_t base>
constexpr std::size_t BigInt<base>::size() const noexcept
{
	return bigits.size();
}

template<std::uint32_t base>
constexpr bool BigInt<base>::positive() const noexcept
{
	return !negative;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::sign(const bool new_sign) noexcept
{
	negative = !new_sign;
	return *this;
}

template<std::uint32_t base>
constexpr bool BigInt<base>::is_zero() const noexcept
{
	return size() == 1 && front() == 0;
}

template<std::uint32_t base>
constexpr BigInt<base> BigInt<base>::clone() const
{
	return *this;
}

template<std::uint32_t base>
constexpr void BigInt<base>::swap(BigInt<base>& other) noexcept
{
	const auto temp{ clone() };
	*this = other;
	other = temp;
}

template<std::uint32_t base>
constexpr const std::vector<std::uint32_t>& BigInt<base>::data() const noexcept
{
	return bigits;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::abs() noexcept
{
	negative = false;
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::sqrt()
{
	if(!positive())
		throw std::domain_error(__THROW_SQRT_DOMAIN);
	if(size() == 1 && front() <= 1)
		return *this;

	return __sqrt_helper();
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::log(const BigInt<base>& base_val)
{
	if(!positive() || is_zero())
		throw std::domain_error(__THROW_LOG_VALUE_DOMAIN);
	else if(base_val <= 1)
		throw std::domain_error(__THROW_LOG_BASE_DOMAIN);

	BigInt<base> res{ 0 };
	while(*this /= base_val)
		++res;
	return *this = res;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::log(const std::int64_t base_val)
{
	if(!positive() || is_zero())
		throw std::domain_error(__THROW_LOG_VALUE_DOMAIN);
	else if(base_val <= 1)
		throw std::domain_error(__THROW_LOG_BASE_DOMAIN);

	BigInt<base> res{ 0 };
	while(*this /= base_val)
		++res;
	return *this = res;
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::log(const T base_val)
{
	if constexpr(INT64Constructible<T>)
		return log(static_cast<std::int64_t>(base_val));
	else
		return log(make_big_int<base>(base_val));
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::pow(const BigInt<base>& expo)
{
	if(!expo.positive())
		return *this = 0;
	else if(expo.is_zero())
		return *this = 1;

	BigInt<base> res{ 1 },
	             expo_copy{ expo };
	while(expo_copy != 1) {
		if(expo_copy & 1)
			res *= *this;
		*this *= *this;
		expo_copy >>= 1;
	}
	return *this *= res;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::pow(std::int64_t expo)
{
	if(expo == 0)
		return *this = 1;
	else if(expo < 0)
		return *this = 0;

	BigInt<base> res{ 1 };
	while(expo != 1) {
		if(expo & 1)
			res *= *this;
		*this *= *this;
		expo >>= 1;
	}
	return *this *= res;
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::pow(const T expo)
{
	if constexpr(INT64Constructible<T>)
		return pow(static_cast<std::int64_t>(expo));
	else
		return pow(make_big_int<base>(expo));
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::gcd(const BigInt<base>& other)
{
	if(other.is_zero() || *this == 1)
		return *this;
	else if(*this == -1)
		return abs();
	else if(is_zero() || other == 1)
		return *this = other;
	else if(other == -1)
		return *this = 1;

	return __gcd_helper(other);
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::gcd(const T other)
{
	return gcd(make_big_int<base>(other));
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::lcm(const BigInt<base>& other)
{
	if(is_zero())
		return *this;
	else if(other == 1 || other == -1)
		return abs();
	else if(other.is_zero())
		return *this = 0;
	else if(*this == 1 || *this == -1) {
		*this = other;
		return abs();
	}

	const auto mul = *this * other;
	*this = mul / gcd(other);
	assert(valid());
	return abs();
}

template<std::uint32_t base>
template<BigIntConstructible T>
constexpr BigInt<base>& BigInt<base>::lcm(const T other)
{
	return lcm(make_big_int<base>(other));
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::factorial()
{
	if(is_zero())
		return *this = 1;
	else if(*this == 1)
		return *this;
	if(!positive())
		throw std::domain_error(__THROW_FACTORIAL_DOMAIN);

	if(size() == 1) {
		auto mult = front();
		if(mult & 1)
			*this *= (mult >> 1) + 1;
		for(auto diff = mult - 2; diff > 1; diff -= 2) {
			mult += diff;
			*this *= mult;
		}
	} else {
#if 1 // Faster algorithm: halves the number of multiplications
		auto mult{ clone() };
		if(mult & 1)
			*this *= (mult >> 1) + 1;
		for(auto diff{ mult - 2 }; diff.size() > 1 || diff.front() > 1; diff -= 2) {
			mult += diff;
			*this *= mult;
		}
#else // Old method
		for(auto mult = clone() - 1; mult; --mult)
			*this *= mult;
#endif
	}
	return *this;
}

template<std::uint32_t base>
constexpr std::uint64_t BigInt<base>::bits() const
{
	return digits(2);
}

template<std::uint32_t base>
constexpr std::uint64_t BigInt<base>::digits(const std::uint32_t smaller_base) const
{
	const auto digit_base = smaller_base ? smaller_base : BASE_ZERO_TRUE_VALUE;
	if(digit_base > true_base())
		throw std::domain_error(__THROW_SMALLER_BASE_DOMAIN);
	if(digit_base == true_base())
		return size();

	std::uint64_t num_digits;
	const auto digits_per_bigit = __int_log_of(true_base(), digit_base);
	if(digits_per_bigit) {
		num_digits = (size() - 1) * digits_per_bigit;
		auto last_bigit = back();
		do
			++num_digits;
		while(last_bigit /= digit_base);
	} else {
		num_digits = 0;
		auto this_copy{ clone() };
		do
			++num_digits;
		while(this_copy /= digit_base);
	}
	return num_digits;
}

template<std::uint32_t base>
constexpr std::uint64_t BigInt<base>::digit_sum(const std::uint32_t smaller_base) const
{
	const auto digit_base = smaller_base ? smaller_base : BASE_ZERO_TRUE_VALUE;
	if(digit_base > true_base())
		throw std::domain_error(__THROW_SMALLER_BASE_DOMAIN);

	constexpr auto limit = std::numeric_limits<std::int64_t>::max();
	std::uint64_t ds = 0;
	const auto digits_per_bigit = __int_log_of(true_base(), digit_base);
	if(digits_per_bigit)
		for(auto bigit : bigits) {
			do
				ds += bigit % digit_base;
			while(bigit /= digit_base);
		}
	else {
		auto this_copy{ clone() };
		do {
			const auto val = this_copy % digit_base;
			const auto int_val = __constexpr_abs(val.to_int());
			assert(int_val < digit_base);
			if(limit - ds < int_val)
				throw std::overflow_error(__THROW_INT64_OVERFLOW);
			ds += int_val;
		} while(this_copy /= digit_base);
	}
	return ds;
}

template<std::uint32_t base>
constexpr std::int64_t BigInt<base>::to_int() const
{ // TODO fix function: should be doing ret -= bigit to allow for a result of numeric_limits<i64>::min()
	constexpr auto limit = std::numeric_limits<std::int64_t>::max();
	std::int64_t ret = back();
	for(const auto& bigit : bigits | __REV_DROP1_V) {
		if(limit / ret < true_base())
			positive() ? throw std::overflow_error(__THROW_INT64_OVERFLOW) : throw std::underflow_error(__THROW_INT64_UNDERFLOW);
		ret *= true_base();
		if(limit - ret < bigit)
			positive() ? throw std::overflow_error(__THROW_INT64_OVERFLOW) : throw std::underflow_error(__THROW_INT64_UNDERFLOW);
		ret += bigit;
	}
	return positive() ? ret : -ret;
}

template<std::uint32_t base>
constexpr std::string BigInt<base>::str(const char str_base) const
{
	using namespace std::string_literals;
	if(str_base < 2 || 36 < str_base)
		throw std::domain_error(__THROW_STR_BASE_DOMAIN);
	if(is_zero()) // Prevent -0
		return "0"s;

	std::string s;

	const auto chars_per_bigit = __int_log_of(true_base(), str_base);
	if(chars_per_bigit) {
		s = __constexpr_to_string(positive() ? back() : -static_cast<std::int64_t>(back()));
		s.reserve(s.size() + (size() - 1) * chars_per_bigit);
		for(const auto& bigit : bigits | __REV_DROP1_V) {
			const auto bigit_s = __constexpr_to_string(bigit);
			if(bigit_s.size() != chars_per_bigit)
				s += std::string(chars_per_bigit - bigit_s.size(), '0');
			s += bigit_s;
		}
	} else {
		auto this_copy{ clone() };
		do {
			const auto val = this_copy % str_base;
			const auto int_val = __constexpr_abs(val.to_int());
			assert(int_val < str_base);
			s.push_back(__value_to_char(static_cast<char>(int_val), str_base));
		} while(this_copy /= str_base);
		if(!positive())
			s.push_back('-');
		std::ranges::reverse(s);
	}

	assert(s.size());
	return s;
}

#endif // BIG_INT_MEMBER_FUNCTIONS_HPP

