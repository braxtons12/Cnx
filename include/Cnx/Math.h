/// @file Math.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module extends `<tgmath.h>` and provides some alternative math functions.
/// @version 0.2.1
/// @date 2022-04-11
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
#include <Cnx/BasicTypes.h>
#define OPTION_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Option.h>
#undef OPTION_INCLUDE_DEFAULT_INSTANTIATIONS

#include <Cnx/Platform.h>
#include <Cnx/math/__StaticBase.h>
#include <Cnx/math/__StaticCheckedAdd.h>
#include <Cnx/math/__StaticCheckedDiv.h>
#include <Cnx/math/__StaticCheckedMul.h>
#include <Cnx/math/__StaticCheckedSub.h>
#include <Cnx/math/__StaticGCD.h>
#include <math.h>

/// @defgroup math Math
/// Cnx provides some extensions to the C99 type-generic math library and will in the future also
/// provide some additional math-related functionality

#ifndef CNX_MATH
	/// @brief Declarations and definitions related to Cnx math functions
	#define CNX_MATH

	/// @brief Returns the minimum of the two arguments
	///
	/// Returns the argument with the minimum value of the two
	///
	/// @param x - the first argument
	/// @param y - the second argument
	///
	/// @return the argument with the minimum value
	/// @ingroup math
	#define cnx_min(x, y)                                                      \
		({                                                                     \
			let UNIQUE_VAR(_x) = x;                                            \
			let UNIQUE_VAR(_y) = y;                                            \
			UNIQUE_VAR(_x) < UNIQUE_VAR(_y) ? UNIQUE_VAR(_x) : UNIQUE_VAR(_y); \
		})

	#if CNX_PLATFORM_WINDOWS && CNX_PLATFORM_COMPILER_CLANG
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
	#define min(x, y) cnx_min((x), (y))

	/// @brief Returns the maximum of the two arguments
	///
	/// Returns the argument with the maximum value of the two
	///
	/// @param x - the first argument
	/// @param y - the second argument
	///
	/// @return the argument with the maximum value
	/// @ingroup math
	#define cnx_max(x, y)                                                      \
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
	#define max(x, y) cnx_max((x), (y))

	#if CNX_PLATFORM_WINDOWS && CNX_PLATFORM_COMPILER_CLANG
	_Pragma("GCC diagnostic pop")
	#endif

	/// @brief Returns the absolute value of the given one
	///
	/// @param x - The value to get the absolute value of
	///
	/// @return the absolute value of `x`
	/// @ingroup math
	#define cnx_abs(x) __cnx_static_abs(x)

	/// @brief Returns the absolute value of the given one
	///
	/// @param x - The value to get the absolute value of
	///
	/// @return the absolute value of `x`
	/// @ingroup math
	#define abs(x) cnx_abs(x)

	/// @brief Calculates the absolute value of the given input at compile time
	///
	/// @param x - The value to get the absolute value of
	///
	/// @return the absolute value of `x`, as a compile-time constant
	/// @ingroup math
	#define cnx_static_abs(x) __cnx_static_abs(x)

	/// @brief Calculates the sign of the given input at compile time
	///
	/// @param x - The value to get the sign of
	///
	/// @return `-1` if negative, `0` if 0, `1` if positive
	/// @ingroup math
	#define cnx_static_sign(x) __cnx_static_sign(x)

	/// @brief Calculates the sign of the given input
	///
	/// @param x - The value to get the sign of
	///
	/// @return `-1` if negative, `0` if 0, `1` if positive
	/// @ingroup math
	#define cnx_sign(x) cnx_static_sign(x)

	/// @brief Calculate the gcd of the two inputs
	///
	/// @param left - One of the pair of numbers to get the gcd of
	/// @param right - One of the pair of numbers to get the gcd of
	///
	/// @return The gcd of the pair of numbers
	/// @ingroup math
	[[nodiscard]] i64 cnx_gcd_i64(i64 left, i64 right);

/// @brief Calculate the gcd of the two inputs
///
/// @param left - One of the pair of numbers to get the gcd of
/// @param right - One of the pair of numbers to get the gcd of
///
/// @return The gcd of the pair of numbers
/// @ingroup math
[[nodiscard]] u64 cnx_gcd_u64(u64 left, u64 right);

