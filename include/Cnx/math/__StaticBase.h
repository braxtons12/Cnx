/// @file __StaticBase.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Base functions for static (compile-time) math functions
/// @version 0.1
/// @date 2022-01-02
///
/// MIT License
/// @copyright Copyright (c) 2022 Braxton Salyer <braxtonsalyer@gmail.com>
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

#include <Cnx/BasicTypes.h>

#ifndef __CNX_STATIC_MATH_BASE
	#define __CNX_STATIC_MATH_BASE

	/// @brief Calculates the absolute value of the given input at compile time
	///
	/// @param x - The value to get the absolute value of
	///
	/// @return the absolute value of `x`, as a compile-time constant
	#define __cnx_static_abs(x) ((x) < 0 ? -(x) : (x))

	/// @brief Calculates the sign of the given input at compile time
	///
	/// @param x - The value to get the sign of
	///
	/// @return `-1` if negative, `0` if 0, `1` if positive
	#define __cnx_static_sign(x) static_cast(typeof(x))((x) == 0 ? 0 : ((x) < 0 ? -1 : 1))

	/// @brief maximum i64 value
	#define __cnx_checked_funcs_max cnx_max_i64
	/// @brief minimum i64 value
	#define __cnx_checked_funcs_min -__cnx_checked_funcs_max
	/// @brief NaN equivalent
	#define __cnx_checked_funcs_nan (__cnx_checked_funcs_min - 1)

#endif // __CNX_STATIC_MATH_BASE
