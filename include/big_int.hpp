#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <istream>
#include <ostream>
#include <ranges>
#include <compare>
#include <type_traits>
#include <limits>
#include <cstddef>
#include <cstdint>


// Types
enum class BitwiseType : std::uint8_t
{
	AND, OR, XOR
};

// Base constants
enum COMMON_BASES : std::uint32_t
{
	MAX_BASE = 0, // Least storage usage, a lot of calculations would be faster with this
	MAX_BINARY_BASE = 0,
	MAX_DECIMAL_BASE = 1'000'000'000,
	PRINT_BASE = MAX_DECIMAL_BASE, // More efficient at printing
	DEFAULT_BASE = MAX_DECIMAL_BASE
};

// Concepts
template<typename T> // Used in BigIntConstructible methods to call specialised overloads
concept INT64Constructible = std::is_convertible_v<T, std::int64_t>;
template<typename T> // All types that can be passed to the constructor (other than BigInt types)
concept BigIntConstructible = INT64Constructible<T> || std::is_convertible_v<T, std::string_view>;

template<std::uint32_t base = COMMON_BASES::DEFAULT_BASE> // Base of 0 is equivalent to UINT32_MAX + 1
class BigInt
{
	static_assert(base != 1); // No unary support

	// Member variables
	std::vector<std::uint32_t> bigits;
	bool negative = false; // Can be true or false if value is 0

  public:
	// Friend BigInt instances with different bases
	template<std::uint32_t other_base>
	friend class BigInt;

	static constexpr auto BASE_ZERO_TRUE_VALUE = static_cast<std::uint64_t>(std::numeric_limits<std::uint32_t>::max()) + 1;

	// Constructors
	constexpr BigInt();
	constexpr BigInt(const BigInt&) = default;
	constexpr BigInt(BigInt&&) = default;
	constexpr BigInt(const std::int64_t);
	constexpr BigInt(const std::string_view, const char = 10);
	constexpr explicit BigInt(const std::vector<std::uint32_t>&, bool);
	constexpr explicit BigInt(std::vector<std::uint32_t>&&, bool);
	template<std::uint32_t other_base>
	constexpr BigInt(const BigInt<other_base>&);
	template<std::uint32_t other_base>
	constexpr BigInt(BigInt<other_base>&&);

	constexpr ~BigInt() = default;

	// Assignment operators. Specialisations not required
	constexpr BigInt& operator=(const BigInt&) = default;
	constexpr BigInt& operator=(BigInt&&) = default;
	constexpr BigInt& operator=(const BigIntConstructible auto);
	template<std::uint32_t other_base>
	constexpr BigInt& operator=(const BigInt<other_base>&);
	template<std::uint32_t other_base>
	constexpr BigInt& operator=(BigInt<other_base>&&);

	// Arithmetic-assignment operators
	constexpr BigInt& operator+=(const BigInt&);
	constexpr BigInt& operator-=(const BigInt&);
	constexpr BigInt& operator*=(const BigInt&);
	constexpr BigInt& operator/=(const BigInt&);
	constexpr BigInt& operator%=(const BigInt&);
	constexpr BigInt& operator&=(const BigInt&);
	constexpr BigInt& operator|=(const BigInt&);
	constexpr BigInt& operator^=(const BigInt&);
	constexpr BigInt& operator<<=(const BigInt&);
	constexpr BigInt& operator>>=(const BigInt&);
	constexpr BigInt& operator+=(const std::int64_t); // Specialisation: addition without construction
	constexpr BigInt& operator-=(const std::int64_t); // Specialisation: subtraction without construction
	constexpr BigInt& operator*=(const std::int64_t); // Specialisation: short multiplication without construction
	constexpr BigInt& operator/=(const std::int64_t); // Specialisation: division without construction nor base conversion
	constexpr BigInt& operator%=(const std::int64_t); // Specialisation: modulo without construction nor base conversion
	constexpr BigInt& operator&=(const std::int64_t); // Specialisation: bitwise AND without construction
	constexpr BigInt& operator|=(const std::int64_t); // Specialisation: bitwise OR without construction
	constexpr BigInt& operator^=(const std::int64_t); // Specialisation: bitwise XOR without construction
	constexpr BigInt& operator<<=(const std::int64_t); // Specialisation
	constexpr BigInt& operator>>=(const std::int64_t); // Specialisation
	constexpr BigInt& operator+=(const BigIntConstructible auto);
	constexpr BigInt& operator-=(const BigIntConstructible auto);
	constexpr BigInt& operator*=(const BigIntConstructible auto);
	constexpr BigInt& operator/=(const BigIntConstructible auto);
	constexpr BigInt& operator%=(const BigIntConstructible auto);
	constexpr BigInt& operator&=(const BigIntConstructible auto);
	constexpr BigInt& operator|=(const BigIntConstructible auto);
	constexpr BigInt& operator^=(const BigIntConstructible auto);
	constexpr BigInt& operator<<=(const BigIntConstructible auto);
	constexpr BigInt& operator>>=(const BigIntConstructible auto);

