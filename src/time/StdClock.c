#include <C2nxt/time/StdClock.h>
#include <time.h>

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
	#include <windows.h>

	#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
		#include <winapifamily.h>
	#endif // _WIN32_WINNT >= _WIN32_WINNT_WIN8

	#if _WIN32_WINNT < _WIN32_WINNT_WIN8
typedef void(WINAPI* GetSystemTimeAsFileTimePtr)(LPFILETIME);

static inline always_inline GetSystemTimeAsFileTimePtr get_win_system_time_function(void) {
	let_mut fp = static_cast(GetSystemTimeAsFileTimePtr)(
		GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetSystemTimePreciseAsFileTime"));
	if(fp == nullptr) {
		fp = GetSystemTimeAsFileTime;
	}
	return fp;
}

	#endif // _WIN32_WINNT < _WIN32_WINNT_WIN8

StdTimePoint __std_system_clock_now(const StdClock* restrict self) {
	let filetime_period = std_ratio_multiply(std_ratio(100, 1), std_nanoseconds_period);
	// UNIX epoch starts at Jan 1 1970,
	// but Windows has to be weird, thus its epoch starts at Jan 1 1601
	let nt_to_unix_epoch = std_seconds(11644473600);

	FILETIME ft;
	#if(_WIN32_WINNT >= _WIN32_WINNT_WIN7 && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)) \
		|| (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
	GetSystemTimePreciseAsFileTime(&ft);
	#elif !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
	GetSystemTimeAsFileTime(&ft);
	#else
	get_win_system_time_function()(&ft);
	#endif

	let duration = (StdDuration) {
		.count = static_cast(i64)(static_cast(i64)(ft.dwHighDateTime) << 32)
				 | static_cast(i64)(ft.dwLowDateTime),
		.period = filetime_period;
	}
	return std_time_point_new_with_clock(
		std_duration_subtract(std_duration_cast(duration, std_microseconds_period), nt_to_unix_epoch),
		self);
}

#elif defined(CLOCK_REALTIME) && __STD_USE_CLOCK_GETTIME

StdTimePoint __std_system_clock_now(const StdClock* restrict self) {
	struct timespec time_spec;
	if(0 != clock_gettime(CLOCK_REALTIME, &time_spec)) {
		std_panic("clock_gettime(CLOCK_REALTIME) failed");
	}
	return std_time_point_new_with_clock(
		std_duration_add(std_microseconds(time_spec.tv_nsec / 1000), std_seconds(time_spec.tv_sec)),
		self);
}

#else
StdTimePoint __std_system_clock_now(const StdClock* restrict self) {
	timeval time_val;
	gettimeofday(&time_val, 0);
	return std_time_point_new_with_clock(
		std_duration_add(std_microseconds(time_val.tv_usec), std_seconds(time_val.tv_sec)),
		self);
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

StdClockResolution __std_system_clock_resolution(const maybe_unused StdClock* restrict self) {
	return STD_CLOCK_MICROSECONDS;
}

StdRatio __std_system_clock_resolution_as_ratio(const maybe_unused StdClock* restrict self) {
	return std_microseconds_period;
}

StdString __std_system_clock_format_with_allocator(const maybe_unused StdClock* restrict self,
												   StdAllocator allocator) {
	return std_string_from_with_allocator("StdClock: std_system_clock", allocator);
}

StdString __std_system_clock_format(const StdClock* restrict self) {
	return __std_system_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

StdString std_clock_format(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_clock_format_with_allocator(self, specifier, DEFAULT_ALLOCATOR);
}

StdString std_clock_format_with_allocator(const StdFormat* restrict self,
										  maybe_unused StdFormatSpecifier specifier,
										  StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT,
			   "Can't format a StdClock with a custom format specifier");

	let _self = static_cast(const StdClock*)(self->m_self);

	return trait_call(format_with_allocator, *_self, allocator);
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
