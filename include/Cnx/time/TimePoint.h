/// @file TimePoint.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief CnxTimePoint provides functionality for working with specific points in time
/// @version 0.1.3
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

#include <Cnx/Ratio.h>
#include <Cnx/time/Duration.h>
#include <time.h>

/// @ingroup cnx_time
/// @{
/// @defgroup cnx_time_point CnxTimePoint
/// `CnxTimePoint` represents a specific point in time, since the UNIX epoch, represented in a
/// particular level of precision
/// @}

#ifndef CNX_TIME_POINT
	#define CNX_TIME_POINT

typedef struct CnxClock CnxClock;

typedef struct tm tm;

/// @brief Indicates whether a `CnxTimePoint` is in UTC time, local time, or an unknown locale
/// @ingroup cnx_time_point
typedef enum CnxTimePointLocale {
	CNX_UTC_TIME = 0,
	CNX_LOCAL_TIME,
	CNX_UNKNOWN_TIME
} CnxTimePointLocale;

/// @brief `CnxTimePoint` represents a specific point in time, since the UNIX epoch, represented in
/// a particular level of precision
/// @ingroup cnx_time_point
typedef struct CnxTimePoint {
	/// @brief The point in time, represented as a duration since the UNIX epoch
	/// @ingroup cnx_time_point
	CnxDuration time_since_epoch;
	/// @brief The clock associated with this time point
	/// @ingroup cnx_time_point
	const CnxClock* clock;
	CnxTimePointLocale locale;
} CnxTimePoint;

/// @brief Constructs a new `CnxTimePoint` representing the given time since the UNIX epoch,
/// assuming the same precision as `cnx_system_clock`
///
/// @param time_since_epoch - The time since the UNIX epoch. The precision of the `CnxTimePoint`
/// will be the period of this `CnxDuration`
///
/// @return a `CnxTimePoint` representing the given time since the epoch
/// @note if `time_since_epoch` has a different period than the precision of `cnx_system_clock`, it
/// will be converted to the precision of `cnx_system_clock`
/// @ingroup cnx_time_point
__attr(nodiscard) CnxTimePoint cnx_time_point_new(CnxDuration time_since_epoch);

/// @brief Constructs a new `CnxTimePoint` representing the given time since the UNIX epoch,
/// with the same precision of the given `CnxClock`
///
/// @param time_since_epoch - The time since the UNIX epoch. The precision of the `CnxTimePoint`
/// will be the period of this `CnxDuration`
/// @param clock - The clock associated with this time point
///
/// @return a `CnxTimePoint` representing the given time since the epoch
/// @note if `time_since_epoch` has a different period than the precision of `clock`, it will be
/// converted to the precision of `clock`
/// @ingroup cnx_time_point
__attr(nodiscard) __attr(not_null(2)) CnxTimePoint
	cnx_time_point_new_with_clock(CnxDuration time_since_epoch, const CnxClock* restrict clock)
		cnx_disable_if(!clock, "Can't create a CnxTimePoint with a nullptr for clock");

/// @brief Constructs a new `CnxTimePoint` representing the given time since the UNIX epoch,
/// with the same precision of the given `CnxClock`
///
/// @param time_since_epoch - The time since the UNIX epoch. The precision of the `CnxTimePoint`
/// will be the period of this `CnxDuration`
/// @param clock - The clock associated with this time point
/// @param locale - Whether this time point is in UTC time, local time, or some unknown time locale
///
/// @return a `CnxTimePoint` representing the given time since the epoch
/// @note if `time_since_epoch` has a different period than the precision of `clock`, it will be
/// converted to the precision of `clock`
/// @ingroup cnx_time_point
__attr(nodiscard) __attr(not_null(2)) CnxTimePoint
	cnx_time_point_new_with_clock_and_locale(CnxDuration time_since_epoch,
											 const CnxClock* restrict clock,
											 CnxTimePointLocale locale)
		cnx_disable_if(!clock, "Can't create a CnxTimePoint with a nullptr for clock");

