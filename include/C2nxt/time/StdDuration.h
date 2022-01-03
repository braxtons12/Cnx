/// @file StdDuration.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides methods for dealing with durations of time
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

#include <C2nxt/StdFormat.h>
#include <C2nxt/StdRatio.h>

#ifndef STD_DURATION
	/// @brief Declarations related to `StdDuration`
	#define STD_DURATION

/// @brief `StdDuration` represents a duration in a particular unit of time
/// A `StdDuration` occurs in a particular unit of time (e.g. seconds, nanoseconds, or years), and
/// can represent a positive or negative amount of that unit
/// @ingroup std_time
typedef struct StdDuration {
	/// @brief The length of the duration
	/// @ingroup std_time
	i64 count;
	/// @brief The unit of the duration as a `StdRatio`
	/// For example, for a `StdDuration` in milliseconds, the `period` would be `std_milli` or
	/// `std_milliseconds_period`
	/// @ingroup std_time
	StdRatio period;
} StdDuration;

	/// @brief Period representing nanoseconds for `StdDuration` and other time-related facilities
	/// @ingroup std_time
	#define std_nanoseconds_period          \
		(StdRatio) {                        \
			.num = 1LL, .den = 1000000000LL \
		}
	/// @brief Period representing microseconds for `StdDuration` and other time-related facilities
	/// @ingroup std_time
	#define std_microseconds_period      \
		(StdRatio) {                     \
			.num = 1LL, .den = 1000000LL \
		}
	/// @brief Period representing milliseconds for `StdDuration` and other time-related facilities
	/// @ingroup std_time
	#define std_milliseconds_period   \
		(StdRatio) {                  \
			.num = 1LL, .den = 1000LL \
		}
	/// @brief Period representing seconds for `StdDuration` and other time-related facilities
	/// @ingroup std_time
	#define std_seconds_period     \
		(StdRatio) {               \
			.num = 1LL, .den = 1LL \
		}
	/// @brief Period representing minutes for `StdDuration` and other time-related facilities
	/// @ingroup std_time
	#define std_minutes_period      \
		(StdRatio) {                \
			.num = 60LL, .den = 1LL \
		}
	/// @brief Period representing hours for `StdDuration` and other time-related facilities
	/// @ingroup std_time
	#define std_hours_period          \
		(StdRatio) {                  \
			.num = 3600LL, .den = 1LL \
		}
	/// @brief Period representing days for `StdDuration` and other time-related facilities
	/// @ingroup std_time
	#define std_days_period            \
		(StdRatio) {                   \
			.num = 86400LL, .den = 1LL \
		}
	/// @brief Period representing weeks for `StdDuration` and other time-related facilities
	/// @ingroup std_time
	#define std_weeks_period            \
		(StdRatio) {                    \
			.num = 604800LL, .den = 1LL \
		}
	/// @brief Period representing months for `StdDuration` and other time-related facilities
	/// @ingroup std_time
	#define std_months_period            \
		(StdRatio) {                     \
			.num = 2629746LL, .den = 1LL \
		}
	/// @brief Period representing years for `StdDuration` and other time-related facilities
	/// @ingroup std_time
	#define std_years_period              \
		(StdRatio) {                      \
			.num = 31556952LL, .den = 1LL \
		}

