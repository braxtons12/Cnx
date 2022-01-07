/// @file StdTimePoint.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdTimePoint provides functionality for working with specific points in time
/// @version 0.1
/// @date 2022-01-06
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

#include <C2nxt/StdRatio.h>
#include <C2nxt/time/StdDuration.h>

/// @ingroup std_time
/// @{
/// @defgroup std_time_point StdTimePoint
/// `StdTimePoint` represents a specific point in time, since the UNIX epoch, represented in a
/// particular level of precision
/// @}


#ifndef STD_TIME_POINT
	#define STD_TIME_POINT

typedef struct StdClock StdClock;

/// @brief `StdTimePoint` represents a specific point in time, since the UNIX epoch, represented in
/// a particular level of precision
/// @ingroup std_time_point
typedef struct StdTimePoint {
	/// @brief The point in time, represented as a duration since the UNIX epoch
	/// @ingroup std_time_point
	StdDuration time_since_epoch;
	const StdClock* clock;
} StdTimePoint;

/// @brief Constructs a new `StdTimePoint` representing the given time since the UNIX epoch,
/// assuming the same precision as `std_system_clock`
///
/// @param time_since_epoch - The time since the UNIX epoch. The precision of the `StdTimePoint`
/// will be the period of this `StdDuration`
///
/// @return a `StdTimePoint` representing the given time since the epoch
/// @note if `time_since_epoch` has a different period than the precision of `std_system_clock`, it
/// will be converted to the precision of `std_system_clock`
/// @ingroup std_time_point
StdTimePoint std_time_point_new(StdDuration time_since_epoch);

/// @brief Constructs a new `StdTimePoint` representing the given time since the UNIX epoch,
/// with the same precision of the given `StdClock`
///
/// @param time_since_epoch - The time since the UNIX epoch. The precision of the `StdTimePoint`
/// will be the period of this `StdDuration`
///
/// @return a `StdTimePoint` representing the given time since the epoch
/// @note if `time_since_epoch` has a different period than the precision of `clock`, it will be
/// converted to the precision of `clock`
/// @ingroup std_time_point
StdTimePoint
std_time_point_new_with_clock(StdDuration time_since_epoch, const StdClock* restrict clock);

/// @brief Returns the time since the UNIX epoch of the given `StdTimePoint`
///
/// @param self - The `StdTimePoint` to get the represented time from
///
/// @return The time since the epoch represented by `self`, as a `StdDuration`
/// @ingroup std_time_point
StdDuration std_time_point_time_since_epoch(StdTimePoint self);

/// @brief Returns the minimum possible `StdTimePoint` with the same precision as the given one
///
/// @param self - The `StdTimePoint` with the desired precision
///
/// @return The minimum possible `StdTimePoint` with the same precision as the given one
/// @ingroup std_time_point
StdTimePoint std_time_point_min(StdTimePoint self);

/// @brief Returns the maximum possible `StdTimePoint` with the same precision as the given one
///
/// @param self - The `StdTimePoint` with the desired precision
///
/// @return The maximum possible `StdTimePoint` with the same precision as the given one
/// @ingroup std_time_point
StdTimePoint std_time_point_max(StdTimePoint self);

/// @brief Converts the first `StdTimePoint` to the same precision as the second one
///
/// @param to_cast - The `StdTimePoint` to convert the precision of
/// @param new_precision  - The `StdTimePoint` with the precision to convert to
///
/// @return `to_cast` converted to the precision of `new_precision`
/// @note The returned value is also associated with the `StdClock` from `new_precision`, not the
/// one from `to_cast`
/// @ingroup std_time_point
StdTimePoint std_time_point_cast(StdTimePoint to_cast, StdTimePoint new_precision);

/// @brief Converts the first `StdTimePoint` to the same precision as the second one, taking the
/// ceiling of any fractional part in the result
///
/// @param to_cast - The `StdTimePoint` to convert the precision of
/// @param new_precision  - The `StdTimePoint` with the precision to convert to
///
/// @return `to_cast` converted to the precision of `new_precision`
/// @note The returned value is also associated with the `StdClock` from `new_precision`, not the
/// one from `to_cast`
/// @ingroup std_time_point
StdTimePoint std_time_point_ceil(StdTimePoint to_cast, StdTimePoint new_precision);

