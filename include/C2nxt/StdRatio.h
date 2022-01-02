/// @file StdRatio.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides lossless methods for dealing with exact fractions
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

#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdFormat.h>
#include <C2nxt/StdMath.h>

/// @defgroup std_ratio StdRatio
/// `StdRatio` provides lossless methods for dealing with exact fractions
///
/// Example:
/// @code {./c}
/// let time_seconds = 1000;
/// let time_milliseconds_ratio = std_ratio_multiply(std_milli, time_seconds);
/// // try to get the scalar value as an integer if it can be represented by one
/// let maybe_exact_milliseconds = std_ratio_as_scalar_exact(time_milliseconds_ratio);
/// let exact = std_option_unwrap(maybe_exact);
/// // get the scalar value as an f64
/// let milliseconds_f64 = std_ratio_as_scalar(time_milliseconds);
/// @endcode

#ifndef STD_RATIO
	/// @brief Declarations related to `StdRatio`
	#define STD_RATIO

/// @brief `StdRatio` is a representation of an exact fraction
/// @ingroup std_ratio
typedef struct StdRatio {
	/// @brief The numerator of the ratio
	const i64 num;
	/// @brief The denominator of the ratio
	const i64 den;
} StdRatio;

	/// @brief Constructs a `StdRatio` at compile time
	///
	/// Constructs a `StdRatio` from the given numerator and denominator, as a simplified fraction
	/// at compile time.
	///
	/// @param numerator - The numerator of the ratio
	/// @param denominator - The denominator of the ratio
	///
	/// @return A `StdRatio` as the simplified fraction of `numerator / denominator`
	/// @ingroup std_ratio
	#define std_ratio(numerator, denominator)                                                  \
		((StdRatio){                                                                           \
			.num = (std_static_sign(numerator) * std_static_sign(denominator)                  \
					* std_static_abs(numerator)                                                \
					/ std_static_gcd(std_static_abs(numerator), std_static_abs(denominator))), \
			.den = (std_static_abs(denominator)                                                \
					/ std_static_gcd(std_static_abs(numerator), std_static_abs(denominator)))})

/// @brief SI "atto" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_atto = (StdRatio){.num = 1LL, .den = 1000000000000000000LL};
/// @brief SI "femto" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_femto = (StdRatio){.num = 1LL, .den = 1000000000000000LL};
/// @brief SI "pico" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_pico = (StdRatio){.num = 1LL, .den = 1000000000000LL};
/// @brief SI "nano" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_nano = (StdRatio){.num = 1LL, .den = 1000000000LL};
/// @brief SI "micro" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_micro = (StdRatio){.num = 1LL, .den = 1000000LL};
/// @brief SI "milli" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_milli = (StdRatio){.num = 1LL, .den = 1000LL};
/// @brief SI "centi" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_centi = (StdRatio){.num = 1LL, .den = 100LL};
/// @brief SI "deci" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_deci = (StdRatio){.num = 1LL, .den = 10LL};
/// @brief SI "deca" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_deca = (StdRatio){.num = 10LL, .den = 1LL};
/// @brief SI "hecto" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_hecto = (StdRatio){.num = 100LL, .den = 1LL};
/// @brief SI "kilo" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_kilo = (StdRatio){.num = 1000LL, .den = 1LL};
/// @brief SI "mega" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_mega = (StdRatio){.num = 1000000LL, .den = 1LL};
/// @brief SI "giga" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_giga = (StdRatio){.num = 1000000000LL, .den = 1LL};
/// @brief SI "tera" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_tera = (StdRatio){.num = 1000000000000LL, .den = 1LL};
/// @brief SI "peta" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_peta = (StdRatio){.num = 1000000000000000LL, .den = 1LL};
/// @brief SI "exa" as a `StdRatio`
/// @ingroup std_ratio
static let maybe_unused std_exa = (StdRatio){.num = 1000000000000000000LL, .den = 1LL};

/// @brief Constructs a `StdRatio`
///
/// Constructs a `StdRatio` from the given numerator and denominator, as a simplified fraction
///
/// @param num - The numerator of the ratio
/// @param den - The denominator of the ratio
///
/// @return A `StdRatio` as the simplified fraction of `numerator / denominator`
/// @ingroup std_ratio
StdRatio std_ratio_new(i64 num, i64 den);

