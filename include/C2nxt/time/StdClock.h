/// @file StdClock.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides methods for operating with system clocks
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

#include <C2nxt/time/StdDuration.h>
#include <C2nxt/time/StdTimePoint.h>

/// @ingroup std_time
/// @{
/// @defgroup std_clock StdClock
/// The `StdClock` module provides direct functions as well as the `StdClock` Trait for getting the
/// current time and resolution from various system clocks
/// @}

#ifndef STD_CLOCK
	#define STD_CLOCK

typedef enum StdClockResolution {
	STD_CLOCK_NANOSECONDS,
	STD_CLOCK_MICROSECONDS,
	STD_CLOCK_MILLISECONDS,
	STD_CLOCK_SECONDS,
} StdClockResolution;

Trait(StdClock, StdTimePoint (*const now)(const StdClock* restrict self);
	  StdTimePoint(*const min_time_point)(const StdClock* restrict self);
	  StdTimePoint(*const max_time_point)(const StdClock* restrict self);
	  StdClockResolution(*const resolution)(const StdClock* restrict self);
	  StdRatio(*const resolution_as_ratio)(const StdClock* restrict self);
	  StdString(*const format)(const StdClock* restrict self);
	  StdString(*const format_with_allocator)(const StdClock* restrict self,
											  StdAllocator allocator););

StdTimePoint std_system_clock_now(void);
StdTimePoint std_system_clock_min_time_point(void);
StdTimePoint std_system_clock_max_time_point(void);
StdClockResolution std_system_clock_resolution(void);
StdRatio std_system_clock_resolution_as_ratio(void);

StdTimePoint std_steady_clock_now(void);
StdTimePoint std_steady_clock_min_time_point(void);
StdTimePoint std_steady_clock_max_time_point(void);
StdClockResolution std_steady_clock_resolution(void);
StdRatio std_steady_clock_resolution_as_ratio(void);

StdTimePoint std_high_resolution_clock_now(void);
StdTimePoint std_high_resolution_clock_min_time_point(void);
StdTimePoint std_high_resolution_clock_max_time_point(void);
StdClockResolution std_high_resolution_clock_resolution(void);
StdRatio std_high_resolution_clock_resolution_as_ratio(void);

StdTimePoint std_utc_clock_now(void);
StdTimePoint std_utc_clock_min_time_point(void);
StdTimePoint std_utc_clock_max_time_point(void);
StdClockResolution std_utc_clock_resolution(void);
StdRatio std_utc_clock_resolution_as_ratio(void);

StdTimePoint std_tai_clock_now(void);
StdTimePoint std_tai_clock_min_time_point(void);
StdTimePoint std_tai_clock_max_time_point(void);
StdClockResolution std_tai_clock_resolution(void);
StdRatio std_tai_clock_resolution_as_ratio(void);

StdTimePoint std_gps_clock_now(void);
StdTimePoint std_gps_clock_min_time_point(void);
StdTimePoint std_gps_clock_max_time_point(void);
StdClockResolution std_gps_clock_resolution(void);
StdRatio std_gps_clock_resolution_as_ratio(void);

StdTimePoint std_file_clock_now(void);
StdTimePoint std_file_clock_min_time_point(void);
StdTimePoint std_file_clock_max_time_point(void);
StdClockResolution std_file_clock_resolution(void);
StdRatio std_file_clock_resolution_as_ratio(void);

StdTimePoint std_local_clock_now(void);
StdTimePoint std_local_clock_min_time_point(void);
StdTimePoint std_local_clock_max_time_point(void);
StdClockResolution std_local_clock_resolution(void);
StdRatio std_local_clock_resolution_as_ratio(void);

IGNORE_RESERVED_IDENTIFIER_WARNING_START

StdTimePoint __std_system_clock_now(const StdClock* restrict self);
StdTimePoint __std_system_clock_min_time_point(const StdClock* restrict self);
StdTimePoint __std_system_clock_max_time_point(const StdClock* restrict self);
StdClockResolution __std_system_clock_resolution(const StdClock* restrict self);
StdRatio __std_system_clock_resolution_as_ratio(const StdClock* restrict self);
StdString __std_system_clock_format(const StdClock* restrict self);
StdString
__std_system_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator);

StdTimePoint __std_steady_clock_now(const StdClock* restrict self);
StdTimePoint __std_steady_clock_min_time_point(const StdClock* restrict self);
StdTimePoint __std_steady_clock_max_time_point(const StdClock* restrict self);
StdClockResolution __std_steady_clock_resolution(const StdClock* restrict self);
StdRatio __std_steady_clock_resolution_as_ratio(const StdClock* restrict self);
StdString __std_steady_clock_format(const StdClock* restrict self);
StdString
__std_steady_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator);

