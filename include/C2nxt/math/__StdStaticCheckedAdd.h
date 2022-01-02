/// @file __StdStaticCheckedAdd.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Compile-time checked addition function
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

#ifndef __STD_STATIC_MATH_CHECKED_ADD
	#define __STD_STATIC_MATH_CHECKED_ADD

	/// @brief Generates the overflow check condition for the addition of the given arguments
	///
	/// @param left - The left number to add
	/// @param right - The right number to add
	///
	/// @return the overflow check condition
	#define __std_static_checked_add_assert_condition(left, right)                             \
		(__std_static_sign(right) == 1 ?                                                       \
			 ((left) <= __std_checked_funcs_max - (right)) :                                   \
			   (__std_static_sign(right) == -1 ? (__std_checked_funcs_min - (right) <= (left)) : \
												 true))
	/// @brief Performs the checked addition of the two arguments
	///
	/// @param left - The left number to add
	/// @param right - The right number to add
	///
	/// @return the addition of the two arguments
	#define __std_static_checked_add(left, right)                                     \
		({                                                                            \
			std_static_assert(__std_static_checked_add_assert_condition(left, right), \
							  "overflow in std_static_checked_add");                  \
			(left) + (right);                                                         \
		})
#endif // __STD_STATIC_MATH_CHECKED_ADD