/// @brief Returns the time since the UNIX epoch of the given `CnxTimePoint`
///
/// @param self - The `CnxTimePoint` to get the represented time from
///
/// @return The time since the epoch represented by `self`, as a `CnxDuration`
/// @ingroup cnx_time_point
__attr(nodiscard) CnxDuration cnx_time_point_time_since_epoch(CnxTimePoint self);

/// @brief Returns the minimum possible `CnxTimePoint` with the same precision as the given one
///
/// @param self - The `CnxTimePoint` with the desired precision
///
/// @return The minimum possible `CnxTimePoint` with the same precision as the given one
/// @ingroup cnx_time_point
__attr(nodiscard) CnxTimePoint cnx_time_point_min(CnxTimePoint self);

/// @brief Returns the maximum possible `CnxTimePoint` with the same precision as the given one
///
/// @param self - The `CnxTimePoint` with the desired precision
///
/// @return The maximum possible `CnxTimePoint` with the same precision as the given one
/// @ingroup cnx_time_point
__attr(nodiscard) CnxTimePoint cnx_time_point_max(CnxTimePoint self);

/// @brief Converts the first `CnxTimePoint` to the same precision as the second one
///
/// @param to_cast - The `CnxTimePoint` to convert the precision of
/// @param new_precision  - The `CnxTimePoint` with the precision to convert to
///
/// @return `to_cast` converted to the precision of `new_precision`
/// @note The returned value is also associated with the `CnxClock` from `new_precision`, not the
/// one from `to_cast`
/// @ingroup cnx_time_point
__attr(nodiscard) CnxTimePoint
	cnx_time_point_cast(CnxTimePoint to_cast, CnxTimePoint new_precision);

/// @brief Converts the first `CnxTimePoint` to the same precision as the second one, taking the
/// ceiling of any fractional part in the result
///
/// @param to_cast - The `CnxTimePoint` to convert the precision of
/// @param new_precision  - The `CnxTimePoint` with the precision to convert to
///
/// @return `to_cast` converted to the precision of `new_precision`
/// @note The returned value is also associated with the `CnxClock` from `new_precision`, not the
/// one from `to_cast`
/// @ingroup cnx_time_point
__attr(nodiscard) CnxTimePoint
	cnx_time_point_ceil(CnxTimePoint to_cast, CnxTimePoint new_precision);

/// @brief Converts the first `CnxTimePoint` to the same precision as the second one, taking the
/// floor of any fractional part in the result
///
/// @param to_cast - The `CnxTimePoint` to convert the precision of
/// @param new_precision  - The `CnxTimePoint` with the precision to convert to
///
/// @return `to_cast` converted to the precision of `new_precision`
/// @note The returned value is also associated with the `CnxClock` from `new_precision`, not the
/// one from `to_cast`
/// @ingroup cnx_time_point
__attr(nodiscard) CnxTimePoint
	cnx_time_point_floor(CnxTimePoint to_cast, CnxTimePoint new_precision);

/// @brief Converts the first `CnxTimePoint` to the same precision as the second one, rounding any
/// fractional part in the result
///
/// @param to_cast - The `CnxTimePoint` to convert the precision of
/// @param new_precision  - The `CnxTimePoint` with the precision to convert to
///
/// @return `to_cast` converted to the precision of `new_precision`
/// @note The returned value is also associated with the `CnxClock` from `new_precision`, not the
/// one from `to_cast`
/// @ingroup cnx_time_point
__attr(nodiscard) CnxTimePoint
	cnx_time_point_round(CnxTimePoint to_cast, CnxTimePoint new_precision);

/// @brief Converts the given `CnxTimePoint` to `time_t`
///
/// @param to_cast - The `CnxTimePoint` to convert
///
/// @return `to_cast` converted to `time_t`
/// @ingroup cnx_time_point
__attr(nodiscard) time_t cnx_time_point_as_time_t(CnxTimePoint to_cast);

	#define RESULT_DECL TRUE
	#define RESULT_T	tm
	#include <Cnx/Result.h>
	#undef RESULT_T
	#undef RESULT_DECL

