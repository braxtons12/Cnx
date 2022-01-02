/// @file __StdStaticCheckedMul.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Compile-time checked multiplication function
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

#include <C2nxt/StdAssert.h>
#include <C2nxt/math/__StdStaticBase.h>

#ifndef __STD_STATIC_MATH_CHECKED_MUL
	#define __STD_STATIC_MATH_CHECKED_MUL

	/// @brief Generates the overflow check condition for the multiplication of the given arguments
	///
	/// @param left - The left number to multiply
	/// @param right - The right number to multiply
	///
	/// @return the overflow check condition
	#define __std_static_checked_mul_assert_condition(left, right)                              \
		(__std_static_sign(right) != 0 ?                                                        \
			 ((left) != __std_checked_funcs_nan && (right) != __std_checked_funcs_nan           \
			  && __std_static_abs(left) <= __std_checked_funcs_max / __std_static_abs(right)) : \
			   true)
	/// @brief Performs the checked multiplication of the two arguments
	///
	/// @param left - The left number to multiply
	/// @param right - The right number to multiply
	///
	/// @return the product of the two arguments
	#define __std_static_checked_mul(left, right)                                     \
		({                                                                            \
			std_static_assert(__std_static_checked_mul_assert_condition(left, right), \
							  "overflow in std_static_checked_mul");                  \
			(left) * (right);                                                         \
		})
#endif // __STD_STATIC_MATH_CHECKED_MUL
