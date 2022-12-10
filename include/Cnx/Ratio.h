/// @file Ratio.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides lossless methods for dealing with exact fractions
/// @version 0.1.3
/// @date 2022-12-09
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
#include <Cnx/Format.h>
#include <Cnx/Math.h>

/// @defgroup cnx_ratio CnxRatio
/// `CnxRatio` provides lossless methods for dealing with exact fractions
///
/// Example:
/// @code {./c}
/// let time_seconds = 1000;
/// let time_milliseconds_ratio = cnx_ratio_multiply(cnx_milli, time_seconds);
/// // try to get the scalar value as an integer if it can be represented by one
/// let maybe_exact_milliseconds = cnx_ratio_as_scalar_exact(time_milliseconds_ratio);
/// let exact = cnx_option_unwrap(maybe_exact);
/// // get the scalar value as an f64
/// let milliseconds_f64 = cnx_ratio_as_scalar(time_milliseconds);
/// @endcode

#ifndef CNX_RATIO
	/// @brief Declarations related to `CnxRatio`
	#define CNX_RATIO

/// @brief `CnxRatio` is a representation of an exact fraction
/// @ingroup cnx_ratio
typedef struct CnxRatio {
	/// @brief The numerator of the ratio
	i64 num;
	/// @brief The denominator of the ratio
	i64 den;
} CnxRatio;

	/// @brief Constructs a `CnxRatio` at compile time
	///
	/// Constructs a `CnxRatio` from the given numerator and denominator, as a simplified fraction
	/// at compile time.
	///
	/// @param numerator - The numerator of the ratio
	/// @param denominator - The denominator of the ratio
	///
	/// @return A `CnxRatio` as the simplified fraction of `numerator / denominator`
	/// @ingroup cnx_ratio
	#define cnx_ratio(numerator, denominator)                                                  \
		((CnxRatio){                                                                           \
			.num = (cnx_static_sign(numerator) * cnx_static_sign(denominator)                  \
					* cnx_static_abs(numerator)                                                \
					/ cnx_static_gcd(cnx_static_abs(numerator), cnx_static_abs(denominator))), \
			.den = (cnx_static_abs(denominator)                                                \
					/ cnx_static_gcd(cnx_static_abs(numerator), cnx_static_abs(denominator)))})

	/// @brief SI "atto" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_atto \
		(CnxRatio) { .num = 1LL, .den = 1000000000000000000LL }
	/// @brief SI "femto" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_femto \
		(CnxRatio) { .num = 1LL, .den = 1000000000000000LL }
	/// @brief SI "pico" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_pico \
		(CnxRatio) { .num = 1LL, .den = 1000000000000LL }
	/// @brief SI "nano" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_nano \
		(CnxRatio) { .num = 1LL, .den = 1000000000LL }
	/// @brief SI "micro" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_micro \
		(CnxRatio) { .num = 1LL, .den = 1000000LL }
	/// @brief SI "milli" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_milli \
		(CnxRatio) { .num = 1LL, .den = 1000LL }
	/// @brief SI "centi" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_centi \
		(CnxRatio) { .num = 1LL, .den = 100LL }
	/// @brief SI "deci" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_deci \
		(CnxRatio) { .num = 1LL, .den = 10LL }
	/// @brief SI "deca" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_deca \
		(CnxRatio) { .num = 10LL, .den = 1LL }
	/// @brief SI "hecto" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_hecto \
		(CnxRatio) { .num = 100LL, .den = 1LL }
	/// @brief SI "kilo" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_kilo \
		(CnxRatio) { .num = 1000LL, .den = 1LL }
	/// @brief SI "mega" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_mega \
		(CnxRatio) { .num = 1000000LL, .den = 1LL }
	/// @brief SI "giga" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_giga \
		(CnxRatio) { .num = 1000000000LL, .den = 1LL }
	/// @brief SI "tera" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_tera \
		(CnxRatio) { .num = 1000000000000LL, .den = 1LL }
	/// @brief SI "peta" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_peta \
		(CnxRatio) { .num = 1000000000000000LL, .den = 1LL }
	/// @brief SI "exa" as a `CnxRatio`
	/// @ingroup cnx_ratio
	#define cnx_exa \
		(CnxRatio) { .num = 1000000000000000000LL, .den = 1LL }

/// @brief Constructs a `CnxRatio`
///
/// Constructs a `CnxRatio` from the given numerator and denominator, as a simplified fraction
///
/// @param num - The numerator of the ratio
/// @param den - The denominator of the ratio
///
/// @return A `CnxRatio` as the simplified fraction of `numerator / denominator`
/// @ingroup cnx_ratio
__attr(nodiscard) CnxRatio cnx_ratio_new(i64 num, i64 den);

