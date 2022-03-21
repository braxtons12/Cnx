/// @file StdTimePoint.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdTimePoint provides functionality for working with specific points in time
/// @version 0.1.2
/// @date 2022-03-20
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
#include <C2nxt/time/StdTimePoint.h>
#include <time.h>

StdTimePoint std_time_point_new(StdDuration time_since_epoch) {
	return std_time_point_new_with_clock(time_since_epoch, &std_system_clock);
}

StdTimePoint
std_time_point_new_with_clock(StdDuration time_since_epoch, const StdClock* restrict clock) {
	let precision = trait_call(resolution_as_ratio, *clock);
	StdDuration time = time_since_epoch;
	if(!std_ratio_equal(precision, time_since_epoch.period)) {
		time = std_duration_cast(time_since_epoch, precision);
	}
	return (StdTimePoint){.time_since_epoch = time, .clock = clock, trait_call(locale, *clock)};
}

StdTimePoint std_time_point_new_with_clock_and_locale(StdDuration time_since_epoch,
													  const StdClock* restrict clock,
													  StdTimePointLocale locale) {
	let precision = trait_call(resolution_as_ratio, *clock);
	StdDuration time = time_since_epoch;
	if(!std_ratio_equal(precision, time_since_epoch.period)) {
		time = std_duration_cast(time_since_epoch, precision);
	}
	return (StdTimePoint){.time_since_epoch = time, .clock = clock, .locale = locale};
}

StdDuration std_time_point_time_since_epoch(StdTimePoint self) {
	return self.time_since_epoch;
}

StdTimePoint std_time_point_min(StdTimePoint self) {
	return trait_call(min_time_point, (*self.clock));
}