/// @def cnx_gcd(left, right)
/// @brief Calculate the gcd of the two inputs
///
/// @param left - One of the pair of numbers to get the gcd of
/// @param right - One of the pair of numbers to get the gcd of
///
/// @return The gcd of the pair of numbers
/// @ingroup math

	#if CNX_PLATFORM_APPLE
	// clang-format off

		#define cnx_gcd(left, right) _Generic(left,		\
				u8 		: cnx_gcd_u64,					\
				u16		: cnx_gcd_u64,					\
				u32		: cnx_gcd_u64, 					\
				u64 	: cnx_gcd_u64,					\
				usize 	: cnx_gcd_u64,					\
				i8		: cnx_gcd_i64,					\
				i16		: cnx_gcd_i64,					\
				i32		: cnx_gcd_i64,					\
				i64		: cnx_gcd_i64,					\
				isize 	: cnx_gcd_i64)(left, right)
	// clang-format on
	#else
	// clang-format off

		#define cnx_gcd(left, right) _Generic(left,		\
				u8 	: cnx_gcd_u64,						\
				u16 : cnx_gcd_u64,						\
				u32 : cnx_gcd_u64, 						\
				u64 : cnx_gcd_u64,						\
				i8	: cnx_gcd_i64,						\
				i16	: cnx_gcd_i64,						\
				i32	: cnx_gcd_i64,						\
				i64 : cnx_gcd_i64)(left, right)
	// clang-format on
	#endif // CNX_PLATFORM_APPLE

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
	#define cnx_static_gcd(left, right) __cnx_static_gcd(left, right)

/// @brief Calculate the lcm of the two inputs
///
/// @param left - One of the pair of numbers to get the lcm of
/// @param right - One of the pair of numbers to get the lcm of
///
/// @return The lcm of the pair of numbers
/// @ingroup math
[[nodiscard]] i64 cnx_lcm_i64(i64 left, i64 right);
/// @brief Calculate the lcm of the two inputs
///
/// @param left - One of the pair of numbers to get the lcm of
/// @param right - One of the pair of numbers to get the lcm of
///
/// @return The lcm of the pair of numbers
/// @ingroup math
[[nodiscard]] u64 cnx_lcm_u64(u64 left, u64 right);

