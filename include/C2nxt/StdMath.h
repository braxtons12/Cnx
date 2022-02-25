/// @file StdMath.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module extends `<tgmath.h>` and provides some alternative math functions.
/// @version 0.1.1
/// @date 2022-02-24
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
#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdOption.h>
#include <C2nxt/StdPlatform.h>
#include <C2nxt/math/__StdStaticBase.h>
#include <C2nxt/math/__StdStaticCheckedAdd.h>
#include <C2nxt/math/__StdStaticCheckedDiv.h>
#include <C2nxt/math/__StdStaticCheckedMul.h>
#include <C2nxt/math/__StdStaticCheckedSub.h>
#include <C2nxt/math/__StdStaticGCD.h>
#include <math.h>

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

	#if STD_PLATFORM_WINDOWS
_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wmacro-redefined\"")
	#endif

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

	#if STD_PLATFORM_WINDOWS
	_Pragma("GCC diagnostic pop")
	#endif

	/// @brief Returns the absolute value of the given one
	///
	/// @param x - The value to get the absolute value of
	///
	/// @return the absolute value of `x`
	/// @ingroup math
	#define std_abs(x) __std_static_abs(x)

	/// @brief Returns the absolute value of the given one
	///
	/// @param x - The value to get the absolute value of
	///
	/// @return the absolute value of `x`
	/// @ingroup math
	#define abs(x) std_abs(x)

	/// @brief Calculates the absolute value of the given input at compile time
	///
	/// @param x - The value to get the absolute value of
	///
	/// @return the absolute value of `x`, as a compile-time constant
	/// @ingroup math
	#define std_static_abs(x) __std_static_abs(x)

	/// @brief Calculates the sign of the given input at compile time
	///
	/// @param x - The value to get the sign of
	///
	/// @return `-1` if negative, `0` if 0, `1` if positive
	/// @ingroup math
	#define std_static_sign(x) __std_static_sign(x)

	/// @brief Calculates the sign of the given input
	///
	/// @param x - The value to get the sign of
	///
	/// @return `-1` if negative, `0` if 0, `1` if positive
	/// @ingroup math
	#define std_sign(x) std_static_sign(x)

	/// @brief Calculate the gcd of the two inputs
	///
	/// @param left - One of the pair of numbers to get the gcd of
	/// @param right - One of the pair of numbers to get the gcd of
	///
	/// @return The gcd of the pair of numbers
	i64 std_gcd_i64(i64 left, i64 right);
/// @brief Calculate the gcd of the two inputs
///
/// @param left - One of the pair of numbers to get the gcd of
/// @param right - One of the pair of numbers to get the gcd of
///
/// @return The gcd of the pair of numbers
u64 std_gcd_u64(u64 left, u64 right);

/// @def std_gcd(left, right)
/// @brief Calculate the gcd of the two inputs
///
/// @param left - One of the pair of numbers to get the gcd of
/// @param right - One of the pair of numbers to get the gcd of
///
/// @return The gcd of the pair of numbers
/// @ingroup math

	#if STD_PLATFORM_APPLE
	// clang-format off

		#define std_gcd(left, right) _Generic(left,		\
				u8 		: std_gcd_u64,					\
				u16		: std_gcd_u64,					\
				u32		: std_gcd_u64, 					\
				u64 	: std_gcd_u64,					\
				usize 	: std_gcd_u64,					\
				i8		: std_gcd_i64,					\
				i16		: std_gcd_i64,					\
				i32		: std_gcd_i64,					\
				i64		: std_gcd_i64,					\
				isize 	: std_gcd_i64)(left, right)
	// clang-format on
	#else
	// clang-format off

		#define std_gcd(left, right) _Generic(left,		\
				u8 	: std_gcd_u64,						\
				u16 : std_gcd_u64,						\
				u32 : std_gcd_u64, 						\
				u64 : std_gcd_u64,						\
				i8	: std_gcd_i64,						\
				i16	: std_gcd_i64,						\
				i32	: std_gcd_i64,						\
				i64 : std_gcd_i64)(left, right)
	// clang-format on
	#endif // STD_PLATFORM_APPLE

	/// @brief Calculate the gcd of the two inputs at compile time
	///
	/// @param left - One of the pair of numbers to get the gcd of
	/// @param right - One of the pair of numbers to get the gcd of
	///
	/// @return The gcd of the pair of numbers, as a compile-time constant
	/// @note This algorithm is good for 8 iterations of Euclid's algorithm.
	/// Inputs that would require further iteration will give incorrect results
	/// @note If using clang-tidy, you probably want to disable or suppress the
	/// "readability-function-cognitive-complexity" lint if using this within a function
	/// @ingroup math
	#define std_static_gcd(left, right) __std_static_gcd(left, right)

