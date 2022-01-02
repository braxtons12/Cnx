#include "C2nxt/StdMath.h"
#include "unity.h"

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void test_gcd(void) {
	TEST_ASSERT_EQUAL_INT(std_gcd(40, 20), 20);
	TEST_ASSERT_EQUAL_INT(std_gcd(41, 21), 1);
	TEST_ASSERT_EQUAL_INT(std_gcd(50, 30), 10);
	TEST_ASSERT_EQUAL_INT(std_gcd(831, 763), 1);

	TEST_ASSERT_EQUAL_INT(std_static_gcd(40, 20), 20);
	TEST_ASSERT_EQUAL_INT(std_static_gcd(41, 21), 1);
	TEST_ASSERT_EQUAL_INT(std_static_gcd(50, 30), 10);
	TEST_ASSERT_EQUAL_INT(std_static_gcd(831, 763), 1);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void test_lcm(void) {
	TEST_ASSERT_EQUAL_INT(std_lcm(40, 20), 40);
	TEST_ASSERT_EQUAL_INT(std_lcm(41, 21), 861);
	TEST_ASSERT_EQUAL_INT(std_lcm(50, 30), 150);
	TEST_ASSERT_EQUAL_INT(std_lcm(831, 763), 634053);

	TEST_ASSERT_EQUAL_INT(std_static_lcm(40, 20), 40);
	TEST_ASSERT_EQUAL_INT(std_static_lcm(41, 21), 861);
	TEST_ASSERT_EQUAL_INT(std_static_lcm(50, 30), 150);
	TEST_ASSERT_EQUAL_INT(std_static_lcm(831, 763), 634053);
}

static bool run_gcd_and_lcm_tests(void) {

	RUN_TEST(test_gcd);
	RUN_TEST(test_lcm);

	return true;
}