StdTimePoint std_time_point_max(StdTimePoint self) {
	return trait_call(max_time_point, (*self.clock));
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
StdTimePoint std_time_point_cast(StdTimePoint to_cast, StdTimePoint new_precision) {
	return (StdTimePoint){
		.time_since_epoch
		= std_duration_cast(to_cast.time_since_epoch, new_precision.time_since_epoch.period),
		.clock = new_precision.clock};
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
StdTimePoint std_time_point_ceil(StdTimePoint to_cast, StdTimePoint new_precision) {
	return (StdTimePoint){
		.time_since_epoch
		= std_duration_ceil(to_cast.time_since_epoch, new_precision.time_since_epoch.period),
		.clock = new_precision.clock};
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
StdTimePoint std_time_point_floor(StdTimePoint to_cast, StdTimePoint new_precision) {
	return (StdTimePoint){
		.time_since_epoch
		= std_duration_floor(to_cast.time_since_epoch, new_precision.time_since_epoch.period),
		.clock = new_precision.clock};
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
StdTimePoint std_time_point_round(StdTimePoint to_cast, StdTimePoint new_precision) {
	return (StdTimePoint){
		.time_since_epoch
		= std_duration_round(to_cast.time_since_epoch, new_precision.time_since_epoch.period),
		.clock = new_precision.clock};
}

time_t std_time_point_as_time_t(StdTimePoint to_cast) {
	return static_cast(time_t)(
		std_duration_cast(to_cast.time_since_epoch, std_seconds_period).count);
}

#define RESULT_IMPL TRUE
#define RESULT_T				  tm
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_IMPL

StdResult(tm) std_time_point_as_tm(StdTimePoint to_cast) {
	if(to_cast.locale == STD_LOCAL_TIME) {
		// localtime requires a
		let point = std_convert_local_time_to_utc(to_cast);
		let time = std_time_point_as_time_t(point);
#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
		tm as_tm;
		let res = localtime_r(&time, &as_tm);
		if(res == nullptr) {
			return Err(tm, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
		}
		return Ok(tm, as_tm);
#else
		let res = localtime(&time);
		if(res == nullptr) {
			return Err(tm, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
		}
		return Ok(tm, *res);
#endif
	}
	else {
		let time = std_time_point_as_time_t(to_cast);
#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
		tm as_tm;
		let res = gmtime_r(&time, &as_tm);
		if(res == nullptr) {
			return Err(tm, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
		}
		return Ok(tm, as_tm);
#else
		let res = gmtime(&time);
		if(res == nullptr) {
			return Err(tm, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
		}
		return Ok(tm, *res);
#endif
	}
}

StdTimePoint std_time_point_from_time_t(time_t time) {
	let duration = std_duration_cast(std_duration_new(static_cast(i64)(time), std_seconds_period),
									 trait_call(resolution_as_ratio, std_system_clock));
	return std_time_point_new_with_clock(duration, &std_system_clock);
}

void std_time_point_increment(StdTimePoint* restrict self) {
	self->time_since_epoch = std_duration_add(self->time_since_epoch,
											  std_duration_new(1, self->time_since_epoch.period));
}

void std_time_point_decrement(StdTimePoint* restrict self) {
	self->time_since_epoch
		= std_duration_subtract(self->time_since_epoch,
								std_duration_new(1, self->time_since_epoch.period));
}

StdTimePoint std_time_point_add(StdTimePoint lhs, StdDuration rhs) {
	return (StdTimePoint){.time_since_epoch = std_duration_add(lhs.time_since_epoch, rhs),
						  .clock = lhs.clock};
}

StdTimePoint std_time_point_add_time_point(StdTimePoint lhs, StdTimePoint rhs) {
	return std_time_point_add(lhs, rhs.time_since_epoch);
}

StdTimePoint std_time_point_add_scalar(StdTimePoint lhs, i64 rhs) {
	return (StdTimePoint){.time_since_epoch = std_duration_add_scalar(lhs.time_since_epoch, rhs),
						  .clock = lhs.clock};
}

StdTimePoint std_time_point_subtract(StdTimePoint lhs, StdDuration rhs) {
	return (StdTimePoint){.time_since_epoch = std_duration_subtract(lhs.time_since_epoch, rhs),
						  .clock = lhs.clock};
}

StdTimePoint std_time_point_subtract_time_point(StdTimePoint lhs, StdTimePoint rhs) {
	return std_time_point_subtract(lhs, rhs.time_since_epoch);
}

StdTimePoint std_time_point_subtract_scalar(StdTimePoint lhs, i64 rhs) {
	return (StdTimePoint){.time_since_epoch
						  = std_duration_subtract_scalar(lhs.time_since_epoch, rhs),
						  .clock = lhs.clock};
}

bool std_time_point_equal(StdTimePoint lhs, StdTimePoint rhs) {
	return std_duration_equal(lhs.time_since_epoch, rhs.time_since_epoch);
}

bool std_time_point_not_equal(StdTimePoint lhs, StdTimePoint rhs) {
	return std_duration_not_equal(lhs.time_since_epoch, rhs.time_since_epoch);
}

bool std_time_point_less_than(StdTimePoint lhs, StdTimePoint rhs) {
	return std_duration_less_than(lhs.time_since_epoch, rhs.time_since_epoch);
}

bool std_time_point_less_than_or_equal(StdTimePoint lhs, StdTimePoint rhs) {
	return std_duration_less_than_or_equal(lhs.time_since_epoch, rhs.time_since_epoch);
}

bool std_time_point_greater_than(StdTimePoint lhs, StdTimePoint rhs) {
	return std_duration_greater_than(lhs.time_since_epoch, rhs.time_since_epoch);
}

bool std_time_point_greater_than_or_equal(StdTimePoint lhs, StdTimePoint rhs) {
	return std_duration_greater_than_or_equal(lhs.time_since_epoch, rhs.time_since_epoch);
}

StdCompare std_time_point_compare(StdTimePoint lhs, StdTimePoint rhs) {
	return std_duration_compare(lhs.time_since_epoch, rhs.time_since_epoch);
}

StdString std_time_point_human_readable_format(StdTimePoint self, StdAllocator allocator) {
	let_mut maybe_parsed = std_time_point_as_tm(self);
	let parsed_lvalue
		= std_result_expect(maybe_parsed, "Failed to convert StdTimePoint to struct tm");
	let parsed = &parsed_lvalue;

	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	char memory[20] = {0};

	if(self.locale != STD_UNKNOWN_TIME) {
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
		char utc[6] = {0};

		// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
		[[maybe_unused]] usize utc_written = strftime(utc, 6, "%z", parsed);
		std_assert(utc_written == 5, "Failed to format time point");
		std_string_scoped utc_string
			// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
			= std_string_new_with_capacity_with_allocator(6, allocator);
		let hours = std_stringview_from(utc, 0, 3);
		let minutes = std_stringview_from(utc, 3, 2);
		std_string_push_back(utc_string, ' ');
		std_string_append(utc_string, &hours);
		std_string_append(utc_string, &minutes);

		// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
		[[maybe_unused]] let written = strftime(memory, 20, "%F|%T", parsed);
		std_assert(written == 19, "Failed to format time point");
		let_mut str = std_string_from_with_allocator(memory, allocator);
		std_string_append(str, &utc_string);
		return str;
	}
	else {
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
		[[maybe_unused]] let written = strftime(memory, 20, "%F|%T", parsed);
		std_assert(written == 19, "Failed to format time point");
		return std_string_from_with_allocator(memory, allocator);
	}
}

StdString std_time_point_format(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_duration_format_with_allocator(self, specifier, DEFAULT_ALLOCATOR);
}

StdString std_time_point_format_with_allocator(const StdFormat* restrict self,
											   [[maybe_unused]] StdFormatSpecifier specifier,
											   StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == STD_FORMAT_TYPE_DEBUG,
			   "Can only format a StdTimePoint with default or debug format specifier");

	let _self = static_cast(const StdTimePoint*)(self->m_self);
	if(specifier.m_type == STD_FORMAT_TYPE_DEBUG) {
		return std_format_with_allocator(
			AS_STRING(StdTimePoint) ": [time_since_epoch = {D}, clock = {D}]",
			allocator,
			as_format_t(StdDuration, _self->time_since_epoch),
			as_format_t(StdClock, *_self->clock));
	}
	else {
		return std_time_point_human_readable_format(*_self, allocator);
	}
}
