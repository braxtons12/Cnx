#ifndef DURATION_TEST
#define DURATION_TEST

#include <C2nxt/time/StdDuration.h>

#include "Criterion.h"

TEST(StdDuration, new_and_comparisons) {
	let duration1 = std_milliseconds(1000);
	let duration2 = std_seconds(1);
	let duration3 = std_microseconds(1500000);

	TEST_ASSERT_EQUAL(duration1.count, 1000);
	TEST_ASSERT(std_ratio_equal(duration1.period, std_milliseconds_period));
	TEST_ASSERT_EQUAL(duration2.count, 1);
	TEST_ASSERT(std_ratio_equal(duration2.period, std_seconds_period));
	TEST_ASSERT_EQUAL(duration3.count, 1500000);
	TEST_ASSERT(std_ratio_equal(duration3.period, std_microseconds_period));

	TEST_ASSERT(std_duration_equal(duration1, duration2));
	TEST_ASSERT(std_duration_equal(duration2, duration1));
	TEST_ASSERT(std_duration_less_than(duration1, duration3));
	TEST_ASSERT(std_duration_less_than(duration2, duration3));
	TEST_ASSERT(std_duration_less_than_or_equal(duration1, duration3));
	TEST_ASSERT(std_duration_less_than_or_equal(duration2, duration3));
	TEST_ASSERT(std_duration_greater_than(duration3, duration1));
	TEST_ASSERT(std_duration_greater_than(duration3, duration2));
	TEST_ASSERT(std_duration_greater_than_or_equal(duration3, duration1));
	TEST_ASSERT(std_duration_greater_than_or_equal(duration3, duration2));

	TEST_ASSERT(std_duration_less_than_or_equal(duration2, duration1));
	TEST_ASSERT(std_duration_less_than_or_equal(duration1, duration2));
	TEST_ASSERT(std_duration_greater_than_or_equal(duration2, duration1));
	TEST_ASSERT(std_duration_greater_than_or_equal(duration1, duration2));
}

TEST(StdDuration, add) {
	let duration1 = std_milliseconds(500);
	let duration2 = std_seconds(1);

	let added = std_duration_add(duration1, duration2);
	let added_reverse = std_duration_add(duration2, duration1);
	let expected = std_milliseconds(1500);
	let expected_reverse = std_seconds(1);

	TEST_ASSERT(std_duration_equal(added, expected));
	TEST_ASSERT(std_duration_equal(added_reverse, expected_reverse));

	let added2 = std_duration_add_scalar(duration1, 500);
	let expected2 = std_milliseconds(1000);

	TEST_ASSERT(std_duration_equal(added2, expected2));
}

TEST(StdDuration, subtract) {
	let duration1 = std_milliseconds(500);
	let duration2 = std_seconds(1);

	let subtracted = std_duration_subtract(duration1, duration2);
	let subtracted_reverse = std_duration_subtract(duration2, duration1);
	let expected = std_milliseconds(-500);
	let expected_reverse = std_seconds(1);

	TEST_ASSERT(std_duration_equal(subtracted, expected));
	TEST_ASSERT(std_duration_equal(subtracted_reverse, expected_reverse));

	let subtracted2 = std_duration_subtract_scalar(duration1, 500);
	let expected2 = std_milliseconds(0);

	TEST_ASSERT(std_duration_equal(subtracted2, expected2));
}

TEST(StdDuration, multiply) {
	let duration1 = std_milliseconds(500);
	let duration2 = std_seconds(1);

	let multiplied = std_duration_multiply(duration1, 20);
	let expected = std_milliseconds(10000);
	let expected2 = std_seconds(10);

	TEST_ASSERT(std_duration_equal(multiplied, expected));
	TEST_ASSERT(std_duration_equal(multiplied, expected2));

	let multiplied2 = std_duration_multiply(duration2, 20);
	let expected3 = std_seconds(20);
	let expected4 = std_milliseconds(20000);

	TEST_ASSERT(std_duration_equal(multiplied2, expected3));
	TEST_ASSERT(std_duration_equal(multiplied2, expected4));
}

TEST(StdDuration, divide) {
	let duration1 = std_milliseconds(500);
	let duration2 = std_seconds(10);

	let divided = std_duration_divide(duration1, 20);
	let expected = std_milliseconds(25);
	let expected2 = std_microseconds(25000);

	TEST_ASSERT(std_duration_equal(divided, expected));
	TEST_ASSERT(std_duration_equal(divided, expected2));

	let divided2 = std_duration_divide(duration2, 2);
	let expected3 = std_seconds(5);
	let expected4 = std_milliseconds(5000);

	TEST_ASSERT(std_duration_equal(divided2, expected3));
	TEST_ASSERT(std_duration_equal(divided2, expected4));
}

#endif // DURATION_TEST
