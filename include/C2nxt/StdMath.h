/// @file StdMath.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module extends `<tgmath.h>` and provides some alternative math functions.
/// @version 0.1
/// @date 2021-07-20
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

#include <tgmath.h>

#include "StdBasicTypes.h"

/// @defgroup math Math
/// C2nxt provides some extensions to the C99 type-generic math library and will in the future also
/// provide some additional math-related functionality

#ifndef STD_MATH
	/// @brief Declarations and definitions related to C2nxt math functions
	#define STD_MATH

	/// @brief Returns the minimum of the two arguments
	///
	/// Returns the argument with the minimum value of the two
	///
	/// @param x - the first argument
	/// @param y - the second argument
	///
	/// @return the argument with the minimum value
	/// @ingroup math
	#define std_min(x, y)                                                      \
		({                                                                     \
			let UNIQUE_VAR(_x) = x;                                            \
			let UNIQUE_VAR(_y) = y;                                            \
			UNIQUE_VAR(_x) < UNIQUE_VAR(_y) ? UNIQUE_VAR(_x) : UNIQUE_VAR(_y); \
		})

	/// @brief Returns the minimum of the two arguments
	///
	/// Returns the argument with the minimum value of the two
	///
	/// @param x - the first argument
	/// @param y - the second argument
	///
	/// @return the argument with the minimum value
	/// @ingroup math
	#define min(x, y) std_min((x), (y))

	/// @brief Returns the maximum of the two arguments
	///
	/// Returns the argument with the maximum value of the two
	///
	/// @param x - the first argument
	/// @param y - the second argument
	///
	/// @return the argument with the maximum value
	/// @ingroup math
	#define std_max(x, y)                                                      \
		({                                                                     \
			let UNIQUE_VAR(_x) = x;                                            \
			let UNIQUE_VAR(_y) = y;                                            \
			UNIQUE_VAR(_x) > UNIQUE_VAR(_y) ? UNIQUE_VAR(_x) : UNIQUE_VAR(_y); \
		})

	/// @brief Returns the maximum of the two arguments
	///
	/// Returns the argument with the maximum value of the two
	///
	/// @param x - the first argument
	/// @param y - the second argument
	///
	/// @return the argument with the maximum value
	/// @ingroup math
	#define max(x, y) std_max((x), (y))

	#include "StdPlatform.h"

	#if STD_PLATFORM_APPLE
	// clang-format off
	
	/// @brief Returns the absolute value of the given one
	///
	/// @param x - The value to get the absolute value of
	///
	/// @return the absolute value of `x`
	/// @ingroup math
	#define std_abs(x) _Generic((x), 				\
		u8		:	x,								\
		u16		:	x,								\
		u32		:	x,								\
		u64		:	x,								\
		usize	:	x,								\
		i8		: 	abs(static_cast(i32)(x)),		\
		i16		:	abs(static_cast(i32)(x)),		\
		i32		:	abs(static_cast(i32)(x)),		\
		i64		: 	llabs(static_cast(i64)(x)),		\
		isize	: 	llabs(static_cast(isize)(x)),	\
		f32		:	fabsf(static_cast(f32)(x)),		\
		f64		:	fabs(static_cast(f64)(x)))
	// clang-format on
	#else
		// clang-format off
		/// @brief Returns the absolute value of the given one
		///
		/// @param x - The value to get the absolute value of
		///
		/// @return the absolute value of `x`
		/// @ingroup math
		#define std_abs(x) _Generic((x), 				\
			u8		:	x,								\
			u16		:	x,								\
			u32		:	x,								\
			u64		:	x,								\
			i8		: 	abs(static_cast(i32)(x)),		\
			i16		:	abs(static_cast(i32)(x)),		\
			i32		:	abs(static_cast(i32)(x)),		\
			i64		: 	llabs(static_cast(i64)(x)),		\
			f32		:	fabsf(static_cast(f32)(x)),		\
			f64		:	fabs(static_cast(f64)(x)))
	// clang-format on
	#endif // STD_PLATFORM_APPLE

	/// @brief Returns the absolute value of the given one
	///
	/// @param x - The value to get the absolute value of
	///
	/// @return the absolute value of `x`
	/// @ingroup math
	#define abs(x) std_abs(x)
#endif
