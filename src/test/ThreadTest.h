#ifndef CNX_THREAD_TEST
#define CNX_THREAD_TEST

#include <Cnx/Atomic.h>
#include <Cnx/IO.h>
#include <Cnx/Thread.h>

#include "Criterion.h"

void LambdaFunction(add_one) {
	let binding = lambda_binding(i32*);
	*(binding._1) += 1;
}

void LambdaFunction(add_loop) {
	let binding = lambda_binding(CnxThreadLambda);
	ScopedLambda lambda = binding._1;

	ranged_for(i, 0, 5) {
		lambda_call(lambda);
	}
}

TEST(CnxThread, test_one) {
	i32 val = 0;

	ScopedLambda add_one_lambda = lambda(add_one, &val);

	// add a new scope so the thread gets joined before our final assert
	// (because it's tagged as `CnxScopedThread` so it's joined when it goes out of scope)
	{
		let_mut res = cnx_thread_new(
			lambda_cast(lambda(add_loop, lambda_clone(add_one_lambda)), CnxThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res));

		CnxScopedThread thread = cnx_result_unwrap(res);

		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_thread_get_id(&thread));
	}

	TEST_ASSERT_EQUAL(val, 5);
}

TEST(CnxThread, test_multiple) {
	i32 val = 0;

	ScopedLambda add_one_lambda = lambda(add_one, &val);

	// add a new scope so the threads get joined before our final assert
	// (because they're tagged as `CnxScopedThread` so they join when they go out of scope)
	{
		let_mut res = cnx_thread_new(
			lambda_cast(lambda(add_loop, lambda_clone(add_one_lambda)), CnxThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res));
		__attr(maybe_unused) CnxScopedThread thread = cnx_result_unwrap(res);

		let_mut res2 = cnx_thread_new(
			lambda_cast(lambda(add_loop, lambda_clone(add_one_lambda)), CnxThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res2));
		CnxScopedThread thread2 = cnx_result_unwrap(res2);

		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_thread_get_id(&thread));
		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_thread_get_id(&thread2));
		TEST_ASSERT_NOT_EQUAL(cnx_thread_get_id(&thread), cnx_thread_get_id(&thread2));
	}

	TEST_ASSERT_GREATER_THAN_OR_EQUAL(val, 5);
}

void LambdaFunction(add_one_synchronized) {
	let binding = lambda_binding(atomic_i32*);
	ignore(atomic_fetch_add(binding._1, 1));
}

TEST(CnxThread, test_synchronized) {
	atomic_i32 val = 0;

	ScopedLambda add_one_lambda = lambda(add_one_synchronized, &val);

	// add a new scope so the threads get joined before our final assert
	// (because they're tagged as `CnxScopedThread` so they join when they go out of scope)
	{
		let_mut res = cnx_thread_new(
			lambda_cast(lambda(add_loop, lambda_clone(add_one_lambda)), CnxThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res));
		__attr(maybe_unused) CnxScopedThread thread = cnx_result_unwrap(res);

		let_mut res2 = cnx_thread_new(
			lambda_cast(lambda(add_loop, lambda_clone(add_one_lambda)), CnxThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res2));
		__attr(maybe_unused) CnxScopedThread thread2 = cnx_result_unwrap(res2);

		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_thread_get_id(&thread));
		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_thread_get_id(&thread2));
		TEST_ASSERT_NOT_EQUAL(cnx_thread_get_id(&thread), cnx_thread_get_id(&thread2));
	}

	TEST_ASSERT_EQUAL(atomic_load(&val), 10);
}

void LambdaFunction(add_loop_jthread, const CnxStopToken* token) {
	let binding = lambda_binding(CnxThreadLambda);
	ScopedLambda lambda = binding._1;

	let_mut index = 0;
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	while(!cnx_stop_token_stop_requested(token) || index < 5) {
		lambda_call(lambda);
		index++;
	}
}

TEST(CnxJThread, test_one) {
	i32 val = 0;

	ScopedLambda add_one_lambda = lambda(add_one, &val);

	{
		let_mut res = cnx_jthread_new(
			lambda_cast(lambda(add_loop_jthread, lambda_clone(add_one_lambda)), CnxJThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res));

		CnxScopedJThread thread = cnx_result_unwrap(res);

		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_jthread_get_id(&thread));
	}

	TEST_ASSERT_GREATER_THAN_OR_EQUAL(val, 5);
}

TEST(CnxJThread, test_multiple) {
	i32 val = 0;

	ScopedLambda add_one_lambda = lambda(add_one, &val);

	// add a new scope so the threads get joined before our final assert
	// (because they're tagged as `CnxScopedThread` so they join when they go out of scope)
	{
		let_mut res = cnx_jthread_new(
			lambda_cast(lambda(add_loop_jthread, lambda_clone(add_one_lambda)), CnxJThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res));
		__attr(maybe_unused) CnxScopedJThread thread = cnx_result_unwrap(res);

		let_mut res2 = cnx_jthread_new(
			lambda_cast(lambda(add_loop_jthread, lambda_clone(add_one_lambda)), CnxJThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res2));
		CnxScopedJThread thread2 = cnx_result_unwrap(res2);

		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_jthread_get_id(&thread));
		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_jthread_get_id(&thread2));
		TEST_ASSERT_NOT_EQUAL(cnx_jthread_get_id(&thread), cnx_jthread_get_id(&thread2));
	}

	TEST_ASSERT_GREATER_THAN_OR_EQUAL(val, 5);
}

TEST(CnxJThread, test_synchronized) {
	atomic_i32 val = 0;

	ScopedLambda add_one_lambda = lambda(add_one_synchronized, &val);

	// add a new scope so the threads get joined before our final assert
	// (because they're tagged as `CnxScopedThread` so they join when they go out of scope)
	{
		let_mut res = cnx_jthread_new(
			lambda_cast(lambda(add_loop_jthread, lambda_clone(add_one_lambda)), CnxJThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res));
		__attr(maybe_unused) CnxScopedJThread thread = cnx_result_unwrap(res);

		let_mut res2 = cnx_jthread_new(
			lambda_cast(lambda(add_loop_jthread, lambda_clone(add_one_lambda)), CnxJThreadLambda));

		TEST_ASSERT_TRUE(cnx_result_is_ok(res2));
		CnxScopedJThread thread2 = cnx_result_unwrap(res2);

		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_jthread_get_id(&thread));
		TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_jthread_get_id(&thread2));
		TEST_ASSERT_NOT_EQUAL(cnx_jthread_get_id(&thread), cnx_jthread_get_id(&thread2));
	}

	TEST_ASSERT_GREATER_THAN_OR_EQUAL(atomic_load(&val), 5);
}

TEST(CnxJThread, test_stop_token) {
	i32 val = 0;

	ScopedLambda add_one_lambda = lambda(add_one, &val);

	let_mut res = cnx_jthread_new(
		lambda_cast(lambda(add_loop_jthread, lambda_clone(add_one_lambda)), CnxJThreadLambda));

	TEST_ASSERT_TRUE(cnx_result_is_ok(res));

	let_mut thread = cnx_result_unwrap(res);

	TEST_ASSERT_NOT_EQUAL(cnx_this_thread_get_id(), cnx_jthread_get_id(&thread));

	cnx_stop_token_request_stop(thread.stop_token);

	ignore(cnx_thread_join(&(thread.handle)));

	TEST_ASSERT_GREATER_THAN_OR_EQUAL(val, 5);
}

#endif // CNX_THREAD_TEST
