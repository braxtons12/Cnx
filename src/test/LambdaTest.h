
#ifndef STD_LAMBDA_TEST
#define STD_LAMBDA_TEST

#include <C2nxt/StdLambda.h>
#include <C2nxt/StdString.h>

#include "Criterion.h"

typedef Lambda(StdCompare, const StdString*) LambdaCmpStr;

StdCompare LambdaFunction(lambda_function, const StdString* restrict string) {
	let binding = lambda_binding(const StdString*);
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

TEST(StdLambda, case1) {
	TEST_ASSERT_EQUAL(STD_LESS_THAN, lambda_caller1());
}

TEST(StdLambda, case2) {
	TEST_ASSERT_EQUAL(STD_EQUAL, lambda_caller2());
}

#endif // STD_LAMBDA_TEST
