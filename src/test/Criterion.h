#ifndef CRITERION_WRAP
#define CRITERION_WRAP

#include <Cnx/Def.h>
#include <Cnx/Math.h>
#include <criterion/criterion.h>

#define TEST(...)                                                                                                        \
	/** NOLINT(readability-function-cognitive-complexity,cppcoreguidelines-avoid-non-const-global-variables) **/         \
	/** NOLINTNEXTLINE(readability-function-cognitive-complexity,cppcoreguidelines-avoid-non-const-global-variables) **/ \
	Test(__VA_ARGS__)
#define TEST_ASSERT(condition, ...) cr_assert(condition __VA_OPT__(, ) __VA_ARGS__)
#define TEST_ASSERT_EQUAL(actual, expected, ...) \
	cr_assert_eq(actual, expected __VA_OPT__(, ) __VA_ARGS__)
#define TEST_ASSERT_NOT_EQUAL(actual, expected, ...) \
	cr_assert_neq(actual, expected __VA_OPT__(, ) __VA_ARGS__)
#define TEST_ASSERT_TRUE(condition, ...)  cr_assert((condition)__VA_OPT__(, ) __VA_ARGS__)
#define TEST_ASSERT_FALSE(condition, ...) cr_assert_not((condition)__VA_OPT__(, ) __VA_ARGS__)
#define TEST_ASSERT_APPROXIMATE(actual, expected, delta, ...) \
	cr_assert_float_eq(actual, expected, delta __VA_OPT__(, ) __VA_ARGS__)
#define TEST_ASSERT_EQUAL_FLOAT(actual, expected, ...) \
	TEST_ASSERT_APPROXIMATE((actual), (expected), 0.001F __VA_OPT__(, ) __VA_ARGS__)
#endif // CRITERION_WRAP