/// @brief Converts the given `CnxTimePoint` to `tm`
///
/// @param to_cast - The `CnxTimePoint` to convert
///
/// @return `to_cast` converted to `tm`
/// @ingroup cnx_time_point
CnxResult(tm) cnx_time_point_as_tm(CnxTimePoint to_cast);

/// @brief Converts the given `time_t` to a `CnxTimePoint` in the system clock precision
///
/// @param time - The `time_t` to convert
///
/// @return `time` converted to a `CnxTimePoint`
/// @ingroup cnx_time_point
CnxTimePoint cnx_time_point_from_time_t(time_t time);

/// @brief Increments the given `CnxTimePoint` by one unit
///
/// @param self - The `CnxTimePoint` to increment
/// @ingroup cnx_time_point
void cnx_time_point_increment(CnxTimePoint* restrict self);
/// @brief Decrements the given `CnxTimePoint` by one unit
///
/// @param self - The `CnxTimePoint` to decrement
/// @ingroup cnx_time_point
void cnx_time_point_decrement(CnxTimePoint* restrict self);

/// @brief Adds the given `CnxDuration` to the given `CnxTimePoint`
///
/// @param lhs - The `CnxTimePoint` to add to
/// @param rhs - The amount of time to add to `lhs`
///
/// @return the result of the addition
/// @ingroup cnx_time_point
CnxTimePoint cnx_time_point_add(CnxTimePoint lhs, CnxDuration rhs);
/// @brief Subtracts the given `CnxDuration` from the given `CnxTimePoint`
///
/// @param lhs - The `CnxTimePoint` to subtract from
/// @param rhs - The amount of time to subtract from `lhs`
///
/// @return the result of the subtraction
/// @ingroup cnx_time_point
CnxTimePoint cnx_time_point_subtract(CnxTimePoint lhs, CnxDuration rhs);
/// @brief Adds the rhs `CnxTimePoint` to the lhs `CnxTimePoint`
///
/// @param lhs - The `CnxTimePoint` to add to
/// @param rhs - The amount of time to add to `lhs`
///
/// @return the result of the addition
/// @ingroup cnx_time_point
CnxTimePoint cnx_time_point_add_time_point(CnxTimePoint lhs, CnxTimePoint rhs);
/// @brief Subtracts the rhs `CnxDuration` from the lhs `CnxTimePoint`
///
/// @param lhs - The `CnxTimePoint` to subtract from
/// @param rhs - The amount of time to subtract from `lhs`
///
/// @return the result of the subtraction
/// @ingroup cnx_time_point
CnxTimePoint cnx_time_point_subtract_time_point(CnxTimePoint lhs, CnxTimePoint rhs);
/// @brief Adds the given scalar value to the given `CnxTimePoint` as if it were a `CnxTimePoint`
/// or `CnxDuration` in the same precision
///
/// @param lhs - The `CnxTimePoint` to add to
/// @param rhs - The amount of time to add to `lhs`
///
/// @return the result of the addition
/// @ingroup cnx_time_point
CnxTimePoint cnx_time_point_add_scalar(CnxTimePoint lhs, i64 rhs);
/// @brief Subtracts the given scalar value from the given `CnxTimePoint` as if it were a
/// `CnxTimePoint` or `CnxDuration` in the same precision
///
/// @param lhs - The `CnxTimePoint` to add to
/// @param rhs - The amount of time to add to `lhs`
///
/// @return the result of the addition
/// @ingroup cnx_time_point
CnxTimePoint cnx_time_point_subtract_scalar(CnxTimePoint lhs, i64 rhs);

