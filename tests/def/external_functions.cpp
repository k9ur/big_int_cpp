#include "../test.hpp"
#include "../../include/def/external_functions.hpp"

consteval bool test_consteval()
{
	using namespace std::string_literals;

	const BigInt bi1 = make_big_int(-123'456);
	const BigInt bi2 = make_big_int("-123456"s);
	const BigInt<1024> bi3 = make_big_int<1024>(-123'456);
	const BigInt bi4 = 123'456_bi;
	const BigInt bi5 = 123'456_big_int;
	const BigInt bi6 = "-123456"_bi;
	const BigInt bi7 = "-123456"_big_int;

	ASSERT(bi1.size() == 1 && bi1.data()[0] == 123'456 && !bi1.positive());
	ASSERT(bi2.size() == 1 && bi2.data()[0] == 123'456 && !bi2.positive());
	ASSERT(bi3.size() == 2 && bi3.data()[0] == 576 && bi3.data()[1] == 120 && !bi3.positive());
	ASSERT(bi4.size() == 1 && bi4.data()[0] == 123'456 && bi4.positive());
	ASSERT(bi5.size() == 1 && bi5.data()[0] == 123'456 && bi5.positive());
	ASSERT(bi6.size() == 1 && bi6.data()[0] == 123'456 && !bi6.positive());
	ASSERT(bi7.size() == 1 && bi7.data()[0] == 123'456 && !bi7.positive());
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

