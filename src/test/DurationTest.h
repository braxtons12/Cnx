#ifndef DURATION_TEST
#define DURATION_TEST

#include <Cnx/time/Duration.h>

#include "Criterion.h"

TEST(CnxDuration, new_and_comparisons) {
	let duration1 = cnx_milliseconds(1000);
	let duration2 = cnx_seconds(1);
	let duration3 = cnx_microseconds(1500000);

	TEST_ASSERT_EQUAL(duration1.count, 1000);
	TEST_ASSERT(cnx_ratio_equal(duration1.period, cnx_milliseconds_period));
	TEST_ASSERT_EQUAL(duration2.count, 1);
	TEST_ASSERT(cnx_ratio_equal(duration2.period, cnx_seconds_period));
	TEST_ASSERT_EQUAL(duration3.count, 1500000);
	TEST_ASSERT(cnx_ratio_equal(duration3.period, cnx_microseconds_period));

	TEST_ASSERT(cnx_duration_equal(duration1, duration2));
	TEST_ASSERT(cnx_duration_equal(duration2, duration1));
	TEST_ASSERT(cnx_duration_less_than(duration1, duration3));
	TEST_ASSERT(cnx_duration_less_than(duration2, duration3));
	TEST_ASSERT(cnx_duration_less_than_or_equal(duration1, duration3));
	TEST_ASSERT(cnx_duration_less_than_or_equal(duration2, duration3));
	TEST_ASSERT(cnx_duration_greater_than(duration3, duration1));
	TEST_ASSERT(cnx_duration_greater_than(duration3, duration2));
	TEST_ASSERT(cnx_duration_greater_than_or_equal(duration3, duration1));
	TEST_ASSERT(cnx_duration_greater_than_or_equal(duration3, duration2));

	TEST_ASSERT(cnx_duration_less_than_or_equal(duration2, duration1));
	TEST_ASSERT(cnx_duration_less_than_or_equal(duration1, duration2));
	TEST_ASSERT(cnx_duration_greater_than_or_equal(duration2, duration1));
	TEST_ASSERT(cnx_duration_greater_than_or_equal(duration1, duration2));
}

TEST(CnxDuration, add) {
	let duration1 = cnx_milliseconds(500);
	let duration2 = cnx_seconds(1);

	let added = cnx_duration_add(duration1, duration2);
	let added_reverse = cnx_duration_add(duration2, duration1);
	let expected = cnx_milliseconds(1500);
	let expected_reverse = cnx_seconds(1);

	TEST_ASSERT(cnx_duration_equal(added, expected));
	TEST_ASSERT(cnx_duration_equal(added_reverse, expected_reverse));

	let added2 = cnx_duration_add_scalar(duration1, 500);
	let expected2 = cnx_milliseconds(1000);

	TEST_ASSERT(cnx_duration_equal(added2, expected2));
}

TEST(CnxDuration, subtract) {
	let duration1 = cnx_milliseconds(500);
	let duration2 = cnx_seconds(1);

	let subtracted = cnx_duration_subtract(duration1, duration2);
	let subtracted_reverse = cnx_duration_subtract(duration2, duration1);
	let expected = cnx_milliseconds(-500);
	let expected_reverse = cnx_seconds(1);

	TEST_ASSERT(cnx_duration_equal(subtracted, expected));
	TEST_ASSERT(cnx_duration_equal(subtracted_reverse, expected_reverse));

	let subtracted2 = cnx_duration_subtract_scalar(duration1, 500);
	let expected2 = cnx_milliseconds(0);

	TEST_ASSERT(cnx_duration_equal(subtracted2, expected2));
}

TEST(CnxDuration, multiply) {
	let duration1 = cnx_milliseconds(500);
	let duration2 = cnx_seconds(1);

	let multiplied = cnx_duration_multiply(duration1, 20);
	let expected = cnx_milliseconds(10000);
	let expected2 = cnx_seconds(10);

	TEST_ASSERT(cnx_duration_equal(multiplied, expected));
	TEST_ASSERT(cnx_duration_equal(multiplied, expected2));

	let multiplied2 = cnx_duration_multiply(duration2, 20);
	let expected3 = cnx_seconds(20);
	let expected4 = cnx_milliseconds(20000);

	TEST_ASSERT(cnx_duration_equal(multiplied2, expected3));
	TEST_ASSERT(cnx_duration_equal(multiplied2, expected4));
}

TEST(CnxDuration, divide) {
	let duration1 = cnx_milliseconds(500);
	let duration2 = cnx_seconds(10);

	let divided = cnx_duration_divide(duration1, 20);
	let expected = cnx_milliseconds(25);
	let expected2 = cnx_microseconds(25000);

	TEST_ASSERT(cnx_duration_equal(divided, expected));
	TEST_ASSERT(cnx_duration_equal(divided, expected2));

	let divided2 = cnx_duration_divide(duration2, 2);
	let expected3 = cnx_seconds(5);
	let expected4 = cnx_milliseconds(5000);

	TEST_ASSERT(cnx_duration_equal(divided2, expected3));
	TEST_ASSERT(cnx_duration_equal(divided2, expected4));
}

#endif // DURATION_TEST