/// @brief Converts the first `StdTimePoint` to the same precision as the second one, taking the
/// floor of any fractional part in the result
///
/// @param to_cast - The `StdTimePoint` to convert the precision of
/// @param new_precision  - The `StdTimePoint` with the precision to convert to
///
/// @return `to_cast` converted to the precision of `new_precision`
/// @note The returned value is also associated with the `StdClock` from `new_precision`, not the
/// one from `to_cast`
/// @ingroup std_time_point
StdTimePoint std_time_point_floor(StdTimePoint to_cast, StdTimePoint new_precision);

/// @brief Converts the first `StdTimePoint` to the same precision as the second one, rounding any
/// fractional part in the result
///
/// @param to_cast - The `StdTimePoint` to convert the precision of
/// @param new_precision  - The `StdTimePoint` with the precision to convert to
///
/// @return `to_cast` converted to the precision of `new_precision`
/// @note The returned value is also associated with the `StdClock` from `new_precision`, not the
/// one from `to_cast`
/// @ingroup std_time_point
StdTimePoint std_time_point_round(StdTimePoint to_cast, StdTimePoint new_precision);

/// @brief Increments the given `StdTimePoint` by one unit
///
/// @param self - The `StdTimePoint` to increment
/// @ingroup std_time_point
void std_time_point_increment(StdTimePoint* restrict self);
/// @brief Decrements the given `StdTimePoint` by one unit
///
/// @param self - The `StdTimePoint` to decrement
/// @ingroup std_time_point
void std_time_point_decrement(StdTimePoint* restrict self);

/// @brief Adds the given `StdDuration` to the given `StdTimePoint`
///
/// @param lhs - The `StdTimePoint` to add to
/// @param rhs - The amount of time to add to `lhs`
///
/// @return the result of the addition
/// @ingroup std_time_point
StdTimePoint std_time_point_add(StdTimePoint lhs, StdDuration rhs);
/// @brief Subtracts the given `StdDuration` from the given `StdTimePoint`
///
/// @param lhs - The `StdTimePoint` to subtract from
/// @param rhs - The amount of time to subtract from `lhs`
///
/// @return the result of the subtraction
/// @ingroup std_time_point
StdTimePoint std_time_point_subtract(StdTimePoint lhs, StdDuration rhs);
/// @brief Adds the rhs `StdTimePoint` to the lhs `StdTimePoint`
///
/// @param lhs - The `StdTimePoint` to add to
/// @param rhs - The amount of time to add to `lhs`
///
/// @return the result of the addition
/// @ingroup std_time_point
StdTimePoint std_time_point_add_time_point(StdTimePoint lhs, StdTimePoint rhs);
/// @brief Subtracts the rhs `StdDuration` from the lhs `StdTimePoint`
///
/// @param lhs - The `StdTimePoint` to subtract from
/// @param rhs - The amount of time to subtract from `lhs`
///
/// @return the result of the subtraction
/// @ingroup std_time_point
StdTimePoint std_time_point_subtract_time_point(StdTimePoint lhs, StdTimePoint rhs);
/// @brief Adds the given scalar value to the given `StdTimePoint` as if it were a `StdTimePoint`
/// or `StdDuration` in the same precision
///
/// @param lhs - The `StdTimePoint` to add to
/// @param rhs - The amount of time to add to `lhs`
///
/// @return the result of the addition
/// @ingroup std_time_point
StdTimePoint std_time_point_add_scalar(StdTimePoint lhs, i64 rhs);
/// @brief Subtracts the given scalar value from the given `StdTimePoint` as if it were a
/// `StdTimePoint` or `StdDuration` in the same precision
///
/// @param lhs - The `StdTimePoint` to add to
/// @param rhs - The amount of time to add to `lhs`
///
/// @return the result of the addition
/// @ingroup std_time_point
StdTimePoint std_time_point_subtract_scalar(StdTimePoint lhs, i64 rhs);