	// Binary arithmetic operators. Specialisations not required
	constexpr BigInt operator+(const BigInt&) const;
	constexpr BigInt operator-(const BigInt&) const;
	constexpr BigInt operator*(const BigInt&) const;
	constexpr BigInt operator/(const BigInt&) const;
	constexpr BigInt operator%(const BigInt&) const;
	constexpr BigInt operator&(const BigInt&) const;
	constexpr BigInt operator|(const BigInt&) const;
	constexpr BigInt operator^(const BigInt&) const;
	constexpr BigInt operator<<(const BigInt&) const;
	constexpr BigInt operator>>(const BigInt&) const;
	constexpr BigInt operator+(const BigIntConstructible auto) const;
	constexpr BigInt operator-(const BigIntConstructible auto) const;
	constexpr BigInt operator*(const BigIntConstructible auto) const;
	constexpr BigInt operator/(const BigIntConstructible auto) const;
	constexpr BigInt operator%(const BigIntConstructible auto) const;
	constexpr BigInt operator&(const BigIntConstructible auto) const;
	constexpr BigInt operator|(const BigIntConstructible auto) const;
	constexpr BigInt operator^(const BigIntConstructible auto) const;
	constexpr BigInt operator<<(const BigIntConstructible auto) const;
	constexpr BigInt operator>>(const BigIntConstructible auto) const;
	
	// Unary operators
	constexpr BigInt operator+() const;
	constexpr BigInt operator-() const;
	constexpr BigInt operator~() const;
	constexpr explicit operator bool() const noexcept;
	constexpr BigInt& operator++();
	constexpr BigInt& operator--();
	constexpr BigInt operator++(int);
	constexpr BigInt operator--(int);

	// Comparison operators
	constexpr bool operator==(const BigInt&) const noexcept;
	constexpr bool operator!=(const BigInt&) const noexcept;
	constexpr std::strong_ordering operator<=>(const BigInt&) const noexcept;
	constexpr bool operator==(std::int64_t) const noexcept; // Specialisation: comparison without construction
	constexpr std::strong_ordering operator<=>(std::int64_t) const noexcept; // Specialisation: comparison without construction
	template<BigIntConstructible T>
	constexpr bool operator==(const T) const noexcept(INT64Constructible<T>);
	template<BigIntConstructible T>
	constexpr bool operator!=(const T) const noexcept(INT64Constructible<T>);
	template<BigIntConstructible T>
	constexpr std::strong_ordering operator<=>(const T) const noexcept(INT64Constructible<T>);

	// Member functions
	constexpr std::size_t size() const noexcept;
	constexpr bool positive() const noexcept;
	constexpr BigInt& sign(const bool) noexcept;
	constexpr bool is_zero() const noexcept;
	constexpr BigInt clone() const;
	constexpr void swap(BigInt&) noexcept;
	constexpr const std::vector<std::uint32_t>& data() const noexcept;

	constexpr BigInt& abs() noexcept;
	constexpr BigInt& sqrt();
	constexpr BigInt& log(const BigInt&);
	constexpr BigInt& log(const std::int64_t); // Specialisation: log without construction
	constexpr BigInt& log(const BigIntConstructible auto);
	constexpr BigInt& pow(const BigInt&);
	constexpr BigInt& pow(std::int64_t); // Specialisation: pow without construction
	constexpr BigInt& pow(const BigIntConstructible auto);
	constexpr BigInt& gcd(const BigInt&);
	constexpr BigInt& gcd(const BigIntConstructible auto);
	constexpr BigInt& lcm(const BigInt&);
	constexpr BigInt& lcm(const BigIntConstructible auto);
	constexpr BigInt& factorial();

	constexpr std::uint64_t bits() const;
	constexpr std::uint64_t digits(std::uint32_t = 10) const;
	constexpr std::uint64_t digit_sum(std::uint32_t = 10) const;
	constexpr std::int64_t to_int() const;
	constexpr std::string str(const char = 10) const;

	// Friend functions
	template<std::uint32_t any_base>
	friend std::ostream& operator<<(std::ostream&, const BigInt<any_base>&);
	template<std::uint32_t any_base>
	friend std::istream& operator>>(std::istream&, BigInt<any_base>&);

	// Static functions: public
	static constexpr std::uint64_t true_base() noexcept;

