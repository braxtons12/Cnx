#include "C2nxt/StdRatio.h"

void test_ratio_new_and_comparisons(void) {
	let ratio1 = std_ratio_new(55, 65);
	let to_compare_ratio1 = std_ratio_new(11, 13);
	let ratio2 = std_ratio_new(77, 50);

	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_INT(ratio1.num, 11);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_INT(ratio1.den, 13);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_INT(std_ratio_compare(ratio1, to_compare_ratio1), STD_EQUAL);
	TEST_ASSERT_TRUE(std_ratio_equal(ratio1, to_compare_ratio1));
	TEST_ASSERT_TRUE(std_ratio_less_than_or_equal(ratio1, to_compare_ratio1));
	TEST_ASSERT_TRUE(std_ratio_greater_than_or_equal(ratio1, to_compare_ratio1));
	TEST_ASSERT_EQUAL_INT(std_ratio_compare(ratio1, ratio2), STD_LESS_THAN);
	TEST_ASSERT_TRUE(std_ratio_less_than(ratio1, ratio2));
	TEST_ASSERT_TRUE(std_ratio_less_than_or_equal(ratio1, ratio2));
	TEST_ASSERT_EQUAL_INT(std_ratio_compare(ratio2, ratio1), STD_GREATER_THAN);
	TEST_ASSERT_TRUE(std_ratio_greater_than(ratio2, ratio1));
	TEST_ASSERT_TRUE(std_ratio_greater_than_or_equal(ratio2, ratio1));
}

void test_ratio_add(void) {
	let ratio1 = std_ratio_new(55, 65);
	let ratio2 = std_ratio_new(77, 50);

	let added = std_ratio_add(ratio1, ratio2);
	let added_reverse = std_ratio_add(ratio2, ratio1);
	let expected = std_ratio_new(7755, 3250);
	let expected_reduced = (StdRatio){.num = 1551, .den = 650};

	TEST_ASSERT_TRUE(std_ratio_equal(expected, expected_reduced));
	TEST_ASSERT_TRUE(std_ratio_equal(added, expected));
	TEST_ASSERT_TRUE(std_ratio_equal(added_reverse, expected));
	TEST_ASSERT_TRUE(std_ratio_equal(added, added_reverse));
}

void test_ratio_subtract(void) {
	let ratio1 = std_ratio_new(55, 65);
	let ratio2 = std_ratio_new(77, 50);

	let subtracted = std_ratio_subtract(ratio1, ratio2);
	let subtracted_reverse = std_ratio_subtract(ratio2, ratio1);
	let expected = std_ratio_new(-2255, 3250);
	let expected_reverse = std_ratio_new(2255, 3250);

	TEST_ASSERT_TRUE(std_ratio_equal(subtracted, expected));
	TEST_ASSERT_TRUE(std_ratio_equal(subtracted_reverse, expected_reverse));
}

void test_ratio_multiply(void) {
	let ratio1 = std_ratio_new(55, 65);
	let ratio2 = std_ratio_new(77, 50);

	let multiplied = std_ratio_multiply(ratio1, ratio2);
	let multiplied_reverse = std_ratio_multiply(ratio2, ratio1);
	let expected = std_ratio_new(4235, 3250);

	TEST_ASSERT_TRUE(std_ratio_equal(multiplied, expected));
	TEST_ASSERT_TRUE(std_ratio_equal(multiplied_reverse, expected));
	TEST_ASSERT_TRUE(std_ratio_equal(multiplied, multiplied_reverse));
}

void test_ratio_divide(void) {
	let ratio1 = std_ratio_new(55, 65);
	let ratio2 = std_ratio_new(77, 50);

	let divided = std_ratio_divide(ratio1, ratio2);
	let divided_reverse = std_ratio_divide(ratio2, ratio1);
	let expected = std_ratio_new(2750, 5005);
	let expected_reverse = std_ratio_new(5005, 2750);

	TEST_ASSERT_TRUE(std_ratio_equal(divided, expected));
	TEST_ASSERT_TRUE(std_ratio_equal(divided_reverse, expected_reverse));
}

