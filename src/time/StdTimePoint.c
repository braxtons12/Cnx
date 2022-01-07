/// @file StdTimePoint.c
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

#include <C2nxt/time/StdClock.h>
#include <C2nxt/time/StdTimePoint.h>

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
	return (StdTimePoint){.time_since_epoch = time, .clock = clock};
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

StdString std_time_point_format(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_duration_format_with_allocator(self, specifier, DEFAULT_ALLOCATOR);
}

StdString std_time_point_format_with_allocator(const StdFormat* restrict self,
											   maybe_unused StdFormatSpecifier specifier,
											   StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT,
			   "Can't format a StdTimePoint with a custom format specifier");

	let _self = static_cast(const StdTimePoint*)(self->m_self);
	return std_format_with_allocator(
		AS_STRING(StdTimePoint) ": [time_since_epoch = {}, clock = {}]",
		allocator,
		as_format_t(StdDuration, _self->time_since_epoch),
		as_format_t(StdClock, *_self->clock));
}
