/// @file StdDuration.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides methods for dealing with durations of time
/// @version 0.1.1
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

#include <C2nxt/StdAssert.h>
#include <C2nxt/time/StdDuration.h>

typedef enum {
	FLOOR = 0,
	CEIL = 1,
	ROUND = 2,
} DurationCastType;

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
static inline i64 cast(f64 value, DurationCastType cast_type) {
	switch(cast_type) { // NOLINT(hicpp-multiway-paths-covered)
		case FLOOR: return static_cast(i64)(value);
		case CEIL: return static_cast(i64)(ceil(value));
		case ROUND: return static_cast(i64)(llround(value));
	}
}

#if STD_PLATFORM_DEBUG
/// @brief checks that the given `StdRatio` is a valid period for a `StdDuration` or other
/// time-related facilities
/// Only enabled in Debug builds
/// @param new_period - The period to check
void duration_period_assert(StdRatio new_period) {
	bool valid = false;
	ranged_for(index,
			   static_cast(usize)(0),
			   static_cast(usize)(sizeof(std_duration_valid_periods) / sizeof(StdRatio))) {
		if(std_ratio_equal(std_duration_valid_periods[index], new_period)) {
			valid = true;
		}
	}
	std_assert(
		valid,
		"Given period to convert to is not a valid time period (not in std_duration_valid_periods");
}

	/// @brief checks that the given `StdRatio` is a valid period for a `StdDuration` or other
	/// time-related facilities
	/// Only enabled in Debug builds
	/// @param new_period - The period to check
	#define DURATION_PERIOD_ASSERT(new_period) duration_period_assert(new_period)
#else
	/// @brief checks that the given `StdRatio` is a valid period for a `StdDuration` or other
	/// time-related facilities
	/// Only enabled in Debug builds
	/// @param new_period - The period to check
	#define DURATION_PERIOD_ASSERT(new_period)
#endif // STD_PLATFORM_DEBUG

StdDuration std_duration_new(i64 count, StdRatio period) {
	DURATION_PERIOD_ASSERT(period);
	return (StdDuration){.count = count, .period = period};
}

static inline StdDuration
duration_cast(StdDuration to_cast, StdRatio new_period, DurationCastType cast_type) {
	if(!std_ratio_equal(to_cast.period, new_period)) {
		DURATION_PERIOD_ASSERT(new_period);
		let seconds = std_ratio_as_scalar(std_ratio_multiply_scalar(to_cast.period, to_cast.count));
		let count = (seconds * static_cast(f64)(new_period.den)) / static_cast(f64)(new_period.num);
		return (StdDuration){.count = cast(count, cast_type), .period = new_period};
	}
	else {
		return to_cast;
	}
}

StdDuration std_duration_cast(StdDuration to_cast, StdRatio new_period) {
	return duration_cast(to_cast, new_period, FLOOR);
}

StdDuration std_duration_floor(StdDuration to_floor, StdRatio new_period) {
	return duration_cast(to_floor, new_period, FLOOR);
}

StdDuration std_duration_ceil(StdDuration to_ceil, StdRatio new_period) {
	return duration_cast(to_ceil, new_period, CEIL);
}

StdDuration std_duration_round(StdDuration to_round, StdRatio new_period) {
	return duration_cast(to_round, new_period, ROUND);
}

StdDuration std_duration_abs(StdDuration duration) {
	return (StdDuration){.count = std_abs(duration.count), .period = duration.period};
}

StdDuration std_duration_add(StdDuration lhs, StdDuration rhs) {
	let as_lhs = std_duration_cast(rhs, lhs.period);
	lhs.count += as_lhs.count;
	return lhs;
}

StdDuration std_duration_add_scalar(StdDuration lhs, i64 rhs) {
	lhs.count += rhs;
	return lhs;
}

StdDuration std_duration_subtract(StdDuration lhs, StdDuration rhs) {
	let as_lhs = std_duration_cast(rhs, lhs.period);
	lhs.count -= as_lhs.count;
	return lhs;
}

StdDuration std_duration_subtract_scalar(StdDuration lhs, i64 rhs) {
	lhs.count -= rhs;
	return lhs;
}

StdDuration std_duration_multiply(StdDuration lhs, i64 rhs) {
	lhs.count *= rhs;
	return lhs;
}

StdDuration std_duration_divide(StdDuration lhs, i64 rhs) {
	lhs.count /= rhs;
	return lhs;
}

bool std_duration_equal(StdDuration lhs, StdDuration rhs) {
	return std_duration_compare(lhs, rhs) == STD_EQUAL;
}

bool std_duration_not_equal(StdDuration lhs, StdDuration rhs) {
	return std_duration_compare(lhs, rhs) != STD_EQUAL;
}

bool std_duration_less_than(StdDuration lhs, StdDuration rhs) {
	return std_duration_compare(lhs, rhs) == STD_LESS_THAN;
}

bool std_duration_less_than_or_equal(StdDuration lhs, StdDuration rhs) {
	let compare = std_duration_compare(lhs, rhs);
	return compare == STD_LESS_THAN || compare == STD_EQUAL;
}

bool std_duration_greater_than(StdDuration lhs, StdDuration rhs) {
	return std_duration_compare(lhs, rhs) == STD_GREATER_THAN;
}

bool std_duration_greater_than_or_equal(StdDuration lhs, StdDuration rhs) {
	let compare = std_duration_compare(lhs, rhs);
	return compare == STD_GREATER_THAN || compare == STD_EQUAL;
}

StdCompare std_duration_compare(StdDuration lhs, StdDuration rhs) {
	StdDuration compare_left;
	StdDuration compare_right;
	// use whichever period of the two is the most precise to do the comparison with
	if(std_ratio_less_than(lhs.period, rhs.period)) {
		compare_left = lhs;
		compare_right = std_duration_cast(rhs, lhs.period);
	}
	else {
		compare_left = std_duration_cast(lhs, rhs.period);
		compare_right = rhs;
	}
	if(compare_left.count == compare_right.count) {
		return STD_EQUAL;
	}
	else if(compare_left.count < compare_right.count) {
		return STD_LESS_THAN;
	}
	else {
		return STD_GREATER_THAN;
	}
}

StdString std_duration_format(const StdFormat* restrict self, StdFormatSpecifier specifier) {
	return std_duration_format_with_allocator(self, specifier, DEFAULT_ALLOCATOR);
}

StdString std_duration_format_with_allocator(const StdFormat* restrict self,
											 maybe_unused StdFormatSpecifier specifier,
											 StdAllocator allocator) {
	std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT,
			   "Can't format a StdDuration with a custom format specifier");

	let _self = static_cast(const StdDuration*)(self->m_self);
	return std_format_with_allocator(AS_STRING(StdDuration) ": [count = {}, period = {}]",
									 allocator,
									 _self->count,
									 as_format_t(StdRatio, _self->period));
}