/// @brief Determines if the two `CnxTimePoint`s are equivalent.
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `CnxTimePoint` to compare
/// @param rhs - The right-hand-side `CnxTimePoint` to compare
/// @return Whether the two `CnxTimePoint`s are equivalent
/// @ingroup cnx_time_point
bool cnx_time_point_equal(CnxTimePoint lhs, CnxTimePoint rhs);
/// @brief Determines if the two `CnxTimePoint`s are **NOT** equivalent.
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `CnxTimePoint` to compare
/// @param rhs - The right-hand-side `CnxTimePoint` to compare
/// @return Whether the two `CnxTimePoint`s are **NOT** equivalent
/// @ingroup cnx_time_point
bool cnx_time_point_not_equal(CnxTimePoint lhs, CnxTimePoint rhs);
/// @brief Determines if the left-hand-side `CnxTimePoint` is less than the right-hand-side
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `CnxTimePoint` to compare
/// @param rhs - The right-hand-side `CnxTimePoint` to compare
/// @return Whether `lhs < rhs`
/// @ingroup cnx_time_point
bool cnx_time_point_less_than(CnxTimePoint lhs, CnxTimePoint rhs);
/// @brief Determines if the left-hand-side `CnxTimePoint` is less than or equal to the
/// right-hand-side.
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `CnxTimePoint` to compare
/// @param rhs - The right-hand-side `CnxTimePoint` to compare
/// @return Whether `lhs <= rhs`
/// @ingroup cnx_time_point
bool cnx_time_point_less_than_or_equal(CnxTimePoint lhs, CnxTimePoint rhs);
/// @brief Determines if the left-hand-side `CnxTimePoint` is greater than the right-hand-side
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `CnxTimePoint` to compare
/// @param rhs - The right-hand-side `CnxTimePoint` to compare
/// @return Whether `lhs > rhs`
/// @ingroup cnx_time_point
bool cnx_time_point_greater_than(CnxTimePoint lhs, CnxTimePoint rhs);
/// @brief Determines if the left-hand-side `CnxTimePoint` is greater than or equal to the
/// right-hand-side.
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `CnxTimePoint` to compare
/// @param rhs - The right-hand-side `CnxTimePoint` to compare
/// @return Whether `lhs >= rhs`
/// @ingroup cnx_time_point
bool cnx_time_point_greater_than_or_equal(CnxTimePoint lhs, CnxTimePoint rhs);
/// @brief Performs a three-way comparison of `lhs` to `rhs`, determining how `lhs` mathematically
/// relates to `rhs`.
/// Converts to the precision of the most precise of the two prior to comparison.
///
/// @param lhs - The left-hand-side `CnxTimePoint` to compare
/// @param rhs - The right-hand-side `CnxTimePoint` to compare
/// @return How `lhs` compares to `rhs`, as `CnxCompare`
/// @ingroup cnx_time_point
CnxCompare cnx_time_point_compare(CnxTimePoint lhs, CnxTimePoint rhs);

/// @brief Implements the allocator-unaware part of the `CnxFormat` trait for `CnxTimePoint`
///
/// @param self - The `CnxTimePoint` to format, as its `CnxFormat` trait representation
/// @param specifier - The format specifier representing how to format `self`. Should be unused/left
/// default in the format string
///
/// @return `self` formatted as a `CnxString`
/// @ingroup cnx_time_point
CnxString cnx_time_point_format(const CnxFormat* restrict self, CnxFormatSpecifier specifier);
/// @brief Implements the allocator-aware part of the `CnxFormat` trait for `CnxTimePoint`
///
/// @param self - The `CnxTimePoint` to format, as its `CnxFormat` trait representation
/// @param specifier - The format specifier representing how to format `self`. Should be unused/left
/// default in the format string
/// @param allocator - The `CnxAllocator` to allocate memory with
///
/// @return `self` formatted as a `CnxString`
/// @ingroup cnx_time_point
CnxString cnx_time_point_format_with_allocator(const CnxFormat* restrict self,
											   CnxFormatSpecifier specifier,
											   CnxAllocator allocator);

/// @brief Implements the `CnxFormat` trait for `CnxTimePoint`
/// @ingroup cnx_time_point
__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 CnxTimePoint,
										 cnx_time_point_format,
										 cnx_time_point_format_with_allocator);
#endif // CNX_TIME_POINT