/// @brief Valid periods for `StdDuration` and other time-related facilities
/// @ingroup std_time
static const StdRatio std_duration_valid_periods[] = {std_nanoseconds_period,
													  std_microseconds_period,
													  std_milliseconds_period,
													  std_seconds_period,
													  std_minutes_period,
													  std_hours_period,
													  std_days_period,
													  std_weeks_period,
													  std_months_period,
													  std_years_period};

	/// @brief Returns a `StdDuration` representing a number of nanoseconds
	/// @param val - The number of nanoseconds
	/// @return a `StdDuration` representing a number of nanoseconds
	/// @ingroup std_time
	#define std_nanoseconds(val) ((StdDuration){.count = (val), .period = std_nanoseconds_period})
	/// @brief Returns a `StdDuration` representing a number of microseconds
	/// @param val - The number of nanoseconds
	/// @return a `StdDuration` representing a number of microseconds
	/// @ingroup std_time
	#define std_microseconds(val) ((StdDuration){.count = (val), .period = std_microseconds_period})
	/// @brief Returns a `StdDuration` representing a number of milliseconds
	/// @param val - The number of nanoseconds
	/// @return a `StdDuration` representing a number of milliseconds
	/// @ingroup std_time
	#define std_milliseconds(val) ((StdDuration){.count = (val), .period = std_milliseconds_period})
	/// @brief Returns a `StdDuration` representing a number of seconds
	/// @param val - The number of nanoseconds
	/// @return a `StdDuration` representing a number of seconds
	/// @ingroup std_time
	#define std_seconds(val) ((StdDuration){.count = (val), .period = std_seconds_period})
	/// @brief Returns a `StdDuration` representing a number of minutes
	/// @param val - The number of nanoseconds
	/// @return a `StdDuration` representing a number of minutes
	/// @ingroup std_time
	#define std_minutes(val) ((StdDuration){.count = (val), .period = std_minutes_period})
	/// @brief Returns a `StdDuration` representing a number of hours
	/// @param val - The number of nanoseconds
	/// @return a `StdDuration` representing a number of hours
	/// @ingroup std_time
	#define std_hours(val) ((StdDuration){.count = (val), .period = std_hours_period})
	/// @brief Returns a `StdDuration` representing a number of days
	/// @param val - The number of nanoseconds
	/// @return a `StdDuration` representing a number of days
	/// @ingroup std_time
	#define std_days(val) ((StdDuration){.count = (val), .period = std_days_period})
	/// @brief Returns a `StdDuration` representing a number of weeks
	/// @param val - The number of nanoseconds
	/// @return a `StdDuration` representing a number of weeks
	/// @ingroup std_time
	#define std_weeks(val) ((StdDuration){.count = (val), .period = std_weeks_period})
	/// @brief Returns a `StdDuration` representing a number of months
	/// @param val - The number of nanoseconds
	/// @return a `StdDuration` representing a number of months
	/// @ingroup std_time
	#define std_months(val) ((StdDuration){.count = (val), .period = std_months_period})
	/// @brief Returns a `StdDuration` representing a number of years
	/// @param val - The number of nanoseconds
	/// @return a `StdDuration` representing a number of years
	/// @ingroup std_time
	#define std_years(val) ((StdDuration){.count = (val), .period = std_years_period})