void test_ratio_as_scalar(void) {
	let ratio1 = std_ratio_new(55, 65);
	let ratio2 = std_ratio_new(200, 100);

	let_mut exact1 = std_ratio_as_scalar_exact(ratio1);
	let_mut exact2 = std_ratio_as_scalar_exact(ratio2);

	let inexact1 = std_ratio_as_scalar(ratio1);
	let inexact2 = std_ratio_as_scalar(ratio2);

	TEST_ASSERT_TRUE(std_option_is_none(exact1));
	TEST_ASSERT_TRUE(std_option_is_some(exact2));
	TEST_ASSERT_EQUAL_INT(std_option_unwrap(exact2), 2);

	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_FLOAT(inexact1, 55.0F / 65.0F);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_FLOAT(inexact2, 2.0F);
}

void test_ratio_add_scalar(void) {
	let ratio1 = std_ratio_new(55, 65);
	let ratio2 = std_ratio_new(77, 50);

	let added1 = std_ratio_add_scalar(ratio1, 2);
	let added2 = std_ratio_add_scalar(ratio2, 2);
	let expected1 = std_ratio_new(55 + 2 * 65, 65);
	let expected2 = std_ratio_new(77 + 2 * 50, 50);

	TEST_ASSERT_TRUE(std_ratio_equal(added1, expected1));
	TEST_ASSERT_TRUE(std_ratio_equal(added2, expected2));
}

void test_ratio_subtract_scalar(void) {
	let ratio1 = std_ratio_new(55, 65);
	let ratio2 = std_ratio_new(77, 50);

	let subtracted1 = std_ratio_subtract_scalar(ratio1, 2);
	let subtracted2 = std_ratio_subtract_scalar(ratio2, 2);
	let expected1 = std_ratio_new(55 - 2 * 65, 65);
	let expected2 = std_ratio_new(77 - 2 * 50, 50);

	TEST_ASSERT_TRUE(std_ratio_equal(subtracted1, expected1));
	TEST_ASSERT_TRUE(std_ratio_equal(subtracted2, expected2));
}

void test_ratio_multiply_scalar(void) {
	let ratio1 = std_ratio_new(55, 65);
	let ratio2 = std_ratio_new(77, 50);

	let multiplied1 = std_ratio_multiply_scalar(ratio1, 2);
	let multiplied2 = std_ratio_multiply_scalar(ratio2, 2);
	let expected1 = std_ratio_new(55 * 2, 65);
	let expected2 = std_ratio_new(77 * 2, 50);

	TEST_ASSERT_TRUE(std_ratio_equal(multiplied1, expected1));
	TEST_ASSERT_TRUE(std_ratio_equal(multiplied2, expected2));
}

void test_ratio_divide_scalar(void) {
	let ratio1 = std_ratio_new(55, 65);
	let ratio2 = std_ratio_new(77, 50);

	let divided1 = std_ratio_divide_scalar(ratio1, 2);
	let divided2 = std_ratio_divide_scalar(ratio2, 2);
	let expected1 = std_ratio_new(55, 65 * 2);
	let expected2 = std_ratio_new(77, 50 * 2);

	TEST_ASSERT_TRUE(std_ratio_equal(divided1, expected1));
	TEST_ASSERT_TRUE(std_ratio_equal(divided2, expected2));
}

static bool run_ratio_tests(void) {

	RUN_TEST(test_ratio_new_and_comparisons);
	RUN_TEST(test_ratio_add);
	RUN_TEST(test_ratio_subtract);
	RUN_TEST(test_ratio_multiply);
	RUN_TEST(test_ratio_divide);
	RUN_TEST(test_ratio_as_scalar);
	RUN_TEST(test_ratio_add_scalar);
	RUN_TEST(test_ratio_subtract_scalar);
	RUN_TEST(test_ratio_multiply_scalar);
	RUN_TEST(test_ratio_divide_scalar);
	return true;
}