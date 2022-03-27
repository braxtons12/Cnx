/// @file StdClock.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides methods for operating with system clocks
/// @version 0.1.2
/// @date 2022-03-26
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

#include <C2nxt/time/StdClock.h>
#include <time.h>

// IBM z/OS support
#if defined(__MVS__)
	// must be defined prior to including any system headers to include struct timespec64 in order
	// to do monotonic clock support
	#define _LARGE_TIME_API
	#include <__support/ibm/gettod_zos.h>
#endif

#if __has_include(<unistd.h>)
	#include <unistd.h>
#endif // __has_include(<unistd.h>)

#if __has_include(<sys/time.h>)
	#include <sys/time.h>
#endif // __has_include(<sys/time.h>)

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#if !STD_PLATFORM_APPLE && defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0
	#define __STD_USE_CLOCK_GETTIME 1
#else
	#define __STD_USE_CLOCK_GETTIME 0
#endif // !STD_PLATFORM_APPLE && defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#if __has_include(<mach/mach_time.h>)
	#include <mach/mach_time.h>
#endif

#if STD_PLATFORM_WINDOWS
	#include <Windows.h>

	#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
		#include <winapifamily.h>
	#endif // _WIN32_WINNT >= _WIN32_WINNT_WIN8

	#if _WIN32_WINNT < _WIN32_WINNT_WIN8
typedef void(WINAPI* GetSystemTimeAsFileTimePtr)(LPFILETIME);

