/// @file Duration.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides methods for dealing with durations of time
/// @version 0.1.2
/// @date 2022-04-30
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

#include <Cnx/Format.h>
#include <Cnx/Ratio.h>

/// @ingroup cnx_time
/// @{
/// @defgroup cnx_duration CnxDuration
/// `CnxDuration` represents a positive or negative duration in a particular unit of time (e.g. a
/// (signed) number of seconds, nanoseconds, or years)
/// @}

#ifndef CNX_DURATION
	/// @brief Declarations related to `CnxDuration`
	#define CNX_DURATION

/// @brief `CnxDuration` represents a duration in a particular unit of time
/// A `CnxDuration` occurs in a particular unit of time (e.g. seconds, nanoseconds, or years), and
/// can represent a positive or negative amount of that unit
/// @ingroup cnx_duration
typedef struct CnxDuration {
	/// @brief The length of the duration
	/// @ingroup cnx_duration
	i64 count;
	/// @brief The unit of the duration as a `CnxRatio`
	/// For example, for a `CnxDuration` in milliseconds, the `period` would be `cnx_milli` or
	/// `cnx_milliseconds_period`
	/// @ingroup cnx_duration
	CnxRatio period;
} CnxDuration;

	/// @brief Period representing nanoseconds for `CnxDuration` and other time-related facilities
	/// @ingroup cnx_duration
	#define cnx_nanoseconds_period          \
		(CnxRatio) {                        \
			.num = 1LL, .den = 1000000000LL \
		}
	/// @brief Period representing microseconds for `CnxDuration` and other time-related facilities
	/// @ingroup cnx_duration
	#define cnx_microseconds_period      \
		(CnxRatio) {                     \
			.num = 1LL, .den = 1000000LL \
		}
	/// @brief Period representing milliseconds for `CnxDuration` and other time-related facilities
	/// @ingroup cnx_duration
	#define cnx_milliseconds_period   \
		(CnxRatio) {                  \
			.num = 1LL, .den = 1000LL \
		}
	/// @brief Period representing seconds for `CnxDuration` and other time-related facilities
	/// @ingroup cnx_duration
	#define cnx_seconds_period     \
		(CnxRatio) {               \
			.num = 1LL, .den = 1LL \
		}
	/// @brief Period representing minutes for `CnxDuration` and other time-related facilities
	/// @ingroup cnx_duration
	#define cnx_minutes_period      \
		(CnxRatio) {                \
			.num = 60LL, .den = 1LL \
		}
	/// @brief Period representing hours for `CnxDuration` and other time-related facilities
	/// @ingroup cnx_duration
	#define cnx_hours_period          \
		(CnxRatio) {                  \
			.num = 3600LL, .den = 1LL \
		}
	/// @brief Period representing days for `CnxDuration` and other time-related facilities
	/// @ingroup cnx_duration
	#define cnx_days_period            \
		(CnxRatio) {                   \
			.num = 86400LL, .den = 1LL \
		}
	/// @brief Period representing weeks for `CnxDuration` and other time-related facilities
	/// @ingroup cnx_duration
	#define cnx_weeks_period            \
		(CnxRatio) {                    \
			.num = 604800LL, .den = 1LL \
		}
	/// @brief Period representing months for `CnxDuration` and other time-related facilities
	/// @ingroup cnx_duration
	#define cnx_months_period            \
		(CnxRatio) {                     \
			.num = 2629746LL, .den = 1LL \
		}
	/// @brief Period representing years for `CnxDuration` and other time-related facilities
	/// @ingroup cnx_duration
	#define cnx_years_period              \
		(CnxRatio) {                      \
			.num = 31556952LL, .den = 1LL \
		}