StdTimePoint __std_high_resolution_clock_now(const StdClock* restrict self);
StdTimePoint __std_high_resolution_clock_min_time_point(const StdClock* restrict self);
StdTimePoint __std_high_resolution_clock_max_time_point(const StdClock* restrict self);
StdClockResolution __std_high_resolution_clock_resolution(const StdClock* restrict self);
StdRatio __std_high_resolution_clock_resolution_as_ratio(const StdClock* restrict self);
StdString __std_high_resolution_clock_format(const StdClock* restrict self);
StdString __std_high_resolution_clock_format_with_allocator(const StdClock* restrict self,
															StdAllocator allocator);

StdTimePoint __std_utc_clock_now(const StdClock* restrict self);
StdTimePoint __std_utc_clock_min_time_point(const StdClock* restrict self);
StdTimePoint __std_utc_clock_max_time_point(const StdClock* restrict self);
StdClockResolution __std_utc_clock_resolution(const StdClock* restrict self);
StdRatio __std_utc_clock_resolution_as_ratio(const StdClock* restrict self);
StdString __std_utc_clock_format(const StdClock* restrict self);
StdString
__std_utc_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator);

StdTimePoint __std_tai_clock_now(const StdClock* restrict self);
StdTimePoint __std_tai_clock_min_time_point(const StdClock* restrict self);
StdTimePoint __std_tai_clock_max_time_point(const StdClock* restrict self);
StdClockResolution __std_tai_clock_resolution(const StdClock* restrict self);
StdRatio __std_tai_clock_resolution_as_ratio(const StdClock* restrict self);
StdString __std_tai_clock_format(const StdClock* restrict self);
StdString
__std_tai_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator);

StdTimePoint __std_gps_clock_now(const StdClock* restrict self);
StdTimePoint __std_gps_clock_min_time_point(const StdClock* restrict self);
StdTimePoint __std_gps_clock_max_time_point(const StdClock* restrict self);
StdClockResolution __std_gps_clock_resolution(const StdClock* restrict self);
StdRatio __std_gps_clock_resolution_as_ratio(const StdClock* restrict self);
StdString __std_gps_clock_format(const StdClock* restrict self);
StdString
__std_gps_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator);

StdTimePoint __std_file_clock_now(const StdClock* restrict self);
StdTimePoint __std_file_clock_min_time_point(const StdClock* restrict self);
StdTimePoint __std_file_clock_max_time_point(const StdClock* restrict self);
StdClockResolution __std_file_clock_resolution(const StdClock* restrict self);
StdRatio __std_file_clock_resolution_as_ratio(const StdClock* restrict self);
StdString __std_file_clock_format(const StdClock* restrict self);
StdString
__std_file_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator);

StdTimePoint __std_local_clock_now(const StdClock* restrict self);
StdTimePoint __std_local_clock_min_time_point(const StdClock* restrict self);
StdTimePoint __std_local_clock_max_time_point(const StdClock* restrict self);
StdClockResolution __std_local_clock_resolution(const StdClock* restrict self);
StdRatio __std_local_clock_resolution_as_ratio(const StdClock* restrict self);
StdString __std_local_clock_format(const StdClock* restrict self);
StdString
__std_local_clock_format_with_allocator(const StdClock* restrict self, StdAllocator allocator);

static maybe_unused ImplTraitFor(StdClock,
								 __StdSystemClock,
								 __std_system_clock_now,
								 __std_system_clock_min_time_point,
								 __std_system_clock_max_time_point,
								 __std_system_clock_resolution,
								 __std_system_clock_resolution_as_ratio,
								 __std_system_clock_format,
								 __std_system_clock_format_with_allocator);

static maybe_unused ImplTraitFor(StdClock,
								 __StdSteadyClock,
								 __std_steady_clock_now,
								 __std_steady_clock_min_time_point,
								 __std_steady_clock_max_time_point,
								 __std_steady_clock_resolution,
								 __std_steady_clock_resolution_as_ratio,
								 __std_steady_clock_format,
								 __std_steady_clock_format_with_allocator);

static maybe_unused ImplTraitFor(StdClock,
								 __StdHighResolutionClock,
								 __std_high_resolution_clock_now,
								 __std_high_resolution_clock_min_time_point,
								 __std_high_resolution_clock_max_time_point,
								 __std_high_resolution_clock_resolution,
								 __std_high_resolution_clock_resolution_as_ratio,
								 __std_high_resolution_clock_format,
								 __std_high_resolution_clock_format_with_allocator);