/// @brief Calculate the lcm of the two inputs
///
/// @param left - One of the pair of numbers to get the lcm of
/// @param right - One of the pair of numbers to get the lcm of
///
/// @return The lcm of the pair of numbers
i64 std_lcm_i64(i64 left, i64 right);
/// @brief Calculate the lcm of the two inputs
///
/// @param left - One of the pair of numbers to get the lcm of
/// @param right - One of the pair of numbers to get the lcm of
///
/// @return The lcm of the pair of numbers
u64 std_lcm_u64(u64 left, u64 right);

/// @def std_lcm(left, right)
/// @brief Calculate the lcm of the two inputs
///
/// @param left - One of the pair of numbers to get the lcm of
/// @param right - One of the pair of numbers to get the lcm of
///
/// @return The lcm of the pair of numbers
/// @ingroup math

	#if STD_PLATFORM_APPLE
	// clang-format off

		#define std_lcm(left, right) _Generic(left,		\
				u8 		: std_lcm_u64,					\
				u16		: std_lcm_u64,					\
				u32		: std_lcm_u64, 					\
				u64		: std_lcm_u64,					\
				usize 	: std_lcm_u64,					\
				i8		: std_lcm_i64,					\
				i16		: std_lcm_i64,					\
				i32		: std_lcm_i64,					\
				i64		: std_lcm_i64,					\
				isize	: std_lcm_i64)(left, right)
	// clang-format on
	#else
	// clang-format off

		#define std_lcm(left, right) _Generic(left,		\
				u8 	: std_lcm_u64,						\
				u16 : std_lcm_u64,						\
				u32 : std_lcm_u64, 						\
				u64 : std_lcm_u64,						\
				i8	: std_lcm_i64,						\
				i16	: std_lcm_i64,						\
				i32	: std_lcm_i64,						\
				i64 : std_lcm_i64)(left, right)
	// clang-format on
	#endif // STD_PLATFORM_APPLE

	/// @brief Calculate the lcm of the two inputs at compile time
	///
	/// @param left - One of the pair of numbers to get the lcm of
	/// @param right - One of the pair of numbers to get the lcm of
	///
	/// @return The lcm of the pair of numbers, as a compile-time constant
	/// @note This algorithm is good for 8 iterations of Euclid's algorithm.
	/// Inputs that would require further iteration will give incorrect results
	/// @note If using clang-tidy, you probably want to disable or suppress the
	/// "readability-function-cognitive-complexity" lint if using this within a function
	/// @ingroup math
	#define std_static_lcm(left, right) ((left) / __std_static_gcd(left, right) * (right))