/// @brief Valid periods for `CnxDuration` and other time-related facilities
/// @ingroup cnx_duration
static const CnxRatio cnx_duration_valid_periods[] = {cnx_nanoseconds_period,
													  cnx_microseconds_period,
													  cnx_milliseconds_period,
													  cnx_seconds_period,
													  cnx_minutes_period,
													  cnx_hours_period,
													  cnx_days_period,
													  cnx_weeks_period,
													  cnx_months_period,
													  cnx_years_period};

	/// @brief Returns a `CnxDuration` representing a number of nanoseconds
	/// @param val - The number of nanoseconds
	/// @return a `CnxDuration` representing a number of nanoseconds
	/// @ingroup cnx_duration
	#define cnx_nanoseconds(val) ((CnxDuration){.count = (val), .period = cnx_nanoseconds_period})
	/// @brief Returns a `CnxDuration` representing a number of microseconds
	/// @param val - The number of nanoseconds
	/// @return a `CnxDuration` representing a number of microseconds
	/// @ingroup cnx_duration
	#define cnx_microseconds(val) ((CnxDuration){.count = (val), .period = cnx_microseconds_period})
	/// @brief Returns a `CnxDuration` representing a number of milliseconds
	/// @param val - The number of nanoseconds
	/// @return a `CnxDuration` representing a number of milliseconds
	/// @ingroup cnx_duration
	#define cnx_milliseconds(val) ((CnxDuration){.count = (val), .period = cnx_milliseconds_period})
	/// @brief Returns a `CnxDuration` representing a number of seconds
	/// @param val - The number of nanoseconds
	/// @return a `CnxDuration` representing a number of seconds
	/// @ingroup cnx_duration
	#define cnx_seconds(val) ((CnxDuration){.count = (val), .period = cnx_seconds_period})
	/// @brief Returns a `CnxDuration` representing a number of minutes
	/// @param val - The number of nanoseconds
	/// @return a `CnxDuration` representing a number of minutes
	/// @ingroup cnx_duration
	#define cnx_minutes(val) ((CnxDuration){.count = (val), .period = cnx_minutes_period})
	/// @brief Returns a `CnxDuration` representing a number of hours
	/// @param val - The number of nanoseconds
	/// @return a `CnxDuration` representing a number of hours
	/// @ingroup cnx_duration
	#define cnx_hours(val) ((CnxDuration){.count = (val), .period = cnx_hours_period})
	/// @brief Returns a `CnxDuration` representing a number of days
	/// @param val - The number of nanoseconds
	/// @return a `CnxDuration` representing a number of days
	/// @ingroup cnx_duration
	#define cnx_days(val) ((CnxDuration){.count = (val), .period = cnx_days_period})
	/// @brief Returns a `CnxDuration` representing a number of weeks
	/// @param val - The number of nanoseconds
	/// @return a `CnxDuration` representing a number of weeks
	/// @ingroup cnx_duration
	#define cnx_weeks(val) ((CnxDuration){.count = (val), .period = cnx_weeks_period})
	/// @brief Returns a `CnxDuration` representing a number of months
	/// @param val - The number of nanoseconds
	/// @return a `CnxDuration` representing a number of months
	/// @ingroup cnx_duration
	#define cnx_months(val) ((CnxDuration){.count = (val), .period = cnx_months_period})
	/// @brief Returns a `CnxDuration` representing a number of years
	/// @param val - The number of nanoseconds
	/// @return a `CnxDuration` representing a number of years
	/// @ingroup cnx_duration
	#define cnx_years(val) ((CnxDuration){.count = (val), .period = cnx_years_period})

/// @brief Creates a new `CnxDuration` with the given count and period
///
/// @param count - The value of the duration. The number of `period`s it represents
/// @param period - The period of the duration. The unit of time it represents (e.g. seconds, hours,
/// etc.)
///
/// @return a new `CnxDuration`
/// @note period must be valid for `CnxDuration`. It must be contained in
/// `cnx_duration_valid_periods`
/// @ingroup cnx_duration
CnxDuration cnx_duration_new(i64 count, CnxRatio period);

