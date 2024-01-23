# big_int_cpp
C++ class for dynamically sized signed integers larger than 64 bits

Works with any base up to and including `UINT_MAX`

Fully constexpr, except for friended I/O stream operators.

## Usage
Compiles with C++23. gcc-13.2.0 with flags `--std=c23` used for testing

Include all header files in `include/` and `include/def/`:
```cpp
#include "big_int.hpp"
#include "def/arithmetic_assignment_operators.hpp"
#include "def/assignment_operators.hpp"
#include "def/binary_arithmetic_operators.hpp"
#include "def/comparison_operators.hpp"
#include "def/constructors.hpp"
#include "def/external_functions.hpp"
#include "def/friend_functions.hpp"
#include "def/helper_functions.hpp"
#include "def/member_functions.hpp"
#include "def/static_functions.hpp"
#include "def/unary_operators.hpp"
#include "def/utility_functions.hpp"
```

## Features
Almost all relevant operators are overloaded, e.g. `operator+=`, `operator/`, `operator>=`, etc., as well as I/O stream operators.

Some simple math functions are provided: `abs`, `sqrt`, `pow`, `log`, `gcd`, `lcm`, `factorial`.

Other member functions: `size`, `positive`, `sign`, `is_zero`, `clone`, `swap`, `data`, `bits`, `digits`, `digit_sum`, `to_int`, `str`, `true_base`.

## Example
The below example prints the 100th Fibonacci number:
```cpp
unsigned i = 0;
BigInt<1'000'000'000> F_p{ 0 }, F_n{ 1 }, temp;
do {
	temp = F_n;
	F_n += F_p;
	F_p = temp;
	++i;
} while(i != 100);
std::cout << F_n << std::endl; // 573147844013817084101
```

## Base presets
List
- `MAX_BASE`: equivalent to `UINT_MAX`
- `MAX_BINARY_BASE`: equivalent to `UINT_MAX`
- `MAX_DECIMAL_BASE`: equivalent to `1'000'000'000`
- `PRINT_BASE`: equivalent to `1'000'000'000`
- `DEFAULT_BASE` (default): equivalent to `1'000'000'000`

No support for unary.

