#include "../test.hpp"
#include "../../include/def/unary_operators.hpp"

consteval bool test_consteval()
{
	const BigInt pos{ 123'456 };
	const BigInt neg{ -123'456 };
	const BigInt zero{ 0 };
	BigInt incr_test{ neg };
	BigInt decr_test{ neg };
	const BigInt pre_incr{ ++incr_test };
	const BigInt pre_decr{ --decr_test };
	const BigInt post_incr{ incr_test++ };
	const BigInt post_decr{ decr_test-- };
	BigInt<1024> not_bin{ pos };
	not_bin = ~not_bin;

	ASSERT((-pos).size() == 1 && (-pos).data()[0] == 123'456 && !(-pos).positive());
	ASSERT((+pos).size() == 1 && (+pos).data()[0] == 123'456 && (+pos).positive());
	ASSERT((-neg).size() == 1 && (-neg).data()[0] == 123'456 && (-neg).positive());
	ASSERT((+neg).size() == 1 && (+neg).data()[0] == 123'456 && (+neg).positive());
	ASSERT(pos && neg);
	ASSERT(!zero);
	ASSERT(pre_incr.size() == 1 && pre_incr.data()[0] == 123'455 && !pre_incr.positive());
	ASSERT(pre_decr.size() == 1 && pre_decr.data()[0] == 123'457 && !pre_decr.positive());
	ASSERT(post_incr.size() == 1 && post_incr.data()[0] == 123'455 && !post_incr.positive());
	ASSERT(post_decr.size() == 1 && post_decr.data()[0] == 123'457 && !post_decr.positive());
	ASSERT(incr_test.size() == 1 && incr_test.data()[0] == 123'454 && !incr_test.positive());
	ASSERT(decr_test.size() == 1 && decr_test.data()[0] == 123'458 && !decr_test.positive());
	ASSERT((~pos).size() == 2 && (~pos).data()[0] == 294'843'839 && (~pos).data()[1] == 4 && !(~pos).positive());
	ASSERT(not_bin.size() == 2 && not_bin.data()[0] == 447 && not_bin.data()[1] == 903 && !not_bin.positive());
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