/// @brief Converts the given `CnxDuration` to one with a different period
/// Converts the given `CnxDuration` to a `CnxDuration` with a different period, truncating any
/// fractional portion caused by the conversion
///
/// @param to_cast - The `CnxDuration` to cast
/// @param new_period - The `CnxRatio` representing the new period
///
/// @return `to_cast` converted a new period
/// @ingroup cnx_duration
CnxDuration cnx_duration_cast(CnxDuration to_cast, CnxRatio new_period);
/// @brief Converts the given `CnxDuration` to one with a different period
/// Converts the given `CnxDuration` to a `CnxDuration` with a different period, flooring any
/// fractional portion caused by the conversion
///
/// @param to_floor - The `CnxDuration` to cast
/// @param new_period - The `CnxRatio` representing the new period
///
/// @return `to_cast` converted a new period
/// @ingroup cnx_duration
CnxDuration cnx_duration_floor(CnxDuration to_floor, CnxRatio new_period);
/// @brief Converts the given `CnxDuration` to one with a different period
/// Converts the given `CnxDuration` to a `CnxDuration` with a different period, taking the ceiling
/// of any fractional portion caused by the conversion
///
/// @param to_ceil - The `CnxDuration` to cast
/// @param new_period - The `CnxRatio` representing the new period
///
/// @return `to_cast` converted a new period
/// @ingroup cnx_duration
CnxDuration cnx_duration_ceil(CnxDuration to_ceil, CnxRatio new_period);
/// @brief Converts the given `CnxDuration` to one with a different period
/// Converts the given `CnxDuration` to a `CnxDuration` with a different period, rounding any
/// fractional portion caused by the conversion
///
/// @param to_round - The `CnxDuration` to cast
/// @param new_period - The `CnxRatio` representing the new period
///
/// @return `to_cast` converted a new period
/// @ingroup cnx_duration
CnxDuration cnx_duration_round(CnxDuration to_round, CnxRatio new_period);
/// @brief Takes the absolute value of the given `CnxDuration
///
/// @param duration - The `CnxDuration` to get the absolute value of
///
/// @return the absolute value of `duration`
/// @ingroup cnx_duration
CnxDuration cnx_duration_abs(CnxDuration duration);
/// @brief Casts `rhs` to the same period as `lhs`, then adds the two
///
/// @param lhs - The lhs `CnxDuration` to add
/// @param rhs - The rhs `CnxDuration` to add. Will be cast to the same period as `lhs`
///
/// @return the equivalent of `lhs + Cnx_duration_cast(rhs, lhs.period)`
/// @ingroup cnx_duration
CnxDuration cnx_duration_add(CnxDuration lhs, CnxDuration rhs);
/// @brief Adds `rhs` to `lhs` as if it were a `CnxDuration` of the same period
///
/// @param lhs - The lhs `CnxDuration` to add
/// @param rhs - The scalar value to add. Will be treated as a `CnxDuration` of the same period as
/// `lhs`
///
/// @return the sum of `lhs` and `rhs`
/// @ingroup cnx_duration
CnxDuration cnx_duration_add_scalar(CnxDuration lhs, i64 rhs);
/// @brief Casts `rhs` to the same period as `lhs`, then subtracts `rhs` from `lhs`
///
/// @param lhs - The lhs `CnxDuration` to subtract from
/// @param rhs - The rhs `CnxDuration` to subtract. Will be cast to the same period as `lhs`
///
/// @return the equivalent of `lhs - Cnx_duration_cast(rhs, lhs.period)`
/// @ingroup cnx_duration
CnxDuration cnx_duration_subtract(CnxDuration lhs, CnxDuration rhs);
/// @brief Subtracts `rhs` from `lhs` as if it were a `CnxDuration` of the same period
///
/// @param lhs - The lhs `CnxDuration` to subtract from
/// @param rhs - The scalar value to subtract. Will be treated as a `CnxDuration` of the same period
/// as `lhs`
///
/// @return the difference between `lhs` and `rhs`
/// @ingroup cnx_duration
CnxDuration cnx_duration_subtract_scalar(CnxDuration lhs, i64 rhs);
/// @brief Multiplies `lhs` by the scalar value `rhs`
///
/// @param lhs - The lhs `CnxDuration` to multiply
/// @param rhs - The scalar value to multiply by.
///
/// @return the product of `lhs` and `rhs`
/// @ingroup cnx_duration
CnxDuration cnx_duration_multiply(CnxDuration lhs, i64 rhs);
/// @brief Divides `lhs` by the scalar value `rhs`
///
/// @param lhs - The lhs `CnxDuration` to divide
/// @param rhs - The scalar value to divide by.
///
/// @return the quotient of `lhs` and `rhs`
/// @ingroup cnx_duration
CnxDuration cnx_duration_divide(CnxDuration lhs, i64 rhs);
/// @brief Determines if the two `CnxDuration`s are equal.
///
/// Determines if the two `CnxDuration`s are equal by comparing them in the most precise period
/// of the two. For example, if `lhs`'s period is `cnx_milliseconds_period` and `rhs`'s period is
/// `cnx_seconds_period`, `rhs` will be converted to a `CnxDuration` in `cnx_milliseconds_period`
/// before the comparison.
///
/// @param lhs - The lhs `CnxDuration` to compare
/// @param rhs - The rhs `CnxDuration` to compare.
///
/// @return whether `lhs` and `rhs` are equivalent
/// @ingroup cnx_duration
bool cnx_duration_equal(CnxDuration lhs, CnxDuration rhs);
/// @brief Determines if the two `CnxDuration`s are **NOT** equal.
///
/// Determines if the two `CnxDuration`s are **NOT** equal by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `cnx_milliseconds_period` and `rhs`'s
/// period is `cnx_seconds_period`, `rhs` will be converted to a `CnxDuration` in
/// `cnx_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `CnxDuration` to compare
/// @param rhs - The rhs `CnxDuration` to compare.
///
/// @return whether `lhs` and `rhs` are **NOT** equivalent
/// @ingroup cnx_duration
bool cnx_duration_not_equal(CnxDuration lhs, CnxDuration rhs);
/// @brief Determines if `lhs` is strictly less than `rhs`.
///
/// Determines if `lhs` is strictly less than `rhs` by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `cnx_milliseconds_period` and `rhs`'s
/// period is `cnx_seconds_period`, `rhs` will be converted to a `CnxDuration` in
/// `cnx_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `CnxDuration` to compare
/// @param rhs - The rhs `CnxDuration` to compare.
///
/// @return whether `lhs` is strictly less than `rhs`
/// @ingroup cnx_duration
bool cnx_duration_less_than(CnxDuration lhs, CnxDuration rhs);
/// @brief Determines if `lhs` is less than or equal to `rhs`.
///
/// Determines if `lhs` is less than or equal to `rhs` by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `cnx_milliseconds_period` and `rhs`'s
/// period is `cnx_seconds_period`, `rhs` will be converted to a `CnxDuration` in
/// `cnx_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `CnxDuration` to compare
/// @param rhs - The rhs `CnxDuration` to compare.
///
/// @return whether `lhs` is less than or equal to `rhs`
/// @ingroup cnx_duration
bool cnx_duration_less_than_or_equal(CnxDuration lhs, CnxDuration rhs);
/// @brief Determines if `lhs` is strictly greater than `rhs`.
///
/// Determines if `lhs` is strictly greater than `rhs` by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `cnx_milliseconds_period` and `rhs`'s
/// period is `cnx_seconds_period`, `rhs` will be converted to a `CnxDuration` in
/// `cnx_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `CnxDuration` to compare
/// @param rhs - The rhs `CnxDuration` to compare.
///
/// @return whether `lhs` is strictly greater than `rhs`
/// @ingroup cnx_duration
bool cnx_duration_greater_than(CnxDuration lhs, CnxDuration rhs);
/// @brief Determines if `lhs` is greater than or equal to `rhs`.
///
/// Determines if `lhs` is greater than or equal to `rhs` by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `cnx_milliseconds_period` and `rhs`'s
/// period is `cnx_seconds_period`, `rhs` will be converted to a `CnxDuration` in
/// `cnx_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `CnxDuration` to compare
/// @param rhs - The rhs `CnxDuration` to compare.
///
/// @return whether `lhs` is greater than or equal to `rhs`
/// @ingroup cnx_duration
bool cnx_duration_greater_than_or_equal(CnxDuration lhs, CnxDuration rhs);
/// @brief Performs a three-way comparison of `lhs` to `rhs`, determining how `lhs` mathematically
/// relates to `rhs`.
///
/// Performs a three-way comparison of `lhs` to `rhs` by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `cnx_milliseconds_period` and `rhs`'s
/// period is `cnx_seconds_period`, `rhs` will be converted to a `CnxDuration` in
/// `cnx_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `CnxDuration` to compare
/// @param rhs - The rhs `CnxDuration` to compare.
///
/// @return how `lhs` mathematically compares to `rhs`
/// @ingroup cnx_duration
CnxCompare cnx_duration_compare(CnxDuration lhs, CnxDuration rhs);