/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(u8, result)`
/// if overflow did __not__ occur or `None(u8)` if overflow occurred
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(u8, result)` if the addition was successful (no overflow occurred), `None(u8)`
/// otherwise
StdOption(u8) std_checked_add_u8(u8 left, u8 right);
/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(u16, result)`
/// if overflow did __not__ occur or `None(u16)` if overflow occurred
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(u16, result)` if the addition was successful (no overflow occurred), `None(u16)`
/// otherwise
StdOption(u16) std_checked_add_u16(u16 left, u16 right);
/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(u32, result)`
/// if overflow did __not__ occur or `None(u32)` if overflow occurred
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(u32, result)` if the addition was successful (no overflow occurred), `None(u32)`
/// otherwise
StdOption(u32) std_checked_add_u32(u32 left, u32 right);
/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(u64, result)`
/// if overflow did __not__ occur or `None(u64)` if overflow occurred
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(u64, result)` if the addition was successful (no overflow occurred), `None(u64)`
/// otherwise
StdOption(u64) std_checked_add_u64(u64 left, u64 right);
/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(i8, result)`
/// if overflow did __not__ occur or `None(i8)` if overflow occurred
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(i8, result)` if the addition was successful (no overflow occurred), `None(i8)`
/// otherwise
StdOption(i8) std_checked_add_i8(i8 left, i8 right);
/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(i16, result)`
/// if overflow did __not__ occur or `None(i16)` if overflow occurred
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(i16, result)` if the addition was successful (no overflow occurred), `None(i16)`
/// otherwise
StdOption(i16) std_checked_add_i16(i16 left, i16 right);
/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(i32, result)`
/// if overflow did __not__ occur or `None(i32)` if overflow occurred
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(i32, result)` if the addition was successful (no overflow occurred), `None(i32)`
/// otherwise
StdOption(i32) std_checked_add_i32(i32 left, i32 right);
/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(i64, result)`
/// if overflow did __not__ occur or `None(i64)` if overflow occurred
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(i64, result)` if the addition was successful (no overflow occurred), `None(i64)`
/// otherwise
StdOption(i64) std_checked_add_i64(i64 left, i64 right);
/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(f32, result)`
/// if overflow did __not__ occur or `None(f32)` if overflow occurred
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(f32, result)` if the addition was successful (no overflow occurred), `None(f32)`
/// otherwise
StdOption(f32) std_checked_add_f32(f32 left, f32 right);
/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(f64, result)`
/// if overflow did __not__ occur or `None(f64)` if overflow occurred
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(f64, result)` if the addition was successful (no overflow occurred), `None(f64)`
/// otherwise
StdOption(f64) std_checked_add_f64(f64 left, f64 right);

/// @def std_checked_add(left, right)
/// @brief Performs the checked addition of `left` and `right`
///
/// Performs the checked addition of `left` and `right`, returning `Some(T, result)`
/// if overflow did __not__ occur or `None(T)` if overflow occurred, where `T` is the type
/// of the passed arguments
///
/// @param left - The left-hand number of the addition
/// @param right - The right-hand number of the addition
///
/// @return `Some(T, result)` if the addition was successful (no overflow occurred), `None(T)`
/// otherwise
/// @ingroup math

	#if STD_PLATFORM_APPLE
		// clang-format off
		#define std_checked_add(left, right) _Generic((left), 		\
				u8 		: std_checked_add_u8,						\
				u16 	: std_checked_add_u16,						\
				u32 	: std_checked_add_u32,						\
				u64 	: std_checked_add_u64,						\
				usize 	: std_checked_add_u64,						\
				i8 		: std_checked_add_i8,						\
				i16 	: std_checked_add_i16,						\
				i32 	: std_checked_add_i32,						\
				i64 	: std_checked_add_i64,						\
				isize 	: std_checked_add_i64,						\
				f32 	: std_checked_add_f32,						\
				f64 	: std_checked_add_f64)(left, right)
	// clang-format on
	#else
		// clang-format off
		#define std_checked_add(left, right) _Generic((left), 		\
				u8 		: std_checked_add_u8,						\
				u16 	: std_checked_add_u16,						\
				u32 	: std_checked_add_u32,						\
				u64 	: std_checked_add_u64,						\
				i8 		: std_checked_add_i8,						\
				i16 	: std_checked_add_i16,						\
				i32 	: std_checked_add_i32,						\
				i64 	: std_checked_add_i64,						\
				f32 	: std_checked_add_f32,						\
				f64 	: std_checked_add_f64)(left, right)
	// clang-format on
	#endif // STD_PLATFORM_APPLE

	/// @brief Performs the checked addition of `left` and `right` at compile-time
	///
	/// Performs the addition of `left` and `right` at compile-time, checking for overflow and
	/// triggering a compiler error if overflow occurs
	///
	/// @param left - The left integer to add
	/// @param right - The right integer to add
	///
	/// @return sum of `left` and `right`
	/// @ingroup math
	#define std_static_checked_add(left, right) __std_static_checked_add(left, right)

