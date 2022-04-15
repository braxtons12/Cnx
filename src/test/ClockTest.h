#ifndef CLOCK_TEST
#define CLOCK_TEST

#include <Cnx/time/Clock.h>

#include "Criterion.h"

#if __has_include(<uniCnx.h>)
	#define HAS_SLEEP 1
	#include <unistd.h>
#else
	#define HAS_SLEEP 0
#endif

TEST(CnxClock, system_clock) {
	let min_val = cnx_min_value(i64);
	let max_val = cnx_max_value(i64);

	TEST_ASSERT(cnx_clock_resolution(&cnx_system_clock) == CNX_CLOCK_MICROSECONDS);
	TEST_ASSERT(
		cnx_ratio_equal(cnx_clock_resolution_as_ratio(&cnx_system_clock), cnx_microseconds_period));
	TEST_ASSERT(cnx_time_point_equal(cnx_clock_min_time_point(&cnx_system_clock),
									 cnx_time_point_new(cnx_microseconds(min_val))));
	TEST_ASSERT(cnx_time_point_equal(cnx_clock_max_time_point(&cnx_system_clock),
									 cnx_time_point_new(cnx_microseconds(max_val))));

	let first = cnx_clock_now(&cnx_system_clock);
	let second = cnx_clock_now(&cnx_system_clock);
	TEST_ASSERT(cnx_time_point_less_than_or_equal(first, second));

#if HAS_SLEEP
	sleep(1);
	let third = cnx_clock_now(&cnx_system_clock);
	TEST_ASSERT(cnx_time_point_less_than(first, third));
#endif
}

#if !CNX_NO_MONOTONIC_CLOCK
TEST(CnxClock, steady_clock) {
	let min_val = cnx_min_value(i64);
	let max_val = cnx_max_value(i64);

	TEST_ASSERT(cnx_clock_resolution(&cnx_steady_clock) == CNX_CLOCK_NANOSECONDS);
	TEST_ASSERT(
		cnx_ratio_equal(cnx_clock_resolution_as_ratio(&cnx_steady_clock), cnx_nanoseconds_period));
	TEST_ASSERT(cnx_time_point_equal(
		cnx_clock_min_time_point(&cnx_steady_clock),
		cnx_time_point_new_with_clock(cnx_nanoseconds(min_val), &cnx_steady_clock)));
	TEST_ASSERT(cnx_time_point_equal(
		cnx_clock_max_time_point(&cnx_steady_clock),
		cnx_time_point_new_with_clock(cnx_nanoseconds(max_val), &cnx_steady_clock)));

	let first = cnx_clock_now(&cnx_steady_clock);
	let second = cnx_clock_now(&cnx_steady_clock);
	TEST_ASSERT(cnx_time_point_less_than_or_equal(first, second));

	#if HAS_SLEEP
	sleep(1);
	let third = cnx_clock_now(&cnx_steady_clock);
	TEST_ASSERT(cnx_time_point_less_than(first, third));
	#endif
}
#endif // !CNX_NO_MONOTONIC_CLOCK

TEST(CnxClock, high_resolution_clock) {
	let min_val = cnx_min_value(i64);
	let max_val = cnx_max_value(i64);

#if !CNX_NO_MONOTONIC_CLOCK
	TEST_ASSERT(cnx_clock_resolution(&cnx_high_resolution_clock) == CNX_CLOCK_NANOSECONDS);
	TEST_ASSERT(cnx_ratio_equal(cnx_clock_resolution_as_ratio(&cnx_high_resolution_clock),
								cnx_nanoseconds_period));
	TEST_ASSERT(cnx_time_point_equal(
		cnx_clock_min_time_point(&cnx_high_resolution_clock),
		cnx_time_point_new_with_clock(cnx_nanoseconds(min_val), &cnx_high_resolution_clock)));
	TEST_ASSERT(cnx_time_point_equal(
		cnx_clock_max_time_point(&cnx_high_resolution_clock),
		cnx_time_point_new_with_clock(cnx_nanoseconds(max_val), &cnx_high_resolution_clock)));
#else
	TEST_ASSERT(cnx_clock_resolution(&cnx_high_resolution_clock) == CNX_CLOCK_MICROSECONDS);
	TEST_ASSERT(cnx_ratio_equal(cnx_clock_resolution_as_ratio(&cnx_high_resolution_clock),
								cnx_microseconds_period));
	TEST_ASSERT(cnx_time_point_equal(
		cnx_clock_min_time_point(&cnx_high_resolution_clock),
		cnx_time_point_new_with_clock(cnx_microseconds(min_val), &cnx_high_resolution_clock)));
	TEST_ASSERT(cnx_time_point_equal(
		cnx_clock_max_time_point(&cnx_high_resolution_clock),
		cnx_time_point_new_with_clock(cnx_microseconds(max_val), &cnx_high_resolution_clock)));
#endif // !CNX_NO_MONOTONIC_CLOCK

	let first = cnx_clock_now(&cnx_high_resolution_clock);
	let second = cnx_clock_now(&cnx_high_resolution_clock);
	TEST_ASSERT(cnx_time_point_less_than_or_equal(first, second));

#if HAS_SLEEP
	sleep(1);
	let third = cnx_clock_now(&cnx_high_resolution_clock);
	TEST_ASSERT(cnx_time_point_less_than(first, third));
#endif
}

