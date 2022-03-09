#include <C2nxt/StdLambda.h>
#include <C2nxt/StdString.h>
#include <unity.h>

#ifndef STD_LAMBDA_TEST
	#define STD_LAMBDA_TEST

typedef Lambda(StdCompare, const StdString*) LambdaCmpStr;

LambdaFunction(StdCompare, lambda_function, const StdString* restrict string) {
	let binding = lambda_binding(LambdaBinding(const StdString*));
	return std_string_equal(*binding._1, string) ? STD_EQUAL : STD_LESS_THAN;
}

StdCompare lambda_receiver(LambdaCmpStr lambda) {
	std_string_scoped string = std_string_from("Test2");

	let ret = lambda_call(lambda, &string);
	lambda_free(lambda);
	return ret;
}

StdCompare lambda_caller1(void) {
	std_string_scoped string = std_string_from("Test1");
	let lambda = lambda(lambda_function, &string);
	return lambda_receiver(lambda_cast(lambda, LambdaCmpStr));
}

StdCompare lambda_caller2(void) {
	std_string_scoped string = std_string_from("Test2");
	let lambda = lambda(lambda_function, &string);
	return lambda_receiver(lambda_cast(lambda, LambdaCmpStr));
}

void test_lambda_caller1(void) {
	TEST_ASSERT_EQUAL_INT(STD_LESS_THAN, lambda_caller1());
}

void test_lambda_caller2(void) {
	TEST_ASSERT_EQUAL_INT(STD_EQUAL, lambda_caller2());
}

static bool run_lambda_tests(void) {
	RUN_TEST(test_lambda_caller1);
	RUN_TEST(test_lambda_caller2);

	return true;
}

#endif // STD_LAMBDA_TEST