/// @def cnx_lcm(left, right)
/// @brief Calculate the lcm of the two inputs
///
/// @param left - One of the pair of numbers to get the lcm of
/// @param right - One of the pair of numbers to get the lcm of
///
/// @return The lcm of the pair of numbers
/// @ingroup math

	#if CNX_PLATFORM_APPLE
	// clang-format off

		#define cnx_lcm(left, right) _Generic(left,		\
				u8 		: cnx_lcm_u64,					\
				u16		: cnx_lcm_u64,					\
				u32		: cnx_lcm_u64, 					\
				u64		: cnx_lcm_u64,					\
				usize 	: cnx_lcm_u64,					\
				i8		: cnx_lcm_i64,					\
				i16		: cnx_lcm_i64,					\
				i32		: cnx_lcm_i64,					\
				i64		: cnx_lcm_i64,					\
				isize	: cnx_lcm_i64)(left, right)
	// clang-format on
	#else
	// clang-format off

		#define cnx_lcm(left, right) _Generic(left,		\
				u8 	: cnx_lcm_u64,						\
				u16 : cnx_lcm_u64,						\
				u32 : cnx_lcm_u64, 						\
				u64 : cnx_lcm_u64,						\
				i8	: cnx_lcm_i64,						\
				i16	: cnx_lcm_i64,						\
				i32	: cnx_lcm_i64,						\
				i64 : cnx_lcm_i64)(left, right)
	// clang-format on
	#endif // CNX_PLATFORM_APPLE

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
	#define cnx_static_lcm(left, right) ((left) / __cnx_static_gcd(left, right) * (right))

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
/// @ingroup math
[[nodiscard]] CnxOption(u8) cnx_checked_add_u8(u8 left, u8 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u16) cnx_checked_add_u16(u16 left, u16 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u32) cnx_checked_add_u32(u32 left, u32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u64) cnx_checked_add_u64(u64 left, u64 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i8) cnx_checked_add_i8(i8 left, i8 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i16) cnx_checked_add_i16(i16 left, i16 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i32) cnx_checked_add_i32(i32 left, i32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i64) cnx_checked_add_i64(i64 left, i64 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(f32) cnx_checked_add_f32(f32 left, f32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(f64) cnx_checked_add_f64(f64 left, f64 right);

/// @def Cnx_checked_add(left, right)
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

	#if CNX_PLATFORM_APPLE
		// clang-format off
		#define cnx_checked_add(left, right) _Generic((left), 		\
				u8 		: cnx_checked_add_u8,						\
				u16 	: cnx_checked_add_u16,						\
				u32 	: cnx_checked_add_u32,						\
				u64 	: cnx_checked_add_u64,						\
				usize 	: cnx_checked_add_u64,						\
				i8 		: cnx_checked_add_i8,						\
				i16 	: cnx_checked_add_i16,						\
				i32 	: cnx_checked_add_i32,						\
				i64 	: cnx_checked_add_i64,						\
				isize 	: cnx_checked_add_i64,						\
				f32 	: cnx_checked_add_f32,						\
				f64 	: cnx_checked_add_f64)(left, right)
	// clang-format on
	#else
		// clang-format off
		#define cnx_checked_add(left, right) _Generic((left), 		\
				u8 		: cnx_checked_add_u8,						\
				u16 	: cnx_checked_add_u16,						\
				u32 	: cnx_checked_add_u32,						\
				u64 	: cnx_checked_add_u64,						\
				i8 		: cnx_checked_add_i8,						\
				i16 	: cnx_checked_add_i16,						\
				i32 	: cnx_checked_add_i32,						\
				i64 	: cnx_checked_add_i64,						\
				f32 	: cnx_checked_add_f32,						\
				f64 	: cnx_checked_add_f64)(left, right)
	// clang-format on
	#endif // CNX_PLATFORM_APPLE

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
	#define cnx_static_checked_add(left, right) __cnx_static_checked_add(left, right)

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
/// @ingroup math
[[nodiscard]] CnxOption(u8) cnx_checked_sub_u8(u8 left, u8 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u16) cnx_checked_sub_u16(u16 left, u16 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u32) cnx_checked_sub_u32(u32 left, u32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u64) cnx_checked_sub_u64(u64 left, u64 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i8) cnx_checked_sub_i8(i8 left, i8 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i16) cnx_checked_sub_i16(i16 left, i16 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i32) cnx_checked_sub_i32(i32 left, i32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i64) cnx_checked_sub_i64(i64 left, i64 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(f32) cnx_checked_sub_f32(f32 left, f32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(f64) cnx_checked_sub_f64(f64 left, f64 right);

/// @def cnx_checked_sub(left, right)
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

	#if CNX_PLATFORM_APPLE
	// clang-format off
		#define cnx_checked_sub(left, right) _Generic((left),	\
				u8 		: 	cnx_checked_sub_u8,					\
				u16 	: 	cnx_checked_sub_u16,				\
				u32 	: 	cnx_checked_sub_u32,				\
				u64 	: 	cnx_checked_sub_u64,				\
				usize 	: 	cnx_checked_sub_u64,				\
				i8 		: 	cnx_checked_sub_i8,					\
				i16 	: 	cnx_checked_sub_i16,				\
				i32 	: 	cnx_checked_sub_i32,				\
				i64 	: 	cnx_checked_sub_i64,				\
				isize 	: 	cnx_checked_sub_i64,				\
				f32 	: 	cnx_checked_sub_f32,				\
				f64 	: 	cnx_checked_sub_f64)(left, right)
	// clang-format on
	#else
	// clang-format off

		#define cnx_checked_sub(left, right) _Generic((left),	\
				u8 		: 	cnx_checked_sub_u8,					\
				u16 	: 	cnx_checked_sub_u16,				\
				u32 	: 	cnx_checked_sub_u32,				\
				u64 	: 	cnx_checked_sub_u64,				\
				i8 		: 	cnx_checked_sub_i8,					\
				i16 	: 	cnx_checked_sub_i16,				\
				i32 	: 	cnx_checked_sub_i32,				\
				i64 	: 	cnx_checked_sub_i64,				\
				f32 	: 	cnx_checked_sub_f32,				\
				f64 	: 	cnx_checked_sub_f64)(left, right)
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
	#define cnx_static_checked_sub(left, right) __cnx_static_checked_sub(left, right)

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
/// @ingroup math
[[nodiscard]] CnxOption(u8) cnx_checked_mul_u8(u8 left, u8 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u16) cnx_checked_mul_u16(u16 left, u16 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u32) cnx_checked_mul_u32(u32 left, u32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u64) cnx_checked_mul_u64(u64 left, u64 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i8) cnx_checked_mul_i8(i8 left, i8 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i16) cnx_checked_mul_i16(i16 left, i16 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i32) cnx_checked_mul_i32(i32 left, i32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i64) cnx_checked_mul_i64(i64 left, i64 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(f32) cnx_checked_mul_f32(f32 left, f32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(f64) cnx_checked_mul_f64(f64 left, f64 right);

/// @def cnx_checked_mul(left, right)
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

	#if CNX_PLATFORM_APPLE
		// clang-format off
		#define cnx_checked_mul(left, right) _Generic((left), 		\
				u8 		: cnx_checked_mul_u8,						\
				u16 	: cnx_checked_mul_u16,						\
				u32 	: cnx_checked_mul_u32,						\
				u64 	: cnx_checked_mul_u64,						\
				usize 	: cnx_checked_mul_u64,						\
				i8 		: cnx_checked_mul_i8,						\
				i16 	: cnx_checked_mul_i16,						\
				i32 	: cnx_checked_mul_i32,						\
				i64 	: cnx_checked_mul_i64,						\
				isize 	: cnx_checked_mul_i64,						\
				f32 	: cnx_checked_mul_f32,						\
				f64 	: cnx_checked_mul_f64)(left, right)
	// clang-format on
	#else
		// clang-format off
		#define cnx_checked_mul(left, right) _Generic((left), 		\
				u8 		: cnx_checked_mul_u8,						\
				u16 	: cnx_checked_mul_u16,						\
				u32 	: cnx_checked_mul_u32,						\
				u64 	: cnx_checked_mul_u64,						\
				i8 		: cnx_checked_mul_i8,						\
				i16 	: cnx_checked_mul_i16,						\
				i32 	: cnx_checked_mul_i32,						\
				i64 	: cnx_checked_mul_i64,						\
				f32 	: cnx_checked_mul_f32,						\
				f64 	: cnx_checked_mul_f64)(left, right)
	// clang-format on
	#endif // CNX_PLATFORM_APPLE

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
	#define cnx_static_checked_mul(left, right) __cnx_static_checked_mul(left, right)

