#ifndef BIG_INT_FRIEND_FUNCTIONS_HPP
#define BIG_INT_FRIEND_FUNCTIONS_HPP

#include "../big_int.hpp"
#include "member_functions.hpp"
#include "external_functions.hpp"

template<std::uint32_t base>
std::ostream& operator<<(std::ostream& os, const BigInt<base>& bi)
{
	return os << bi.str();
}

template<std::uint32_t base>
std::istream& operator>>(std::istream& is, BigInt<base>& bi)
{
	std::string input;
	is >> input;
	bi = make_big_int<base>(input);
	return is;
}

#endif // BIG_INT_FRIEND_FUNCTIONS_HPP

