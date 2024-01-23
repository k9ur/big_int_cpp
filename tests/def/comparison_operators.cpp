#include "../test.hpp"
#include "../../include/def/comparison_operators.hpp"

consteval bool test_consteval()
{
	using namespace std::string_literals;

	const BigInt zero;
	const BigInt negative_zero{ "-0"s };
	const BigInt one{ 1 };
	const BigInt negative_one{ -1 };
	const BigInt<1024> larger_size{ 123'456 };

	ASSERT(!(zero >= one));
	ASSERT(zero < one);
	ASSERT(!(one < zero));
	ASSERT(one >= zero);
	ASSERT(zero >= negative_one);
	ASSERT(!(zero < negative_one));
	ASSERT(negative_one < zero);
	ASSERT(!(negative_one >= zero));

	ASSERT(zero == negative_zero);
	ASSERT(zero != one);
	ASSERT(one != negative_one);
	ASSERT(larger_size != BigInt<1024>{ one });
	ASSERT(zero <= negative_zero);
	ASSERT(one > zero);
	ASSERT(zero > negative_one);
	ASSERT(negative_zero > negative_one);
	ASSERT(negative_one < one);
	ASSERT(larger_size > BigInt<1024>{ one });
	ASSERT(larger_size > BigInt<1024>{ negative_one });
	ASSERT(negative_zero == 0);
	ASSERT(one != -1);
	ASSERT(0 <= negative_zero);
	ASSERT(zero != 1);
	ASSERT(larger_size != 1);
	ASSERT(-1 < one);
	ASSERT(!(larger_size <= -1));
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

