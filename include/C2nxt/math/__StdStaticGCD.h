/// @file __StdStaticGCD.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Compile-time GCD calculation
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

#ifndef STD_STATIC_GCD
#define STD_STATIC_GCD
#include "../StdBasicTypes.h"
#include "../StdPlatform.h"

/// @brief Final iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd1(left, right) ((left) == 0 ? 1 : (left))
/// @brief 19th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd2(left, right) \
	(((right) == 0 && (left) == 0) ?   \
		   1 :                           \
		   ((right) == 0 ? (left) : __std_static_gcd1(right, (left) % (right))))
/// @brief 18th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd3(left, right) \
	(((right) == 0 && (left) == 0) ?   \
		   1 :                           \
		   ((right) == 0 ? (left) : __std_static_gcd2(right, (left) % (right))))
/// @brief 17th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd4(left, right) \
	(((right) == 0 && (left) == 0) ?   \
		   1 :                           \
		   ((right) == 0 ? (left) : __std_static_gcd3(right, (left) % (right))))
/// @brief 16th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd5(left, right) \
	(((right) == 0 && (left) == 0) ?   \
		   1 :                           \
		   ((right) == 0 ? (left) : __std_static_gcd4(right, (left) % (right))))
/// @brief 15th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd6(left, right) \
	(((right) == 0 && (left) == 0) ?   \
		   1 :                           \
		   ((right) == 0 ? (left) : __std_static_gcd5(right, (left) % (right))))
/// @brief 14th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd7(left, right) \
	(((right) == 0 && (left) == 0) ?   \
		   1 :                           \
		   ((right) == 0 ? (left) : __std_static_gcd6(right, (left) % (right))))
/// @brief 13th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd8(left, right) \
	(((right) == 0 && (left) == 0) ?   \
		   1 :                           \
		   ((right) == 0 ? (left) : __std_static_gcd7(right, (left) % (right))))
/// @brief 12th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd9(left, right) \
	(((right) == 0 && (left) == 0) ?   \
		   1 :                           \
		   ((right) == 0 ? (left) : __std_static_gcd8(right, (left) % (right))))
/// @brief 11th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd10(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd9(right, (left) % (right))))
/// @brief 10th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd11(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd10(right, (left) % (right))))
/// @brief 9th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd12(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd11(right, (left) % (right))))
/// @brief 8th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd13(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd12(right, (left) % (right))))
/// @brief 7th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd14(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd13(right, (left) % (right))))
/// @brief 6th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd15(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd14(right, (left) % (right))))
/// @brief 5th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd16(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd15(right, (left) % (right))))
/// @brief 4th iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd17(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd16(right, (left) % (right))))
/// @brief 3rd iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd18(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd17(right, (left) % (right))))
/// @brief 2nd iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd19(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd18(right, (left) % (right))))
/// @brief 1st iteration of static GCD calculation
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
/// @return the GCD of left and right
#define __std_static_gcd20(left, right) \
	(((right) == 0 && (left) == 0) ?    \
		   1 :                            \
		   ((right) == 0 ? (left) : __std_static_gcd19(right, (left) % (right))))

/// @def __std_static_gcd(left, right)
/// @brief Calculates the GCD of left and right
///
/// @param left - The first number of the pair to get the GCD of
/// @param right - The second number of the pair to get the GCD of
///
/// @return The GCD of left and right

// maybe change these to a higher iteration in future
// clang-format off
#if STD_PLATFORM_COMPILER_CLANG
	#define __std_static_gcd(left, right)                             							   \
		_Pragma("GCC diagnostic push")                                							   \
		_Pragma("GCC diagnostic ignored \"-Wdivision-by-zero\"")								   \
		((left) >= (right) ? __std_static_gcd10(left, right) /** NOLINT(bugprone-branch-clone) **/ \
						   : __std_static_gcd10(right, left))  									   \
		_Pragma("GCC diagnostic pop")
#else
	#define __std_static_gcd(left, right)                      									   \
		((left) >= (right) ? __std_static_gcd10(left, right) /** NOLINT(bugprone-branch-clone) **/ \
						   : __std_static_gcd10(right, left))
#endif

// clang-format on
#endif