/// @brief Adds the given `StdRatio`s
///
/// Adds the given `StdRatio`s, returning the result as a new `StdRatio` in simplest form
///
/// @param lhs - The left-hand ratio to add
/// @param rhs - The right-hand ratio to add
///
/// @return The sum of `lhs` and `rhs` as a `StdRatio` in simplest form
/// @ingroup std_ratio
StdRatio std_ratio_add(StdRatio lhs, StdRatio rhs);
/// @brief Subtracts the given `StdRatio`s
///
/// Subtracts `rhs` from `lhs`, returning the result as a new `StdRatio` in simplest form
///
/// @param lhs - The left-hand ratio to subtract from
/// @param rhs - The right-hand ratio to subtract
///
/// @return The difference of `lhs` and `rhs` as a `StdRatio` in simplest form
/// @ingroup std_ratio
StdRatio std_ratio_subtract(StdRatio lhs, StdRatio rhs);
/// @brief Multiplies the given `StdRatio`s
///
/// Multiplies the given `StdRatio`s, returning the result as a new `StdRatio` in simplest form
///
/// @param lhs - The left-hand ratio to multiply
/// @param rhs - The right-hand ratio to multiply
///
/// @return The product of `lhs` and `rhs` as a `StdRatio` in simplest form
/// @ingroup std_ratio
StdRatio std_ratio_multiply(StdRatio lhs, StdRatio rhs);
/// @brief Divides the given `StdRatio`s
///
/// Divides `lhs` by `rhs`, returning the result as a new `StdRatio` in simplest form
///
/// @param lhs - The `StdRatio` dividend
/// @param rhs - The `StdRatio` divisor
///
/// @return The quotient of `lhs` and `rhs` as a `StdRatio` in simplest form
/// @ingroup std_ratio
StdRatio std_ratio_divide(StdRatio lhs, StdRatio rhs);
/// @brief Returns whether the two `StdRatio`s are equal
///
/// @param lhs - The left-hand ratio to check for equality
/// @param rhs - The right-hand ratio to check for equality
///
/// @return Whether the given ratios are equal
/// @note This assumes that the given ratios are in simplest form. It does not check if either
/// ratio has not been reduced.
/// @ingroup std_ratio
bool std_ratio_equal(StdRatio lhs, StdRatio rhs);
/// @brief Returns whether the two `StdRatio`s are __not__ equal
///
/// @param lhs - The left-hand ratio to check for inequality
/// @param rhs - The right-hand ratio to check for inequality
///
/// @return Whether the given ratios are __not__ equal
/// @note This assumes that the given ratios are in simplest form. It does not check if either
/// ratio has not been reduced.
/// @ingroup std_ratio
bool std_ratio_not_equal(StdRatio lhs, StdRatio rhs);
/// @brief Returns whether the left-hand `StdRatio` is less than the other
///
/// @param lhs - The ratio to compare
/// @param rhs - The ratio to compare against
///
/// @return Whether `lhs` is less than `rhs`
/// @ingroup std_ratio
bool std_ratio_less_than(StdRatio lhs, StdRatio rhs);
/// @brief Returns whether the left-hand `StdRatio` is less than or equal to the other
///
/// @param lhs - The ratio to compare
/// @param rhs - The ratio to compare against
///
/// @return Whether `lhs` is less than or equal to `rhs`
/// @ingroup std_ratio
bool std_ratio_less_than_or_equal(StdRatio lhs, StdRatio rhs);
/// @brief Returns whether the left-hand `StdRatio` is greater than the other
///
/// @param lhs - The ratio to compare
/// @param rhs - The ratio to compare against
///
/// @return Whether `lhs` is greater than `rhs`
/// @ingroup std_ratio
bool std_ratio_greater_than(StdRatio lhs, StdRatio rhs);
/// @brief Returns whether the left-hand `StdRatio` is greater than or equal to the other
///
/// @param lhs - The ratio to compare
/// @param rhs - The ratio to compare against
///
/// @return Whether `lhs` is greater than or equal to `rhs`
/// @ingroup std_ratio
bool std_ratio_greater_than_or_equal(StdRatio lhs, StdRatio rhs);
/// @brief Returns the comparison of the left-hand `StdRatio` to the other
///
/// @param lhs - The ratio to compare
/// @param rhs - The ratio to compare against
///
/// @return the comparison of `lhs` to `rhs`
/// @ingroup std_ratio
StdCompare std_ratio_compare(StdRatio lhs, StdRatio rhs);
/// @brief Returns the given `StdRatio` as a scalar value
///
/// Returns the given `StdRatio` as a scalar value (`Some(i64, value)`), if it can be exactly
/// represented as one, otherwise returns `None(i64)`
///
/// @param ratio - The `StdRatio` to convert to a scalar
///
/// @return `Some(i64, value)` if `ratio` can be represented as a scalar, `None(i64)` otherwise
/// @ingroup std_ratio
StdOption(i64) std_ratio_as_scalar_exact(StdRatio ratio);
/// @brief Returns the given `StdRatio` as a scalar value
///
/// @param ratio - The `StdRatio` to convert to a scalar
///
/// @return `ratio` as a scalar value
/// @note The resulting scalar may not be an exact match to the ratio represented by `ratio` due to
/// loss due to floating-point precision
/// @ingroup std_ratio
f64 std_ratio_as_scalar(StdRatio ratio);
/// @brief Adds the given ratio and scalar value
///
/// Adds the given `StdRatio` and scalar value and returns the result as a `StdRatio`
///
/// @param ratio - The `StdRatio` to add to
/// @param scalar - The scalar value to add
///
/// @return the `StdRatio` resulting from the addition
/// @ingroup std_ratio
StdRatio std_ratio_add_scalar(StdRatio ratio, i64 scalar);
/// @brief Subtracts the given scalar value from the given ratio
///
/// Subtracts the scalar value from the given `StdRatio` and returns the result as a `StdRatio`
///
/// @param ratio - The `StdRatio` to subtract from
/// @param scalar - The scalar value to subtract
///
/// @return the `StdRatio` resulting from the subtraction
/// @ingroup std_ratio
StdRatio std_ratio_subtract_scalar(StdRatio ratio, i64 scalar);
/// @brief Multiplies the given ratio and scalar value
///
/// Multiplies the given `StdRatio` and scalar value and returns the result as a `StdRatio`
///
/// @param ratio - The `StdRatio` to multiply with
/// @param scalar - The scalar value to multiply with
///
/// @return the `StdRatio` resulting from the multiplication
/// @ingroup std_ratio
StdRatio std_ratio_multiply_scalar(StdRatio ratio, i64 scalar);
/// @brief Divides the given ratio by the given scalar value
///
/// Divides the given `StdRatio` by the given scalar value and returns the result as a `StdRatio`
///
/// @param ratio - The `StdRatio` dividend
/// @param scalar - The scalar value divisor
///
/// @return the `StdRatio` resulting from the division
/// @ingroup std_ratio
StdRatio std_ratio_divide_scalar(StdRatio ratio, i64 scalar);

/// @brief Implement `StdFormat.format` for `StdRatio`
///
/// @param self - The `StdRatio` to format
/// @param specifier - The format specifier
///
/// @return `self` formatted as a `StdString`
StdString std_ratio_format(const StdFormat* restrict self, StdFormatSpecifier specifier);

/// @brief Implement `StdFormat.format_with_allocator` for `StdRatio`
///
/// @param self - The `StdRatio` to format
/// @param specifier - The format specifier
/// @param allocator - The allocator to allocate memory for the formatted string with
///
/// @return `self` formatted as a `StdString`
StdString std_ratio_format_with_allocator(const StdFormat* restrict self,
										  StdFormatSpecifier specifier,
										  StdAllocator allocator);

/// @brief Implement `StdFormat` for `StdRatio`
/// @ingroup std_ratio
static maybe_unused
	ImplTraitFor(StdFormat, StdRatio, std_ratio_format, std_ratio_format_with_allocator);

#endif // STD_RATIO
