/// @file StdAssert.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides wrappers for standard C compile-time and runtime asserts and other
/// custom asserts and assert-like facilities
/// @version 0.1
/// @date 2021-08-15
///
/// MIT License
/// @copyright Copyright (c) 2021 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#include "../include/C2nxt/StdAssert.h"

#include <assert.h>

#include "../include/C2nxt/StdIO.h"

#if STD_PLATFORM_DEBUG && !defined(STD_DISABLE_ASSERTIONS)
void std_assert_cstring(bool condition, const_cstring error_message, const_cstring file, i64 line) {
	if(!condition) {
		let message = std_string_from(error_message);
		let _file = std_string_from(file);
		eprintln("Assertion triggered at {}:{}: {}", _file, line, message);
		assert(condition);
	}
}
#endif // (defined(DEBUG) || !defined(NDEBUG)) && !defined(STD_DISABLE_ASSERTIONS)