/// @brief Implements the allocator-unaware part of the `CnxFormat` trait for `CnxDuration`
///
/// @param self - The `CnxDuration` to format, as its `CnxFormat` trait representation
/// @param specifier - The format specifier representing how to format `self`. Should be unused/left
/// default in the format string
///
/// @return `self` formatted as a `CnxString`
/// @ingroup cnx_duration
CnxString cnx_duration_format(const CnxFormat* restrict self, CnxFormatSpecifier specifier);
/// @brief Implements the allocator-aware part of the `CnxFormat` trait for `CnxDuration`
///
/// @param self - The `CnxDuration` to format, as its `CnxFormat` trait representation
/// @param specifier - The format specifier representing how to format `self`. Should be unused/left
/// default in the format string
/// @param allocator - The `CnxAllocator` to allocate memory with
///
/// @return `self` formatted as a `CnxString`
/// @ingroup cnx_duration
CnxString cnx_duration_format_with_allocator(const CnxFormat* restrict self,
											 CnxFormatSpecifier specifier,
											 CnxAllocator allocator);

/// @brief Implements the `CnxFormat` trait for `CnxDuration`
/// @ingroup cnx_duration
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 CnxDuration,
										 cnx_duration_format,
										 cnx_duration_format_with_allocator);
#endif // CNX_DURATION
