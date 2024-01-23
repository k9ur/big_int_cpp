#ifndef BIG_INT_HELPER_FUNCTIONS_HPP
#define BIG_INT_HELPER_FUNCTIONS_HPP

#include <utility>
#include <algorithm>
#include <iterator>
#include <cassert>

#include "../big_int.hpp"
#include "constructors.hpp"
#include "assignment_operators.hpp"
#include "arithmetic_assignment_operators.hpp"
#include "binary_arithmetic_operators.hpp"
#include "unary_operators.hpp"
#include "member_functions.hpp"
#include "static_functions.hpp"
#include "utility_functions.hpp"
#include "external_functions.hpp"

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__sqrt_helper()
{
	if constexpr(__int_log_of(true_base(), 2)) {
		const auto bit_count = bits();
		const auto max_size = (size() + 1) >> 1;

		BigInt<base> bit{ 1 };
		bit <<= bit_count & 1u ? bit_count - 1 : bit_count - 2;
	
		auto this_copy{ *this - bit }; // Make first loop iteration faster
		*this = bit;

		while(bit >>= 2) { // Bit-by-bit calculation: O(N)
			const auto sum{ *this + bit };
			if(this_copy >= sum) {
				this_copy -= sum;
				*this >>= 1;
				*this += bit;
			} else
				*this >>= 1;
		}
	} else { // Convert to a binary base, then re-call, then convert base back: O(N^2)
		BigInt<MAX_BASE> this_bin_base{ clone() };
		return *this = this_bin_base.__sqrt_helper();
	}

	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__gcd_helper(const BigInt<base>& other)
{
	BigInt<base> r, other_copy{ other };
	do {
		r = *this % other_copy;
		*this = other_copy;
		other_copy = r;
	} while(r);

	return abs();
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__not_helper()
{
	constexpr auto bits_per_bigit = __int_log_of(true_base(), 2);
	constexpr std::uint8_t unused_bits_per_bigit = std::numeric_limits<std::uint32_t>::digits - bits_per_bigit;
	static_assert(bits_per_bigit);
	
	std::size_t used_idx = 0;
	for(auto idx = size(); idx; --idx) { // Standard NOT algorithm: O(N)
		auto& bigit = bigits[idx - 1];
		bigit = ~bigit << unused_bits_per_bigit;
		bigit >>= unused_bits_per_bigit;
		if(!used_idx && bigit)
			used_idx = idx - 1;
	}

	resize(used_idx + 1);
	negative = positive();
	assert(valid());
	return *this;
}

template<std::uint32_t base>
template<BitwiseType type> // AND = 0, OR = 1, XOR = 2
constexpr BigInt<base>& BigInt<base>::__bitwise_signless(const BigInt<base>& other)
{
	if constexpr(__int_log_of(true_base(), 2)) {
		std::size_t used_idx = 0;
		const auto min_size = std::min(size(), other.size());
		if constexpr(type != BitwiseType::AND) { // AND will always use the least
			if(size() < other.size()) { // When only one bigit is set, OR and XOR copy it, AND nulls it
				reserve(other.size());
				bigits.insert(bigits.end(), std::next(other.bigits.cbegin(), min_size), other.bigits.cend());
				used_idx = other.size() - 1;
			} else if(size() > other.size() || type == BitwiseType::OR) // OR will always use the most
				used_idx = size() - 1;
		}

		std::uint32_t b;
		for(auto idx = min_size; idx; --idx) { // Standard bitwise AND/OR/XOR algorithm: O(N)
			auto& a = bigits[idx - 1];
			b = other.bigits[idx - 1];
			if constexpr(type == BitwiseType::AND)
				a &= b;
			else if constexpr(type == BitwiseType::OR)
				a |= b;
			else
				a ^= b;
			if constexpr(type != BitwiseType::OR) // Will have already been set for OR
				if(idx - 1 > used_idx && a)
					used_idx = idx - 1;
		}
		if constexpr(type != BitwiseType::OR)
			resize(used_idx + 1);
	} else { // Convert to a binary base, then re-call, then convert base back: O(N^2)
		BigInt<MAX_BASE> this_bin_base{ clone() }, other_bin_base{ other.clone() };
		return *this = this_bin_base.template __bitwise_signless<type>(other_bin_base);
	}

	assert(valid());
	return *this;
}

template<std::uint32_t base>
template<BitwiseType type> // AND = 0, OR = 1, XOR = 2
constexpr BigInt<base>& BigInt<base>::__bitwise_signless(const std::uint64_t other)
{
	if(other >= true_base()) // other < true_base() required for specialisation
		return __bitwise_signless<type>(make_big_int<base>(other));

	if constexpr(__int_log_of(true_base(), 2)) { // Specialisation: standard bitwise AND/OR/XOR: O(1)
		const std::uint32_t other_u32 = other;
		if constexpr(type == BitwiseType::AND) {
			bigits[0] &= other_u32;
			resize(1);
		} else if constexpr(type == BitwiseType::OR)
			bigits[0] |= other_u32;
		else
			bigits[0] ^= other_u32;
	} else if(other == 1 && !(base & 1u) && type == BitwiseType::AND) { // Specialisation: can do a shortcut since & 1 is commonly used: O(1)
		bigits[0] &= 1u;
		resize(1);
	} else { // Convert to a binary base, then re-call, then convert base back: O(N^2)
		BigInt<MAX_BASE> this_bin_base{ clone() };
		return *this = this_bin_base.template __bitwise_signless<type>(other);
	}

	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__add_eq_signless(const BigInt<base>& other)
{
	const auto max_new_size = std::max(size(), other.size()) + 1;
	reserve(max_new_size); // Know we will be using at most max(size() + other.size()) + 1
	resize(max_new_size - 1, 0); // Know we will be using at least max(size() + other.size())

	std::uint64_t a, b, n = 0;
	for(std::size_t idx = 0; idx != max_new_size - 1; ++idx) { // Standard carry (n) addition algorithm: O(N)
		a = bigits[idx];
		b = idx < other.size() ? other.bigits[idx] : 0;
		n += a + b;
		bigits[idx] = n % true_base();
		n = n >= true_base();
	}
	if(n) { // We have overflowed: next bigit will be n
		assert(n == 1); // ... which is always 1
		bigits.push_back(1);
	}

	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__add_eq_signless(const std::uint64_t other)
{
	if(other >= true_base()) // other < true_base() required for specialisation
		return __add_eq_signless(make_big_int<base>(other));

	auto n = other + front();
	bigits[0] = n % true_base();
	n = n >= true_base();

	if(n) { // If n == 0, no carry required
		reserve(size() + 1);
		for(auto& a : bigits | __DROP1_V) { // Standard carry (n) addition algorithm: O(N)
			n += a;
			a = n % true_base();
			n = n == true_base(); // Can only possibly overflow by 1, since n was previously 1
			if(n == 0)
				break;
		}
		if(n) { // We have overflowed: next bigit will be n
			assert(n == 1); // ... which is always 1
			bigits.push_back(1);
		}
	}

	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__sub_eq_signless(const BigInt<base>& other)
{
	const auto max_new_size = std::max(size(), other.size());
	reserve(max_new_size); // Know we will be using at most max_new_size

	std::int64_t a, b, n = 0;
	std::size_t used_idx = 0;
	const auto larger = __compare_signless(other) != __LESS_SO;
	for(std::size_t idx = 0; idx != max_new_size; ++idx) { // Standard borrowing (n) subtraction algorithm: O(N)
		a = idx < size() ? bigits[idx] : 0;
		b = idx < other.size() ? other.bigits[idx] : 0;
		n += larger ? a - b : b - a;
		const auto borrowed = n < 0;
		const std::uint32_t val = borrowed ? n + true_base() : n;
		if(idx < size())
			bigits[idx] = val;
		else
			bigits.push_back(val);
		if(val)
			used_idx = idx;
		n = -borrowed; // If n < 0 set to -1, otherwise 0
	}
	assert(n == 0);
	if(used_idx != max_new_size - 1) // Chop off leading zeros
		resize(used_idx + 1);

	negative ^= !larger;
	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__sub_eq_signless(const std::uint64_t other)
{
	if(other >= true_base()) // other < true_base() required for specialisation
		return __sub_eq_signless(make_big_int<base>(other));

	const auto larger = __compare_signless(other) != __LESS_SO;
	auto n = larger ? static_cast<std::int64_t>(front()) - other : static_cast<std::int64_t>(other) - front();
	auto borrowed = n < 0;
	bigits[0] = borrowed ? n + true_base() : n;
	n = -borrowed;

	if(!borrowed) {
		for(auto& a : bigits | __DROP1_V) { // Standard borrowing (n) subtraction algorithm: O(N)
			larger ? n += a : n -= a;
			borrowed = n < 0;
			a = borrowed ? n + true_base() : n;
			n = -borrowed;
			if(!borrowed)
				break;
		}
		assert(n == 0);
		if(back() == 0) // Can only possibly reduce size by 1
			resize(size() - 1);
	}

	negative ^= !larger;
	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__mul_eq_signless(const BigInt<base>& other)
{
	std::vector<std::uint32_t> prod_bigits;
	prod_bigits.reserve(size() + other.size()); // Know we will be using at most size() + other.size()
	prod_bigits.resize(prod_bigits.capacity() - 1, 0); // Know we will be using at least size() + other.size() - 1

	const auto& a_bi = size() < other.size() ? bigits : other.bigits;
	const auto& b_bi = size() < other.size() ? other.bigits : bigits; // a_bi is smaller than b_bi

	std::uint64_t a, b, n = 0;
	for(std::size_t b_idx, a_idx = 0; a_idx != a_bi.size(); ++a_idx) { // Standard long multiplication method: O(N^2)
		a = a_bi[a_idx];
		for(b_idx = 0; b_idx < b_bi.size() || n; ++b_idx) {
			b = b_idx < b_bi.size() ? b_bi[b_idx] : 0;
			n += a * b;
			const auto idx = a_idx + b_idx;
			const auto under_size = idx < prod_bigits.size();
			if(under_size)
				n += prod_bigits[idx];
			const auto val = n % true_base();
			if(under_size)
				prod_bigits[idx] = val;
			else
				prod_bigits.push_back(val);
			n /= true_base();
		}
	}

	bigits = std::move(prod_bigits);
	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__mul_eq_signless(const std::uint64_t other)
{
	const auto bigit_shl_count = __int_log_of(other, true_base());
	if(bigit_shl_count) { // Skip calculations and shift all bigits left: O(N)
		bigits.insert(bigits.begin(), bigit_shl_count, 0);
		assert(valid());
		return *this;
	} else if(other > true_base()) // other < true_base() required for specialisation
		return __mul_eq_signless(make_big_int<base>(other));

	reserve(size() + 1);

	std::uint64_t n = 0;
	for(auto& a : bigits) { // Specialisation: standard (short) multiplication method: O(N)
		n += other * a;
		a = n % true_base();
		n /= true_base();
	}
	if(n) {
		assert(n < true_base() - 1);
		bigits.push_back(n);
	}

	assert(valid());
	return *this;
}

template<std::uint32_t base>
template<bool remainder> // If true, acts as mod (%), otherwise acts as div (/)
constexpr BigInt<base>& BigInt<base>::__div_mod_eq_signless(const BigInt<base>& other)
{
	constexpr auto bits_per_bigit = __int_log_of(true_base(), 2);
	if constexpr(bits_per_bigit) { // Specialisation: binary long division method with remainder: O(N)
		BigInt<base> r; // *this can be used as q, since only one bit is updated at a time
		[[maybe_unused]] std::size_t used_idx = 0;

		auto bit = bits();
		auto* q_bigit = &bigits[bit / bits_per_bigit];
		std::uint32_t single_bit = 1u << (bit % bits_per_bigit);
		do {
			if(single_bit & 1u) {
				--q_bigit;
				single_bit = 1u << (bits_per_bigit - 1);
			} else
				single_bit >>= 1;
			const bool bit_set = *q_bigit & single_bit;

			r <<= 1;
			if(bit_set)
				r |= 1u;
			if constexpr(remainder) {
				if(r.__compare_signless(other) != __LESS_SO) // Treat other as if it's positive
					r.__sub_eq_signless(other);
			} else {
				if(r.__compare_signless(other) != __LESS_SO) { // ...
					r.__sub_eq_signless(other);
					if(!bit_set)
						*q_bigit |= single_bit; // Set bit
					if(!used_idx)
						used_idx = bit;
				} else if(bit_set)
					*q_bigit ^= single_bit; // Unset bit
			}
		} while(--bit);

		if constexpr(remainder)
			*this = r;
		else
			resize((used_idx / bits_per_bigit) + 1);
	} else { // Convert to a binary base, then re-call, then convert base back: O(N^2)
		BigInt<MAX_BASE> this_bin_base{ clone() }, other_bin_base{ other.clone() };
		return *this = this_bin_base.template __div_mod_eq_signless<remainder>(other_bin_base);
	}

	assert(valid());
	return *this;
}

template<std::uint32_t base>
template<bool remainder> // If true, acts as mod (%), otherwise acts as div (/)
constexpr BigInt<base>& BigInt<base>::__div_mod_eq_signless(const std::uint64_t other)
{
	const auto bigit_shr_count = __int_log_of(other, true_base());
	if(bigit_shr_count) { // Skip calculations and shift all bigits right: O(N)
		if(size() > bigit_shr_count)
			bigits.erase(bigits.begin(), std::next(bigits.begin(), bigit_shr_count));
		else {
			resize(1);
			bigits[0] = 0;
		}
		assert(valid());
		return *this;
	} else if(other > true_base()) // TODO confirm: other < true_base() required for specialisation
		return __div_mod_eq_signless<remainder>(make_big_int<base>(other));

	if constexpr(remainder) {
		if(other == 2 && !(base & 1u)) { // Specialisation: % 2 is same as & 1, can do a shortcut since it's commonly used: O(1)
			bigits[0] = front() & 1u;
			resize(1);
		} else {
			std::uint64_t r = back() % other;
			for(const auto& q_bigit : bigits | __REV_DROP1_V) {
				r *= true_base();
				r += q_bigit;
				r %= other;
			}
			*this = r;
		}
	} else {
		std::uint64_t r = back();
		bigits[size() - 1] = r / other;
		r %= other;
		std::size_t used_idx = back() ? size() - 1 : 0;
		for(auto idx = size() - 1; idx; --idx) { // Specialisation: standard long division method with remainder: O(N)
			auto& q_bigit = bigits[idx - 1];
			r *= true_base();
			r += q_bigit;
			q_bigit = r / other; // Division algorithm requires division: hence why this method can't be used for BigInt overload
			r %= other; // ... and same for modulo
			if(!used_idx && q_bigit)
				used_idx = idx - 1;
		}
		resize(used_idx + 1);
	}

	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__shl_eq_signless(const BigInt<base>& other)
{
	constexpr auto bits_per_bigit = __int_log_of(true_base(), 2);
	constexpr std::uint8_t unused_bits_per_bigit = std::numeric_limits<std::uint32_t>::digits - bits_per_bigit;
	static_assert(bits_per_bigit);

	auto shift_bigits = __constexpr_abs((other / bits_per_bigit).to_int()); // Consequently, may throw
	const std::uint8_t shift_bits = __constexpr_abs((other % bits_per_bigit).to_int());
	const auto unshift_bits = bits_per_bigit - shift_bits;
	const auto old_size = size();
	const auto last_bigit = back() >> (bits_per_bigit - shift_bits);
	resize(old_size + shift_bigits + (last_bigit != 0)); // New size must be size_t, so this can't work for all BigInts

	if(last_bigit)
		bigits[size() - 1] = last_bigit;
	for(auto idx = old_size - 1; idx; --idx) { // Standard linear shift: O(N)
		auto& bigit = bigits[idx + shift_bigits];
		bigit = bigits[idx] << (unused_bits_per_bigit + shift_bits);
		bigit >>= unused_bits_per_bigit;
		bigit |= bigits[idx - 1] >> unshift_bits;
	}
	bigits[shift_bigits] = static_cast<std::uint32_t>(front() << (unused_bits_per_bigit + shift_bits)) >> unused_bits_per_bigit; // Last has no carry from next
	while(shift_bigits)
		bigits[--shift_bigits] = 0;

	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__shl_eq_signless(const std::uint64_t other)
{
	constexpr auto bits_per_bigit = __int_log_of(true_base(), 2);
	constexpr std::uint8_t unused_bits_per_bigit = std::numeric_limits<std::uint32_t>::digits - bits_per_bigit;
	static_assert(bits_per_bigit);

	auto shift_bigits = other / bits_per_bigit;
	const std::uint8_t shift_bits = other % bits_per_bigit;
	const auto unshift_bits = bits_per_bigit - shift_bits;
	const auto old_size = size();
	const auto last_bigit = back() >> (bits_per_bigit - shift_bits);
	resize(old_size + shift_bigits + (last_bigit != 0));

	if(last_bigit)
		bigits[size() - 1] = last_bigit;
	for(auto idx = old_size - 1; idx; --idx) { // Standard linear shift: O(N)
		auto& bigit = bigits[idx + shift_bigits];
		bigit = bigits[idx] << (unused_bits_per_bigit + shift_bits);
		bigit >>= unused_bits_per_bigit;
		bigit |= bigits[idx - 1] >> unshift_bits;
	}
	bigits[shift_bigits] = static_cast<std::uint32_t>(front() << (unused_bits_per_bigit + shift_bits)) >> unused_bits_per_bigit; // Last has no carry from next
	while(shift_bigits)
		bigits[--shift_bigits] = 0;

	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__shr_eq_signless(const BigInt<base>& other)
{
	constexpr auto bits_per_bigit = __int_log_of(true_base(), 2);
	constexpr std::uint8_t unused_bits_per_bigit = std::numeric_limits<std::uint32_t>::digits - bits_per_bigit;
	static_assert(bits_per_bigit);

	if(other.__compare_signless(bits()) != __LESS_SO)
		return *this = 0;
	const auto shift_bigits = __constexpr_abs((other / bits_per_bigit).to_int()); // Consequently, may throw
	const std::uint8_t shift_bits = __constexpr_abs((other % bits_per_bigit).to_int());
	const auto unshift_bits = bits_per_bigit - shift_bits;
	const auto new_size = size() - shift_bigits; // New size must be size_t, so this can't work for all BigInts
	assert(new_size);

	for(std::size_t idx = 0; idx != new_size - 1; ++idx) { // Standard linear shift: O(N)
		auto bigit = bigits[idx + shift_bigits + 1] << (unused_bits_per_bigit + unshift_bits);
		bigit >>= unused_bits_per_bigit;
		bigit |= bigits[idx + shift_bigits] >> shift_bits;
		bigits[idx] = bigit; // Can't make bigit a reference since shift_bigits may be 0
	}
	bigits[new_size - 1] = bigits[new_size - 1 + shift_bigits] >> shift_bits; // Last has no carry from next
	resize(new_size - (bigits[new_size - 1] == 0 && new_size != 1));

	assert(valid());
	return *this;
}

template<std::uint32_t base>
constexpr BigInt<base>& BigInt<base>::__shr_eq_signless(const std::uint64_t other)
{
	constexpr auto bits_per_bigit = __int_log_of(true_base(), 2);
	constexpr std::uint8_t unused_bits_per_bigit = std::numeric_limits<std::uint32_t>::digits - bits_per_bigit;
	static_assert(bits_per_bigit);

	if(other >= bits())
		return *this = 0;
	const auto shift_bigits = other / bits_per_bigit;
	const std::uint8_t shift_bits = other % bits_per_bigit;
	const auto unshift_bits = bits_per_bigit - shift_bits;
	const auto new_size = size() - shift_bigits;
	assert(new_size);

	for(std::size_t idx = 0; idx != new_size - 1; ++idx) { // Standard linear shift: O(N)
		auto bigit = bigits[idx + shift_bigits + 1] << (unused_bits_per_bigit + unshift_bits);
		bigit >>= unused_bits_per_bigit;
		bigit |= bigits[idx + shift_bigits] >> shift_bits;
		bigits[idx] = bigit; // Can't make bigit a reference since shift_bigits may be 0
	}
	bigits[new_size - 1] = bigits[new_size - 1 + shift_bigits] >> shift_bits; // Last has no carry from next
	resize(new_size - (bigits[new_size - 1] == 0 && new_size != 1));

	assert(valid());
	return *this;
}

template<std::uint32_t base>
template<bool right>
constexpr BigInt<base>& BigInt<base>::__shl_shr_eq_helper(const BigInt<base>& other)
{
	if(is_zero() || other.is_zero())
		return *this;
	if constexpr(__int_log_of(true_base(), 2))
		return right != other.positive() ? __shl_eq_signless(other) : __shr_eq_signless(other);
	else { // Convert to a binary base, then re-call, then convert base back: O(N^2)
		BigInt<MAX_BASE> this_bin_base{ clone() }, other_bin_base{ other.clone() };
		return *this = this_bin_base.template __shl_shr_eq_helper<right>(other_bin_base);
	}
}

template<std::uint32_t base>
template<bool right>
constexpr BigInt<base>& BigInt<base>::__shl_shr_eq_helper(const std::int64_t other)
{
	if(is_zero() || other == 0)
		return *this;
	const auto other_positive = other >= 0;
	if constexpr(__int_log_of(true_base(), 2))
		return right != other_positive ? __shl_eq_signless(other) : __shr_eq_signless(other);
	else { // Convert to a binary base, then re-call, then convert base back: O(N^2)
		BigInt<MAX_BASE> this_bin_base{ clone() };
		return *this = this_bin_base.template __shl_shr_eq_helper<right>(other);
	}
}

template<std::uint32_t base>
constexpr std::strong_ordering BigInt<base>::__compare_signless(const BigInt<base>& other, const bool sign) const noexcept
{
	if(size() != other.size())
		return sign ? size() <=> other.size() : other.size() <=> size();
	for(auto idx = size() - 1; idx; --idx) {
		const auto bigit = bigits[idx],
		           other_bigit = other.bigits[idx];
		if(bigit != other_bigit)
			return sign ? bigit <=> other_bigit : other_bigit <=> bigit;
	}
	return sign ? front() <=> other.front() : other.front() <=> front();
}

template<std::uint32_t base>
constexpr std::strong_ordering BigInt<base>::__compare_signless(std::uint64_t other, const bool sign) const noexcept
{
	const auto other_size = __int_size(other);
	if(size() != other_size)
		return sign ? size() <=> other_size : other_size <=> size();
	std::strong_ordering ret = sign ? front() <=> other % true_base() : other % true_base() <=> front();
	for(const auto& bigit : bigits | __DROP1_V) {
		other /= true_base();
		const auto other_bigit = other % true_base();
		if(bigit != other_bigit)
			ret = sign ? bigit <=> other_bigit : other_bigit <=> bigit;
	}
	return ret;
}

#endif // BIG_INT_HELPER_FUNCTIONS_HPP

