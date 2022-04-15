#ifndef RATIO_TEST
#define RATIO_TEST

#include <Cnx/Ratio.h>

#include "Criterion.h"

TEST(CnxRatio, new_and_comparisons) {
	let ratio1 = cnx_ratio_new(55, 65);
	let to_compare_ratio1 = cnx_ratio_new(11, 13);
	let ratio2 = cnx_ratio_new(77, 50);

	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL(ratio1.num, 11);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL(ratio1.den, 13);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL(cnx_ratio_compare(ratio1, to_compare_ratio1), CNX_EQUAL);
	TEST_ASSERT(cnx_ratio_equal(ratio1, to_compare_ratio1));
	TEST_ASSERT(cnx_ratio_less_than_or_equal(ratio1, to_compare_ratio1));
	TEST_ASSERT(cnx_ratio_greater_than_or_equal(ratio1, to_compare_ratio1));
	TEST_ASSERT_EQUAL(cnx_ratio_compare(ratio1, ratio2), CNX_LESS_THAN);
	TEST_ASSERT(cnx_ratio_less_than(ratio1, ratio2));
	TEST_ASSERT(cnx_ratio_less_than_or_equal(ratio1, ratio2));
	TEST_ASSERT_EQUAL(cnx_ratio_compare(ratio2, ratio1), CNX_GREATER_THAN);
	TEST_ASSERT(cnx_ratio_greater_than(ratio2, ratio1));
	TEST_ASSERT(cnx_ratio_greater_than_or_equal(ratio2, ratio1));
}

TEST(CnxRatio, add) {
	let ratio1 = cnx_ratio_new(55, 65);
	let ratio2 = cnx_ratio_new(77, 50);

	let added = cnx_ratio_add(ratio1, ratio2);
	let added_reverse = cnx_ratio_add(ratio2, ratio1);
	let expected = cnx_ratio_new(7755, 3250);
	let expected_reduced = (CnxRatio){.num = 1551, .den = 650};

	TEST_ASSERT(cnx_ratio_equal(expected, expected_reduced));
	TEST_ASSERT(cnx_ratio_equal(added, expected));
	TEST_ASSERT(cnx_ratio_equal(added_reverse, expected));
	TEST_ASSERT(cnx_ratio_equal(added, added_reverse));
}

TEST(CnxRatio, subtract) {
	let ratio1 = cnx_ratio_new(55, 65);
	let ratio2 = cnx_ratio_new(77, 50);

	let subtracted = cnx_ratio_subtract(ratio1, ratio2);
	let subtracted_reverse = cnx_ratio_subtract(ratio2, ratio1);
	let expected = cnx_ratio_new(-2255, 3250);
	let expected_reverse = cnx_ratio_new(2255, 3250);

	TEST_ASSERT(cnx_ratio_equal(subtracted, expected));
	TEST_ASSERT(cnx_ratio_equal(subtracted_reverse, expected_reverse));
}

TEST(CnxRatio, multiply) {
	let ratio1 = cnx_ratio_new(55, 65);
	let ratio2 = cnx_ratio_new(77, 50);

	let multiplied = cnx_ratio_multiply(ratio1, ratio2);
	let multiplied_reverse = cnx_ratio_multiply(ratio2, ratio1);
	let expected = cnx_ratio_new(4235, 3250);

	TEST_ASSERT(cnx_ratio_equal(multiplied, expected));
	TEST_ASSERT(cnx_ratio_equal(multiplied_reverse, expected));
	TEST_ASSERT(cnx_ratio_equal(multiplied, multiplied_reverse));
}

TEST(CnxRatio, divide) {
	let ratio1 = cnx_ratio_new(55, 65);
	let ratio2 = cnx_ratio_new(77, 50);

	let divided = cnx_ratio_divide(ratio1, ratio2);
	let divided_reverse = cnx_ratio_divide(ratio2, ratio1);
	let expected = cnx_ratio_new(2750, 5005);
	let expected_reverse = cnx_ratio_new(5005, 2750);

	TEST_ASSERT(cnx_ratio_equal(divided, expected));
	TEST_ASSERT(cnx_ratio_equal(divided_reverse, expected_reverse));
}

