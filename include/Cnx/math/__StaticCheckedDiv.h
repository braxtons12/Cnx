/// @file __StaticCheckedDiv.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Compile-time checked division function
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

#include <Cnx/Assert.h>
#include <Cnx/math/__StaticBase.h>

#ifndef __CNX_STATIC_MATH_CHECKED_DIV
	#define __CNX_STATIC_MATH_CHECKED_DIV

	/// @brief Performs the checked division of the two arguments
	///
	/// @param left - The dividend
	/// @param right - The divisor
	///
	/// @return the quotient of the two arguments
	#define __cnx_static_checked_div(left, right)                                        \
		({                                                                               \
			cnx_static_assert((left) != __cnx_checked_funcs_nan                          \
								  && (right) != __cnx_checked_funcs_nan && (right) != 0, \
							  "overflow in cnx_static_checked_div");                     \
			(left) / (right);                                                            \
		})
#endif // __CNX_STATIC_MATH_CHECKED_DIV
