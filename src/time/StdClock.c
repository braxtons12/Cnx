#include <C2nxt/time/StdClock.h>

StdTimePoint __std_system_clock_now(const StdClock* restrict self) {
	return std_time_point_new_with_clock(std_duration_new(0, std_seconds_period), self);
}

StdTimePoint __std_system_clock_min_time_point(const StdClock* restrict self) {
	return std_time_point_new_with_clock(std_duration_new(std_min_value(i64), std_seconds_period),
										 self);
}

StdTimePoint __std_system_clock_max_time_point(const StdClock* restrict self) {
	return std_time_point_new_with_clock(std_duration_new(std_max_value(i64), std_seconds_period),
										 self);
}

StdClockResolution __std_system_clock_resolution(const maybe_unused StdClock* restrict self) {
	return STD_CLOCK_MICROSECONDS;
}

StdRatio __std_system_clock_resolution_as_ratio(const maybe_unused StdClock* restrict self) {
	return std_seconds_period;
}

StdString __std_system_clock_format_with_allocator(const maybe_unused StdClock* restrict self,
										   StdAllocator allocator) {
	return std_string_from_with_allocator("std_system_clock", allocator);
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
