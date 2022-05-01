#ifndef TIME_POINT_TEST
#define TIME_POINT_TEST

#include <Cnx/IO.h>
#include <Cnx/time/Clock.h>
#include <Cnx/time/TimePoint.h>

#include "Criterion.h"

// Implement a "mock" `CnxClock` so we can use it to test `CnxTimePoint`

typedef struct MockClock {
} MockClock;

CnxTimePoint mock_clock_now(const CnxClock* restrict self) {
	return cnx_time_point_new_with_clock(cnx_duration_new(0, cnx_seconds_period), self);
}

CnxTimePoint mock_clock_min_time_point(const CnxClock* restrict self) {
	return cnx_time_point_new_with_clock(cnx_duration_new(cnx_min_value(i64), cnx_seconds_period),
										 self);
}

CnxTimePoint mock_clock_max_time_point(const CnxClock* restrict self) {
	return cnx_time_point_new_with_clock(cnx_duration_new(cnx_max_value(i64), cnx_seconds_period),
										 self);
}

CnxClockResolution mock_clock_resolution(__attr(maybe_unused) const CnxClock* restrict self) {
	return CNX_CLOCK_SECONDS;
}

CnxRatio mock_clock_resolution_as_ratio(__attr(maybe_unused) const CnxClock* restrict self) {
	return cnx_seconds_period;
}

CnxTimePointLocale mock_clock_locale(__attr(maybe_unused) const CnxClock* restrict self) {
	return CNX_LOCAL_TIME;
}

CnxString mock_clock_format_with_allocator(__attr(maybe_unused) const CnxClock* restrict self,
										   CnxAllocator allocator) {
	return cnx_string_from_with_allocator("MockClock", allocator);
}

