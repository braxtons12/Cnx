#include <C2nxt/StdMath.h>
#include <unity.h>

void test_checked_add(void) {
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut add1 = std_checked_add(std_max_value(u8), 1);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut add2 = std_checked_add(static_cast(u8)(1), std_max_value(u8));
	let_mut add3 = std_checked_add(10U, 10U);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut add4 = std_checked_add(std_max_value(i64), 1);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut add5 = std_checked_add(static_cast(i64)(1000), static_cast(i64)(9000));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut add6 = std_checked_add(std_max_value(f32), std_max_value(f32));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut add7 = std_checked_add(20.0F, 20.0F);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut add8 = std_checked_add(20.0F, -20.0F);

	TEST_ASSERT_TRUE(std_option_is_none(add1));
	TEST_ASSERT_TRUE(std_option_is_none(add2));
	TEST_ASSERT_TRUE(std_option_is_some(add3));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_INT(std_option_unwrap(add3), 20U);
	TEST_ASSERT_TRUE(std_option_is_none(add4));
	TEST_ASSERT_TRUE(std_option_is_some(add5));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_INT(std_option_unwrap(add5), 10000);
	TEST_ASSERT_TRUE(std_option_is_none(add6));
	TEST_ASSERT_TRUE(std_option_is_some(add7));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_FLOAT(std_option_unwrap(add7), 40.0F);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_FLOAT(std_option_unwrap(add8), 0.0F);
}

void test_checked_sub(void) {
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut sub1 = std_checked_sub(std_min_value(u8), 1);
	let_mut sub2 = std_checked_sub(static_cast(u8)(1), 2);
	let_mut sub3 = std_checked_sub(10U, 10U);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut sub4 = std_checked_sub(std_min_value(i64), 1);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut sub5 = std_checked_sub(static_cast(i64)(1000), static_cast(i64)(9000));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut sub6 = std_checked_sub(std_min_value(f32), std_max_value(f32));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut sub7 = std_checked_sub(20.0F, 20.0F);

	TEST_ASSERT_TRUE(std_option_is_none(sub1));
	TEST_ASSERT_TRUE(std_option_is_none(sub2));
	TEST_ASSERT_TRUE(std_option_is_some(sub3));
	TEST_ASSERT_EQUAL_INT(std_option_unwrap(sub3), 0);
	TEST_ASSERT_TRUE(std_option_is_none(sub4));
	TEST_ASSERT_TRUE(std_option_is_some(sub5));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_INT(std_option_unwrap(sub5), -8000);
	TEST_ASSERT_TRUE(std_option_is_none(sub6));
	TEST_ASSERT_TRUE(std_option_is_some(sub7));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_FLOAT(std_option_unwrap(sub7), 0.0F);
}

void test_checked_mul(void) {
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut mul1 = std_checked_mul(std_max_value(u8), 2);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut mul2 = std_checked_mul(static_cast(u8)(2), std_max_value(u8));
	let_mut mul3 = std_checked_mul(10U, 10U);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut mul4 = std_checked_mul(std_min_value(i64), 2);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut mul5 = std_checked_mul(static_cast(i64)(1000), static_cast(i64)(-9000));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut mul6 = std_checked_mul(std_max_value(f32), 2.0F);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut mul7 = std_checked_mul(50.0F, 50.0F);

	TEST_ASSERT_TRUE(std_option_is_none(mul1));
	TEST_ASSERT_TRUE(std_option_is_none(mul2));
	TEST_ASSERT_TRUE(std_option_is_some(mul3));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_INT(std_option_unwrap(mul3), 100);
	TEST_ASSERT_TRUE(std_option_is_none(mul4));
	TEST_ASSERT_TRUE(std_option_is_some(mul5));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_INT(std_option_unwrap(mul5), -9000000);
	TEST_ASSERT_TRUE(std_option_is_none(mul6));
	TEST_ASSERT_TRUE(std_option_is_some(mul7));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_FLOAT(std_option_unwrap(mul7), 2500.0F);
}

void test_checked_div(void) {
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut div1 = std_checked_div(std_max_value(u8), 0);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut div2 = std_checked_div(static_cast(u8)(2), 0);
	let_mut div3 = std_checked_div(100U, 10U);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut div4 = std_checked_div(std_min_value(i64), 0);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut div5 = std_checked_div(static_cast(i64)(1000), static_cast(i64)(-9000));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut div6 = std_checked_div(std_max_value(f32), 0.0F);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut div7 = std_checked_div(50.0F, 50.0F);

	TEST_ASSERT_TRUE(std_option_is_none(div1));
	TEST_ASSERT_TRUE(std_option_is_none(div2));
	TEST_ASSERT_TRUE(std_option_is_some(div3));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_INT(std_option_unwrap(div3), 10U);
	TEST_ASSERT_TRUE(std_option_is_none(div4));
	TEST_ASSERT_TRUE(std_option_is_some(div5));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_INT(std_option_unwrap(div5), 0);
	TEST_ASSERT_TRUE(std_option_is_none(div6));
	TEST_ASSERT_TRUE(std_option_is_some(div7));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	TEST_ASSERT_EQUAL_FLOAT(std_option_unwrap(div7), 1.0F);
}
static bool run_checked_math_tests(void) {

	RUN_TEST(test_checked_add);
	RUN_TEST(test_checked_sub);
	RUN_TEST(test_checked_mul);
	RUN_TEST(test_checked_div);

	return true;
}
