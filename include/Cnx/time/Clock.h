/// @file Clock.h
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

#include <Cnx/time/Duration.h>
#include <Cnx/time/TimePoint.h>

/// @ingroup cnx_time
/// @{
/// @defgroup cnx_clock CnxClock
/// The `CnxClock` module provides types and functions for working with system, local, and global
/// clocks. It provides functionality similar to C++'s `std::chrono` clocks and makes cross-platform
/// time-related operations simple and straight forward. Access to and functionality of the realtime
/// system clock, a high-resolution monotonic clock, a realtime UTC clock, and realtime local time
/// clock are all provided.
///
/// The system clock (`cnx_system_clock`) and related functions always operate in local machine
/// time, operating relative to the UNIX epoch. On Windows, machine time is traditionally in local
/// time. On other platforms, it is usually in UTC.
///
/// The steady clock (or monotonic clock, `cnx_steady_clock`) is a high-resolution monotonic clock
/// suitable for performance, timing, metrics, etc. It is only available if a monotonic clock is
/// available and supported on the being-compiled-for platform (`CNX_NO_MONOTONIC_CLOCK` must be
/// false).
///
/// The generic high resolution clock (`cnx_high_resolution_clock`) is the highest resolution clock
/// available and supported on the system. If `cnx_steady_clock` is available,
/// `cnx_high_resolution_clock` will be equivalent to it. Otherwise, it will be `cnx_system_clock`.
/// `cnx_high_resolution_clock` is provided for convenience, but because of the uncertainty of what
/// clock it will actually be if perfectly accurate timing is necessary, it is preferred to check
/// for `cnx_steady_clock` availability explicitly.
///
/// The UTC clock (`cnx_utc_clock`) and related functions operate in UTC mean time, operating
/// relative to the UNIX epoch.
///
/// The local time clock (`cnx_local_clock`) and related functions operate in local timezone time,
/// operating relative to the UNIX epoch.
///
/// Example:
/// @code {.c}
/// #include <Cnx/time/Clock.h>
/// #include <Cnx/IO.h>
///
///	// print the local time formatted in ISO 8601 format (YYYY-MM-DD|H:M:S+-hh:mm))
/// void local_time_example(void) {
///		let current_time = cnx_clock_now(&cnx_local_clock);
///		println("{}", as_format_t(CnxTimePoint, current_time));
/// }
///
/// // time a function call and print the duration in debug format
/// void time_to_call_example(void) {
///		// `cnx_steady_clock` is only available if a monotonic clock is available
///		#if CNX_NO_MONOTONIC_CLOCK
///			let timer_clock_ptr = &cnx_system_clock;
///		#else
///			let timer_clock_ptr = &cnx_steady_clock;
///		#endif
///		// at this point, `timer_clock_ptr` is equivalent to `&cnx_high_resolution_clock`.
///		// On platforms when one is available, `cnx_high_resolution_clock` will be the monotonic
///		// `cnx_steady_clock`, otherwise it will be `cnx_system_clock`
/// 	let start_time = cnx_clock_now(timer_clock_ptr);
/// 	local_time_example();
/// 	let end_time = cnx_clock_now(timer_clock_ptr);
///		let difference = cnx_time_point_subtract(end_time, start_time).time_since_epoch;
///		println("Took {D} to call local_time_example()", difference);
/// }
///
/// // print the current UTC time formatted in ISO 8601 format (YYYY-MM-DD|H:M:S+-hh:mm))
/// void utc_time_example(void) {
///		let current_utc_time = cnx_clock_now(&cnx_utc_clock);
///		println("{}", as_format_t(CnxTimePoint, current_utc_time));
/// }
///
/// // print the current system time formatted in ISO 8601 format (YYYY-MM-DD|H:M:S+-hh:mm))
/// // (This should be local time on Windows, UTC time on others)
///	void system_time_example(void) {
///		let current_system_time = cnx_clock_now(&cnx_system_clock);
///		println("{}", as_format_t(CnxTimePoint, current_system_time));
/// }
/// @endcode
/// @}

#ifndef CNX_CLOCK
	#define CNX_CLOCK