  private:
	// Static functions: private
	static constexpr std::size_t __int_size(const std::int64_t) noexcept;
	static constexpr std::uint8_t __int_log_of(std::uint64_t, std::uint64_t) noexcept;
	static constexpr bool __valid_char(const char, const char = 10);
	static constexpr char __char_value(const char, const char = 10);
	static constexpr char __value_to_char(const char, const char = 10);
	static constexpr std::string __constexpr_to_string(std::int64_t, const char = 10); // NOTE: constexpr version of std::to_string
	static constexpr std::uint64_t __constexpr_abs(const std::int64_t) noexcept; // NOTE: constexpr version of std::abs

	// Static helper constants
	static constexpr auto __LESS_SO = std::strong_ordering::less;
	static constexpr auto __GREATER_SO = std::strong_ordering::greater;
	static constexpr auto __EQUAL_SO = std::strong_ordering::equal;
	static constexpr auto __REV_V = std::views::reverse;
	static constexpr auto __DROP1_V = std::views::drop(1);
	static constexpr auto __REV_DROP1_V = __REV_V | __DROP1_V;
	static constexpr auto __THROW_SV_BASE_DOMAIN = "sv_base must be between 2 and 36 (default 10)";
	static constexpr auto __THROW_STR_BASE_DOMAIN = "str_base must be between 2 and 36 (default 10)";
	static constexpr auto __THROW_SMALLER_BASE_DOMAIN = "smaller_base must be less than or equal to base";
	static constexpr auto __THROW_DIVIDE_ZERO_DOMAIN = "Cannot divide by zero";
	static constexpr auto __THROW_LOG_VALUE_DOMAIN = "Cannot log a non-positive number";
	static constexpr auto __THROW_LOG_BASE_DOMAIN = "Log base must be greater than 1";
	static constexpr auto __THROW_SQRT_DOMAIN = "Cannot find the square root of a negative number";
	static constexpr auto __THROW_FACTORIAL_DOMAIN = "Cannot factorial a negative number";
	static constexpr auto __THROW_INT64_OVERFLOW = "Overflowed when converting to int64_t";
	static constexpr auto __THROW_INT64_UNDERFLOW = "Underflowed when converting to int64_t";
	static constexpr auto __THROW_CHAR_INVALID = "Invalid char used";
	static constexpr auto __THROW_VALUE_TO_CHAR_INVALID = "Invalid value used (must be less than sv_base)";

	// Helper functions
	constexpr BigInt& __sqrt_helper();
	constexpr BigInt& __gcd_helper(const BigInt&);
	constexpr BigInt& __not_helper();
	template<BitwiseType type = BitwiseType::AND>
	constexpr BigInt& __bitwise_signless(const BigInt&);
	constexpr BigInt& __add_eq_signless(const BigInt&);
	constexpr BigInt& __sub_eq_signless(const BigInt&);
	constexpr BigInt& __mul_eq_signless(const BigInt&);
	template<bool remainder = false>
	constexpr BigInt& __div_mod_eq_signless(const BigInt&);
	constexpr BigInt& __shl_eq_signless(const BigInt&);
	constexpr BigInt& __shr_eq_signless(const BigInt&);
	template<bool right = false>
	constexpr BigInt& __shl_shr_eq_helper(const BigInt&);
	constexpr std::strong_ordering __compare_signless(const BigInt&, const bool = true) const noexcept;
	template<BitwiseType type = BitwiseType::AND>
	constexpr BigInt& __bitwise_signless(const std::uint64_t);
	constexpr BigInt& __add_eq_signless(const std::uint64_t);
	constexpr BigInt& __sub_eq_signless(const std::uint64_t);
	constexpr BigInt& __mul_eq_signless(const std::uint64_t);
	template<bool remainder = false>
	constexpr BigInt& __div_mod_eq_signless(const std::uint64_t);
	constexpr BigInt& __shl_eq_signless(const std::uint64_t);
	constexpr BigInt& __shr_eq_signless(const std::uint64_t);
	template<bool right = false>
	constexpr BigInt& __shl_shr_eq_helper(const std::int64_t);
	constexpr std::strong_ordering __compare_signless(std::uint64_t, const bool = true) const noexcept;

	// Utility functions
	constexpr bool valid() const;
	constexpr void reserve(const std::size_t);
	constexpr void resize(const std::size_t);
	constexpr void resize(const std::size_t, const std::uint32_t);
	constexpr void assign(const std::size_t, const std::uint32_t);
	constexpr std::uint32_t& front();
	constexpr const std::uint32_t& front() const;
	constexpr std::uint32_t& back();
	constexpr const std::uint32_t& back() const;
};

// External functions
template<std::uint32_t base = COMMON_BASES::DEFAULT_BASE>
constexpr BigInt<base> make_big_int(const BigIntConstructible auto);
constexpr BigInt<> operator""_bi(std::uint64_t);
constexpr BigInt<> operator""_big_int(std::uint64_t);
constexpr BigInt<> operator""_bi(const char*, std::size_t);
constexpr BigInt<> operator""_big_int(const char*, std::size_t);


#endif // BIG_INT_HPP

