#include "../test.hpp"
#include "../../include/def/constructors.hpp"

consteval bool test_consteval()
{
	using namespace std::string_view_literals;
	using namespace std::string_literals;
	const std::vector<std::uint32_t> vec11{ 456'789, 123 };

	const BigInt bi1;
	const BigInt bi2{ bi1 };
	const BigInt bi3{ BigInt{ -123'456 } };
	const BigInt<1024> bi4{ bi1 };
	const BigInt<1024> bi5{ BigInt{ -123'456 } };
	const BigInt bi6{ -123'456 };
	const BigInt bi7{ "-123456"sv };
	const BigInt<1024> bi8{ "-123456" };
	const BigInt bi9{ "1e240"sv, 16 };
	const BigInt<1024> bi10{ "+1E240"s, 16 };
	const BigInt bi11{ vec11, true };
	const BigInt bi12{ std::vector<std::uint32_t>{ 456'789, 123 }, true };

	ASSERT(bi1.size() == 1 && bi1.data()[0] == 0);
	ASSERT(bi2.size() == 1 && bi2.data()[0] == 0);
	ASSERT(bi3.size() == 1 && bi3.data()[0] == 123'456 && !bi3.positive());
	ASSERT(bi4.size() == 1 && bi4.data()[0] == 0);
	ASSERT(bi5.size() == 2 && bi5.data()[0] == 576 && bi5.data()[1] == 120 && !bi5.positive());
	ASSERT(bi6.size() == 1 && bi6.data()[0] == 123'456 && !bi6.positive());
	ASSERT(bi7.size() == 1 && bi7.data()[0] == 123'456 && !bi7.positive());
	ASSERT(bi8.size() == 2 && bi8.data()[0] == 576 && bi8.data()[1] == 120 && !bi8.positive());
	ASSERT(bi9.size() == 1 && bi9.data()[0] == 123'456 && bi9.positive());
	ASSERT(bi10.size() == 2 && bi10.data()[0] == 576 && bi10.data()[1] == 120 && bi10.positive());
	ASSERT(bi11.size() == 2 && bi11.data()[0] == 456'789 && bi11.data()[1] == 123 && !bi11.positive());
	ASSERT(bi12.size() == 2 && bi12.data()[0] == 456'789 && bi12.data()[1] == 123 && !bi12.positive());
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