/// @def CNX_NO_MONOTONIC_CLOCK
/// @brief If `CNX_NO_MONOTONIC_CLOCK` is true, then either Cnx doesn't support a monotonic clock
/// on the being-compiled-for-platform, the being-compiled-for platform itself doesn't, or both, and
/// cnx_steady_clock and related functions are not available
/// @ingroup cnx_clock

	#if !CNX_PLATFORM_APPLE && !CNX_PLATFORM_WINDOWS && !CNX_PLATFORM_ZOS \
		&& !defined(CLOCK_MONOTONIC)
		#warn Monotonic clock not supported on this platform
		#define CNX_NO_MONOTONIC_CLOCK 1
	#else
		#define CNX_NO_MONOTONIC_CLOCK 0
	#endif // !CNX_PLATFORM_APPLE && !CNX_PLATFORM_WINDOWS && !CNX_PLATFORM_ZOS \
		   // && !defined(CLOCK_MONOTONIC)

/// @brief `CnxClockResolution` lists the valid resolutions for `CnxClock` compatible clocks
/// @ingroup cnx_clock
typedef enum CnxClockResolution {
	CNX_CLOCK_NANOSECONDS,
	CNX_CLOCK_MICROSECONDS,
	CNX_CLOCK_MILLISECONDS,
	CNX_CLOCK_SECONDS,
} CnxClockResolution;

/// @struct CnxClock
/// @brief The `CnxClock` trait defines the interface that must be implemented by any
/// Cnx-compatible clock. It intentionally mirrors the API of C++'s standard clock types, along
/// with some additional functionality
/// @ingroup cnx_clock
Trait(CnxClock,
	  /// @brief Returns a `CnxTimePoint` corresponding to the current time on the given clock
	  ///
	  /// @param self - The `CnxClock` to retrieve the current time from
	  ///
	  /// @return The current time on `self`
	  /// @ingroup cnx_clock
	  CnxTimePoint (*const now)(const CnxClock* restrict self);
	  /// @brief Returns the minimum possible `CnxTimePoint` able to be associated with the given
	  /// clock
	  ///
	  /// @param self - The `CnxClock` to get the minimum possible associatable `CnxTimePoint` of
	  ///
	  /// @return The minimum `CnxTimePoint` associatable with `self`
	  /// @ingroup cnx_clock
	  CnxTimePoint(*const min_time_point)(const CnxClock* restrict self);
	  /// @brief Returns the maximum possible `CnxTimePoint` able to be associated with the given
	  /// clock
	  ///
	  /// @param self - The `CnxClock` to get the maximum possible associatable `CnxTimePoint` of
	  ///
	  /// @return The minimum `CnxTimePoint` associatable with `self`
	  /// @ingroup cnx_clock
	  CnxTimePoint(*const max_time_point)(const CnxClock* restrict self);
	  /// @brief Returns the resolution of the given clock
	  ///
	  /// @param self - The `CnxClock` to get the resolution of
	  ///
	  /// @return The resolution of `self` as a `CnxClockResolution`
	  /// @ingroup cnx_clock
	  CnxClockResolution(*const resolution)(const CnxClock* restrict self);
	  /// @brief Returns the resolution of the given clock
	  ///
	  /// @param self - The `CnxClock` to get the resolution of
	  ///
	  /// @return The resolution of `self` as a `CnxRatio` relative to seconds
	  /// @ingroup cnx_clock
	  CnxRatio(*const resolution_as_ratio)(const CnxClock* restrict self);
	  /// @brief Returns the locale associated with the given clock
	  ///
	  /// @param self - The `CnxClock` to get the locale of
	  ///
	  /// @return The locale of `self`
	  /// @ingroup cnx_clock
	  CnxTimePointLocale(*const locale)(const CnxClock* restrict self);
	  /// @brief Returns the text representation of the given clock
	  ///
	  /// @param self - The `CnxClock` to get the text representation of
	  ///
	  /// @return The text representation of `self` as a `CnxString`
	  /// @ingroup cnx_clock
	  CnxString(*const format)(const CnxClock* restrict self);
	  /// @brief Returns the text representation of the given clock, allocating necessary memory
	  /// with the given `CnxAllocator`
	  ///
	  /// @param self - The `CnxClock` to get the text representation of
	  /// @param allocator - The `CnxAllocator` to use for any necessary memory allocation
	  ///
	  /// @return The text representation of `self` as a `CnxString`
	  /// @ingroup cnx_clock
	  CnxString(*const format_with_allocator)(const CnxClock* restrict self,
											  CnxAllocator allocator););

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!(self), "Can't perform a clock operation with a nullptr")