static maybe_unused ImplTraitFor(StdClock,
								 __StdUTCClock,
								 __std_utc_clock_now,
								 __std_utc_clock_min_time_point,
								 __std_utc_clock_max_time_point,
								 __std_utc_clock_resolution,
								 __std_utc_clock_resolution_as_ratio,
								 __std_utc_clock_format,
								 __std_utc_clock_format_with_allocator);

static maybe_unused ImplTraitFor(StdClock,
								 __StdTAIClock,
								 __std_tai_clock_now,
								 __std_tai_clock_min_time_point,
								 __std_tai_clock_max_time_point,
								 __std_tai_clock_resolution,
								 __std_tai_clock_resolution_as_ratio,
								 __std_tai_clock_format,
								 __std_tai_clock_format_with_allocator);

static maybe_unused ImplTraitFor(StdClock,
								 __StdGPSClock,
								 __std_gps_clock_now,
								 __std_gps_clock_min_time_point,
								 __std_gps_clock_max_time_point,
								 __std_gps_clock_resolution,
								 __std_gps_clock_resolution_as_ratio,
								 __std_gps_clock_format,
								 __std_gps_clock_format_with_allocator);

static maybe_unused ImplTraitFor(StdClock,
								 __StdFileClock,
								 __std_file_clock_now,
								 __std_file_clock_min_time_point,
								 __std_file_clock_max_time_point,
								 __std_file_clock_resolution,
								 __std_file_clock_resolution_as_ratio,
								 __std_file_clock_format,
								 __std_file_clock_format_with_allocator);

static maybe_unused ImplTraitFor(StdClock,
								 __StdLocalClock,
								 __std_local_clock_now,
								 __std_local_clock_min_time_point,
								 __std_local_clock_max_time_point,
								 __std_local_clock_resolution,
								 __std_local_clock_resolution_as_ratio,
								 __std_local_clock_format,
								 __std_local_clock_format_with_allocator);

typedef struct __StdSysteSteady {
} __StdSystemClock;
typedef struct __StdSteadyClock {
} __StdSteadyClock;
typedef struct __StdHighResolutionClock {
} __StdHighResolutionClock;
typedef struct __StdUTCClock {
} __StdUTCClock;
typedef struct __StdTAIClock {
} __StdTAIClock;
typedef struct __StdGPSClock {
} __StdGPSClock;
typedef struct __StdFileClock {
} __StdFileClock;
typedef struct __StdLocalClock {
} __StdLocalClock;

	#define __std_system_clock \
		(__StdSystemClock) {   \
		}
	#define __std_steady_clock \
		(__StdSteadyClock) {   \
		}
	#define __std_high_resolution_clock \
		(__StdHighResolutionClock) {    \
		}
	#define __std_utc_clock \
		(__StdUTCClock) {   \
		}
	#define __std_tai_clock \
		(__StdTAIClock) {   \
		}
	#define __std_gps_clock \
		(__StdGPSClock) {   \
		}
	#define __std_file_clock \
		(__StdFileClock) {   \
		}
	#define __std_local_clock \
		(__StdLocalClock) {   \
		}

static let std_system_clock = as_trait(StdClock, __StdSystemClock, __std_system_clock);
static let std_steady_clock = as_trait(StdClock, __StdSteadyClock, __std_steady_clock);
static let std_high_resolution_clock
	= as_trait(StdClock, __StdHighResolutionClock, __std_high_resolution_clock);
static let std_utc_clock = as_trait(StdClock, __StdUTCClock, __std_utc_clock);
static let std_tai_clock = as_trait(StdClock, __StdTAIClock, __std_tai_clock);
static let std_gps_clock = as_trait(StdClock, __StdGPSClock, __std_gps_clock);
static let std_file_clock = as_trait(StdClock, __StdFileClock, __std_file_clock);
static let std_local_clock = as_trait(StdClock, __StdLocalClock, __std_local_clock);

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

StdString std_clock_format(const StdFormat* restrict self, StdFormatSpecifier specifier);
StdString std_clock_format_with_allocator(const StdFormat* restrict self,
										  StdFormatSpecifier specifier,
										  StdAllocator allocator);

/// @brief Implements the `StdFormat` trait for `StdClock`
/// @ingroup std_clock
static maybe_unused
	ImplTraitFor(StdFormat, StdClock, std_clock_format, std_clock_format_with_allocator);
#endif // STD_CLOCK
