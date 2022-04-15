/// @file PPMath.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief PPMath provides metaprogramming macros for operating on/with numeric values
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

#include "Cnx/mpl/PPStrings.h"

/// @defgroup cnx_pp_math Numeric Metaprogramming
/// This module provides various macros for metaprogramming on/with numeric values

#ifndef CNX_PP_MATH
	#define CNX_PP_MATH

	/// @brief Adds 1 to 0, returning the result as a single token
	#define ___ADD1_0 1
	/// @brief Adds 1 to 1, returning the result as a single token
	#define ___ADD1_1 2
	/// @brief Adds 1 to 2, returning the result as a single token
	#define ___ADD1_2 3
	/// @brief Adds 1 to 3, returning the result as a single token
	#define ___ADD1_3 4
	/// @brief Adds 1 to 4, returning the result as a single token
	#define ___ADD1_4 5
	/// @brief Adds 1 to 5, returning the result as a single token
	#define ___ADD1_5 6
	/// @brief Adds 1 to 6, returning the result as a single token
	#define ___ADD1_6 7
	/// @brief Adds 1 to 7, returning the result as a single token
	#define ___ADD1_7 8
	/// @brief Adds 1 to 8, returning the result as a single token
	#define ___ADD1_8 9
	/// @brief Adds 1 to 9, returning the result as a single token
	#define ___ADD1_9 10
	/// @brief Adds 1 to 10, returning the result as a single token
	#define ___ADD1_10 11
	/// @brief Adds 1 to 11, returning the result as a single token
	#define ___ADD1_11 12
	/// @brief Adds 1 to 12, returning the result as a single token
	#define ___ADD1_12 13

	/// @brief Adds 1 to the given value, returning the result as a single token
	///
	/// @param N - The number to increment
	///
	/// @return The sum, `N` + 1, as a single token
	/// @ingroup cnx_pp_math
	#define ADD1(N) CONCAT2_DEFERRED(___ADD1_, N)

	/// @brief Subtracts 1 from 0, returning the result as a single token
	#define ___SUBTRACT1_0() 0
	/// @brief Subtracts 1 from 1, returning the result as a single token
	#define ___SUBTRACT1_1() 0
	/// @brief Subtracts 1 from 2, returning the result as a single token
	#define ___SUBTRACT1_2() 1
	/// @brief Subtracts 1 from 3, returning the result as a single token
	#define ___SUBTRACT1_3() 2
	/// @brief Subtracts 1 from 4, returning the result as a single token
	#define ___SUBTRACT1_4() 3
	/// @brief Subtracts 1 from 5, returning the result as a single token
	#define ___SUBTRACT1_5() 4
	/// @brief Subtracts 1 from 6, returning the result as a single token
	#define ___SUBTRACT1_6() 5
	/// @brief Subtracts 1 from 7, returning the result as a single token
	#define ___SUBTRACT1_7() 6
	/// @brief Subtracts 1 from 8, returning the result as a single token
	#define ___SUBTRACT1_8() 7
	/// @brief Subtracts 1 from 9, returning the result as a single token
	#define ___SUBTRACT1_9() 8
	/// @brief Subtracts 1 from 10, returning the result as a single token
	#define ___SUBTRACT1_10() 9
	/// @brief Subtracts 1 from 11, returning the result as a single token
	#define ___SUBTRACT1_11() 10
	/// @brief Subtracts 1 from 12, returning the result as a single token
	#define ___SUBTRACT1_12() 11

	/// @brief Subtracts 1 from the given value, returning the result as a single token
	///
	/// @param N - The number to decrement
	///
	/// @return The difference, `N` - 1, as a single token
	/// @ingroup cnx_pp_math
	#define SUBTRACT1(N) CONCAT2_DEFERRED(___SUBTRACT1_, N)()

#endif // CNX_PP_MATH
