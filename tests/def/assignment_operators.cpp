#include "../test.hpp"
#include "../../include/def/assignment_operators.hpp"

consteval bool test_consteval()
{
	using namespace std::string_literals;

	BigInt lhs1;
	BigInt lhs2;
	BigInt lhs3;
	BigInt lhs4;
	BigInt<1024> lhs5;
	BigInt<1024> lhs6;
	BigInt<1024> lhs7;
	BigInt<1024> lhs8;
	const BigInt rhs1{ -123'456 };
	const BigInt rhs5{ -123'456 };
	lhs1 = rhs1;
	lhs2 = BigInt{ -123'456 };
	lhs3 = -123'456;
	lhs4 = "-123456"s;
	lhs5 = rhs5;
	lhs6 = -123'456;
	lhs7 = rhs1;
	lhs8 = BigInt{ -123'456 };

	ASSERT(lhs1.size() == 1 && lhs1.data()[0] == 123'456 && !lhs1.positive());
	ASSERT(lhs2.size() == 1 && lhs2.data()[0] == 123'456 && !lhs2.positive());
	ASSERT(lhs3.size() == 1 && lhs3.data()[0] == 123'456 && !lhs3.positive());
	ASSERT(lhs4.size() == 1 && lhs4.data()[0] == 123'456 && !lhs4.positive());
	ASSERT(lhs5.size() == 2 && lhs5.data()[0] == 576 && lhs5.data()[1] == 120 && !lhs5.positive());
	ASSERT(lhs6.size() == 2 && lhs6.data()[0] == 576 && lhs6.data()[1] == 120 && !lhs6.positive());
	ASSERT(lhs7.size() == 2 && lhs7.data()[0] == 576 && lhs7.data()[1] == 120 && !lhs7.positive());
	ASSERT(lhs8.size() == 2 && lhs8.data()[0] == 576 && lhs8.data()[1] == 120 && !lhs8.positive());
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