/// @brief Performs the checked division of `left` and `right`
///
///
/// @param left - The dividend
/// @param right - The divisor
///
/// @return `Some(u8, result)` if the division was successful (no overflow occurred),
/// `None(u8)` otherwise
/// @ingroup math
[[nodiscard]] CnxOption(u8) cnx_checked_div_u8(u8 left, u8 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u16) cnx_checked_div_u16(u16 left, u16 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u32) cnx_checked_div_u32(u32 left, u32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(u64) cnx_checked_div_u64(u64 left, u64 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i8) cnx_checked_div_i8(i8 left, i8 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i16) cnx_checked_div_i16(i16 left, i16 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i32) cnx_checked_div_i32(i32 left, i32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(i64) cnx_checked_div_i64(i64 left, i64 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(f32) cnx_checked_div_f32(f32 left, f32 right);
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
/// @ingroup math
[[nodiscard]] CnxOption(f64) cnx_checked_div_f64(f64 left, f64 right);

/// @def cnx_checked_div(left, right)
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

	#if CNX_PLATFORM_APPLE
		// clang-format off
		#define cnx_checked_div(left, right) _Generic((left), 		\
				u8 		: cnx_checked_div_u8,						\
				u16 	: cnx_checked_div_u16,						\
				u32 	: cnx_checked_div_u32,						\
				u64 	: cnx_checked_div_u64,						\
				usize 	: cnx_checked_div_u64,						\
				i8 		: cnx_checked_div_i8,						\
				i16 	: cnx_checked_div_i16,						\
				i32 	: cnx_checked_div_i32,						\
				i64 	: cnx_checked_div_i64,						\
				isize 	: cnx_checked_div_i64,						\
				f32 	: cnx_checked_div_f32,						\
				f64 	: cnx_checked_div_f64)(left, right)
	// clang-format on
	#else
		// clang-format off
		#define cnx_checked_div(left, right) _Generic((left), 		\
				u8 		: cnx_checked_div_u8,						\
				u16 	: cnx_checked_div_u16,						\
				u32 	: cnx_checked_div_u32,						\
				u64 	: cnx_checked_div_u64,						\
				i8 		: cnx_checked_div_i8,						\
				i16 	: cnx_checked_div_i16,						\
				i32 	: cnx_checked_div_i32,						\
				i64 	: cnx_checked_div_i64,						\
				f32 	: cnx_checked_div_f32,						\
				f64 	: cnx_checked_div_f64)(left, right)
	// clang-format on
	#endif // CNX_PLATFORM_APPLE

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
	#define cnx_static_checked_div(left, right) __cnx_static_checked_div(left, right)
#endif