CnxString mock_clock_format(const CnxClock* restrict self) {
	return mock_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

__attr(maybe_unused) static ImplTraitFor(CnxClock,
										 MockClock,
										 mock_clock_now,
										 mock_clock_min_time_point,
										 mock_clock_max_time_point,
										 mock_clock_resolution,
										 mock_clock_resolution_as_ratio,
										 mock_clock_locale,
										 mock_clock_format,
										 mock_clock_format_with_allocator);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __mock_clock \
	(MockClock) {    \
	}
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

static let mock_clock = as_trait(CnxClock, MockClock, __mock_clock);

// NOLINTNEXTLINE(readability-function-cognitive-complexity,cppcoreguidelines-avoid-non-const-global-variables)
TEST(CnxTimePoint, new_and_comparisons) {
	let period = trait_call(resolution_as_ratio, mock_clock);
	let min = trait_call(min_time_point, mock_clock);
	let max = trait_call(max_time_point, mock_clock);

	let point0 = cnx_time_point_new_with_clock(cnx_duration_new(0, period), &mock_clock);
	let point5 = cnx_time_point_new_with_clock(cnx_duration_new(5, period), &mock_clock);
	let point10 = cnx_time_point_new_with_clock(cnx_duration_new(10, period), &mock_clock);
	let alternate
		= (CnxTimePoint){.time_since_epoch = cnx_duration_new(20, cnx_milliseconds_period),
						 .clock = &mock_clock};

	let tp_min = cnx_time_point_min(point0);
	let tp_max = cnx_time_point_max(point0);

	TEST_ASSERT(cnx_time_point_equal(tp_min, min));
	TEST_ASSERT(cnx_time_point_equal(tp_max, max));

	TEST_ASSERT(cnx_time_point_less_than(point0, point5));
	TEST_ASSERT(cnx_time_point_less_than(point0, point10));
	TEST_ASSERT(cnx_time_point_less_than_or_equal(point0, point5));
	TEST_ASSERT(cnx_time_point_less_than_or_equal(point0, point10));

	TEST_ASSERT(cnx_time_point_greater_than(point10, point0));
	TEST_ASSERT(cnx_time_point_greater_than(point10, point5));
	TEST_ASSERT(cnx_time_point_greater_than_or_equal(point10, point0));
	TEST_ASSERT(cnx_time_point_greater_than_or_equal(point10, point5));

	TEST_ASSERT_EQUAL(cnx_time_point_compare(tp_min, min), CNX_EQUAL);
	TEST_ASSERT_EQUAL(cnx_time_point_compare(point5, point0), CNX_GREATER_THAN);
	TEST_ASSERT_EQUAL(cnx_time_point_compare(point5, point10), CNX_LESS_THAN);

	TEST_ASSERT(cnx_time_point_less_than(alternate, point5));

	let casted = cnx_time_point_cast(alternate, point0);
	let ceiled = cnx_time_point_ceil(alternate, point0);
	let floored = cnx_time_point_floor(alternate, point0);
	let rounded = cnx_time_point_round(alternate, point0);

	let point1 = cnx_time_point_new_with_clock(cnx_duration_new(1, period), &mock_clock);
	TEST_ASSERT(cnx_time_point_equal(casted, point0));
	TEST_ASSERT(cnx_time_point_equal(ceiled, point1));
	TEST_ASSERT(cnx_time_point_equal(floored, point0));
	TEST_ASSERT(cnx_time_point_equal(rounded, point0));

	let alternate2
		= (CnxTimePoint){.time_since_epoch = cnx_duration_new(5000, cnx_milliseconds_period),
						 .clock = &mock_clock};

	let casted2 = cnx_time_point_cast(alternate2, point0);
	TEST_ASSERT(cnx_time_point_equal(casted2, point5));

	let dur = cnx_duration_new(10, period);
	let time_since_epoch = cnx_time_point_time_since_epoch(point10);
	TEST_ASSERT(cnx_duration_equal(dur, time_since_epoch));
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity,cppcoreguidelines-avoid-non-const-global-variables)
TEST(CnxTimePoint, add) {
	let period = trait_call(resolution_as_ratio, mock_clock);
	let point0 = cnx_time_point_new_with_clock(cnx_duration_new(0, period), &mock_clock);
	let point5 = cnx_time_point_new_with_clock(cnx_duration_new(5, period), &mock_clock);
	let point10 = cnx_time_point_new_with_clock(cnx_duration_new(10, period), &mock_clock);
	let point11 = cnx_time_point_new_with_clock(cnx_duration_new(11, period), &mock_clock);
	let dur = cnx_duration_new(5, period);

	let added = cnx_time_point_add(point0, dur);
	TEST_ASSERT(cnx_time_point_equal(added, point5));
	let_mut added2 = cnx_time_point_add_time_point(added, point5);
	TEST_ASSERT(cnx_time_point_equal(added2, point10));
	cnx_time_point_increment(&added2);
	TEST_ASSERT(cnx_time_point_equal(added2, point11));
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity,cppcoreguidelines-avoid-non-const-global-variables)
TEST(CnxTimePoint, subtract) {
	let period = trait_call(resolution_as_ratio, mock_clock);
	let point0 = cnx_time_point_new_with_clock(cnx_duration_new(0, period), &mock_clock);
	let point5 = cnx_time_point_new_with_clock(cnx_duration_new(5, period), &mock_clock);
	let point10 = cnx_time_point_new_with_clock(cnx_duration_new(10, period), &mock_clock);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
	let_mut point11 = cnx_time_point_new_with_clock(cnx_duration_new(11, period), &mock_clock);
	let dur = cnx_duration_new(5, period);

	cnx_time_point_decrement(&point11);
	TEST_ASSERT(cnx_time_point_equal(point11, point10));
	let subtracted = cnx_time_point_subtract_time_point(point11, point5);
	TEST_ASSERT(cnx_time_point_equal(subtracted, point5));
	let_mut subtracted2 = cnx_time_point_subtract(subtracted, dur);
	TEST_ASSERT(cnx_time_point_equal(subtracted2, point0));
}

#endif // TIME_POINT_TEST
