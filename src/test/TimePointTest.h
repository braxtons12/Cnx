#ifndef TIME_POINT_TEST
#define TIME_POINT_TEST

#include <C2nxt/StdIO.h>
#include <C2nxt/time/StdClock.h>
#include <C2nxt/time/StdTimePoint.h>

#include "Criterion.h"

// Implement a "mock" `StdClock` so we can use it to test `StdTimePoint`

typedef struct MockClock {
} MockClock;

StdTimePoint mock_clock_now(const StdClock* restrict self) {
	return std_time_point_new_with_clock(std_duration_new(0, std_seconds_period), self);
}

StdTimePoint mock_clock_min_time_point(const StdClock* restrict self) {
	return std_time_point_new_with_clock(std_duration_new(std_min_value(i64), std_seconds_period),
										 self);
}

StdTimePoint mock_clock_max_time_point(const StdClock* restrict self) {
	return std_time_point_new_with_clock(std_duration_new(std_max_value(i64), std_seconds_period),
										 self);
}

StdClockResolution mock_clock_resolution([[maybe_unused]] const StdClock* restrict self) {
	return STD_CLOCK_SECONDS;
}

StdRatio mock_clock_resolution_as_ratio([[maybe_unused]] const StdClock* restrict self) {
	return std_seconds_period;
}

StdTimePointLocale mock_clock_locale([[maybe_unused]] const StdClock* restrict self) {
	return STD_LOCAL_TIME;
}

StdString mock_clock_format_with_allocator([[maybe_unused]] const StdClock* restrict self,
										   StdAllocator allocator) {
	return std_string_from_with_allocator("MockClock", allocator);
}

StdString mock_clock_format(const StdClock* restrict self) {
	return mock_clock_format_with_allocator(self, DEFAULT_ALLOCATOR);
}

[[maybe_unused]] static ImplTraitFor(StdClock,
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
	(MockClock) { }
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

static let mock_clock = as_trait(StdClock, MockClock, __mock_clock);

TEST(StdTimePoint, new_and_comparisons) {
	let period = trait_call(resolution_as_ratio, mock_clock);
	let min = trait_call(min_time_point, mock_clock);
	let max = trait_call(max_time_point, mock_clock);

	let point0 = std_time_point_new_with_clock(std_duration_new(0, period), &mock_clock);
	let point5 = std_time_point_new_with_clock(std_duration_new(5, period), &mock_clock);
	let point10 = std_time_point_new_with_clock(std_duration_new(10, period), &mock_clock);
	let alternate
		= (StdTimePoint){.time_since_epoch = std_duration_new(20, std_milliseconds_period),
						 .clock = &mock_clock};

	let tp_min = std_time_point_min(point0);
	let tp_max = std_time_point_max(point0);

	TEST_ASSERT(std_time_point_equal(tp_min, min));
	TEST_ASSERT(std_time_point_equal(tp_max, max));

	TEST_ASSERT(std_time_point_less_than(point0, point5));
	TEST_ASSERT(std_time_point_less_than(point0, point10));
	TEST_ASSERT(std_time_point_less_than_or_equal(point0, point5));
	TEST_ASSERT(std_time_point_less_than_or_equal(point0, point10));

	TEST_ASSERT(std_time_point_greater_than(point10, point0));
	TEST_ASSERT(std_time_point_greater_than(point10, point5));
	TEST_ASSERT(std_time_point_greater_than_or_equal(point10, point0));
	TEST_ASSERT(std_time_point_greater_than_or_equal(point10, point5));

	TEST_ASSERT_EQUAL(std_time_point_compare(tp_min, min), STD_EQUAL);
	TEST_ASSERT_EQUAL(std_time_point_compare(point5, point0), STD_GREATER_THAN);
	TEST_ASSERT_EQUAL(std_time_point_compare(point5, point10), STD_LESS_THAN);

	TEST_ASSERT(std_time_point_less_than(alternate, point5));

	let casted = std_time_point_cast(alternate, point0);
	let ceiled = std_time_point_ceil(alternate, point0);
	let floored = std_time_point_floor(alternate, point0);
	let rounded = std_time_point_round(alternate, point0);

	let point1 = std_time_point_new_with_clock(std_duration_new(1, period), &mock_clock);
	TEST_ASSERT(std_time_point_equal(casted, point0));
	TEST_ASSERT(std_time_point_equal(ceiled, point1));
	TEST_ASSERT(std_time_point_equal(floored, point0));
	TEST_ASSERT(std_time_point_equal(rounded, point0));

	let alternate2
		= (StdTimePoint){.time_since_epoch = std_duration_new(5000, std_milliseconds_period),
						 .clock = &mock_clock};

	let casted2 = std_time_point_cast(alternate2, point0);
	TEST_ASSERT(std_time_point_equal(casted2, point5));

	let dur = std_duration_new(10, period);
	let time_since_epoch = std_time_point_time_since_epoch(point10);
	TEST_ASSERT(std_duration_equal(dur, time_since_epoch));
}

TEST(StdTimePoint, add) {
	let period = trait_call(resolution_as_ratio, mock_clock);
	let point0 = std_time_point_new_with_clock(std_duration_new(0, period), &mock_clock);
	let point5 = std_time_point_new_with_clock(std_duration_new(5, period), &mock_clock);
	let point10 = std_time_point_new_with_clock(std_duration_new(10, period), &mock_clock);
	let point11 = std_time_point_new_with_clock(std_duration_new(11, period), &mock_clock);
	let dur = std_duration_new(5, period);

	let added = std_time_point_add(point0, dur);
	TEST_ASSERT(std_time_point_equal(added, point5));
	let_mut added2 = std_time_point_add_time_point(added, point5);
	TEST_ASSERT(std_time_point_equal(added2, point10));
	std_time_point_increment(&added2);
	TEST_ASSERT(std_time_point_equal(added2, point11));
}

TEST(StdTimePoint, subtract) {
	let period = trait_call(resolution_as_ratio, mock_clock);
	let point0 = std_time_point_new_with_clock(std_duration_new(0, period), &mock_clock);
	let point5 = std_time_point_new_with_clock(std_duration_new(5, period), &mock_clock);
	let point10 = std_time_point_new_with_clock(std_duration_new(10, period), &mock_clock);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
	let_mut point11 = std_time_point_new_with_clock(std_duration_new(11, period), &mock_clock);
	let dur = std_duration_new(5, period);

	std_time_point_decrement(&point11);
	TEST_ASSERT(std_time_point_equal(point11, point10));
	let subtracted = std_time_point_subtract_time_point(point11, point5);
	TEST_ASSERT(std_time_point_equal(subtracted, point5));
	let_mut subtracted2 = std_time_point_subtract(subtracted, dur);
	TEST_ASSERT(std_time_point_equal(subtracted2, point0));
}

#endif // TIME_POINT_TEST
