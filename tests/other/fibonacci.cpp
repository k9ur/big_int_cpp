#include "../test.hpp"
#include "../../include/def/constructors.hpp"
#include "../../include/def/assignment_operators.hpp"
#include "../../include/def/arithmetic_assignment_operators.hpp"
#include "../../include/def/member_functions.hpp"

consteval bool test_fib()
{ // Calculate the index of the first term in the Fibonacci sequence to contain 200 digits
	std::uint64_t i = 2;
	BigInt F_p{ 1 }, F_n{ 1 }, temp;
	do {
		temp = F_n;
		F_n += F_p;
		F_p = temp;
		++i;
	} while(F_n.digits() < 200);
	return i == 954; // Should be 954
}

int main(void)
{
	if constexpr(test_fib())
		std::cout << "Test passed";
	else
		std::cout << "Test failed";
	std::cout << std::endl;

	return EXIT_SUCCESS;
}

