/// @file Clock.c
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

#include <Cnx/time/Clock.h>
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
#if !CNX_PLATFORM_APPLE && defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0
	#define __CNX_USE_CLOCK_GETTIME 1
#else
	#define __CNX_USE_CLOCK_GETTIME 0
#endif // !CNX_PLATFORM_APPLE && defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#if __has_include(<mach/mach_time.h>)
	#include <mach/mach_time.h>
#endif

#if CNX_PLATFORM_WINDOWS
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
CnxTimePoint __cnx_system_clock_now(const CnxClock* restrict self) {
	let filetime_period = cnx_ratio_multiply(cnx_ratio(100, 1), cnx_nanoseconds_period);
	// UNIX epoch starts at Jan 1 1970,
	// but Windows has to be weird, thus its epoch starts at Jan 1 1601
	let nt_to_unix_epoch = cnx_seconds(11644473600);

	FILETIME file_time;
	#if(_WIN32_WINNT >= _WIN32_WINNT_WIN7 && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)) \
		|| (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
	GetSystemTimePreciseAsFileTime(&file_time);
	#elif !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
	GetSystemTimeAsFileTime(&file_time);
	#else
	get_win_system_time_function()(&file_time);
	#endif

	let duration = (CnxDuration){
		.count
		= static_cast(i64)((static_cast(u64)(file_time.dwHighDateTime) << static_cast(u64)(32))
						   | static_cast(u64)(file_time.dwLowDateTime)),
		.period = filetime_period,
	};
	return cnx_time_point_new_with_clock_and_locale(
		cnx_duration_subtract(cnx_duration_cast(duration, cnx_microseconds_period),
							  nt_to_unix_epoch),
		self,
		CNX_LOCAL_TIME);
}

#elif defined(CLOCK_REALTIME) && __CNX_USE_CLOCK_GETTIME

CnxTimePoint __cnx_system_clock_now(const CnxClock* restrict self) {
	struct timespec time_spec;
	if(0 != clock_gettime(CLOCK_REALTIME, &time_spec)) {
		cnx_panic("clock_gettime(CLOCK_REALTIME) failed");
	}
	return cnx_time_point_new_with_clock_and_locale(
		cnx_duration_add(cnx_microseconds(time_spec.tv_nsec / 1000), cnx_seconds(time_spec.tv_sec)),
		self,
		CNX_UTC_TIME);
}

#else
CnxTimePoint __cnx_system_clock_now(const CnxClock* restrict self) {
	timeval time_val;
	gettimeofday(&time_val, 0);
	return cnx_time_point_new_with_clock_and_locale(
		cnx_duration_add(cnx_microseconds(time_val.tv_usec), cnx_seconds(time_val.tv_sec)),
		self,
		CNX_UTC_TIME);
}
#endif

CnxTimePoint __cnx_system_clock_min_time_point(const CnxClock* restrict self) {
	return cnx_time_point_new_with_clock(
		cnx_duration_new(cnx_min_value(i64), cnx_microseconds_period),
		self);
}

CnxTimePoint __cnx_system_clock_max_time_point(const CnxClock* restrict self) {
	return cnx_time_point_new_with_clock(
		cnx_duration_new(cnx_max_value(i64), cnx_microseconds_period),
		self);
}

CnxClockResolution __cnx_system_clock_resolution([[maybe_unused]] const CnxClock* restrict self) {
	return CNX_CLOCK_MICROSECONDS;
}

CnxRatio __cnx_system_clock_resolution_as_ratio([[maybe_unused]] const CnxClock* restrict self) {
	return cnx_microseconds_period;
}

CnxTimePointLocale __cnx_system_clock_locale([[maybe_unused]] const CnxClock* restrict self) {
	return CNX_LOCAL_TIME;
}

CnxString __cnx_system_clock_format_with_allocator([[maybe_unused]] const CnxClock* restrict self,
												   CnxAllocator allocator) {
	return cnx_string_from_with_allocator("CnxClock: cnx_system_clock", allocator);
}

CnxString __cnx_system_clock_format(const CnxClock* restrict self) {
	return __cnx_system_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

CnxTimePoint cnx_system_clock_now(void) {
	return __cnx_system_clock_now(&cnx_system_clock);
}

CnxTimePoint cnx_system_clock_min_time_point(void) {
	return __cnx_system_clock_min_time_point(&cnx_system_clock);
}

CnxTimePoint cnx_system_clock_max_time_point(void) {
	return __cnx_system_clock_max_time_point(&cnx_system_clock);
}

CnxClockResolution cnx_system_clock_resolution(void) {
	return __cnx_system_clock_resolution(&cnx_system_clock);
}

CnxRatio cnx_system_clock_resolution_as_ratio(void) {
	return __cnx_system_clock_resolution_as_ratio(&cnx_system_clock);
}

CnxTimePointLocale cnx_system_clock_locale(void) {
	return __cnx_system_clock_locale(&cnx_system_clock);
}

#if !cnx_NO_MONOTONIC_CLOCK
	#if CNX_PLATFORM_APPLE
CnxTimePoint __cnx_steady_clock_now(const CnxClock* restrict self) {
	struct timespec spec;
	if(0 != clock_gettime(CLOCK_MONOTONIC_RAW, &spec)) {
		cnx_panic("clock_gettime(CLOCK_MONOTONIC_RAW) failed!");
	}
	return cnx_time_point_new_with_clock_and_locale(
		cnx_duration_add(cnx_nanoseconds(spec.tv_nsec), cnx_seconds(spec.tv_sec)),
		self,
		CNX_UNKNOWN_TIME);
}
	#elif CNX_PLATFORM_WINDOWS
[[always_inline]] static inline LARGE_INTEGER query_performance_frequency(void) {
	LARGE_INTEGER val;
	ignore(QueryPerformanceFrequency(&val));
	return val;
}

CnxTimePoint __cnx_steady_clock_now(const CnxClock* restrict self) {
	let frequency = query_performance_frequency();

	LARGE_INTEGER counter;
	ignore(QueryPerformanceCounter(&counter));
	let seconds = counter.QuadPart / frequency.QuadPart;
	let remainder = counter.QuadPart % frequency.QuadPart;
	let nanoseconds = seconds * cnx_nanoseconds_period.den
					  + remainder * cnx_nanoseconds_period.den / frequency.QuadPart;
	return cnx_time_point_new_with_clock_and_locale(cnx_nanoseconds(nanoseconds),
													self,
													CNX_UNKNOWN_TIME);
}
	#elif CNX_PLATFORM_ZOS
CnxTimePoint __cnx_steady_clock_now(const CnxClock* restrict self) {
	struct timespec64 spec;
	if(0 != gettimeofdayMonotonic(&spec)) {
		cnx_panic("gettimeofdayMonotonic failed!");
	}

	return cnx_time_point_new_with_clock_and_locale(
		cnx_duration_add(cnx_nanoseconds(spec.tv_nsec), cnx_seconds(spec.tv_sec)),
		self,
		CNX_UNKNOWN_TIME);
}
	#elif defined(CLOCK_MONOTONIC)
CnxTimePoint __cnx_steady_clock_now(const CnxClock* restrict self) {
	struct timespec spec;
	if(0 != clock_gettime(CLOCK_MONOTONIC, &spec)) {
		cnx_panic("clock_gettime(CLOCK_MONOTONIC) failed!");
	}
	return cnx_time_point_new_with_clock_and_locale(
		cnx_duration_add(cnx_nanoseconds(spec.tv_nsec), cnx_seconds(spec.tv_sec)),
		self,
		CNX_UNKNOWN_TIME);
}
	#endif

CnxTimePoint __cnx_steady_clock_min_time_point(const CnxClock* restrict self) {
	return cnx_time_point_new_with_clock_and_locale(
		cnx_duration_new(cnx_min_value(i64), cnx_nanoseconds_period),
		self,
		CNX_UNKNOWN_TIME);
}

CnxTimePoint __cnx_steady_clock_max_time_point(const CnxClock* restrict self) {
	return cnx_time_point_new_with_clock_and_locale(
		cnx_duration_new(cnx_max_value(i64), cnx_nanoseconds_period),
		self,
		CNX_UNKNOWN_TIME);
}

CnxClockResolution __cnx_steady_clock_resolution([[maybe_unused]] const CnxClock* restrict self) {
	return CNX_CLOCK_NANOSECONDS;
}

CnxRatio __cnx_steady_clock_resolution_as_ratio([[maybe_unused]] const CnxClock* restrict self) {
	return cnx_nanoseconds_period;
}

CnxTimePointLocale __cnx_steady_clock_locale([[maybe_unused]] const CnxClock* restrict self) {
	return CNX_UNKNOWN_TIME;
}

CnxString __cnx_steady_clock_format_with_allocator([[maybe_unused]] const CnxClock* restrict self,
												   CnxAllocator allocator) {
	return cnx_string_from_with_allocator("CnxClock: cnx_steady_clock", allocator);
}

CnxString __cnx_steady_clock_format(const CnxClock* restrict self) {
	return __cnx_steady_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

CnxTimePoint cnx_steady_clock_now(void) {
	return __cnx_steady_clock_now(&cnx_steady_clock);
}

CnxTimePoint cnx_steady_clock_min_time_point(void) {
	return __cnx_steady_clock_min_time_point(&cnx_steady_clock);
}

CnxTimePoint cnx_steady_clock_max_time_point(void) {
	return __cnx_steady_clock_max_time_point(&cnx_steady_clock);
}

CnxClockResolution cnx_steady_clock_resolution(void) {
	return __cnx_steady_clock_resolution(&cnx_steady_clock);
}

CnxRatio cnx_steady_clock_resolution_as_ratio(void) {
	return __cnx_steady_clock_resolution_as_ratio(&cnx_steady_clock);
}

CnxTimePointLocale cnx_steady_clock_locale(void) {
	return __cnx_steady_clock_locale(&cnx_steady_clock);
}

#endif // !cnx_NO_MONOTONIC_CLOCK

CnxTimePoint cnx_high_resolution_clock_now(void) {
	return trait_call(now, cnx_high_resolution_clock);
}

CnxTimePoint cnx_high_resolution_clock_min_time_point(void) {
	return trait_call(min_time_point, cnx_high_resolution_clock);
}

CnxTimePoint cnx_high_resolution_clock_max_time_point(void) {
	return trait_call(min_time_point, cnx_high_resolution_clock);
}

CnxClockResolution cnx_high_resolution_clock_resolution(void) {
	return trait_call(resolution, cnx_high_resolution_clock);
}

CnxRatio cnx_high_resolution_clock_resolution_as_ratio(void) {
	return trait_call(resolution_as_ratio, cnx_high_resolution_clock);
}

CnxTimePointLocale cnx_high_resolution_clock_locale(void) {
	return trait_call(locale, cnx_high_resolution_clock);
}

CnxDuration local_time_gmt_offset(void) {
	let local = cnx_time_point_as_time_t(trait_call(now, cnx_system_clock));
	let_mut parsed = gmtime(&local);

	// request mktime looks up daylight savings time in tz database
	parsed->tm_isdst = -1;
	let gmt = mktime(parsed);
	let raw = difftime(local, gmt);
	let seconds = static_cast(i64)(raw);
	let microseconds = static_cast(i64)((raw - static_cast(double)(seconds))
										* static_cast(double)(cnx_microseconds_period.den));
	return cnx_microseconds(seconds * cnx_microseconds_period.den + microseconds);
}

CnxTimePoint __cnx_utc_clock_now([[maybe_unused]] const CnxClock* restrict self) {
#if CNX_PLATFORM_WINDOWS
	return cnx_convert_local_time_to_utc(trait_call(now, cnx_system_clock));
#else
	let_mut time = trait_call(now, cnx_system_clock);
	time.clock = self;
	return time;
#endif
}

CnxTimePoint __cnx_utc_clock_min_time_point([[maybe_unused]] const CnxClock* restrict self) {
#if CNX_PLATFORM_WINDOWS
	return cnx_convert_local_time_to_utc(trait_call(min_time_point, cnx_system_clock));
#else
	let_mut time = trait_call(min_time_point, cnx_system_clock);
	time.clock = self;
	return time;
#endif
}

CnxTimePoint __cnx_utc_clock_max_time_point([[maybe_unused]] const CnxClock* restrict self) {
#if CNX_PLATFORM_WINDOWS
	return cnx_convert_local_time_to_utc(trait_call(max_time_point, cnx_system_clock));
#else
	let_mut time = trait_call(max_time_point, cnx_system_clock);
	time.clock = self;
	return time;
#endif
}

CnxClockResolution __cnx_utc_clock_resolution([[maybe_unused]] const CnxClock* restrict self) {
	return trait_call(resolution, cnx_system_clock);
}

CnxRatio __cnx_utc_clock_resolution_as_ratio([[maybe_unused]] const CnxClock* restrict self) {
	return trait_call(resolution_as_ratio, cnx_system_clock);
}

CnxTimePointLocale __cnx_utc_clock_locale([[maybe_unused]] const CnxClock* restrict self) {
	return CNX_UTC_TIME;
}

CnxString __cnx_utc_clock_format(const CnxClock* restrict self) {
	return __cnx_utc_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

CnxString __cnx_utc_clock_format_with_allocator([[maybe_unused]] const CnxClock* restrict self,
												CnxAllocator allocator) {
	return cnx_string_from_with_allocator("CnxClock: cnx_utc_clock", allocator);
}

CnxTimePoint cnx_utc_clock_now(void) {
	return trait_call(now, cnx_utc_clock);
}

CnxTimePoint cnx_utc_clock_min_time_point(void) {
	return trait_call(min_time_point, cnx_utc_clock);
}

CnxTimePoint cnx_utc_clock_max_time_point(void) {
	return trait_call(max_time_point, cnx_utc_clock);
}

CnxClockResolution cnx_utc_clock_resolution(void) {
	return trait_call(resolution, cnx_utc_clock);
}

CnxRatio cnx_utc_clock_resolution_as_ratio(void) {
	return trait_call(resolution_as_ratio, cnx_utc_clock);
}

CnxTimePointLocale cnx_utc_clock_locale(void) {
	return trait_call(locale, cnx_utc_clock);
}

CnxTimePoint cnx_convert_utc_to_local_time(CnxTimePoint utc) {
	cnx_assert(utc.locale == CNX_UTC_TIME,
			   "Can't convert a non-UTC time CnxTimePoint to local time");
	let offset = local_time_gmt_offset();
	let checked = cnx_checked_add_i64(utc.time_since_epoch.count, offset.count);
	if(cnx_option_is_some(checked)) {
		utc.time_since_epoch = cnx_duration_add(utc.time_since_epoch, offset);
		utc.clock = &cnx_local_clock;
		utc.locale = CNX_LOCAL_TIME;
		return utc;
	}
	else {
		let_mut point = utc.time_since_epoch.count > 0 ?
							trait_call(max_time_point, cnx_system_clock) :
							trait_call(min_time_point, cnx_system_clock);
		point.clock = &cnx_local_clock;
		point.locale = CNX_LOCAL_TIME;
		return point;
	}
}

CnxTimePoint cnx_convert_local_time_to_utc(CnxTimePoint local_time) {
	cnx_assert(local_time.locale == CNX_LOCAL_TIME,
			   "Can't convert a non-local-time CnxTimePoint to UTC time");
	let offset = local_time_gmt_offset();
	let checked = cnx_checked_sub_i64(local_time.time_since_epoch.count, offset.count);
	if(cnx_option_is_some(checked)) {
		local_time.time_since_epoch = cnx_duration_subtract(local_time.time_since_epoch, offset);
		local_time.clock = &cnx_utc_clock;
		local_time.locale = CNX_UTC_TIME;
		return local_time;
	}
	else {
		let_mut point = local_time.time_since_epoch.count > 0 ?
							trait_call(max_time_point, cnx_system_clock) :
							trait_call(min_time_point, cnx_system_clock);
		point.clock = &cnx_local_clock;
		point.locale = CNX_UTC_TIME;
		return point;
	}
}

CnxTimePoint __cnx_local_clock_now([[maybe_unused]] const CnxClock* restrict self) {
#if CNX_PLATFORM_WINDOWS
	let_mut time = trait_call(now, cnx_system_clock);
	time.clock = self;
	return time;
#else
	return cnx_convert_utc_to_local_time(trait_call(now, cnx_system_clock));
#endif
}

CnxTimePoint __cnx_local_clock_min_time_point([[maybe_unused]] const CnxClock* restrict self) {
#if CNX_PLATFORM_WINDOWS
	let_mut time = trait_call(min_time_point, cnx_system_clock);
	time.clock = self;
	return time;
#else
	return cnx_convert_utc_to_local_time(trait_call(min_time_point, cnx_system_clock));
#endif
}

CnxTimePoint __cnx_local_clock_max_time_point([[maybe_unused]] const CnxClock* restrict self) {
#if CNX_PLATFORM_WINDOWS
	let_mut time = trait_call(max_time_point, cnx_system_clock);
	time.clock = self;
	return time;
#else
	return cnx_convert_utc_to_local_time(trait_call(max_time_point, cnx_system_clock));
#endif
}

CnxClockResolution __cnx_local_clock_resolution([[maybe_unused]] const CnxClock* restrict self) {
	return trait_call(resolution, cnx_system_clock);
}

CnxRatio __cnx_local_clock_resolution_as_ratio([[maybe_unused]] const CnxClock* restrict self) {
	return trait_call(resolution_as_ratio, cnx_system_clock);
}

CnxTimePointLocale __cnx_local_clock_locale([[maybe_unused]] const CnxClock* restrict self) {
	return CNX_LOCAL_TIME;
}

CnxString __cnx_local_clock_format(const CnxClock* restrict self) {
	return __cnx_local_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

CnxString __cnx_local_clock_format_with_allocator([[maybe_unused]] const CnxClock* restrict self,
												  CnxAllocator allocator) {
	return cnx_string_from_with_allocator("CnxClock: cnx_local_clock", allocator);
}

CnxTimePoint cnx_local_clock_now(void) {
	return trait_call(now, cnx_local_clock);
}

CnxTimePoint cnx_local_clock_min_time_point(void) {
	return trait_call(min_time_point, cnx_local_clock);
}

CnxTimePoint cnx_local_clock_max_time_point(void) {
	return trait_call(max_time_point, cnx_local_clock);
}

CnxClockResolution cnx_local_clock_resolution(void) {
	return trait_call(resolution, cnx_local_clock);
}

CnxRatio cnx_local_clock_resolution_as_ratio(void) {
	return trait_call(resolution_as_ratio, cnx_local_clock);
}

CnxTimePointLocale cnx_local_clock_locale(void) {
	return trait_call(locale, cnx_local_clock);
}

CnxTimePoint cnx_clock_now(const CnxClock* restrict self) {
	return trait_call(now, *self);
}

CnxTimePoint cnx_clock_min_time_point(const CnxClock* restrict self) {
	return trait_call(min_time_point, *self);
}

CnxTimePoint cnx_clock_max_time_point(const CnxClock* restrict self) {
	return trait_call(max_time_point, *self);
}

CnxClockResolution cnx_clock_resolution(const CnxClock* restrict self) {
	return trait_call(resolution, *self);
}

CnxRatio cnx_clock_resolution_as_ratio(const CnxClock* restrict self) {
	return trait_call(resolution_as_ratio, *self);
}

CnxTimePointLocale cnx_clock_locale(const CnxClock* restrict self) {
	return trait_call(locale, *self);
}

CnxString cnx_clock_format(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_clock_format_with_allocator(self, specifier, DEFAULT_ALLOCATOR);
}

CnxString cnx_clock_format_with_allocator(const CnxFormat* restrict self,
										  [[maybe_unused]] CnxFormatSpecifier specifier,
										  CnxAllocator allocator) {
	cnx_assert(specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == CNX_FORMAT_TYPE_DEBUG,
			   "Can only format a CnxClock with default or debug format specifier");

	let _self = static_cast(const CnxClock*)(self->m_self);

	return trait_call(format_with_allocator, *_self, allocator);
}