/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(u8, result)`
/// if overflow did __not__ occur or `None(u8)` if overflow occurred
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(u8, result)` if the subtraction was successful (no overflow occurred), `None(u8)`
/// otherwise
StdOption(u8) std_checked_sub_u8(u8 left, u8 right);
/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(u16, result)`
/// if overflow did __not__ occur or `None(u16)` if overflow occurred
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(u16, result)` if the subtraction was successful (no overflow occurred),
/// `None(u16)` otherwise
StdOption(u16) std_checked_sub_u16(u16 left, u16 right);
/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(u32, result)`
/// if overflow did __not__ occur or `None(u32)` if overflow occurred
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(u32, result)` if the subtraction was successful (no overflow occurred),
/// `None(u32)` otherwise
StdOption(u32) std_checked_sub_u32(u32 left, u32 right);
/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(u64, result)`
/// if overflow did __not__ occur or `None(u64)` if overflow occurred
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(u64, result)` if the subtraction was successful (no overflow occurred),
/// `None(u64)` otherwise
StdOption(u64) std_checked_sub_u64(u64 left, u64 right);
/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(i8, result)`
/// if overflow did __not__ occur or `None(i8)` if overflow occurred
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(i8, result)` if the subtraction was successful (no overflow occurred), `None(i8)`
/// otherwise
StdOption(i8) std_checked_sub_i8(i8 left, i8 right);
/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(i16, result)`
/// if overflow did __not__ occur or `None(i16)` if overflow occurred
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(i16, result)` if the subtraction was successful (no overflow occurred),
/// `None(i16)` otherwise
StdOption(i16) std_checked_sub_i16(i16 left, i16 right);
/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(i32, result)`
/// if overflow did __not__ occur or `None(i32)` if overflow occurred
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(i32, result)` if the subtraction was successful (no overflow occurred),
/// `None(i32)` otherwise
StdOption(i32) std_checked_sub_i32(i32 left, i32 right);
/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(i64, result)`
/// if overflow did __not__ occur or `None(i64)` if overflow occurred
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(i64, result)` if the subtraction was successful (no overflow occurred),
/// `None(i64)` otherwise
StdOption(i64) std_checked_sub_i64(i64 left, i64 right);
/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(f32, result)`
/// if overflow did __not__ occur or `None(f32)` if overflow occurred
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(f32, result)` if the subtraction was successful (no overflow occurred),
/// `None(f32)` otherwise
StdOption(f32) std_checked_sub_f32(f32 left, f32 right);
/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(f64, result)`
/// if overflow did __not__ occur or `None(f64)` if overflow occurred
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(f64, result)` if the subtraction was successful (no overflow occurred),
/// `None(f64)` otherwise
StdOption(f64) std_checked_sub_f64(f64 left, f64 right);

/// @def std_checked_sub(left, right)
/// @brief Performs the checked subtraction of `right` from `left`
///
/// Performs the checked subtraction of `right` from `left`, returning `Some(T, result)`
/// if overflow did __not__ occur or `None(T)` if overflow occurred, where `T` is the type
/// of the passed arguments
///
/// @param left - The number to subtract from
/// @param right - The number to subtract
///
/// @return `Some(T, result)` if the subtraction was successful (no overflow occurred), `None(T)`
/// otherwise
/// @ingroup math

	#if STD_PLATFORM_APPLE
	// clang-format off
		#define std_checked_sub(left, right) _Generic((left),	\
				u8 		: 	std_checked_sub_u8,					\
				u16 	: 	std_checked_sub_u16,				\
				u32 	: 	std_checked_sub_u32,				\
				u64 	: 	std_checked_sub_u64,				\
				usize 	: 	std_checked_sub_u64,				\
				i8 		: 	std_checked_sub_i8,					\
				i16 	: 	std_checked_sub_i16,				\
				i32 	: 	std_checked_sub_i32,				\
				i64 	: 	std_checked_sub_i64,				\
				isize 	: 	std_checked_sub_i64,				\
				f32 	: 	std_checked_sub_f32,				\
				f64 	: 	std_checked_sub_f64)(left, right)
	// clang-format on
	#else
	// clang-format off

		#define std_checked_sub(left, right) _Generic((left),	\
				u8 		: 	std_checked_sub_u8,					\
				u16 	: 	std_checked_sub_u16,				\
				u32 	: 	std_checked_sub_u32,				\
				u64 	: 	std_checked_sub_u64,				\
				i8 		: 	std_checked_sub_i8,					\
				i16 	: 	std_checked_sub_i16,				\
				i32 	: 	std_checked_sub_i32,				\
				i64 	: 	std_checked_sub_i64,				\
				f32 	: 	std_checked_sub_f32,				\
				f64 	: 	std_checked_sub_f64)(left, right)
	// clang-format on
	#endif

	/// @brief Performs the checked subtraction of `right` from `left` at compile-time
	///
	/// Performs the subtraction of `right` from `left` at compile-time, checking for overflow and
	/// triggering a compiler error if overflow occurs
	///
	/// @param left - The integer to subtract from
	/// @param right - The integer to subtract
	///
	/// @return subtraction of `right` from `left`
	/// @ingroup math
	#define std_static_checked_sub(left, right) __std_static_checked_sub(left, right)

