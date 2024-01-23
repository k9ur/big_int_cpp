#include <iostream>
#include <cstdlib>

#include "../include/big_int.hpp"

#define ASSERT(condition) \
{                         \
	if(!(condition))      \
		return false;     \
}

consteval bool test_consteval();
bool test_runtime();

