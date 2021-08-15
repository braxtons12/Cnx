/// @file __StdStaticBase.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Base functions for static (compile-time) math functions
/// @version 0.1
/// @date 2021-08-14
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

#include "../StdBasicTypes.h"

#ifndef __STD_STATIC_MATH_BASE
	#define __STD_STATIC_MATH_BASE

	/// @brief Calculates the absolute value of the given input at compile time
	///
	/// @param x - The value to get the absolute value of
	///
	/// @return the absolute value of `x`, as a compile-time constant
	#define __std_static_abs(x) ((x) < 0 ? -(x) : (x))

	/// @brief Calculates the sign of the given input at compile time
	///
	/// @param x - The value to get the sign of
	///
	/// @return `-1` if negative, `0` if 0, `1` if positive
	#define __std_static_sign(x) ((x) == 0 ? 0 : ((x) < 0 ? -1 : 1))

	/// @brief maximum i64 value
	#define __std_checked_funcs_max std_max_i64
	/// @brief minimum i64 value
	#define __std_checked_funcs_min -__std_checked_funcs_max
	/// @brief NaN equivalent
	#define __std_checked_funcs_nan (__std_checked_funcs_min - 1)

#endif // __STD_STATIC_MATH_BASE