/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(u8, result)`
/// if overflow did __not__ occur or `None(u8)` if overflow occurred
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(u8, result)` if the multiplication was successful (no overflow occurred),
/// `None(u8)` otherwise
StdOption(u8) std_checked_mul_u8(u8 left, u8 right);
/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(u16, result)`
/// if overflow did __not__ occur or `None(u16)` if overflow occurred
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(u16, result)` if the multiplication was successful (no overflow occurred),
/// `None(u16)` otherwise
StdOption(u16) std_checked_mul_u16(u16 left, u16 right);
/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(u32, result)`
/// if overflow did __not__ occur or `None(u32)` if overflow occurred
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(u32, result)` if the multiplication was successful (no overflow occurred),
/// `None(u32)` otherwise
StdOption(u32) std_checked_mul_u32(u32 left, u32 right);
/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(u64, result)`
/// if overflow did __not__ occur or `None(u64)` if overflow occurred
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(u64, result)` if the multiplication was successful (no overflow occurred),
/// `None(u64)` otherwise
StdOption(u64) std_checked_mul_u64(u64 left, u64 right);
/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(i8, result)`
/// if overflow did __not__ occur or `None(i8)` if overflow occurred
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(i8, result)` if the multiplication was successful (no overflow occurred),
/// `None(i8)` otherwise
StdOption(i8) std_checked_mul_i8(i8 left, i8 right);
/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(i16, result)`
/// if overflow did __not__ occur or `None(i16)` if overflow occurred
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(i16, result)` if the multiplication was successful (no overflow occurred),
/// `None(i16)` otherwise
StdOption(i16) std_checked_mul_i16(i16 left, i16 right);
/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(i32, result)`
/// if overflow did __not__ occur or `None(i32)` if overflow occurred
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(i32, result)` if the multiplication was successful (no overflow occurred),
/// `None(i32)` otherwise
StdOption(i32) std_checked_mul_i32(i32 left, i32 right);
/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(i64, result)`
/// if overflow did __not__ occur or `None(i64)` if overflow occurred
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(i64, result)` if the multiplication was successful (no overflow occurred),
/// `None(i64)` otherwise
StdOption(i64) std_checked_mul_i64(i64 left, i64 right);
/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(f32, result)`
/// if overflow did __not__ occur or `None(f32)` if overflow occurred
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(f32, result)` if the multiplication was successful (no overflow occurred),
/// `None(f32)` otherwise
StdOption(f32) std_checked_mul_f32(f32 left, f32 right);
/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(f64, result)`
/// if overflow did __not__ occur or `None(f64)` if overflow occurred
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(f64, result)` if the multiplication was successful (no overflow occurred),
/// `None(f64)` otherwise
StdOption(f64) std_checked_mul_f64(f64 left, f64 right);