/// @brief Adds the given `CnxRatio`s
///
/// Adds the given `CnxRatio`s, returning the result as a new `CnxRatio` in simplest form
///
/// @param lhs - The left-hand ratio to add
/// @param rhs - The right-hand ratio to add
///
/// @return The sum of `lhs` and `rhs` as a `CnxRatio` in simplest form
/// @ingroup cnx_ratio
__attr(nodiscard) CnxRatio cnx_ratio_add(CnxRatio lhs, CnxRatio rhs);
/// @brief Subtracts the given `CnxRatio`s
///
/// Subtracts `rhs` from `lhs`, returning the result as a new `CnxRatio` in simplest form
///
/// @param lhs - The left-hand ratio to subtract from
/// @param rhs - The right-hand ratio to subtract
///
/// @return The difference of `lhs` and `rhs` as a `CnxRatio` in simplest form
/// @ingroup cnx_ratio
__attr(nodiscard) CnxRatio cnx_ratio_subtract(CnxRatio lhs, CnxRatio rhs);
/// @brief Multiplies the given `CnxRatio`s
///
/// Multiplies the given `CnxRatio`s, returning the result as a new `CnxRatio` in simplest form
///
/// @param lhs - The left-hand ratio to multiply
/// @param rhs - The right-hand ratio to multiply
///
/// @return The product of `lhs` and `rhs` as a `CnxRatio` in simplest form
/// @ingroup cnx_ratio
__attr(nodiscard) CnxRatio cnx_ratio_multiply(CnxRatio lhs, CnxRatio rhs);
/// @brief Divides the given `CnxRatio`s
///
/// Divides `lhs` by `rhs`, returning the result as a new `CnxRatio` in simplest form
///
/// @param lhs - The `CnxRatio` dividend
/// @param rhs - The `CnxRatio` divisor
///
/// @return The quotient of `lhs` and `rhs` as a `CnxRatio` in simplest form
/// @ingroup cnx_ratio
__attr(nodiscard) CnxRatio cnx_ratio_divide(CnxRatio lhs, CnxRatio rhs);
/// @brief Returns whether the two `CnxRatio`s are equal
///
/// @param lhs - The left-hand ratio to check for equality
/// @param rhs - The right-hand ratio to check for equality
///
/// @return Whether the given ratios are equal
/// @note This assumes that the given ratios are in simplest form. It does not check if either
/// ratio has not been reduced.
/// @ingroup cnx_ratio
__attr(nodiscard) bool cnx_ratio_equal(CnxRatio lhs, CnxRatio rhs);
/// @brief Returns whether the two `CnxRatio`s are __not__ equal
///
/// @param lhs - The left-hand ratio to check for inequality
/// @param rhs - The right-hand ratio to check for inequality
///
/// @return Whether the given ratios are __not__ equal
/// @note This assumes that the given ratios are in simplest form. It does not check if either
/// ratio has not been reduced.
/// @ingroup cnx_ratio
__attr(nodiscard) bool cnx_ratio_not_equal(CnxRatio lhs, CnxRatio rhs);
/// @brief Returns whether the left-hand `CnxRatio` is less than the other
///
/// @param lhs - The ratio to compare
/// @param rhs - The ratio to compare against
///
/// @return Whether `lhs` is less than `rhs`
/// @ingroup cnx_ratio
__attr(nodiscard) bool cnx_ratio_less_than(CnxRatio lhs, CnxRatio rhs);
/// @brief Returns whether the left-hand `CnxRatio` is less than or equal to the other
///
/// @param lhs - The ratio to compare
/// @param rhs - The ratio to compare against
///
/// @return Whether `lhs` is less than or equal to `rhs`
/// @ingroup cnx_ratio
__attr(nodiscard) bool cnx_ratio_less_than_or_equal(CnxRatio lhs, CnxRatio rhs);
/// @brief Returns whether the left-hand `CnxRatio` is greater than the other
///
/// @param lhs - The ratio to compare
/// @param rhs - The ratio to compare against
///
/// @return Whether `lhs` is greater than `rhs`
/// @ingroup cnx_ratio
__attr(nodiscard) bool cnx_ratio_greater_than(CnxRatio lhs, CnxRatio rhs);
/// @brief Returns whether the left-hand `CnxRatio` is greater than or equal to the other
///
/// @param lhs - The ratio to compare
/// @param rhs - The ratio to compare against
///
/// @return Whether `lhs` is greater than or equal to `rhs`
/// @ingroup cnx_ratio
__attr(nodiscard) bool cnx_ratio_greater_than_or_equal(CnxRatio lhs, CnxRatio rhs);
/// @brief Returns the comparison of the left-hand `CnxRatio` to the other
///
/// @param lhs - The ratio to compare
/// @param rhs - The ratio to compare against
///
/// @return the comparison of `lhs` to `rhs`
/// @ingroup cnx_ratio
__attr(nodiscard) CnxCompare cnx_ratio_compare(CnxRatio lhs, CnxRatio rhs);
/// @brief Returns the given `CnxRatio` as a scalar value
///
/// Returns the given `CnxRatio` as a scalar value (`Some(i64, value)`), if it can be exactly
/// represented as one, otherwise returns `None(i64)`
///
/// @param ratio - The `CnxRatio` to convert to a scalar
///
/// @return `Some(i64, value)` if `ratio` can be represented as a scalar, `None(i64)` otherwise
/// @ingroup cnx_ratio
__attr(nodiscard) CnxOption(i64) cnx_ratio_as_scalar_exact(CnxRatio ratio);
/// @brief Returns the given `CnxRatio` as a scalar value
///
/// @param ratio - The `CnxRatio` to convert to a scalar
///
/// @return `ratio` as a scalar value
/// @note The resulting scalar may not be an exact match to the ratio represented by `ratio` due to
/// loss due to floating-point precision
/// @ingroup cnx_ratio
__attr(nodiscard) f64 cnx_ratio_as_scalar(CnxRatio ratio);
/// @brief Adds the given ratio and scalar value
///
/// Adds the given `CnxRatio` and scalar value and returns the result as a `CnxRatio`
///
/// @param ratio - The `CnxRatio` to add to
/// @param scalar - The scalar value to add
///
/// @return the `CnxRatio` resulting from the addition
/// @ingroup cnx_ratio
__attr(nodiscard) CnxRatio cnx_ratio_add_scalar(CnxRatio ratio, i64 scalar);
/// @brief Subtracts the given scalar value from the given ratio
///
/// Subtracts the scalar value from the given `CnxRatio` and returns the result as a `CnxRatio`
///
/// @param ratio - The `CnxRatio` to subtract from
/// @param scalar - The scalar value to subtract
///
/// @return the `CnxRatio` resulting from the subtraction
/// @ingroup cnx_ratio
__attr(nodiscard) CnxRatio cnx_ratio_subtract_scalar(CnxRatio ratio, i64 scalar);
/// @brief Multiplies the given ratio and scalar value
///
/// Multiplies the given `CnxRatio` and scalar value and returns the result as a `CnxRatio`
///
/// @param ratio - The `CnxRatio` to multiply with
/// @param scalar - The scalar value to multiply with
///
/// @return the `CnxRatio` resulting from the multiplication
/// @ingroup cnx_ratio
__attr(nodiscard) CnxRatio cnx_ratio_multiply_scalar(CnxRatio ratio, i64 scalar);
/// @brief Divides the given ratio by the given scalar value
///
/// Divides the given `CnxRatio` by the given scalar value and returns the result as a `CnxRatio`
///
/// @param ratio - The `CnxRatio` dividend
/// @param scalar - The scalar value divisor
///
/// @return the `CnxRatio` resulting from the division
/// @ingroup cnx_ratio
__attr(nodiscard) CnxRatio cnx_ratio_divide_scalar(CnxRatio ratio, i64 scalar);

