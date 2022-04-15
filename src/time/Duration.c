/// @file Duration.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides methods for dealing with durations of time
/// @version 0.1.2
/// @date 2022-01-11
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

#include <Cnx/Assert.h>
#include <Cnx/time/Duration.h>

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
#if CNX_PLATFORM_COMPILER_GCC
		default: unreachable();
#endif
	}
}

#if CNX_PLATFORM_DEBUG
/// @brief checks that the given `CnxRatio` is a valid period for a `CnxDuration` or other
/// time-related facilities
/// Only enabled in Debug builds
/// @param new_period - The period to check
void duration_period_assert(CnxRatio new_period) {
	bool valid = false;
	ranged_for(index,
			   static_cast(usize)(0),
			   static_cast(usize)(sizeof(cnx_duration_valid_periods) / sizeof(CnxRatio))) {
		if(cnx_ratio_equal(cnx_duration_valid_periods[index], new_period)) {
			valid = true;
		}
	}
	cnx_assert(
		valid,
		"Given period to convert to is not a valid time period (not in cnx_duration_valid_periods");
}

	/// @brief checks that the given `CnxRatio` is a valid period for a `CnxDuration` or other
	/// time-related facilities
	/// Only enabled in Debug builds
	/// @param new_period - The period to check
	#define DURATION_PERIOD_ASSERT(new_period) duration_period_assert(new_period)
#else
	/// @brief checks that the given `CnxRatio` is a valid period for a `CnxDuration` or other
	/// time-related facilities
	/// Only enabled in Debug builds
	/// @param new_period - The period to check
	#define DURATION_PERIOD_ASSERT(new_period)
#endif // CNX_PLATFORM_DEBUG

CnxDuration cnx_duration_new(i64 count, CnxRatio period) {
	DURATION_PERIOD_ASSERT(period);
	return (CnxDuration){.count = count, .period = period};
}

static inline CnxDuration
duration_cast(CnxDuration to_cast, CnxRatio new_period, DurationCastType cast_type) {
	if(!cnx_ratio_equal(to_cast.period, new_period)) {
		DURATION_PERIOD_ASSERT(new_period);
		let seconds = cnx_ratio_as_scalar(cnx_ratio_multiply_scalar(to_cast.period, to_cast.count));
		let count = (seconds * static_cast(f64)(new_period.den)) / static_cast(f64)(new_period.num);
		return (CnxDuration){.count = cast(count, cast_type), .period = new_period};
	}
	else {
		return to_cast;
	}
}

CnxDuration cnx_duration_cast(CnxDuration to_cast, CnxRatio new_period) {
	return duration_cast(to_cast, new_period, FLOOR);
}

CnxDuration cnx_duration_floor(CnxDuration to_floor, CnxRatio new_period) {
	return duration_cast(to_floor, new_period, FLOOR);
}

CnxDuration cnx_duration_ceil(CnxDuration to_ceil, CnxRatio new_period) {
	return duration_cast(to_ceil, new_period, CEIL);
}

CnxDuration cnx_duration_round(CnxDuration to_round, CnxRatio new_period) {
	return duration_cast(to_round, new_period, ROUND);
}

CnxDuration cnx_duration_abs(CnxDuration duration) {
	return (CnxDuration){.count = cnx_abs(duration.count), .period = duration.period};
}

CnxDuration cnx_duration_add(CnxDuration lhs, CnxDuration rhs) {
	let as_lhs = cnx_duration_cast(rhs, lhs.period);
	lhs.count += as_lhs.count;
	return lhs;
}

CnxDuration cnx_duration_add_scalar(CnxDuration lhs, i64 rhs) {
	lhs.count += rhs;
	return lhs;
}

CnxDuration cnx_duration_subtract(CnxDuration lhs, CnxDuration rhs) {
	let as_lhs = cnx_duration_cast(rhs, lhs.period);
	lhs.count -= as_lhs.count;
	return lhs;
}

CnxDuration cnx_duration_subtract_scalar(CnxDuration lhs, i64 rhs) {
	lhs.count -= rhs;
	return lhs;
}

CnxDuration cnx_duration_multiply(CnxDuration lhs, i64 rhs) {
	lhs.count *= rhs;
	return lhs;
}

CnxDuration cnx_duration_divide(CnxDuration lhs, i64 rhs) {
	lhs.count /= rhs;
	return lhs;
}

bool cnx_duration_equal(CnxDuration lhs, CnxDuration rhs) {
	return cnx_duration_compare(lhs, rhs) == CNX_EQUAL;
}

bool cnx_duration_not_equal(CnxDuration lhs, CnxDuration rhs) {
	return cnx_duration_compare(lhs, rhs) != CNX_EQUAL;
}

bool cnx_duration_less_than(CnxDuration lhs, CnxDuration rhs) {
	return cnx_duration_compare(lhs, rhs) == CNX_LESS_THAN;
}

bool cnx_duration_less_than_or_equal(CnxDuration lhs, CnxDuration rhs) {
	let compare = cnx_duration_compare(lhs, rhs);
	return compare == CNX_LESS_THAN || compare == CNX_EQUAL;
}

bool cnx_duration_greater_than(CnxDuration lhs, CnxDuration rhs) {
	return cnx_duration_compare(lhs, rhs) == CNX_GREATER_THAN;
}

bool cnx_duration_greater_than_or_equal(CnxDuration lhs, CnxDuration rhs) {
	let compare = cnx_duration_compare(lhs, rhs);
	return compare == CNX_GREATER_THAN || compare == CNX_EQUAL;
}

CnxCompare cnx_duration_compare(CnxDuration lhs, CnxDuration rhs) {
	CnxDuration compare_left;
	CnxDuration compare_right;
	// use whichever period of the two is the most precise to do the comparison with
	if(cnx_ratio_less_than(lhs.period, rhs.period)) {
		compare_left = lhs;
		compare_right = cnx_duration_cast(rhs, lhs.period);
	}
	else {
		compare_left = cnx_duration_cast(lhs, rhs.period);
		compare_right = rhs;
	}
	if(compare_left.count == compare_right.count) {
		return CNX_EQUAL;
	}
	else if(compare_left.count < compare_right.count) {
		return CNX_LESS_THAN;
	}
	else {
		return CNX_GREATER_THAN;
	}
}

CnxString cnx_duration_format(const CnxFormat* restrict self, CnxFormatSpecifier specifier) {
	return cnx_duration_format_with_allocator(self, specifier, DEFAULT_ALLOCATOR);
}

CnxString cnx_duration_format_with_allocator(const CnxFormat* restrict self,
											 [[maybe_unused]] CnxFormatSpecifier specifier,
											 CnxAllocator allocator) {
	cnx_assert(specifier.m_type == CNX_FORMAT_TYPE_DEFAULT
				   || specifier.m_type == CNX_FORMAT_TYPE_DEBUG,
			   "Can only format a CnxDuration with default or debug format specifier");

	let _self = static_cast(const CnxDuration*)(self->m_self);
	if(specifier.m_type == CNX_FORMAT_TYPE_DEBUG) {
		return cnx_format_with_allocator(AS_STRING(CnxDuration) ": [count = {D}, period = {D}]",
										 allocator,
										 _self->count,
										 as_format_t(CnxRatio, _self->period));
	}
	else {
		return cnx_format_with_allocator("{} * {} seconds",
										 allocator,
										 _self->count,
										 as_format_t(CnxRatio, _self->period));
	}
}