[[always_inline]] static inline GetSystemTimeAsFileTimePtr get_win_system_time_function(void) {
	IGNORE_CAST_FUNCTION_TYPE_WARNING_START
	let_mut func_ptr = static_cast(GetSystemTimeAsFileTimePtr)(
		GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetSystemTimePreciseAsFileTime"));
	IGNORE_CAST_FUNCTION_TYPE_WARNING_STOP
	if(func_ptr == nullptr) {
		func_ptr = GetSystemTimeAsFileTime;
	}
	return func_ptr;
}

	#endif // _WIN32_WINNT < _WIN32_WINNT_WIN8

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
StdTimePoint __std_system_clock_now(const StdClock* restrict self) {
	let filetime_period = std_ratio_multiply(std_ratio(100, 1), std_nanoseconds_period);
	// UNIX epoch starts at Jan 1 1970,
	// but Windows has to be weird, thus its epoch starts at Jan 1 1601
	let nt_to_unix_epoch = std_seconds(11644473600);

	FILETIME file_time;
	#if(_WIN32_WINNT >= _WIN32_WINNT_WIN7 && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)) \
		|| (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
	GetSystemTimePreciseAsFileTime(&file_time);
	#elif !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
	GetSystemTimeAsFileTime(&file_time);
	#else
	get_win_system_time_function()(&file_time);
	#endif

	let duration = (StdDuration){
		.count
		= static_cast(i64)((static_cast(u64)(file_time.dwHighDateTime) << static_cast(u64)(32))
						   | static_cast(u64)(file_time.dwLowDateTime)),
		.period = filetime_period,
	};
	return std_time_point_new_with_clock_and_locale(
		std_duration_subtract(std_duration_cast(duration, std_microseconds_period),
							  nt_to_unix_epoch),
		self,
		STD_LOCAL_TIME);
}

#elif defined(CLOCK_REALTIME) && __STD_USE_CLOCK_GETTIME

StdTimePoint __std_system_clock_now(const StdClock* restrict self) {
	struct timespec time_spec;
	if(0 != clock_gettime(CLOCK_REALTIME, &time_spec)) {
		std_panic("clock_gettime(CLOCK_REALTIME) failed");
	}
	return std_time_point_new_with_clock_and_locale(
		std_duration_add(std_microseconds(time_spec.tv_nsec / 1000), std_seconds(time_spec.tv_sec)),
		self,
		STD_UTC_TIME);
}

#else
StdTimePoint __std_system_clock_now(const StdClock* restrict self) {
	timeval time_val;
	gettimeofday(&time_val, 0);
	return std_time_point_new_with_clock_and_locale(
		std_duration_add(std_microseconds(time_val.tv_usec), std_seconds(time_val.tv_sec)),
		self,
		STD_UTC_TIME);
}
#endif

StdTimePoint __std_system_clock_min_time_point(const StdClock* restrict self) {
	return std_time_point_new_with_clock(
		std_duration_new(std_min_value(i64), std_microseconds_period),
		self);
}

StdTimePoint __std_system_clock_max_time_point(const StdClock* restrict self) {
	return std_time_point_new_with_clock(
		std_duration_new(std_max_value(i64), std_microseconds_period),
		self);
}

StdClockResolution __std_system_clock_resolution([[maybe_unused]] const StdClock* restrict self) {
	return STD_CLOCK_MICROSECONDS;
}

StdRatio __std_system_clock_resolution_as_ratio([[maybe_unused]] const StdClock* restrict self) {
	return std_microseconds_period;
}

StdTimePointLocale __std_system_clock_locale([[maybe_unused]] const StdClock* restrict self) {
	return STD_LOCAL_TIME;
}

StdString __std_system_clock_format_with_allocator([[maybe_unused]] const StdClock* restrict self,
												   StdAllocator allocator) {
	return std_string_from_with_allocator("StdClock: std_system_clock", allocator);
}

StdString __std_system_clock_format(const StdClock* restrict self) {
	return __std_system_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

StdTimePoint std_system_clock_now(void) {
	return __std_system_clock_now(&std_system_clock);
}

StdTimePoint std_system_clock_min_time_point(void) {
	return __std_system_clock_min_time_point(&std_system_clock);
}

StdTimePoint std_system_clock_max_time_point(void) {
	return __std_system_clock_max_time_point(&std_system_clock);
}

StdClockResolution std_system_clock_resolution(void) {
	return __std_system_clock_resolution(&std_system_clock);
}

StdRatio std_system_clock_resolution_as_ratio(void) {
	return __std_system_clock_resolution_as_ratio(&std_system_clock);
}

StdTimePointLocale std_system_clock_locale(void) {
	return __std_system_clock_locale(&std_system_clock);
}

#if !STD_NO_MONOTONIC_CLOCK
	#if STD_PLATFORM_APPLE
StdTimePoint __std_steady_clock_now(const StdClock* restrict self) {
	struct timespec spec;
	if(0 != clock_gettime(CLOCK_MONOTONIC_RAW, &spec)) {
		std_panic("clock_gettime(CLOCK_MONOTONIC_RAW) failed!");
	}
	return std_time_point_new_with_clock_and_locale(
		std_duration_add(std_nanoseconds(spec.tv_nsec), std_seconds(spec.tv_sec)),
		self,
		STD_UNKNOWN_TIME);
}
	#elif STD_PLATFORM_WINDOWS
[[always_inline]] static inline LARGE_INTEGER query_performance_frequency(void) {
	LARGE_INTEGER val;
	ignore(QueryPerformanceFrequency(&val));
	return val;
}

StdTimePoint __std_steady_clock_now(const StdClock* restrict self) {
	let frequency = query_performance_frequency();

	LARGE_INTEGER counter;
	ignore(QueryPerformanceCounter(&counter));
	let seconds = counter.QuadPart / frequency.QuadPart;
	let remainder = counter.QuadPart % frequency.QuadPart;
	let nanoseconds = seconds * std_nanoseconds_period.den
					  + remainder * std_nanoseconds_period.den / frequency.QuadPart;
	return std_time_point_new_with_clock_and_locale(std_nanoseconds(nanoseconds),
													self,
													STD_UNKNOWN_TIME);
}
	#elif STD_PLATFORM_ZOS
StdTimePoint __std_steady_clock_now(const StdClock* restrict self) {
	struct timespec64 spec;
	if(0 != gettimeofdayMonotonic(&spec)) {
		std_panic("gettimeofdayMonotonic failed!");
	}

	return std_time_point_new_with_clock_and_locale(
		std_duration_add(std_nanoseconds(spec.tv_nsec), std_seconds(spec.tv_sec)),
		self,
		STD_UNKNOWN_TIME);
}
	#elif defined(CLOCK_MONOTONIC)
StdTimePoint __std_steady_clock_now(const StdClock* restrict self) {
	struct timespec spec;
	if(0 != clock_gettime(CLOCK_MONOTONIC, &spec)) {
		std_panic("clock_gettime(CLOCK_MONOTONIC) failed!");
	}
	return std_time_point_new_with_clock_and_locale(
		std_duration_add(std_nanoseconds(spec.tv_nsec), std_seconds(spec.tv_sec)),
		self,
		STD_UNKNOWN_TIME);
}
	#endif

StdTimePoint __std_steady_clock_min_time_point(const StdClock* restrict self) {
	return std_time_point_new_with_clock_and_locale(
		std_duration_new(std_min_value(i64), std_nanoseconds_period),
		self,
		STD_UNKNOWN_TIME);
}

StdTimePoint __std_steady_clock_max_time_point(const StdClock* restrict self) {
	return std_time_point_new_with_clock_and_locale(
		std_duration_new(std_max_value(i64), std_nanoseconds_period),
		self,
		STD_UNKNOWN_TIME);
}

StdClockResolution __std_steady_clock_resolution([[maybe_unused]] const StdClock* restrict self) {
	return STD_CLOCK_NANOSECONDS;
}

StdRatio __std_steady_clock_resolution_as_ratio([[maybe_unused]] const StdClock* restrict self) {
	return std_nanoseconds_period;
}

StdTimePointLocale __std_steady_clock_locale([[maybe_unused]] const StdClock* restrict self) {
	return STD_UNKNOWN_TIME;
}

StdString __std_steady_clock_format_with_allocator([[maybe_unused]] const StdClock* restrict self,
												   StdAllocator allocator) {
	return std_string_from_with_allocator("StdClock: std_steady_clock", allocator);
}

StdString __std_steady_clock_format(const StdClock* restrict self) {
	return __std_steady_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

StdTimePoint std_steady_clock_now(void) {
	return __std_steady_clock_now(&std_steady_clock);
}

StdTimePoint std_steady_clock_min_time_point(void) {
	return __std_steady_clock_min_time_point(&std_steady_clock);
}

StdTimePoint std_steady_clock_max_time_point(void) {
	return __std_steady_clock_max_time_point(&std_steady_clock);
}

StdClockResolution std_steady_clock_resolution(void) {
	return __std_steady_clock_resolution(&std_steady_clock);
}

StdRatio std_steady_clock_resolution_as_ratio(void) {
	return __std_steady_clock_resolution_as_ratio(&std_steady_clock);
}

StdTimePointLocale std_steady_clock_locale(void) {
	return __std_steady_clock_locale(&std_steady_clock);
}

#endif // !STD_NO_MONOTONIC_CLOCK

StdTimePoint std_high_resolution_clock_now(void) {
	return trait_call(now, std_high_resolution_clock);
}

StdTimePoint std_high_resolution_clock_min_time_point(void) {
	return trait_call(min_time_point, std_high_resolution_clock);
}

StdTimePoint std_high_resolution_clock_max_time_point(void) {
	return trait_call(min_time_point, std_high_resolution_clock);
}

StdClockResolution std_high_resolution_clock_resolution(void) {
	return trait_call(resolution, std_high_resolution_clock);
}

StdRatio std_high_resolution_clock_resolution_as_ratio(void) {
	return trait_call(resolution_as_ratio, std_high_resolution_clock);
}

StdTimePointLocale std_high_resolution_clock_locale(void) {
	return trait_call(locale, std_high_resolution_clock);
}

StdDuration local_time_gmt_offset(void) {
	let local = std_time_point_as_time_t(trait_call(now, std_system_clock));
	let_mut parsed = gmtime(&local);

	// request mktime looks up daylight savings time in tz database
	parsed->tm_isdst = -1;
	let gmt = mktime(parsed);
	let raw = difftime(local, gmt);
	let seconds = static_cast(i64)(raw);
	let microseconds = static_cast(i64)((raw - static_cast(double)(seconds))
										* static_cast(double)(std_microseconds_period.den));
	return std_microseconds(seconds * std_microseconds_period.den + microseconds);
}

StdTimePoint __std_utc_clock_now([[maybe_unused]] const StdClock* restrict self) {
#if STD_PLATFORM_WINDOWS
	return std_convert_local_time_to_utc(trait_call(now, std_system_clock));
#else
	let_mut time = trait_call(now, std_system_clock);
	time.clock = self;
	return time;
#endif
}

StdTimePoint __std_utc_clock_min_time_point([[maybe_unused]] const StdClock* restrict self) {
#if STD_PLATFORM_WINDOWS
	return std_convert_local_time_to_utc(trait_call(min_time_point, std_system_clock));
#else
	let_mut time = trait_call(min_time_point, std_system_clock);
	time.clock = self;
	return time;
#endif
}

StdTimePoint __std_utc_clock_max_time_point([[maybe_unused]] const StdClock* restrict self) {
#if STD_PLATFORM_WINDOWS
	return std_convert_local_time_to_utc(trait_call(max_time_point, std_system_clock));
#else
	let_mut time = trait_call(max_time_point, std_system_clock);
	time.clock = self;
	return time;
#endif
}

StdClockResolution __std_utc_clock_resolution([[maybe_unused]] const StdClock* restrict self) {
	return trait_call(resolution, std_system_clock);
}

StdRatio __std_utc_clock_resolution_as_ratio([[maybe_unused]] const StdClock* restrict self) {
	return trait_call(resolution_as_ratio, std_system_clock);
}

StdTimePointLocale __std_utc_clock_locale([[maybe_unused]] const StdClock* restrict self) {
	return STD_UTC_TIME;
}

StdString __std_utc_clock_format(const StdClock* restrict self) {
	return __std_utc_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

StdString __std_utc_clock_format_with_allocator([[maybe_unused]] const StdClock* restrict self,
												StdAllocator allocator) {
	return std_string_from_with_allocator("StdClock: std_utc_clock", allocator);
}

StdTimePoint std_utc_clock_now(void) {
	return trait_call(now, std_utc_clock);
}

StdTimePoint std_utc_clock_min_time_point(void) {
	return trait_call(min_time_point, std_utc_clock);
}

StdTimePoint std_utc_clock_max_time_point(void) {
	return trait_call(max_time_point, std_utc_clock);
}

StdClockResolution std_utc_clock_resolution(void) {
	return trait_call(resolution, std_utc_clock);
}

StdRatio std_utc_clock_resolution_as_ratio(void) {
	return trait_call(resolution_as_ratio, std_utc_clock);
}

StdTimePointLocale std_utc_clock_locale(void) {
	return trait_call(locale, std_utc_clock);
}

StdTimePoint std_convert_utc_to_local_time(StdTimePoint utc) {
	std_assert(utc.locale == STD_UTC_TIME,
			   "Can't convert a non-UTC time StdTimePoint to local time");
	let offset = local_time_gmt_offset();
	let checked = std_checked_add_i64(utc.time_since_epoch.count, offset.count);
	if(std_option_is_some(checked)) {
		utc.time_since_epoch = std_duration_add(utc.time_since_epoch, offset);
		utc.clock = &std_local_clock;
		utc.locale = STD_LOCAL_TIME;
		return utc;
	}
	else {
		let_mut point = utc.time_since_epoch.count > 0 ?
							trait_call(max_time_point, std_system_clock) :
							trait_call(min_time_point, std_system_clock);
		point.clock = &std_local_clock;
		point.locale = STD_LOCAL_TIME;
		return point;
	}
}

StdTimePoint std_convert_local_time_to_utc(StdTimePoint local_time) {
	std_assert(local_time.locale == STD_LOCAL_TIME,
			   "Can't convert a non-local-time StdTimePoint to UTC time");
	let offset = local_time_gmt_offset();
	let checked = std_checked_sub_i64(local_time.time_since_epoch.count, offset.count);
	if(std_option_is_some(checked)) {
		local_time.time_since_epoch = std_duration_subtract(local_time.time_since_epoch, offset);
		local_time.clock = &std_utc_clock;
		local_time.locale = STD_UTC_TIME;
		return local_time;
	}
	else {
		let_mut point = local_time.time_since_epoch.count > 0 ?
							trait_call(max_time_point, std_system_clock) :
							trait_call(min_time_point, std_system_clock);
		point.clock = &std_local_clock;
		point.locale = STD_UTC_TIME;
		return point;
	}
}

StdTimePoint __std_local_clock_now([[maybe_unused]] const StdClock* restrict self) {
#if STD_PLATFORM_WINDOWS
	let_mut time = trait_call(now, std_system_clock);
	time.clock = self;
	return time;
#else
	return std_convert_utc_to_local_time(trait_call(now, std_system_clock));
#endif
}

StdTimePoint __std_local_clock_min_time_point([[maybe_unused]] const StdClock* restrict self) {
#if STD_PLATFORM_WINDOWS
	let_mut time = trait_call(min_time_point, std_system_clock);
	time.clock = self;
	return time;
#else
	return std_convert_utc_to_local_time(trait_call(min_time_point, std_system_clock));
#endif
}

StdTimePoint __std_local_clock_max_time_point([[maybe_unused]] const StdClock* restrict self) {
#if STD_PLATFORM_WINDOWS
	let_mut time = trait_call(max_time_point, std_system_clock);
	time.clock = self;
	return time;
#else
	return std_convert_utc_to_local_time(trait_call(max_time_point, std_system_clock));
#endif
}

StdClockResolution __std_local_clock_resolution([[maybe_unused]] const StdClock* restrict self) {
	return trait_call(resolution, std_system_clock);
}

StdRatio __std_local_clock_resolution_as_ratio([[maybe_unused]] const StdClock* restrict self) {
	return trait_call(resolution_as_ratio, std_system_clock);
}

StdTimePointLocale __std_local_clock_locale([[maybe_unused]] const StdClock* restrict self) {
	return STD_LOCAL_TIME;
}

StdString __std_local_clock_format(const StdClock* restrict self) {
	return __std_local_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

StdString __std_local_clock_format_with_allocator([[maybe_unused]] const StdClock* restrict self,
												  StdAllocator allocator) {
	return std_string_from_with_allocator("StdClock: std_local_clock", allocator);
}

StdTimePoint std_local_clock_now(void) {
	return trait_call(now, std_local_clock);
}

StdTimePoint std_local_clock_min_time_point(void) {
	return trait_call(min_time_point, std_local_clock);
}

StdTimePoint std_local_clock_max_time_point(void) {
	return trait_call(max_time_point, std_local_clock);
}

StdClockResolution std_local_clock_resolution(void) {
	return trait_call(resolution, std_local_clock);
}

StdRatio std_local_clock_resolution_as_ratio(void) {
	return trait_call(resolution_as_ratio, std_local_clock);
}

StdTimePointLocale std_local_clock_locale(void) {
	return trait_call(locale, std_local_clock);
}

StdTimePoint std_clock_now(const StdClock* restrict self) {
	return trait_call(now, *self);
}

StdTimePoint std_clock_min_time_point(const StdClock* restrict self) {
	return trait_call(min_time_point, *self);
}

StdTimePoint std_clock_max_time_point(const StdClock* restrict self) {
	return trait_call(max_time_point, *self);
}

StdClockResolution std_clock_resolution(const StdClock* restrict self) {
	return trait_call(resolution, *self);
}

StdRatio std_clock_resolution_as_ratio(const StdClock* restrict self) {
	return trait_call(resolution_as_ratio, *self);
}

StdTimePointLocale std_clock_locale(const StdClock* restrict self) {
	return trait_call(locale, *self);
}

StdString std_clock_format(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_clock_format_with_allocator(self, specifier, DEFAULT_ALLOCATOR);
}

StdString std_clock_format_with_allocator(const StdFormat* restrict self,
										  [[maybe_unused]] StdFormatSpecifier specifier,
										  StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == STD_FORMAT_TYPE_DEBUG,
			   "Can only format a StdClock with default or debug format specifier");

	let _self = static_cast(const StdClock*)(self->m_self);

	return trait_call(format_with_allocator, *_self, allocator);
}
