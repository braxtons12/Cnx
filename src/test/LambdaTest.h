
#ifndef CNX_LAMBDA_TEST
#define CNX_LAMBDA_TEST

#include <Cnx/Lambda.h>
#include <Cnx/String.h>

#include "Criterion.h"

typedef Lambda(CnxCompare, const CnxString*) LambdaCmpStr;

CnxCompare LambdaFunction(lambda_function, const CnxString* restrict string) {
	let binding = lambda_binding(const CnxString*);
	return cnx_string_equal(*binding._1, string) ? CNX_EQUAL : CNX_LESS_THAN;
}

CnxCompare lambda_receiver(LambdaCmpStr lambda) {
	CnxScopedString string = cnx_string_from("Test2");

	let ret = lambda_call(lambda, &string);
	lambda_free(&lambda);
	return ret;
}

CnxCompare lambda_caller1(void) {
	CnxScopedString string = cnx_string_from("Test1");
	lambda_scoped lambda = lambda(lambda_function, &string);
	return lambda_receiver(lambda_cast(lambda_clone(lambda), LambdaCmpStr));
}

CnxCompare lambda_caller2(void) {
	CnxScopedString string = cnx_string_from("Test2");
	lambda_scoped lambda = lambda(lambda_function, &string);
	return lambda_receiver(lambda_cast(lambda_clone(lambda), LambdaCmpStr));
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
TEST(CnxLambda, case1) {
	TEST_ASSERT_EQUAL(CNX_LESS_THAN, lambda_caller1());
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
TEST(CnxLambda, case2) {
	TEST_ASSERT_EQUAL(CNX_EQUAL, lambda_caller2());
}

#endif // CNX_LAMBDA_TEST
