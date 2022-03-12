/// @file StdClock.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides methods for operating with system clocks
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

#include <C2nxt/time/StdDuration.h>
#include <C2nxt/time/StdTimePoint.h>

/// @ingroup std_time
/// @{
/// @defgroup std_clock StdClock
/// The `StdClock` module provides types and functions for working with system, local, and global
/// clocks. It provides functionality similar to C++'s `std::chrono` clocks and makes cross-platform
/// time-related operations simple and straight forward. Access to and functionality of the realtime
/// system clock, a high-resolution monotonic clock, a realtime UTC clock, and realtime local time
/// clock are all provided.
///
/// The system clock (`std_system_clock`) and related functions always operate in local machine
/// time, operating relative to the UNIX epoch. On Windows, machine time is traditionally in local
/// time. On other platforms, it is usually in UTC.
///
/// The steady clock (or monotonic clock, `std_steady_clock`) is a high-resolution monotonic clock
/// suitable for performance, timing, metrics, etc. It is only available if a monotonic clock is
/// available and supported on the being-compiled-for platform (`STD_NO_MONOTONIC_CLOCK` must be
/// false).
///
/// The generic high resolution clock (`std_high_resolution_clock`) is the highest resolution clock
/// available and supported on the system. If `std_steady_clock` is available,
/// `std_high_resolution_clock` will be equivalent to it. Otherwise, it will be `std_system_clock`.
/// `std_high_resolution_clock` is provided for convenience, but because of the uncertainty of what
/// clock it will actually be if perfectly accurate timing is necessary, it is preferred to check
/// for `std_steady_clock` availability explicitly.
///
/// The UTC clock (`std_utc_clock`) and related functions operate in UTC mean time, operating
/// relative to the UNIX epoch.
///
/// The local time clock (`std_local_clock`) and related functions operate in local timezone time,
/// operating relative to the UNIX epoch.
///
/// Example:
/// @code {.c}
/// #include <C2nxt/time/StdClock.h>
/// #include <C2nxt/StdIO.h>
///
///	// print the local time formatted in ISO 8601 format (YYYY-MM-DD|H:M:S+-hh:mm))
/// void local_time_example(void) {
///		let current_time = std_clock_now(&std_local_clock);
///		println("{}", as_format_t(StdTimePoint, current_time));
/// }
///
/// // time a function call and print the duration in debug format
/// void time_to_call_example(void) {
///		// `std_steady_clock` is only available if a monotonic clock is available
///		#if STD_NO_MONOTONIC_CLOCK
///			let timer_clock_ptr = &std_system_clock;
///		#else
///			let timer_clock_ptr = &std_steady_clock;
///		#endif
///		// at this point, `timer_clock_ptr` is equivalent to `&std_high_resolution_clock`.
///		// On platforms when one is available, `std_high_resolution_clock` will be the monotonic
///		// `std_steady_clock`, otherwise it will be `std_system_clock`
/// 	let start_time = std_clock_now(timer_clock_ptr);
/// 	local_time_example();
/// 	let end_time = std_clock_now(timer_clock_ptr);
///		let difference = std_time_point_subtract(end_time, start_time).time_since_epoch;
///		println("Took {D} to call local_time_example()", difference);
/// }
///
/// // print the current UTC time formatted in ISO 8601 format (YYYY-MM-DD|H:M:S+-hh:mm))
/// void utc_time_example(void) {
///		let current_utc_time = std_clock_now(&std_utc_clock);
///		println("{}", as_format_t(StdTimePoint, current_utc_time));
/// }
///
/// // print the current system time formatted in ISO 8601 format (YYYY-MM-DD|H:M:S+-hh:mm))
/// // (This should be local time on Windows, UTC time on others)
///	void system_time_example(void) {
///		let current_system_time = std_clock_now(&std_system_clock);
///		println("{}", as_format_t(StdTimePoint, current_system_time));
/// }
/// @endcode
/// @}

#ifndef STD_CLOCK
	#define STD_CLOCK