/// @brief Implementation of `CnxFormat.is_specifier_valid` for `CnxRatio`
///
/// @param self - The `CnxRatio` to format as a `CnxFormat` trait object
/// @param specifier - The `CnxStringView` viewing the format specifier to validate
///
/// @return The `CnxFormatContext` indicating whether specifier was valid and storing the state
/// holding the format settings and necessary info to format the `CnxRatio`
__attr(nodiscard) __attr(not_null(1)) CnxFormatContext
	cnx_ratio_is_specifier_valid(const CnxFormat* restrict self, CnxStringView specifier)
		cnx_disable_if(!self,
					   "Can't format a nullptr (self must be a valid CnxRatio cast to CnxFormat)");
/// @brief Implement `CnxFormat.format` for `CnxRatio`
///
/// @param self - The `CnxRatio` to format
/// @param context - The `CnxFormatContext` specifying how formatting should be done
///
/// @return `self` formatted as a `CnxString`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_ratio_format(const CnxFormat* restrict self, CnxFormatContext context)
		cnx_disable_if(!self,
					   "Can't format a nullptr (self must be a valid CnxRatio cast to CnxFormat)");

/// @brief Implement `CnxFormat.format_with_allocator` for `CnxRatio`
///
/// @param self - The `CnxRatio` to format
/// @param context - The `CnxFormatContext` specifying how formatting should be done
/// @param allocator - The allocator to allocate memory for the formatted string with
///
/// @return `self` formatted as a `CnxString`
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_ratio_format_with_allocator(const CnxFormat* restrict self,
									CnxFormatContext context,
									CnxAllocator allocator)
		cnx_disable_if(!self,
					   "Can't format a nullptr (self must be a valid CnxRatio cast to CnxFormat)");

/// @brief Implement `CnxFormat` for `CnxRatio`
/// @ingroup cnx_ratio
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 CnxRatio,
										 cnx_ratio_is_specifier_valid,
										 cnx_ratio_format,
										 cnx_ratio_format_with_allocator);

#endif // CNX_RATIO
