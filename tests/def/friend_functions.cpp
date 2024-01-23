#include <sstream>

#include "../test.hpp"
#include "../../include/def/friend_functions.hpp"

bool test_runtime()
{
	using namespace std::string_literals;
	std::istringstream iss{ "-123456"s };
	std::ostringstream oss;

	BigInt bi1;
	iss >> bi1;
	BigInt<1024> bi2;
	iss.clear();
	iss.seekg(0);
	iss >> bi2;
	oss << bi1 << ' ' << bi2;

	ASSERT(bi1.size() == 1 && bi1.data()[0] == 123'456 && !bi1.positive());
	ASSERT(bi2.size() == 2 && bi2.data()[0] == 576 && bi2.data()[1] == 120 && !bi2.positive());
	ASSERT(oss.str() == "-123456 -123456"s);
	return true;
}

int main(void) {
	if(test_runtime())
		std::cout << "Test passed";
	else
		std::cout << "Test failed";
	std::cout << std::endl;

	return EXIT_SUCCESS;
}