/// @def STD_NO_MONOTONIC_CLOCK
/// @brief If `STD_NO_MONOTONIC_CLOCK` is true, then either C2nxt doesn't support a monotonic clock
/// on the being-compiled-for-platform, the being-compiled-for platform itself doesn't, or both, and
/// std_steady_clock and related functions are not available
/// @ingroup std_clock

	#if !STD_PLATFORM_APPLE && !STD_PLATFORM_WINDOWS && !STD_PLATFORM_ZOS \
		&& !defined(CLOCK_MONOTONIC)
		#warn Monotonic clock not supported on this platform
		#define STD_NO_MONOTONIC_CLOCK 1
	#else
		#define STD_NO_MONOTONIC_CLOCK 0
	#endif // !STD_PLATFORM_APPLE && !STD_PLATFORM_WINDOWS && !STD_PLATFORM_ZOS \
		   // && !defined(CLOCK_MONOTONIC)

/// @brief `StdClockResolution` lists the valid resolutions for `StdClock` compatible clocks
/// @ingroup std_clock
typedef enum StdClockResolution {
	STD_CLOCK_NANOSECONDS,
	STD_CLOCK_MICROSECONDS,
	STD_CLOCK_MILLISECONDS,
	STD_CLOCK_SECONDS,
} StdClockResolution;

/// @struct StdClock
/// @brief The `StdClock` trait defines the interface that must be implemented by any
/// C2nxt-compatible clock. It intentionally mirrors the API of C++'s standard clock types, along
/// with some additional functionality
/// @ingroup std_clock
Trait(StdClock,
	  /// @brief Returns a `StdTimePoint` corresponding to the current time on the given clock
	  ///
	  /// @param self - The `StdClock` to retrieve the current time from
	  ///
	  /// @return The current time on `self`
	  /// @ingroup std_clock
	  StdTimePoint (*const now)(const StdClock* restrict self);
	  /// @brief Returns the minimum possible `StdTimePoint` able to be associated with the given
	  /// clock
	  ///
	  /// @param self - The `StdClock` to get the minimum possible associatable `StdTimePoint` of
	  ///
	  /// @return The minimum `StdTimePoint` associatable with `self`
	  /// @ingroup std_clock
	  StdTimePoint(*const min_time_point)(const StdClock* restrict self);
	  /// @brief Returns the maximum possible `StdTimePoint` able to be associated with the given
	  /// clock
	  ///
	  /// @param self - The `StdClock` to get the maximum possible associatable `StdTimePoint` of
	  ///
	  /// @return The minimum `StdTimePoint` associatable with `self`
	  /// @ingroup std_clock
	  StdTimePoint(*const max_time_point)(const StdClock* restrict self);
	  /// @brief Returns the resolution of the given clock
	  ///
	  /// @param self - The `StdClock` to get the resolution of
	  ///
	  /// @return The resolution of `self` as a `StdClockResolution`
	  /// @ingroup std_clock
	  StdClockResolution(*const resolution)(const StdClock* restrict self);
	  /// @brief Returns the resolution of the given clock
	  ///
	  /// @param self - The `StdClock` to get the resolution of
	  ///
	  /// @return The resolution of `self` as a `StdRatio` relative to seconds
	  /// @ingroup std_clock
	  StdRatio(*const resolution_as_ratio)(const StdClock* restrict self);
	  /// @brief Returns the locale associated with the given clock
	  ///
	  /// @param self - The `StdClock` to get the locale of
	  ///
	  /// @return The locale of `self`
	  /// @ingroup std_clock
	  StdTimePointLocale(*const locale)(const StdClock* restrict self);
	  /// @brief Returns the text representation of the given clock
	  ///
	  /// @param self - The `StdClock` to get the text representation of
	  ///
	  /// @return The text representation of `self` as a `StdString`
	  /// @ingroup std_clock
	  StdString(*const format)(const StdClock* restrict self);
	  /// @brief Returns the text representation of the given clock, allocating necessary memory
	  /// with the given `StdAllocator`
	  ///
	  /// @param self - The `StdClock` to get the text representation of
	  /// @param allocator - The `StdAllocator` to use for any necessary memory allocation
	  ///
	  /// @return The text representation of `self` as a `StdString`
	  /// @ingroup std_clock
	  StdString(*const format_with_allocator)(const StdClock* restrict self,
											  StdAllocator allocator););

	#define ___DISABLE_IF_NULL(self) \
		std_disable_if(!(self), "Can't perform a clock operation with a nullptr")

