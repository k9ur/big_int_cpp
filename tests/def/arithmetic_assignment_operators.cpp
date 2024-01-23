#include "../test.hpp"
#include "../../include/def/external_functions.hpp"

consteval bool test_consteval()
{
	using namespace std::string_literals;

	const BigInt<999> val1{ -123'456 };
	const BigInt<5000> val2{ 987'654 };
	const BigInt<0> val3{ 12 };
	BigInt<1024> bi1{ val1 };
	BigInt<1024> bi2{ val1 };
	BigInt<1024> bi3{ val1 };
	BigInt<1024> bi4{ val2 };
	BigInt<1024> bi5{ val2 };
	BigInt<> bi6{ val1 };
	BigInt<> bi7{ val1 };
	BigInt<> bi8{ val1 };
	BigInt<> bi9{ val2 };
	BigInt<> bi10{ val2 };
	BigInt<> bi11{ val1 };
	BigInt<> bi12{ val1 };
	BigInt<> bi13{ val1 };
	BigInt<> bi14{ val2 };
	BigInt<> bi15{ val2 };
	BigInt<> bi16{ val1 };
	BigInt<> bi17{ val1 };
	BigInt<> bi18{ val1 };
	BigInt<> bi19{ val2 };
	BigInt<> bi20{ val2 };
	BigInt<1024> bi21{ val1 };
	BigInt<1024> bi22{ val1 };
	BigInt<1024> bi23{ val1 };
	BigInt<1024> bi24{ val2 };
	BigInt<1024> bi25{ val2 };
	BigInt<> bi26{ val1 };
	BigInt<> bi27{ val1 };
	BigInt<> bi28{ val1 };
	BigInt<> bi29{ val2 };
	BigInt<> bi30{ val2 };
	bi1 += val2;
	bi2 -= val2;
	bi3 *= val2;
	bi4 /= val1;
	bi5 %= val1;
	bi6 &= val2;
	bi7 |= val2;
	bi8 ^= val2;
	bi9 <<= val3;
	bi10 >>= val3;
	bi11 += 987'654;
	bi12 -= 987'654;
	bi13 *= 987'654;
	bi14 /= -123'456;
	bi15 %= -123'456;
	bi16 &= 987'654;
	bi17 |= 987'654;
	bi18 ^= 987'654;
	bi19 <<= 12;
	bi20 >>= 12;
	bi21 += "987654"s;
	bi22 -= "987654"s;
	bi23 *= "987654"s;
	bi24 /= "-123456"s;
	bi25 %= "-123456"s;
	bi26 &= "987654"s;
	bi27 |= "987654"s;
	bi28 ^= "987654"s;
	bi29 <<= "12"s;
	bi30 >>= "12"s;

	ASSERT(bi1.size() == 2 && bi1.data()[0] == 966 && bi1.data()[1] == 843 && bi1.positive());
	ASSERT(bi2.size() == 3 && bi2.data()[0] == 70 && bi2.data()[1] == 61 && bi2.data()[2] == 1 && !bi2.positive());
	ASSERT(bi3.size() == 4 && bi3.data()[0] == 384 && bi3.data()[1] == 243 && bi3.data()[2] == 571 && bi3.data()[3] == 113 && !bi3.positive());
	ASSERT(bi4.size() == 1 && bi4.data()[0] == 8 && !bi4.positive());
	ASSERT(bi5.size() == 1 && bi5.data()[0] == 6 && bi5.positive());
	ASSERT(bi6.size() == 1 && bi6.data()[0] == 66'048 && !bi6.positive());
	ASSERT(bi7.size() == 1 && bi7.data()[0] == 1'045'062 && bi7.positive());
	ASSERT(bi8.size() == 1 && bi8.data()[0] == 979'014 && bi8.positive());
	ASSERT(bi9.size() == 2 && bi9.data()[0] == 45'430'784 && bi9.data()[1] == 4 && bi9.positive());
	ASSERT(bi10.size() == 1 && bi10.data()[0] == 241 && bi10.positive());
	ASSERT(bi11.size() == 1 && bi11.data()[0] == 864'198 && bi11.positive());
	ASSERT(bi12.size() == 1 && bi12.data()[0] == 1'111'110 && !bi12.positive());
	ASSERT(bi13.size() == 2 && bi13.data()[0] == 931'812'224 && bi13.data()[1] == 121 && !bi13.positive());
	ASSERT(bi14.size() == 1 && bi14.data()[0] == 8 && !bi14.positive());
	ASSERT(bi15.size() == 1 && bi15.data()[0] == 6 && bi15.positive());
	ASSERT(bi16.size() == 1 && bi16.data()[0] == 66'048 && !bi16.positive());
	ASSERT(bi17.size() == 1 && bi17.data()[0] == 1'045'062 && bi17.positive());
	ASSERT(bi18.size() == 1 && bi18.data()[0] == 979'014 && bi18.positive());
	ASSERT(bi19.size() == 2 && bi19.data()[0] == 45'430'784 && bi19.data()[1] == 4 && bi9.positive());
	ASSERT(bi20.size() == 1 && bi20.data()[0] == 241 && bi20.positive());
	ASSERT(bi21.size() == 2 && bi21.data()[0] == 966 && bi21.data()[1] == 843 && bi21.positive());
	ASSERT(bi22.size() == 3 && bi22.data()[0] == 70 && bi22.data()[1] == 61 && bi22.data()[2] == 1 && !bi22.positive());
	ASSERT(bi23.size() == 4 && bi23.data()[0] == 384 && bi23.data()[1] == 243 && bi23.data()[2] == 571 && bi23.data()[3] == 113 && !bi23.positive());
	ASSERT(bi24.size() == 1 && bi24.data()[0] == 8 && !bi24.positive());
	ASSERT(bi25.size() == 1 && bi25.data()[0] == 6 && bi25.positive());
	ASSERT(bi26.size() == 1 && bi26.data()[0] == 66'048 && !bi26.positive());
	ASSERT(bi27.size() == 1 && bi27.data()[0] == 1'045'062 && bi27.positive());
	ASSERT(bi28.size() == 1 && bi28.data()[0] == 979'014 && bi28.positive());
	ASSERT(bi29.size() == 2 && bi29.data()[0] == 45'430'784 && bi29.data()[1] == 4 && bi29.positive());
	ASSERT(bi30.size() == 1 && bi30.data()[0] == 241 && bi30.positive());
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