/// @def std_checked_mul(left, right)
/// @brief Performs the checked multiplication of `left` and `right`
///
/// Performs the checked multiplication of `left` and `right`, returning `Some(T, result)`
/// if overflow did __not__ occur or `None(T)` if overflow occurred, where `T` is the type
/// of the passed arguments
///
/// @param left - The left-hand number of the multiplication
/// @param right - The right-hand number of the multiplication
///
/// @return `Some(T, result)` if the multiplication was successful (no overflow occurred), `None(T)`
/// otherwise
/// @ingroup math

	#if STD_PLATFORM_APPLE
		// clang-format off
		#define std_checked_mul(left, right) _Generic((left), 		\
				u8 		: std_checked_mul_u8,						\
				u16 	: std_checked_mul_u16,						\
				u32 	: std_checked_mul_u32,						\
				u64 	: std_checked_mul_u64,						\
				usize 	: std_checked_mul_u64,						\
				i8 		: std_checked_mul_i8,						\
				i16 	: std_checked_mul_i16,						\
				i32 	: std_checked_mul_i32,						\
				i64 	: std_checked_mul_i64,						\
				isize 	: std_checked_mul_i64,						\
				f32 	: std_checked_mul_f32,						\
				f64 	: std_checked_mul_f64)(left, right)
	// clang-format on
	#else
		// clang-format off
		#define std_checked_mul(left, right) _Generic((left), 		\
				u8 		: std_checked_mul_u8,						\
				u16 	: std_checked_mul_u16,						\
				u32 	: std_checked_mul_u32,						\
				u64 	: std_checked_mul_u64,						\
				i8 		: std_checked_mul_i8,						\
				i16 	: std_checked_mul_i16,						\
				i32 	: std_checked_mul_i32,						\
				i64 	: std_checked_mul_i64,						\
				f32 	: std_checked_mul_f32,						\
				f64 	: std_checked_mul_f64)(left, right)
	// clang-format on
	#endif // STD_PLATFORM_APPLE

	/// @brief Performs the checked multiplication of `left` and `right` at compile-time
	///
	/// Performs the multiplication of `left` and `right` at compile-time, checking for overflow and
	/// triggering a compiler error if overflow occurs
	///
	/// @param left - The left integer to multiply
	/// @param right - The right integer to multiply
	///
	/// @return product of `left` and `right`
	/// @ingroup math
	#define std_static_checked_mul(left, right) __std_static_checked_mul(left, right)