/// @brief Returns a `StdTimePoint` corresponding to the current time on the given clock
///
/// @param self - The `StdClock` to retrieve the current time from
///
/// @return The current time on `self`
/// @ingroup std_clock
[[nodiscard]] [[not_null(1)]] StdTimePoint
std_clock_now(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the minimum possible `StdTimePoint` able to be associated with the given
/// clock
///
/// @param self - The `StdClock` to get the minimum possible associatable `StdTimePoint` of
///
/// @return The minimum `StdTimePoint` associatable with `self`
/// @ingroup std_clock
[[nodiscard]] [[not_null(1)]] StdTimePoint
std_clock_min_time_point(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the maximum possible `StdTimePoint` able to be associated with the given
/// clock
///
/// @param self - The `StdClock` to get the maximum possible associatable `StdTimePoint` of
///
/// @return The minimum `StdTimePoint` associatable with `self`
/// @ingroup std_clock
[[nodiscard]] [[not_null(1)]] StdTimePoint
std_clock_max_time_point(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the resolution of the given clock
///
/// @param self - The `StdClock` to get the resolution of
///
/// @return The resolution of `self` as a `StdClockResolution`
/// @ingroup std_clock
[[nodiscard]] [[not_null(1)]] StdClockResolution
std_clock_resolution(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the resolution of the given clock
///
/// @param self - The `StdClock` to get the resolution of
///
/// @return The resolution of `self` as a `StdRatio` relative to seconds
/// @ingroup std_clock
[[nodiscard]] [[not_null(1)]] StdRatio
std_clock_resolution_as_ratio(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the locale associated with the given clock
///
/// @param self - The `StdClock` to get the locale of
///
/// @return The locale of `self`
/// @ingroup std_clock
[[nodiscard]] [[not_null(1)]] StdTimePointLocale
std_clock_locale(const StdClock* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Implements the allocator un-aware portion of the `StdFormat` trait for all `StdClock`s
///
/// @param self - The `StdFormat` implementor to get the text representation of
/// @param specifier - The `StdFormatSpecifier` specifying how to format the text
///
/// @return The text representation of `self` as a `StdString`
/// @ingroup std_clock
[[nodiscard]] [[not_null(1)]] StdString
std_clock_format(const StdFormat* restrict self, StdFormatSpecifier specifier)
	___DISABLE_IF_NULL(self);
/// @brief Implements the allocator aware portion of the `StdFormat` trait for all `StdClock`s
///
/// @param self - The `StdFormat` implementor to get the text representation of
/// @param specifier - The `StdFormatSpecifier` specifying how to format the text
/// @param allocator - The `StdAllocator` to use for any necessary memory allocation
///
/// @return The text representation of `self` as a `StdString`
/// @ingroup std_clock
[[nodiscard]] [[not_null(1)]] StdString
std_clock_format_with_allocator(const StdFormat* restrict self,
								StdFormatSpecifier specifier,
								StdAllocator allocator) ___DISABLE_IF_NULL(self);

/// @brief Implements the `StdFormat` trait for `StdClock`
/// @ingroup std_clock
[[maybe_unused]] static ImplTraitFor(StdFormat,
									 StdClock,
									 std_clock_format,
									 std_clock_format_with_allocator);

/// @brief Returns a `StdTimePoint` corresponding to the current time on the system clock
///
/// @return The current time on the system clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_system_clock_now(void);
/// @brief Returns the minimum possible `StdTimePoint` able to be associated with the system
/// clock
///
/// @return The minimum `StdTimePoint` associatable with the system clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_system_clock_min_time_point(void);
/// @brief Returns the maximum possible `StdTimePoint` able to be associated with the system
/// clock
///
/// @return The maximum `StdTimePoint` associatable with the system clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_system_clock_max_time_point(void);
/// @brief Returns the resolution of the system clock
///
/// @return The resolution of the system clock as a `StdClockResolution`
/// @ingroup std_clock
[[nodiscard]] StdClockResolution std_system_clock_resolution(void);
/// @brief Returns the resolution of the system clock
///
/// @return The resolution of the system clock as a `StdRatio` relative to seconds
/// @ingroup std_clock
[[nodiscard]] StdRatio std_system_clock_resolution_as_ratio(void);
/// @brief Returns the locale of the system clock
///
/// @return The locale of the system clock
/// @ingroup std_clock
[[nodiscard]] StdTimePointLocale std_system_clock_locale(void);

	#if !STD_NO_MONOTONIC_CLOCK
/// @brief Returns a `StdTimePoint` corresponding to the current time on the steady clock
///
/// @return The current time on the steady clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_steady_clock_now(void);
/// @brief Returns the minimum possible `StdTimePoint` able to be associated with the steady
/// clock
///
/// @return The minimum `StdTimePoint` associatable with the steady clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_steady_clock_min_time_point(void);
/// @brief Returns the maximum possible `StdTimePoint` able to be associated with the steady
/// clock
///
/// @return The maximum `StdTimePoint` associatable with the steady clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_steady_clock_max_time_point(void);
/// @brief Returns the resolution of the steady clock
///
/// @return The resolution of the steady clock as a `StdClockResolution`
/// @ingroup std_clock
[[nodiscard]] StdClockResolution std_steady_clock_resolution(void);
/// @brief Returns the resolution of the steady clock
///
/// @return The resolution of the steady clock as a `StdRatio` relative to seconds
/// @ingroup std_clock
[[nodiscard]] StdRatio std_steady_clock_resolution_as_ratio(void);
/// @brief Returns the locale of the steady clock
///
/// @return The locale of the steady clock
/// @ingroup std_clock
[[nodiscard]] StdTimePointLocale std_steady_clock_locale(void);
	#endif // !STD_NO_MONOTONIC_CLOCK

/// @brief Returns a `StdTimePoint` corresponding to the current time on the high resolution clock
///
/// @return The current time on the high resolution clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_high_resolution_clock_now(void);
/// @brief Returns the minimum possible `StdTimePoint` able to be associated with the high
/// resolution clock
///
/// @return The minimum `StdTimePoint` associatable with the high resolution clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_high_resolution_clock_min_time_point(void);
/// @brief Returns the maximum possible `StdTimePoint` able to be associated with the high
/// resolution clock
///
/// @return The maximum `StdTimePoint` associatable with the high resolution clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_high_resolution_clock_max_time_point(void);
/// @brief Returns the resolution of the high resolution clock
///
/// @return The resolution of the high resolution clock as a `StdClockResolution`
/// @ingroup std_clock
[[nodiscard]] StdClockResolution std_high_resolution_clock_resolution(void);
/// @brief Returns the resolution of the high resolution clock
///
/// @return The resolution of the high resolution clock as a `StdRatio` relative to seconds
/// @ingroup std_clock
[[nodiscard]] StdRatio std_high_resolution_clock_resolution_as_ratio(void);
/// @brief Returns the locale of the high resolution clock
///
/// @return The locale of the high resolution clock
/// @ingroup std_clock
[[nodiscard]] StdTimePointLocale std_high_resolution_clock_locale(void);

/// @brief Returns a `StdTimePoint` corresponding to the current time on the UTC clock
///
/// @return The current time on the UTC clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_utc_clock_now(void);
/// @brief Returns the minimum possible `StdTimePoint` able to be associated with the UTC
/// clock
///
/// @return The minimum `StdTimePoint` associatable with the UTC clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_utc_clock_min_time_point(void);
/// @brief Returns the maximum possible `StdTimePoint` able to be associated with the UTC
/// clock
///
/// @return The maximum `StdTimePoint` associatable with the UTC clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_utc_clock_max_time_point(void);
/// @brief Returns the resolution of the UTC clock
///
/// @return The resolution of the UTC clock as a `StdClockResolution`
/// @ingroup std_clock
[[nodiscard]] StdClockResolution std_utc_clock_resolution(void);
/// @brief Returns the resolution of the UTC clock
///
/// @return The resolution of the UTC clock as a `StdRatio` relative to seconds
/// @ingroup std_clock
[[nodiscard]] StdRatio std_utc_clock_resolution_as_ratio(void);
/// @brief Returns the locale of the UTC clock
///
/// @return The locale of the UTC clock
/// @ingroup std_clock
[[nodiscard]] StdTimePointLocale std_utc_clock_locale(void);

/// @brief Returns a `StdTimePoint` corresponding to the current time on the local clock
///
/// @return The current time on the local clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_local_clock_now(void);
/// @brief Returns the minimum possible `StdTimePoint` able to be associated with the local
/// clock
///
/// @return The minimum `StdTimePoint` associatable with the local clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_local_clock_min_time_point(void);
/// @brief Returns the maximum possible `StdTimePoint` able to be associated with the local
/// clock
///
/// @return The maximum `StdTimePoint` associatable with the local clock
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_local_clock_max_time_point(void);
/// @brief Returns the resolution of the local clock
///
/// @return The resolution of the local clock as a `StdClockResolution`
/// @ingroup std_clock
[[nodiscard]] StdClockResolution std_local_clock_resolution(void);
/// @brief Returns the resolution of the local clock
///
/// @return The resolution of the local clock as a `StdRatio` relative to seconds
/// @ingroup std_clock
[[nodiscard]] StdRatio std_local_clock_resolution_as_ratio(void);
/// @brief Returns the locale of the local clock
///
/// @return The locale of the local clock
/// @ingroup std_clock
[[nodiscard]] StdTimePointLocale std_local_clock_locale(void);

/// @brief Converts the given `StdTimePoint` in UTC time to a `StdTimePoint` in local time
///
/// @param utc - The time point to convert to local time
///
/// @return `utc` converted to local time
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_convert_utc_to_local_time(StdTimePoint utc);
/// @brief Converts the given `StdTimePoint` in local time to a `StdTimePoint` in UTC time
///
/// @param local_time - The time point to convert to UTC time
///
/// @return `local_time` converted to UTC time
/// @ingroup std_clock
[[nodiscard]] StdTimePoint std_convert_local_time_to_utc(StdTimePoint local_time);

IGNORE_RESERVED_IDENTIFIER_WARNING_START

[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_system_clock_now(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_system_clock_min_time_point(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_system_clock_max_time_point(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdClockResolution
__std_system_clock_resolution(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRatio
__std_system_clock_resolution_as_ratio(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePointLocale
__std_system_clock_locale(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdString
__std_system_clock_format(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdString
__std_system_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator)
	___DISABLE_IF_NULL(self);

	#if !STD_NO_MONOTONIC_CLOCK
[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_steady_clock_now(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_steady_clock_min_time_point(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_steady_clock_max_time_point(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdClockResolution
__std_steady_clock_resolution(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRatio
__std_steady_clock_resolution_as_ratio(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePointLocale
__std_steady_clock_locale(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdString
__std_steady_clock_format(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdString
__std_steady_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator)
	___DISABLE_IF_NULL(self);
	#endif // !STD_NO_MONOTONIC_CLOCK

[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_utc_clock_now(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_utc_clock_min_time_point(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_utc_clock_max_time_point(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdClockResolution
__std_utc_clock_resolution(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRatio
__std_utc_clock_resolution_as_ratio(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePointLocale
__std_utc_clock_locale(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdString
__std_utc_clock_format(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdString
__std_utc_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator)
	___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_local_clock_now(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_local_clock_min_time_point(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePoint
__std_local_clock_max_time_point(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdClockResolution
__std_local_clock_resolution(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRatio
__std_local_clock_resolution_as_ratio(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdTimePointLocale
__std_local_clock_locale(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdString
__std_local_clock_format(const StdClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdString
__std_local_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator)
	___DISABLE_IF_NULL(self);

[[maybe_unused]] static ImplTraitFor(StdClock,
									 __StdSystemClock,
									 __std_system_clock_now,
									 __std_system_clock_min_time_point,
									 __std_system_clock_max_time_point,
									 __std_system_clock_resolution,
									 __std_system_clock_resolution_as_ratio,
									 __std_system_clock_locale,
									 __std_system_clock_format,
									 __std_system_clock_format_with_allocator);

	#if !STD_NO_MONOTONIC_CLOCK
[[maybe_unused]] static ImplTraitFor(StdClock,
									 __StdSteadyClock,
									 __std_steady_clock_now,
									 __std_steady_clock_min_time_point,
									 __std_steady_clock_max_time_point,
									 __std_steady_clock_resolution,
									 __std_steady_clock_resolution_as_ratio,
									 __std_steady_clock_locale,
									 __std_steady_clock_format,
									 __std_steady_clock_format_with_allocator);
	#endif // !STD_NO_MONOTONIC_CLOCK

	#if !STD_NO_MONOTONIC_CLOCK
[[maybe_unused]] static ImplTraitFor(StdClock,
									 __StdHighResolutionClock,
									 __std_steady_clock_now,
									 __std_steady_clock_min_time_point,
									 __std_steady_clock_max_time_point,
									 __std_steady_clock_resolution,
									 __std_steady_clock_resolution_as_ratio,
									 __std_steady_clock_locale,
									 __std_steady_clock_format,
									 __std_steady_clock_format_with_allocator);
	#else
[[maybe_unused]] static ImplTraitFor(StdClock,
									 __StdHighResolutionClock,
									 __std_system_clock_now,
									 __std_system_clock_min_time_point,
									 __std_system_clock_max_time_point,
									 __std_system_clock_resolution,
									 __std_system_clock_resolution_as_ratio,
									 __std_system_clock_locale,
									 __std_system_clock_format,
									 __std_system_clock_format_with_allocator);
	#endif // !STD_NO_MONOTONIC_CLOCK

[[maybe_unused]] static ImplTraitFor(StdClock,
									 __StdUTCClock,
									 __std_utc_clock_now,
									 __std_utc_clock_min_time_point,
									 __std_utc_clock_max_time_point,
									 __std_utc_clock_resolution,
									 __std_utc_clock_resolution_as_ratio,
									 __std_utc_clock_locale,
									 __std_utc_clock_format,
									 __std_utc_clock_format_with_allocator);

[[maybe_unused]] static ImplTraitFor(StdClock,
									 __StdLocalClock,
									 __std_local_clock_now,
									 __std_local_clock_min_time_point,
									 __std_local_clock_max_time_point,
									 __std_local_clock_resolution,
									 __std_local_clock_resolution_as_ratio,
									 __std_local_clock_locale,
									 __std_local_clock_format,
									 __std_local_clock_format_with_allocator);

typedef struct __StdSystemClock {
} __StdSystemClock;

	#if !STD_NO_MONOTONIC_CLOCK
typedef struct __StdSteadyClock {
} __StdSteadyClock;
	#endif // !STD_NO_MONOTONIC_CLOCK

	#if !STD_NO_MONOTONIC_CLOCK
typedef __StdSteadyClock __StdHighResolutionClock;
	#else
typedef __StdSystemClock __StdHighResolutionClock;
	#endif // !STD_NO_MONOTONIC_CLOCK

typedef struct __StdUTCClock {
} __StdUTCClock;
typedef struct __StdLocalClock {
} __StdLocalClock;

	#define __std_system_clock \
		(__StdSystemClock) { }

	#if !STD_NO_MONOTONIC_CLOCK
		#define __std_steady_clock \
			(__StdSteadyClock) { }
	#endif // !STD_NO_MONOTONIC_CLOCK

	#define __std_high_resolution_clock \
		(__StdHighResolutionClock) { }
	#define __std_utc_clock \
		(__StdUTCClock) { }
	#define __std_local_clock \
		(__StdLocalClock) { }

/// @brief The system clock
/// @ingroup std_clock
static let std_system_clock = as_trait(StdClock, __StdSystemClock, __std_system_clock);

	#if !STD_NO_MONOTONIC_CLOCK
/// @brief The monotonic clock
/// @ingroup std_clock
static let std_steady_clock = as_trait(StdClock, __StdSteadyClock, __std_steady_clock);
	#endif // !STD_NO_MONOTONIC_CLOCK

/// @brief The high resolution clock
/// This will be equivalent to `std_system_clock` if `STD_NO_MONOTONIC_CLOCK` is true (in which case
/// a monotonic clock is not supported on the being-compiled-for platform).
/// Otherwise, it will be a monotonic clock equivalent to `std_steady_clock`
/// @ingroup std_clock
static let std_high_resolution_clock
	= as_trait(StdClock, __StdHighResolutionClock, __std_high_resolution_clock);
/// @brief The UTC clock
/// @ingroup std_clock
static let std_utc_clock = as_trait(StdClock, __StdUTCClock, __std_utc_clock);
/// @brief The local time clock
/// @ingroup std_clock
static let std_local_clock = as_trait(StdClock, __StdLocalClock, __std_local_clock);

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

	#undef ___DISABLE_IF_NULL
#endif // STD_CLOCK