/// @brief Determines if the two `StdTimePoint`s are equivalent.
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `StdTimePoint` to compare
/// @param rhs - The right-hand-side `StdTimePoint` to compare
/// @return Whether the two `StdTimePoint`s are equivalent
/// @ingroup std_time_point
bool std_time_point_equal(StdTimePoint lhs, StdTimePoint rhs);
/// @brief Determines if the two `StdTimePoint`s are **NOT** equivalent.
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `StdTimePoint` to compare
/// @param rhs - The right-hand-side `StdTimePoint` to compare
/// @return Whether the two `StdTimePoint`s are **NOT** equivalent
/// @ingroup std_time_point
bool std_time_point_not_equal(StdTimePoint lhs, StdTimePoint rhs);
/// @brief Determines if the left-hand-side `StdTimePoint` is less than the right-hand-side
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `StdTimePoint` to compare
/// @param rhs - The right-hand-side `StdTimePoint` to compare
/// @return Whether `lhs < rhs`
/// @ingroup std_time_point
bool std_time_point_less_than(StdTimePoint lhs, StdTimePoint rhs);
/// @brief Determines if the left-hand-side `StdTimePoint` is less than or equal to the
/// right-hand-side.
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `StdTimePoint` to compare
/// @param rhs - The right-hand-side `StdTimePoint` to compare
/// @return Whether `lhs <= rhs`
/// @ingroup std_time_point
bool std_time_point_less_than_or_equal(StdTimePoint lhs, StdTimePoint rhs);
/// @brief Determines if the left-hand-side `StdTimePoint` is greater than the right-hand-side
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `StdTimePoint` to compare
/// @param rhs - The right-hand-side `StdTimePoint` to compare
/// @return Whether `lhs > rhs`
/// @ingroup std_time_point
bool std_time_point_greater_than(StdTimePoint lhs, StdTimePoint rhs);
/// @brief Determines if the left-hand-side `StdTimePoint` is greater than or equal to the
/// right-hand-side.
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `StdTimePoint` to compare
/// @param rhs - The right-hand-side `StdTimePoint` to compare
/// @return Whether `lhs >= rhs`
/// @ingroup std_time_point
bool std_time_point_greater_than_or_equal(StdTimePoint lhs, StdTimePoint rhs);
/// @brief Performs a three-way comparison of `lhs` to `rhs`, determining how `lhs` mathematically
/// relates to `rhs`.
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `StdTimePoint` to compare
/// @param rhs - The right-hand-side `StdTimePoint` to compare
/// @return How `lhs` compares to `rhs`, as `StdCompare`
/// @ingroup std_time_point
StdCompare std_time_point_compare(StdTimePoint lhs, StdTimePoint rhs);

/// @brief Implements the allocator-unaware part of the `StdFormat` trait for `StdTimePoint`
///
/// @param self - The `StdTimePoint` to format, as its `StdFormat` trait representation
/// @param specifier - The format specifier representing how to format `self`. Should be unused/left
/// default in the format string
///
/// @return `self` formatted as a `StdString`
/// @ingroup std_time_point
StdString std_time_point_format(const StdFormat* restrict self, StdFormatSpecifier specifier);
/// @brief Implements the allocator-aware part of the `StdFormat` trait for `StdTimePoint`
///
/// @param self - The `StdTimePoint` to format, as its `StdFormat` trait representation
/// @param specifier - The format specifier representing how to format `self`. Should be unused/left
/// default in the format string
/// @param allocator - The `StdAllocator` to allocate memory with
///
/// @return `self` formatted as a `StdString`
/// @ingroup std_time_point
StdString std_time_point_format_with_allocator(const StdFormat* restrict self,
											   StdFormatSpecifier specifier,
											   StdAllocator allocator);

/// @brief Implements the `StdFormat` trait for `StdTimePoint`
/// @ingroup std_time_point
static maybe_unused ImplTraitFor(StdFormat,
								 StdTimePoint,
								 std_time_point_format,
								 std_time_point_format_with_allocator);
#endif // STD_TIME_POINT
