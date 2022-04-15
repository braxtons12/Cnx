#ifndef CNX_THREAD_TEST
#define CNX_THREAD_TEST

#include <Cnx/Atomic.h>
#include <Cnx/Thread.h>

#include "Criterion.h"

void LambdaFunction(add_one) {
	let binding = lambda_binding(i32*);
	*(binding._1) += 1;
}

void LambdaFunction(add_loop) {
	let binding = lambda_binding(CnxThreadLambda);
	lambda_scoped lambda = binding._1;

	ranged_for(i, 0, 5) {
		lambda_call(lambda);
	}
}

TEST(CnxThread, test_one) {
	i32 val = 0;

	lambda_scoped add_one_lambda = lambda(add_one, &val);

	// add a new scope so the thread gets joined before our final assert
	// (because it's tagged as `CnxJThread` so it's joined when it goes out of scope)
	{
		let_mut res = cnx_thread_new(
			lambda_cast(lambda(add_loop, lambda_clone(add_one_lambda)), CnxThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res));

		CnxJThread thread = cnx_result_unwrap(res);

		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_thread_get_id(&thread));
	}

	TEST_ASSERT_EQUAL(val, 5);
}

TEST(CnxThread, test_multiple) {
	i32 val = 0;

	lambda_scoped add_one_lambda = lambda(add_one, &val);

	// add a new scope so the threads get joined before our final assert
	// (because they're tagged as `CnxJThread` so they join when they go out of scope)
	{
		let_mut res = cnx_thread_new(
			lambda_cast(lambda(add_loop, lambda_clone(add_one_lambda)), CnxThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res));
		[[maybe_unused]] CnxJThread thread = cnx_result_unwrap(res);

		let_mut res2 = cnx_thread_new(
			lambda_cast(lambda(add_loop, lambda_clone(add_one_lambda)), CnxThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res2));
		CnxJThread thread2 = cnx_result_unwrap(res2);

		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_thread_get_id(&thread));
		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_thread_get_id(&thread2));
		TEST_ASSERT_NOT_EQUAL(cnx_thread_get_id(&thread), cnx_thread_get_id(&thread2));
	}

	TEST_ASSERT_TRUE(val >= 5);
}

void LambdaFunction(add_one_synchronized) {
	let binding = lambda_binding(atomic_i32*);
	ignore(atomic_fetch_add(binding._1, 1));
}

TEST(CnxThread, test_synchronized) {
	atomic_i32 val = 0;

	lambda_scoped add_one_lambda = lambda(add_one_synchronized, &val);

	// add a new scope so the threads get joined before our final assert
	// (because they're tagged as `CnxJThread` so they join when they go out of scope)
	{
		let_mut res = cnx_thread_new(
			lambda_cast(lambda(add_loop, lambda_clone(add_one_lambda)), CnxThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res));
		[[maybe_unused]] CnxJThread thread = cnx_result_unwrap(res);

		let_mut res2 = cnx_thread_new(
			lambda_cast(lambda(add_loop, lambda_clone(add_one_lambda)), CnxThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res2));
		[[maybe_unused]] CnxJThread thread2 = cnx_result_unwrap(res2);

		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_thread_get_id(&thread));
		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_thread_get_id(&thread2));
		TEST_ASSERT_NOT_EQUAL(cnx_thread_get_id(&thread), cnx_thread_get_id(&thread2));
	}

	TEST_ASSERT_EQUAL(atomic_load(&val), 10);
}

#endif // CNX_THREAD_TEST