/// @brief Returns a `CnxTimePoint` corresponding to the current time on the given clock
///
/// @param self - The `CnxClock` to retrieve the current time from
///
/// @return The current time on `self`
/// @ingroup cnx_clock
[[nodiscard]] [[not_null(1)]] CnxTimePoint
cnx_clock_now(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the minimum possible `CnxTimePoint` able to be associated with the given
/// clock
///
/// @param self - The `CnxClock` to get the minimum possible associatable `CnxTimePoint` of
///
/// @return The minimum `CnxTimePoint` associatable with `self`
/// @ingroup cnx_clock
[[nodiscard]] [[not_null(1)]] CnxTimePoint
cnx_clock_min_time_point(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the maximum possible `CnxTimePoint` able to be associated with the given
/// clock
///
/// @param self - The `CnxClock` to get the maximum possible associatable `CnxTimePoint` of
///
/// @return The minimum `CnxTimePoint` associatable with `self`
/// @ingroup cnx_clock
[[nodiscard]] [[not_null(1)]] CnxTimePoint
cnx_clock_max_time_point(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the resolution of the given clock
///
/// @param self - The `CnxClock` to get the resolution of
///
/// @return The resolution of `self` as a `CnxClockResolution`
/// @ingroup cnx_clock
[[nodiscard]] [[not_null(1)]] CnxClockResolution
cnx_clock_resolution(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the resolution of the given clock
///
/// @param self - The `CnxClock` to get the resolution of
///
/// @return The resolution of `self` as a `CnxRatio` relative to seconds
/// @ingroup cnx_clock
[[nodiscard]] [[not_null(1)]] CnxRatio
cnx_clock_resolution_as_ratio(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
/// @brief Returns the locale associated with the given clock
///
/// @param self - The `CnxClock` to get the locale of
///
/// @return The locale of `self`
/// @ingroup cnx_clock
[[nodiscard]] [[not_null(1)]] CnxTimePointLocale
cnx_clock_locale(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);

/// @brief Implements the allocator un-aware portion of the `CnxFormat` trait for all `CnxClock`s
///
/// @param self - The `CnxFormat` implementor to get the text representation of
/// @param specifier - The `CnxFormatSpecifier` specifying how to format the text
///
/// @return The text representation of `self` as a `CnxString`
/// @ingroup cnx_clock
[[nodiscard]] [[not_null(1)]] CnxString
cnx_clock_format(const CnxFormat* restrict self, CnxFormatSpecifier specifier)
	___DISABLE_IF_NULL(self);
/// @brief Implements the allocator aware portion of the `CnxFormat` trait for all `CnxClock`s
///
/// @param self - The `CnxFormat` implementor to get the text representation of
/// @param specifier - The `CnxFormatSpecifier` specifying how to format the text
/// @param allocator - The `CnxAllocator` to use for any necessary memory allocation
///
/// @return The text representation of `self` as a `CnxString`
/// @ingroup cnx_clock
[[nodiscard]] [[not_null(1)]] CnxString
cnx_clock_format_with_allocator(const CnxFormat* restrict self,
								CnxFormatSpecifier specifier,
								CnxAllocator allocator) ___DISABLE_IF_NULL(self);

/// @brief Implements the `CnxFormat` trait for `CnxClock`
/// @ingroup cnx_clock
[[maybe_unused]] static ImplTraitFor(CnxFormat,
									 CnxClock,
									 cnx_clock_format,
									 cnx_clock_format_with_allocator);

/// @brief Returns a `CnxTimePoint` corresponding to the current time on the system clock
///
/// @return The current time on the system clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_system_clock_now(void);
/// @brief Returns the minimum possible `CnxTimePoint` able to be associated with the system
/// clock
///
/// @return The minimum `CnxTimePoint` associatable with the system clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_system_clock_min_time_point(void);
/// @brief Returns the maximum possible `CnxTimePoint` able to be associated with the system
/// clock
///
/// @return The maximum `CnxTimePoint` associatable with the system clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_system_clock_max_time_point(void);
/// @brief Returns the resolution of the system clock
///
/// @return The resolution of the system clock as a `CnxClockResolution`
/// @ingroup cnx_clock
[[nodiscard]] CnxClockResolution cnx_system_clock_resolution(void);
/// @brief Returns the resolution of the system clock
///
/// @return The resolution of the system clock as a `CnxRatio` relative to seconds
/// @ingroup cnx_clock
[[nodiscard]] CnxRatio cnx_system_clock_resolution_as_ratio(void);
/// @brief Returns the locale of the system clock
///
/// @return The locale of the system clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePointLocale cnx_system_clock_locale(void);

	#if !CNX_NO_MONOTONIC_CLOCK
/// @brief Returns a `CnxTimePoint` corresponding to the current time on the steady clock
///
/// @return The current time on the steady clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_steady_clock_now(void);
/// @brief Returns the minimum possible `CnxTimePoint` able to be associated with the steady
/// clock
///
/// @return The minimum `CnxTimePoint` associatable with the steady clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_steady_clock_min_time_point(void);
/// @brief Returns the maximum possible `CnxTimePoint` able to be associated with the steady
/// clock
///
/// @return The maximum `CnxTimePoint` associatable with the steady clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_steady_clock_max_time_point(void);
/// @brief Returns the resolution of the steady clock
///
/// @return The resolution of the steady clock as a `CnxClockResolution`
/// @ingroup cnx_clock
[[nodiscard]] CnxClockResolution cnx_steady_clock_resolution(void);
/// @brief Returns the resolution of the steady clock
///
/// @return The resolution of the steady clock as a `CnxRatio` relative to seconds
/// @ingroup cnx_clock
[[nodiscard]] CnxRatio cnx_steady_clock_resolution_as_ratio(void);
/// @brief Returns the locale of the steady clock
///
/// @return The locale of the steady clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePointLocale cnx_steady_clock_locale(void);
	#endif // !CNX_NO_MONOTONIC_CLOCK

/// @brief Returns a `CnxTimePoint` corresponding to the current time on the high resolution clock
///
/// @return The current time on the high resolution clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_high_resolution_clock_now(void);
/// @brief Returns the minimum possible `CnxTimePoint` able to be associated with the high
/// resolution clock
///
/// @return The minimum `CnxTimePoint` associatable with the high resolution clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_high_resolution_clock_min_time_point(void);
/// @brief Returns the maximum possible `CnxTimePoint` able to be associated with the high
/// resolution clock
///
/// @return The maximum `CnxTimePoint` associatable with the high resolution clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_high_resolution_clock_max_time_point(void);
/// @brief Returns the resolution of the high resolution clock
///
/// @return The resolution of the high resolution clock as a `CnxClockResolution`
/// @ingroup cnx_clock
[[nodiscard]] CnxClockResolution cnx_high_resolution_clock_resolution(void);
/// @brief Returns the resolution of the high resolution clock
///
/// @return The resolution of the high resolution clock as a `CnxRatio` relative to seconds
/// @ingroup cnx_clock
[[nodiscard]] CnxRatio cnx_high_resolution_clock_resolution_as_ratio(void);
/// @brief Returns the locale of the high resolution clock
///
/// @return The locale of the high resolution clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePointLocale cnx_high_resolution_clock_locale(void);

/// @brief Returns a `CnxTimePoint` corresponding to the current time on the UTC clock
///
/// @return The current time on the UTC clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_utc_clock_now(void);
/// @brief Returns the minimum possible `CnxTimePoint` able to be associated with the UTC
/// clock
///
/// @return The minimum `CnxTimePoint` associatable with the UTC clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_utc_clock_min_time_point(void);
/// @brief Returns the maximum possible `CnxTimePoint` able to be associated with the UTC
/// clock
///
/// @return The maximum `CnxTimePoint` associatable with the UTC clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_utc_clock_max_time_point(void);
/// @brief Returns the resolution of the UTC clock
///
/// @return The resolution of the UTC clock as a `CnxClockResolution`
/// @ingroup cnx_clock
[[nodiscard]] CnxClockResolution cnx_utc_clock_resolution(void);
/// @brief Returns the resolution of the UTC clock
///
/// @return The resolution of the UTC clock as a `CnxRatio` relative to seconds
/// @ingroup cnx_clock
[[nodiscard]] CnxRatio cnx_utc_clock_resolution_as_ratio(void);
/// @brief Returns the locale of the UTC clock
///
/// @return The locale of the UTC clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePointLocale cnx_utc_clock_locale(void);

/// @brief Returns a `CnxTimePoint` corresponding to the current time on the local clock
///
/// @return The current time on the local clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_local_clock_now(void);
/// @brief Returns the minimum possible `CnxTimePoint` able to be associated with the local
/// clock
///
/// @return The minimum `CnxTimePoint` associatable with the local clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_local_clock_min_time_point(void);
/// @brief Returns the maximum possible `CnxTimePoint` able to be associated with the local
/// clock
///
/// @return The maximum `CnxTimePoint` associatable with the local clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_local_clock_max_time_point(void);
/// @brief Returns the resolution of the local clock
///
/// @return The resolution of the local clock as a `CnxClockResolution`
/// @ingroup cnx_clock
[[nodiscard]] CnxClockResolution cnx_local_clock_resolution(void);
/// @brief Returns the resolution of the local clock
///
/// @return The resolution of the local clock as a `CnxRatio` relative to seconds
/// @ingroup cnx_clock
[[nodiscard]] CnxRatio cnx_local_clock_resolution_as_ratio(void);
/// @brief Returns the locale of the local clock
///
/// @return The locale of the local clock
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePointLocale cnx_local_clock_locale(void);

/// @brief Converts the given `CnxTimePoint` in UTC time to a `CnxTimePoint` in local time
///
/// @param utc - The time point to convert to local time
///
/// @return `utc` converted to local time
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_convert_utc_to_local_time(CnxTimePoint utc);
/// @brief Converts the given `CnxTimePoint` in local time to a `CnxTimePoint` in UTC time
///
/// @param local_time - The time point to convert to UTC time
///
/// @return `local_time` converted to UTC time
/// @ingroup cnx_clock
[[nodiscard]] CnxTimePoint cnx_convert_local_time_to_utc(CnxTimePoint local_time);

IGNORE_RESERVED_IDENTIFIER_WARNING_START

[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_system_clock_now(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_system_clock_min_time_point(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_system_clock_max_time_point(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxClockResolution
__cnx_system_clock_resolution(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxRatio
__cnx_system_clock_resolution_as_ratio(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePointLocale
__cnx_system_clock_locale(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxString
__cnx_system_clock_format(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxString
__cnx_system_clock_format_with_allocator(const CnxClock* restrict self, CnxAllocator allocator)
	___DISABLE_IF_NULL(self);

	#if !CNX_NO_MONOTONIC_CLOCK
[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_steady_clock_now(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_steady_clock_min_time_point(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_steady_clock_max_time_point(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxClockResolution
__cnx_steady_clock_resolution(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxRatio
__cnx_steady_clock_resolution_as_ratio(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePointLocale
__cnx_steady_clock_locale(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxString
__cnx_steady_clock_format(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxString
__cnx_steady_clock_format_with_allocator(const CnxClock* restrict self, CnxAllocator allocator)
	___DISABLE_IF_NULL(self);
	#endif // !CNX_NO_MONOTONIC_CLOCK

[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_utc_clock_now(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_utc_clock_min_time_point(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_utc_clock_max_time_point(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxClockResolution
__cnx_utc_clock_resolution(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxRatio
__cnx_utc_clock_resolution_as_ratio(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePointLocale
__cnx_utc_clock_locale(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxString
__cnx_utc_clock_format(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxString
__cnx_utc_clock_format_with_allocator(const CnxClock* restrict self, CnxAllocator allocator)
	___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_local_clock_now(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_local_clock_min_time_point(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePoint
__cnx_local_clock_max_time_point(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxClockResolution
__cnx_local_clock_resolution(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxRatio
__cnx_local_clock_resolution_as_ratio(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxTimePointLocale
__cnx_local_clock_locale(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxString
__cnx_local_clock_format(const CnxClock* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxString
__cnx_local_clock_format_with_allocator(const CnxClock* restrict self, CnxAllocator allocator)
	___DISABLE_IF_NULL(self);

[[maybe_unused]] static ImplTraitFor(CnxClock,
									 __CnxSystemClock,
									 __cnx_system_clock_now,
									 __cnx_system_clock_min_time_point,
									 __cnx_system_clock_max_time_point,
									 __cnx_system_clock_resolution,
									 __cnx_system_clock_resolution_as_ratio,
									 __cnx_system_clock_locale,
									 __cnx_system_clock_format,
									 __cnx_system_clock_format_with_allocator);

	#if !CNX_NO_MONOTONIC_CLOCK
[[maybe_unused]] static ImplTraitFor(CnxClock,
									 __CnxSteadyClock,
									 __cnx_steady_clock_now,
									 __cnx_steady_clock_min_time_point,
									 __cnx_steady_clock_max_time_point,
									 __cnx_steady_clock_resolution,
									 __cnx_steady_clock_resolution_as_ratio,
									 __cnx_steady_clock_locale,
									 __cnx_steady_clock_format,
									 __cnx_steady_clock_format_with_allocator);
	#endif // !CNX_NO_MONOTONIC_CLOCK

	#if !CNX_NO_MONOTONIC_CLOCK
[[maybe_unused]] static ImplTraitFor(CnxClock,
									 __CnxHighResolutionClock,
									 __cnx_steady_clock_now,
									 __cnx_steady_clock_min_time_point,
									 __cnx_steady_clock_max_time_point,
									 __cnx_steady_clock_resolution,
									 __cnx_steady_clock_resolution_as_ratio,
									 __cnx_steady_clock_locale,
									 __cnx_steady_clock_format,
									 __cnx_steady_clock_format_with_allocator);
	#else
[[maybe_unused]] static ImplTraitFor(CnxClock,
									 __cnxHighResolutionClock,
									 __cnx_system_clock_now,
									 __cnx_system_clock_min_time_point,
									 __cnx_system_clock_max_time_point,
									 __cnx_system_clock_resolution,
									 __cnx_system_clock_resolution_as_ratio,
									 __cnx_system_clock_locale,
									 __cnx_system_clock_format,
									 __cnx_system_clock_format_with_allocator);
	#endif // !CNX_NO_MONOTONIC_CLOCK

[[maybe_unused]] static ImplTraitFor(CnxClock,
									 __CnxUTCClock,
									 __cnx_utc_clock_now,
									 __cnx_utc_clock_min_time_point,
									 __cnx_utc_clock_max_time_point,
									 __cnx_utc_clock_resolution,
									 __cnx_utc_clock_resolution_as_ratio,
									 __cnx_utc_clock_locale,
									 __cnx_utc_clock_format,
									 __cnx_utc_clock_format_with_allocator);

[[maybe_unused]] static ImplTraitFor(CnxClock,
									 __CnxLocalClock,
									 __cnx_local_clock_now,
									 __cnx_local_clock_min_time_point,
									 __cnx_local_clock_max_time_point,
									 __cnx_local_clock_resolution,
									 __cnx_local_clock_resolution_as_ratio,
									 __cnx_local_clock_locale,
									 __cnx_local_clock_format,
									 __cnx_local_clock_format_with_allocator);

typedef struct __CnxSystemClock {
} __CnxSystemClock;

	#if !CNX_NO_MONOTONIC_CLOCK
typedef struct __CnxSteadyClock {
} __CnxSteadyClock;
	#endif // !CNX_NO_MONOTONIC_CLOCK

	#if !CNX_NO_MONOTONIC_CLOCK
typedef __CnxSteadyClock __CnxHighResolutionClock;
	#else
typedef __CnxSystemClock __CnxHighResolutionClock;
	#endif // !CNX_NO_MONOTONIC_CLOCK

typedef struct __CnxUTCClock {
} __CnxUTCClock;
typedef struct __CnxLocalClock {
} __CnxLocalClock;

	#define __cnx_system_clock \
		(__CnxSystemClock) {   \
		}

	#if !CNX_NO_MONOTONIC_CLOCK
		#define __cnx_steady_clock \
			(__CnxSteadyClock) {   \
			}
	#endif // !CNX_NO_MONOTONIC_CLOCK

	#define __cnx_high_resolution_clock \
		(__CnxHighResolutionClock) {    \
		}
	#define __cnx_utc_clock \
		(__CnxUTCClock) {   \
		}
	#define __cnx_local_clock \
		(__CnxLocalClock) {   \
		}

/// @brief The system clock
/// @ingroup cnx_clock
static let cnx_system_clock = as_trait(CnxClock, __CnxSystemClock, __cnx_system_clock);

	#if !CNX_NO_MONOTONIC_CLOCK
/// @brief The monotonic clock
/// @ingroup cnx_clock
static let cnx_steady_clock = as_trait(CnxClock, __CnxSteadyClock, __cnx_steady_clock);
	#endif // !CNX_NO_MONOTONIC_CLOCK

/// @brief The high resolution clock
/// This will be equivalent to `cnx_system_clock` if `CNX_NO_MONOTONIC_CLOCK` is true (in which case
/// a monotonic clock is not supported on the being-compiled-for platform).
/// Otherwise, it will be a monotonic clock equivalent to `cnx_steady_clock`
/// @ingroup cnx_clock
static let cnx_high_resolution_clock
	= as_trait(CnxClock, __CnxHighResolutionClock, __cnx_high_resolution_clock);
/// @brief The UTC clock
/// @ingroup cnx_clock
static let cnx_utc_clock = as_trait(CnxClock, __CnxUTCClock, __cnx_utc_clock);
/// @brief The local time clock
/// @ingroup cnx_clock
static let cnx_local_clock = as_trait(CnxClock, __CnxLocalClock, __cnx_local_clock);

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

	#undef ___DISABLE_IF_NULL
#endif // CNX_CLOCK