/// @brief Performs the checked division of `left` and `right`
///
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(u8, result)` if the division was successful (no overflow occurred),
/// `None(u8)` otherwise
StdOption(u8) std_checked_div_u8(u8 left, u8 right);
/// @brief Performs the checked division of `left` and `right`
///
/// Performs the checked division of `left` and `right`, returning `Some(u16, result)`
/// if overflow did __not__ occur or `None(u16)` if overflow occurred
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(u16, result)` if the division was successful (no overflow occurred),
/// `None(u16)` otherwise
StdOption(u16) std_checked_div_u16(u16 left, u16 right);
/// @brief Performs the checked division of `left` and `right`
///
/// Performs the checked division of `left` and `right`, returning `Some(u32, result)`
/// if overflow did __not__ occur or `None(u32)` if overflow occurred
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(u32, result)` if the division was successful (no overflow occurred),
/// `None(u32)` otherwise
StdOption(u32) std_checked_div_u32(u32 left, u32 right);
/// @brief Performs the checked division of `left` and `right`
///
/// Performs the checked division of `left` and `right`, returning `Some(u64, result)`
/// if overflow did __not__ occur or `None(u64)` if overflow occurred
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(u64, result)` if the division was successful (no overflow occurred),
/// `None(u64)` otherwise
StdOption(u64) std_checked_div_u64(u64 left, u64 right);
/// @brief Performs the checked division of `left` and `right`
///
/// Performs the checked division of `left` and `right`, returning `Some(i8, result)`
/// if overflow did __not__ occur or `None(i8)` if overflow occurred
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(i8, result)` if the division was successful (no overflow occurred),
/// `None(i8)` otherwise
StdOption(i8) std_checked_div_i8(i8 left, i8 right);
/// @brief Performs the checked division of `left` and `right`
///
/// Performs the checked division of `left` and `right`, returning `Some(i16, result)`
/// if overflow did __not__ occur or `None(i16)` if overflow occurred
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(i16, result)` if the division was successful (no overflow occurred),
/// `None(i16)` otherwise
StdOption(i16) std_checked_div_i16(i16 left, i16 right);
/// @brief Performs the checked division of `left` and `right`
///
/// Performs the checked division of `left` and `right`, returning `Some(i32, result)`
/// if overflow did __not__ occur or `None(i32)` if overflow occurred
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(i32, result)` if the division was successful (no overflow occurred),
/// `None(i32)` otherwise
StdOption(i32) std_checked_div_i32(i32 left, i32 right);
/// @brief Performs the checked division of `left` and `right`
///
/// Performs the checked division of `left` and `right`, returning `Some(i64, result)`
/// if overflow did __not__ occur or `None(i64)` if overflow occurred
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(i64, result)` if the division was successful (no overflow occurred),
/// `None(i64)` otherwise
StdOption(i64) std_checked_div_i64(i64 left, i64 right);
/// @brief Performs the checked division of `left` and `right`
///
/// Performs the checked division of `left` and `right`, returning `Some(f32, result)`
/// if overflow did __not__ occur or `None(f32)` if overflow occurred
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(f32, result)` if the division was successful (no overflow occurred),
/// `None(f32)` otherwise
StdOption(f32) std_checked_div_f32(f32 left, f32 right);
/// @brief Performs the checked division of `left` and `right`
///
/// Performs the checked division of `left` and `right`, returning `Some(f64, result)`
/// if overflow did __not__ occur or `None(f64)` if overflow occurred
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(f64, result)` if the division was successful (no overflow occurred),
/// `None(f64)` otherwise
StdOption(f64) std_checked_div_f64(f64 left, f64 right);

/// @def std_checked_div(left, right)
/// @brief Performs the checked division of `left` and `right`
///
/// Performs the checked division of `left` and `right`, returning `Some(T, result)`
/// if overflow did __not__ occur or `None(T)` if overflow occurred, where `T` is the type
/// of the passed arguments
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(T, result)` if the division was successful (no overflow occurred), `None(T)`
/// otherwise
/// @ingroup math

	#if STD_PLATFORM_APPLE
		// clang-format off
		#define std_checked_div(left, right) _Generic((left), 		\
				u8 		: std_checked_div_u8,						\
				u16 	: std_checked_div_u16,						\
				u32 	: std_checked_div_u32,						\
				u64 	: std_checked_div_u64,						\
				usize 	: std_checked_div_u64,						\
				i8 		: std_checked_div_i8,						\
				i16 	: std_checked_div_i16,						\
				i32 	: std_checked_div_i32,						\
				i64 	: std_checked_div_i64,						\
				isize 	: std_checked_div_i64,						\
				f32 	: std_checked_div_f32,						\
				f64 	: std_checked_div_f64)(left, right)
	// clang-format on
	#else
		// clang-format off
		#define std_checked_div(left, right) _Generic((left), 		\
				u8 		: std_checked_div_u8,						\
				u16 	: std_checked_div_u16,						\
				u32 	: std_checked_div_u32,						\
				u64 	: std_checked_div_u64,						\
				i8 		: std_checked_div_i8,						\
				i16 	: std_checked_div_i16,						\
				i32 	: std_checked_div_i32,						\
				i64 	: std_checked_div_i64,						\
				f32 	: std_checked_div_f32,						\
				f64 	: std_checked_div_f64)(left, right)
	// clang-format on
	#endif // STD_PLATFORM_APPLE

	/// @brief Performs the checked division of `left` and `right` at compile-time
	///
	/// Performs the division of `left` and `right` at compile-time, checking for overflow and
	/// triggering a compiler error if overflow occurs
	///
	/// @param left - The integer dividend
	/// @param right - The integer divisor
	///
	/// @return quotient of `left / right`
	/// @ingroup math
	#define std_static_checked_div(left, right) __std_static_checked_div(left, right)
#endif