TEST(CnxClock, utc_clock) {
	let min_val = cnx_min_value(i64);
	let max_val = cnx_max_value(i64);

	TEST_ASSERT(cnx_clock_resolution(&cnx_utc_clock) == CNX_CLOCK_MICROSECONDS);
	TEST_ASSERT(
		cnx_ratio_equal(cnx_clock_resolution_as_ratio(&cnx_utc_clock), cnx_microseconds_period));
#if CNX_PLATFORM_WINDOWS
	// Windows uses local time, so the minimum time point is actually
	// the minimum system time point converted to UTC locale
	let_mut point = cnx_time_point_new_with_clock(cnx_microseconds(min_val), &cnx_system_clock);
	point.clock = &cnx_utc_clock;
	TEST_ASSERT(cnx_time_point_equal(cnx_clock_min_time_point(&cnx_utc_clock),
									 cnx_convert_local_time_to_utc(point)));
#else
	TEST_ASSERT(cnx_time_point_equal(
		cnx_clock_min_time_point(&cnx_utc_clock),
		cnx_time_point_new_with_clock(cnx_microseconds(min_val), &cnx_utc_clock)));
#endif
	TEST_ASSERT(cnx_time_point_equal(
		cnx_clock_max_time_point(&cnx_utc_clock),
		cnx_time_point_new_with_clock(cnx_microseconds(max_val), &cnx_utc_clock)));

	let first = cnx_clock_now(&cnx_utc_clock);
	let second = cnx_clock_now(&cnx_utc_clock);
	TEST_ASSERT(cnx_time_point_less_than_or_equal(first, second));

#if HAS_SLEEP
	sleep(1);
	let third = cnx_clock_now(&cnx_utc_clock);
	TEST_ASSERT(cnx_time_point_less_than(first, third));
#endif
}

TEST(CnxClock, local_clock) {
	// TODO(braxtons12): Come up with a reliable way to test min and max time point, considering the
	// UTC offset will be different for each person who runs the tests

	// let min_val = cnx_min_value(i64);
	// let max_val = cnx_max_value(i64);

	TEST_ASSERT(cnx_clock_resolution(&cnx_local_clock) == CNX_CLOCK_MICROSECONDS);
	TEST_ASSERT(
		cnx_ratio_equal(cnx_clock_resolution_as_ratio(&cnx_local_clock), cnx_microseconds_period));
	//	TEST_ASSERT(cnx_time_point_equal(
	//		cnx_clock_min_time_point(&cnx_local_clock),
	//		cnx_time_point_new_with_clock(cnx_microseconds(min_val), &cnx_local_clock)));
	//	TEST_ASSERT(cnx_time_point_equal(
	//		cnx_clock_max_time_point(&cnx_local_clock),
	//		cnx_time_point_new_with_clock(cnx_microseconds(max_val), &cnx_local_clock)));

	let first = cnx_clock_now(&cnx_local_clock);
	let second = cnx_clock_now(&cnx_local_clock);
	TEST_ASSERT(cnx_time_point_less_than_or_equal(first, second));

#if HAS_SLEEP
	sleep(1);
	let third = cnx_clock_now(&cnx_local_clock);
	TEST_ASSERT(cnx_time_point_less_than(first, third));
#endif
}

#endif // CLOCK_TEST