/// @brief Converts the given `StdDuration` to one with a different period
/// Converts the given `StdDuration` to a `StdDuration` with a different period, truncating any
/// fractional portion caused by the conversion
///
/// @param to_cast - The `StdDuration` to cast
/// @param new_period - The `StdRatio` representing the new period
///
/// @return `to_cast` converted a new period
/// @ingroup std_time
StdDuration std_duration_cast(StdDuration to_cast, StdRatio new_period);
/// @brief Converts the given `StdDuration` to one with a different period
/// Converts the given `StdDuration` to a `StdDuration` with a different period, flooring any
/// fractional portion caused by the conversion
///
/// @param to_floor - The `StdDuration` to cast
/// @param new_period - The `StdRatio` representing the new period
///
/// @return `to_cast` converted a new period
/// @ingroup std_time
StdDuration std_duration_floor(StdDuration to_floor, StdRatio new_period);
/// @brief Converts the given `StdDuration` to one with a different period
/// Converts the given `StdDuration` to a `StdDuration` with a different period, taking the ceiling
/// of any fractional portion caused by the conversion
///
/// @param to_ceil - The `StdDuration` to cast
/// @param new_period - The `StdRatio` representing the new period
///
/// @return `to_cast` converted a new period
/// @ingroup std_time
StdDuration std_duration_ceil(StdDuration to_ceil, StdRatio new_period);
/// @brief Converts the given `StdDuration` to one with a different period
/// Converts the given `StdDuration` to a `StdDuration` with a different period, rounding any
/// fractional portion caused by the conversion
///
/// @param to_round - The `StdDuration` to cast
/// @param new_period - The `StdRatio` representing the new period
///
/// @return `to_cast` converted a new period
/// @ingroup std_time
StdDuration std_duration_round(StdDuration to_round, StdRatio new_period);
/// @brief Takes the absolute value of the given `StdDuration
///
/// @param duration - The `StdDuration` to get the absolute value of
///
/// @return the absolute value of `duration`
/// @ingroup std_time
StdDuration std_duration_abs(StdDuration duration);
/// @brief Casts `rhs` to the same period as `lhs`, then adds the two
///
/// @param lhs - The lhs `StdDuration` to add
/// @param rhs - The rhs `StdDuration` to add. Will be cast to the same period as `lhs`
///
/// @return the equivalent of `lhs + std_duration_cast(rhs, lhs.period)`
/// @ingroup std_time
StdDuration std_duration_add(StdDuration lhs, StdDuration rhs);
/// @brief Adds `rhs` to `lhs` as if it were a `StdDuration` of the same period
///
/// @param lhs - The lhs `StdDuration` to add
/// @param rhs - The scalar value to add. Will be treated as a `StdDuration` of the same period as
/// `lhs`
///
/// @return the sum of `lhs` and `rhs`
/// @ingroup std_time
StdDuration std_duration_add_scalar(StdDuration lhs, i64 rhs);
/// @brief Casts `rhs` to the same period as `lhs`, then subtracts `rhs` from `lhs`
///
/// @param lhs - The lhs `StdDuration` to subtract from
/// @param rhs - The rhs `StdDuration` to subtract. Will be cast to the same period as `lhs`
///
/// @return the equivalent of `lhs - std_duration_cast(rhs, lhs.period)`
/// @ingroup std_time
StdDuration std_duration_subtract(StdDuration lhs, StdDuration rhs);
/// @brief Subtracts `rhs` from `lhs` as if it were a `StdDuration` of the same period
///
/// @param lhs - The lhs `StdDuration` to subtract from
/// @param rhs - The scalar value to subtract. Will be treated as a `StdDuration` of the same period
/// as `lhs`
///
/// @return the difference between `lhs` and `rhs`
/// @ingroup std_time
StdDuration std_duration_subtract_scalar(StdDuration lhs, i64 rhs);
/// @brief Multiplies `lhs` by the scalar value `rhs`
///
/// @param lhs - The lhs `StdDuration` to multiply
/// @param rhs - The scalar value to multiply by.
///
/// @return the product of `lhs` and `rhs`
/// @ingroup std_time
StdDuration std_duration_multiply(StdDuration lhs, i64 rhs);
/// @brief Divides `lhs` by the scalar value `rhs`
///
/// @param lhs - The lhs `StdDuration` to divide
/// @param rhs - The scalar value to divide by.
///
/// @return the quotient of `lhs` and `rhs`
/// @ingroup std_time
StdDuration std_duration_divide(StdDuration lhs, i64 rhs);
/// @brief Determines if the two `StdDuration`s are equal.
///
/// Determines if the two `StdDuration`s are equal by comparing them in the most precise period
/// of the two. For example, if `lhs`'s period is `std_milliseconds_period` and `rhs`'s period is
/// `std_seconds_period`, `rhs` will be converted to a `StdDuration` in `std_milliseconds_period`
/// before the comparison.
///
/// @param lhs - The lhs `StdDuration` to compare
/// @param rhs - The rhs `StdDuration` to compare.
///
/// @return whether `lhs` and `rhs` are equivalent
/// @ingroup std_time
bool std_duration_equal(StdDuration lhs, StdDuration rhs);
/// @brief Determines if the two `StdDuration`s are **NOT** equal.
///
/// Determines if the two `StdDuration`s are **NOT** equal by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `std_milliseconds_period` and `rhs`'s
/// period is `std_seconds_period`, `rhs` will be converted to a `StdDuration` in
/// `std_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `StdDuration` to compare
/// @param rhs - The rhs `StdDuration` to compare.
///
/// @return whether `lhs` and `rhs` are **NOT** equivalent
/// @ingroup std_time
bool std_duration_not_equal(StdDuration lhs, StdDuration rhs);
/// @brief Determines if `lhs` is strictly less than `rhs`.
///
/// Determines if `lhs` is strictly less than `rhs` by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `std_milliseconds_period` and `rhs`'s
/// period is `std_seconds_period`, `rhs` will be converted to a `StdDuration` in
/// `std_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `StdDuration` to compare
/// @param rhs - The rhs `StdDuration` to compare.
///
/// @return whether `lhs` is strictly less than `rhs`
/// @ingroup std_time
bool std_duration_less_than(StdDuration lhs, StdDuration rhs);
/// @brief Determines if `lhs` is less than or equal to `rhs`.
///
/// Determines if `lhs` is less than or equal to `rhs` by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `std_milliseconds_period` and `rhs`'s
/// period is `std_seconds_period`, `rhs` will be converted to a `StdDuration` in
/// `std_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `StdDuration` to compare
/// @param rhs - The rhs `StdDuration` to compare.
///
/// @return whether `lhs` is less than or equal to `rhs`
/// @ingroup std_time
bool std_duration_less_than_or_equal(StdDuration lhs, StdDuration rhs);
/// @brief Determines if `lhs` is strictly greater than `rhs`.
///
/// Determines if `lhs` is strictly greater than `rhs` by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `std_milliseconds_period` and `rhs`'s
/// period is `std_seconds_period`, `rhs` will be converted to a `StdDuration` in
/// `std_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `StdDuration` to compare
/// @param rhs - The rhs `StdDuration` to compare.
///
/// @return whether `lhs` is strictly greater than `rhs`
/// @ingroup std_time
bool std_duration_greater_than(StdDuration lhs, StdDuration rhs);
/// @brief Determines if `lhs` is greater than or equal to `rhs`.
///
/// Determines if `lhs` is greater than or equal to `rhs` by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `std_milliseconds_period` and `rhs`'s
/// period is `std_seconds_period`, `rhs` will be converted to a `StdDuration` in
/// `std_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `StdDuration` to compare
/// @param rhs - The rhs `StdDuration` to compare.
///
/// @return whether `lhs` is greater than or equal to `rhs`
/// @ingroup std_time
bool std_duration_greater_than_or_equal(StdDuration lhs, StdDuration rhs);
/// @brief Performs a three-way comparison of `lhs` to `rhs`, determining how `lhs` mathematically
/// relates to `rhs`.
///
/// Performs a three-way comparison of `lhs` to `rhs` by comparing them in the most precise
/// period of the two. For example, if `lhs`'s period is `std_milliseconds_period` and `rhs`'s
/// period is `std_seconds_period`, `rhs` will be converted to a `StdDuration` in
/// `std_milliseconds_period` before the comparison.
///
/// @param lhs - The lhs `StdDuration` to compare
/// @param rhs - The rhs `StdDuration` to compare.
///
/// @return how `lhs` mathematically compares to `rhs`
/// @ingroup std_time
StdCompare std_duration_compare(StdDuration lhs, StdDuration rhs);

/// @brief Implements the allocator-unaware part of the `StdFormat` trait for `StdDuration`
///
/// @param self - The `StdDuration` to format, as its `StdFormat` trait representation
/// @param specifier - The format specifier representing how to format `self`. Should be unused/left
/// default in the format string
///
/// @return `self` formatted as a `StdString`
/// @ingroup std_time
StdString std_duration_format(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief Implements the allocator-aware part of the `StdFormat` trait for `StdDuration`
///
/// @param self - The `StdDuration` to format, as its `StdFormat` trait representation
/// @param specifier - The format specifier representing how to format `self`. Should be unused/left
/// default in the format string
/// @param allocator - The `StdAllocator` to allocate memory with
///
/// @return `self` formatted as a `StdString`
/// @ingroup std_time
StdString std_duration_format_with_allocator(const StdFormat* restrict self,
											 StdFormatSpecifier specifier,
											 StdAllocator allocator);

/// @brief Implements the `StdFormat` trait for `StdDuration`
/// @ingroup std_time
static maybe_unused
	ImplTraitFor(StdFormat, StdDuration, std_duration_format, std_duration_format_with_allocator);
#endif // STD_DURATION
