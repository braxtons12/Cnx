#ifndef GCD_AND_LCM_TEST
#define GCD_AND_LCM_TEST
#include <C2nxt/StdMath.h>

#include "Criterion.h"

TEST(Math, gcd) {
	TEST_ASSERT_EQUAL(std_gcd(40, 20), 20);
	TEST_ASSERT_EQUAL(std_gcd(41, 21), 1);
	TEST_ASSERT_EQUAL(std_gcd(50, 30), 10);
	TEST_ASSERT_EQUAL(std_gcd(831, 763), 1);

	TEST_ASSERT_EQUAL(std_static_gcd(40, 20), 20);
	TEST_ASSERT_EQUAL(std_static_gcd(41, 21), 1);
	TEST_ASSERT_EQUAL(std_static_gcd(50, 30), 10);
	TEST_ASSERT_EQUAL(std_static_gcd(831, 763), 1);
}

TEST(Math, lcm) {
	TEST_ASSERT_EQUAL(std_lcm(40, 20), 40);
	TEST_ASSERT_EQUAL(std_lcm(41, 21), 861);
	TEST_ASSERT_EQUAL(std_lcm(50, 30), 150);
	TEST_ASSERT_EQUAL(std_lcm(831, 763), 634053);

	TEST_ASSERT_EQUAL(std_static_lcm(40, 20), 40);
	TEST_ASSERT_EQUAL(std_static_lcm(41, 21), 861);
	TEST_ASSERT_EQUAL(std_static_lcm(50, 30), 150);
	TEST_ASSERT_EQUAL(std_static_lcm(831, 763), 634053);
}

#endif // GCD_AND_LCM_TEST
