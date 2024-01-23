#ifndef BIG_INT_CONSTRUCTORS_HPP
#define BIG_INT_CONSTRUCTORS_HPP

#include <utility>
#include <cmath>
#include <cctype>
#include <cassert>
#include <stdexcept>

#include "../big_int.hpp"
#include "arithmetic_assignment_operators.hpp"
#include "member_functions.hpp"
#include "static_functions.hpp"
#include "utility_functions.hpp"

template<std::uint32_t base>
constexpr BigInt<base>::BigInt()
  : bigits(1, 0)
  , negative(false)
{}

template<std::uint32_t base>
constexpr BigInt<base>::BigInt(const std::vector<std::uint32_t>& _bigits, bool _negative)
  : bigits(_bigits)
  , negative(_negative)
{
	assert(valid());
}

template<std::uint32_t base>
constexpr BigInt<base>::BigInt(std::vector<std::uint32_t>&& _bigits, bool _negative)
  : bigits(std::move(_bigits))
  , negative(_negative)
{
	assert(valid());
}

template<std::uint32_t base>
constexpr BigInt<base>::BigInt(const std::int64_t num)
  : bigits(__int_size(num), 0)
  , negative(num < 0)
{
	std::uint64_t num_copy = __constexpr_abs(num);
	for(auto& bigit : bigits) {
		bigit = num_copy % true_base();
		num_copy /= true_base();
	}
	assert(valid());
}

template<std::uint32_t base>
constexpr BigInt<base>::BigInt(const std::string_view sv, const char sv_base)
  : BigInt()
{
	if(sv_base < 2 || 36 < sv_base)
		throw std::domain_error(__THROW_SV_BASE_DOMAIN);

	if(sv.size() == 0)
		return;
	std::size_t cutoff = sv.front() == '-' || sv.front() == '+';
	while(cutoff < sv.size() && sv[cutoff] == '0')
		++cutoff;
	if(cutoff == sv.size())
		return;
	const auto sv_stripped = sv.substr(cutoff);

	const auto chars_per_bigit = __int_log_of(true_base(), sv_base);
	if(chars_per_bigit) {
		// base is a power of sv_base: can construct BigInt quicker (no operator*= or operator+= calls)
		resize(((sv_stripped.size() - 1) / chars_per_bigit) + 1, 0);
		std::size_t idx = sv_stripped.size();

		for(bool on_last; auto& bigit : bigits) {
			on_last = idx <= chars_per_bigit;
			if(!on_last)
				idx -= chars_per_bigit;
			for(std::size_t i = on_last ? 0 : idx; i < (on_last ? idx : idx + chars_per_bigit); ++i) {
				bigit *= sv_base;
				bigit += __char_value(sv_stripped[i], sv_base);
			}
		}
	} else {
		*this += __char_value(sv_stripped.front(), sv_base);
		for(const auto c : sv_stripped | __DROP1_V) {
			*this *= sv_base;
			*this += __char_value(c, sv_base);
		}
	}
	negative = sv.front() == '-';
	assert(valid());
}

template<std::uint32_t base>
template<std::uint32_t other_base>
constexpr BigInt<base>::BigInt(const BigInt<other_base>& other)
  : BigInt(other.back())
{
	for(const auto bigit : other.bigits | __REV_DROP1_V) {
		*this *= other.true_base();
		*this += bigit;
	}
	negative = other.negative;
	assert(valid());
}

template<std::uint32_t base>
template<std::uint32_t other_base>
constexpr BigInt<base>::BigInt(BigInt<other_base>&& other)
  : BigInt(other.back())
{
	for(const auto bigit : std::move(other.bigits) | __REV_DROP1_V) {
		*this *= other.true_base();
		*this += bigit;
	}
	negative = std::move(other.negative);
	assert(valid());
}

#endif // BIG_INT_CONSTRUCTORS_HPP