TEST(CnxRatio, as_scalar) {
	let ratio1 = cnx_ratio_new(55, 65);
	let ratio2 = cnx_ratio_new(200, 100);

	let_mut exact1 = cnx_ratio_as_scalar_exact(ratio1);
	let_mut exact2 = cnx_ratio_as_scalar_exact(ratio2);

	let inexact1 = cnx_ratio_as_scalar(ratio1);
	let inexact2 = cnx_ratio_as_scalar(ratio2);

	TEST_ASSERT(cnx_option_is_none(exact1));
	TEST_ASSERT(cnx_option_is_some(exact2));
	TEST_ASSERT_EQUAL(cnx_option_unwrap(exact2), 2);

	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_FLOAT(inexact1, 55.0F / 65.0F);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_FLOAT(inexact2, 2.0F);
}

TEST(CnxRatio, add_scalar) {
	let ratio1 = cnx_ratio_new(55, 65);
	let ratio2 = cnx_ratio_new(77, 50);

	let added1 = cnx_ratio_add_scalar(ratio1, 2);
	let added2 = cnx_ratio_add_scalar(ratio2, 2);
	let expected1 = cnx_ratio_new(55 + 2 * 65, 65);
	let expected2 = cnx_ratio_new(77 + 2 * 50, 50);

	TEST_ASSERT(cnx_ratio_equal(added1, expected1));
	TEST_ASSERT(cnx_ratio_equal(added2, expected2));
}

TEST(CnxRatio, subtract_scalar) {
	let ratio1 = cnx_ratio_new(55, 65);
	let ratio2 = cnx_ratio_new(77, 50);

	let subtracted1 = cnx_ratio_subtract_scalar(ratio1, 2);
	let subtracted2 = cnx_ratio_subtract_scalar(ratio2, 2);
	let expected1 = cnx_ratio_new(55 - 2 * 65, 65);
	let expected2 = cnx_ratio_new(77 - 2 * 50, 50);

	TEST_ASSERT(cnx_ratio_equal(subtracted1, expected1));
	TEST_ASSERT(cnx_ratio_equal(subtracted2, expected2));
}

TEST(CnxRatio, multiply_scalar) {
	let ratio1 = cnx_ratio_new(55, 65);
	let ratio2 = cnx_ratio_new(77, 50);

	let multiplied1 = cnx_ratio_multiply_scalar(ratio1, 2);
	let multiplied2 = cnx_ratio_multiply_scalar(ratio2, 2);
	// NOLINTNEXTLINE(bugprone-implicit-widening-of-multiplication-result)
	let expected1 = cnx_ratio_new(55 * 2, 65);
	// NOLINTNEXTLINE(bugprone-implicit-widening-of-multiplication-result)
	let expected2 = cnx_ratio_new(77 * 2, 50);

	TEST_ASSERT(cnx_ratio_equal(multiplied1, expected1));
	TEST_ASSERT(cnx_ratio_equal(multiplied2, expected2));
}

TEST(CnxRatio, divide_scalar) {
	let ratio1 = cnx_ratio_new(55, 65);
	let ratio2 = cnx_ratio_new(77, 50);

	let divided1 = cnx_ratio_divide_scalar(ratio1, 2);
	let divided2 = cnx_ratio_divide_scalar(ratio2, 2);
	// NOLINTNEXTLINE(bugprone-implicit-widening-of-multiplication-result)
	let expected1 = cnx_ratio_new(55, 65 * 2);
	// NOLINTNEXTLINE(bugprone-implicit-widening-of-multiplication-result)
	let expected2 = cnx_ratio_new(77, 50 * 2);

	TEST_ASSERT(cnx_ratio_equal(divided1, expected1));
	TEST_ASSERT(cnx_ratio_equal(divided2, expected2));
}

#endif // RATIO_TEST
