#include "../test.hpp"
#include "../../include/def/member_functions.hpp"

consteval bool test_consteval()
{
	using namespace std::string_literals;

	BigInt pos{ 123'456 };
	BigInt neg{ -123'456 };
	const BigInt large{ 987'654 };
	const BigInt zero{ 0 };
	const BigInt three{ 3 };
	const BigInt cloned{ pos.clone() };
	BigInt swap1{ 1 };
	BigInt swap2{ -2 };
	BigInt pos_abs{ pos };
	BigInt neg_abs{ neg };
	BigInt pos_sqrt{ pos };
	BigInt pos_log1{ pos };
	BigInt pos_log2{ pos };
	BigInt pos_log3{ pos };
	BigInt pos_pow{ pos };
	BigInt neg_pow1{ neg };
	BigInt neg_pow2{ neg };
	BigInt neg_gcd1{ neg };
	BigInt neg_gcd2{ neg };
	BigInt neg_lcm1{ neg };
	BigInt neg_lcm2{ neg };
	BigInt fact{ 18 };
	BigInt neg_int{ neg };
	const auto pos_size = pos.size();
	const auto pos_positive = pos.positive();
	const auto neg_positive = neg.positive();
	const auto pos_data = pos.data();
	const auto neg_str_dec = neg.str();
	const auto neg_str_hex = neg.str(16);
	pos.sign(false);
	neg.sign(true);
	swap1.swap(swap2);
	pos_abs.abs();
	neg_abs.abs();
	pos_sqrt.sqrt();
	pos_log1.log(three);
	pos_log2.log(3);
	pos_log3.log("3"s);
	pos_pow.pow(three);
	neg_pow1.pow(3);
	neg_pow2.pow("3"s);
	neg_gcd1.gcd(large);
	neg_gcd2.gcd("987654"s);
	neg_lcm1.lcm(large);
	neg_lcm2.lcm("987654"s);
	fact.factorial();

	ASSERT(pos_size == 1);
	ASSERT(pos_positive && !neg_positive);
	ASSERT(pos.size() == 1 && pos.data()[0] == 123'456 && !pos.positive());
	ASSERT(neg.size() == 1 && neg.data()[0] == 123'456 && neg.positive());
	ASSERT(zero.is_zero() && !pos.is_zero() && !neg.is_zero());
	ASSERT(cloned.size() == 1 && cloned.data()[0] == 123'456 && cloned.positive());
	ASSERT(swap1.size() == 1 && swap2.size() == 1 && swap1.data()[0] == 2 && swap2.data()[0] == 1 && !swap1.positive() && swap2.positive());
	ASSERT(pos_data.size() == 1 && pos_data[0] == 123'456);
	ASSERT(pos_abs.size() == 1 && pos_abs.data()[0] == 123'456 && pos_abs.positive());
	ASSERT(neg_abs.size() == 1 && neg_abs.data()[0] == 123'456 && neg_abs.positive());
	ASSERT(pos_sqrt.size() == 1 && pos_sqrt.data()[0] == 351 && pos_sqrt.positive());
	ASSERT(pos_log1.size() == 1 && pos_log1.data()[0] == 10 && pos_log1.positive());
	ASSERT(pos_log2.size() == 1 && pos_log2.data()[0] == 10 && pos_log2.positive());
	ASSERT(pos_log3.size() == 1 && pos_log3.data()[0] == 10 && pos_log3.positive());
	ASSERT(pos_pow.size() == 2 && pos_pow.data()[0] == 295'202'816 && pos_pow.data()[1] == 1'881'640 && pos_pow.positive());
	ASSERT(neg_pow1.size() == 2 && neg_pow1.data()[0] == 295'202'816 && neg_pow1.data()[1] == 1'881'640 && !neg_pow1.positive());
	ASSERT(neg_pow2.size() == 2 && neg_pow2.data()[0] == 295'202'816 && neg_pow2.data()[1] == 1'881'640 && !neg_pow2.positive());
	ASSERT(neg_gcd1.size() == 1 && neg_gcd1.data()[0] == 6 && neg_gcd1.positive());
	ASSERT(neg_gcd2.size() == 1 && neg_gcd2.data()[0] == 6 && neg_gcd2.positive());
	ASSERT(neg_lcm1.size() == 2 && neg_lcm1.data()[0] == 321'968'704 && neg_lcm1.data()[1] == 20 && neg_lcm1.positive());
	ASSERT(neg_lcm2.size() == 2 && neg_lcm2.data()[0] == 321'968'704 && neg_lcm2.data()[1] == 20 && neg_lcm2.positive());
	ASSERT(fact.size() == 2 && fact.data()[0] == 705'728'000 && fact.data()[1] == 6'402'373 && fact.positive());
	ASSERT(large.bits() == 20);
	ASSERT(large.digits() == 6);
	ASSERT(large.digits(16) == 5);
	ASSERT(large.digit_sum() == 39);
	ASSERT(large.digit_sum(16) == 24);
	ASSERT(neg_int.to_int() == -123'456);
	ASSERT(neg_str_dec == "-123456"s && neg_str_hex == "-1e240"s);
	return true;
}

int main(void)
{
	if constexpr(test_consteval())
		std::cout << "Test passed";
	else
		std::cout << "Test failed";
	std::cout << std::endl;

	return EXIT_SUCCESS;
}

