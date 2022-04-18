/// @file Thread.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Function implementations for threading functionality
/// @version 0.2.0
/// @date 2022-04-10
///
/// MIT License
/// @copyright Copyright (c) 2022 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#include <Cnx/Assert.h>
#include <Cnx/Atomic.h>
#include <Cnx/Thread.h>
#include <Cnx/time/Clock.h>
#include <Cnx/time/TimePoint.h>
#include <time.h>

#define RESULT_T	CnxBasicMutex
#define RESULT_IMPL TRUE
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_IMPL

#define RESULT_T	CnxRecursiveBasicMutex
#define RESULT_IMPL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_IMPL

#define RESULT_T	CnxCondvar
#define RESULT_IMPL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_IMPL

#define RESULT_T	CnxThread
#define RESULT_IMPL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_IMPL

#define RESULT_T	CnxJThread
#define RESULT_IMPL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_IMPL

#define RESULT_T	CnxTLSKey
#define RESULT_IMPL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_IMPL

struct timespec duration_to_timespec(CnxDuration duration) {
	let now = cnx_clock_now(&cnx_utc_clock);
	let added = cnx_time_point_add(now, duration);
	let as_nanoseconds
		= cnx_duration_cast(cnx_time_point_time_since_epoch(added), cnx_nanoseconds_period);
	let seconds = as_nanoseconds.count / as_nanoseconds.period.den;
	let nanoseconds = as_nanoseconds.count % as_nanoseconds.period.den;
	return (struct timespec){.tv_sec = seconds, .tv_nsec = static_cast(long)(nanoseconds)};
}

#define CHECK_ERROR_POSIX(res)                                           \
	if((res) != 0) {                                                     \
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY)); \
	}                                                                    \
                                                                         \
	return Ok(i32, 0)

#if ___CNX_HAS_C11_THREADS
	#include <threads.h>

CnxResult(CnxBasicMutex) cnx_basic_mutex_new(void) {
	CnxBasicMutex mutex;
	let_mut res = cnx_basic_mutex_init(&mutex);
	return cnx_result_and(res, Ok(CnxBasicMutex, mutex));
}

CnxResult cnx_basic_mutex_init(CnxBasicMutex* restrict mutex) {
	let res = mtx_init(mutex, mtx_plain);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_basic_mutex_lock(CnxBasicMutex* restrict mutex) {
	let res = mtx_lock(mutex);
	CHECK_ERROR_POSIX(res);
}

bool cnx_basic_mutex_try_lock(CnxBasicMutex* restrict mutex) {
	return mtx_trylock(mutex) == thrd_success;
}

CnxResult cnx_basic_mutex_unlock(CnxBasicMutex* restrict mutex) {
	let res = mtx_unlock(mutex);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_basic_mutex_free(CnxBasicMutex* restrict mutex) {
	mtx_destroy(mutex);
	return Ok(i32, 0);
}

CnxResult(CnxRecursiveBasicMutex) cnx_recursive_basic_mutex_new(void) {
	CnxRecursiveBasicMutex mutex;
	let_mut res = cnx_recursive_basic_mutex_init(&mutex);
	return cnx_result_and(res, Ok(CnxRecursiveBasicMutex, mutex));
}

CnxResult cnx_recursive_basic_mutex_init(CnxRecursiveBasicMutex* restrict mutex) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	let res = mtx_init(mutex, mtx_plain | mtx_recursive);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_recursive_basic_mutex_lock(CnxRecursiveBasicMutex* restrict mutex) {
	let res = mtx_lock(mutex);
	CHECK_ERROR_POSIX(res);
}

bool cnx_recursive_basic_mutex_try_lock(CnxRecursiveBasicMutex* restrict mutex) {
	return mtx_lock(mutex) == thrd_success;
}

CnxResult cnx_recursive_basic_mutex_unlock(CnxRecursiveBasicMutex* restrict mutex) {
	let res = mtx_unlock(mutex);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_recursive_basic_mutex_free(CnxRecursiveBasicMutex* restrict mutex) {
	mtx_destroy(mutex);
	return Ok(i32, 0);
}

CnxResult(CnxCondvar) cnx_condvar_new(void) {
	CnxCondvar condvar;
	let_mut res = cnx_condvar_init(&condvar);
	return cnx_result_and(res, Ok(CnxCondvar, condvar));
}

CnxResult cnx_condvar_init(CnxCondvar* restrict condvar) {
	let res = cnd_init(condvar);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_condvar_signal(CnxCondvar* restrict condvar) {
	let res = cnd_signal(condvar);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_condvar_broadcast(CnxCondvar* restrict condvar) {
	let res = cnd_broadcast(condvar);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_condvar_wait(CnxCondvar* restrict condvar, CnxBasicMutex* restrict mutex) {
	let res = cnd_wait(condvar, mutex);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_condvar_wait_for(CnxCondvar* restrict condvar,
							   CnxBasicMutex* restrict mutex,
							   CnxDuration to_wait) {
	let spec = duration_to_timespec(to_wait);

	let res = cnd_timedwait(condvar, mutex, &spec);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_condvar_free(CnxCondvar* restrict condvar) {
	cnd_destroy(condvar);

	return Ok(i32, 0);
}

CnxResult cnx_execute_once(CnxOnceFlag* restrict flag, void (*function)(void)) {
	call_once(flag, function);
	return Ok(i32, 0);
}

CnxCompare cnx_thread_id_compare(CnxThreadID lhs, CnxThreadID rhs) {
	return thrd_equal(lhs, rhs) != 0 ? CNX_EQUAL : (lhs < rhs ? CNX_LESS_THAN : CNX_GREATER_THAN);
}

bool cnx_thread_id_equal(CnxThreadID lhs, CnxThreadID rhs) {
	return thrd_equal(lhs, rhs) != 0;
}

bool cnx_thread_id_less_than(CnxThreadID lhs, CnxThreadID rhs) {
	return lhs < rhs;
}

bool cnx_thread_id_less_than_or_equal(CnxThreadID lhs, CnxThreadID rhs) {
	return cnx_thread_id_equal(lhs, rhs) || cnx_thread_id_less_than(lhs, rhs);
}

bool cnx_thread_id_greater_than(CnxThreadID lhs, CnxThreadID rhs) {
	return lhs > rhs;
}

bool cnx_thread_id_greater_than_or_equal(CnxThreadID lhs, CnxThreadID rhs) {
	return cnx_thread_id_equal(lhs, rhs) || cnx_thread_id_greater_than(lhs, rhs);
}

[[always_inline]] [[not_null(1)]] static inline int thread_invoke(void* lambda) {
	lambda_scoped _lambda = static_cast(CnxThreadLambda)(lambda);
	lambda_call(_lambda);
	return 0;
}

CnxResult(CnxThread) cnx_thread_new(CnxThreadLambda lambda) {
	CnxThread thread; // NOLINT
	let_mut res = cnx_thread_init(&thread, lambda);
	return cnx_result_and(res, Ok(CnxThread, thread));
}

CnxResult cnx_thread_init(CnxThread* thread, CnxThreadLambda lambda) {
	let res = thrd_create(thread, thread_invoke, lambda);
	CHECK_ERROR_POSIX(res);
}

bool cnx_thread_is_null(const CnxThread* restrict thread) {
	return cnx_thread_get_id(thread) == 0;
}

CnxThreadID cnx_thread_get_id(const CnxThread* restrict thread) {
	return *thread;
}

// NOLINTNEXTLINE(readability-non-const-parameter)
CnxResult cnx_thread_join(CnxThread* restrict thread) {
	i32 res = 0;
	let result = thrd_join(*thread, &res);
	if(result != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, res);
}

// NOLINTNEXTLINE(readability-non-const-parameter)
CnxResult cnx_thread_detach(CnxThread* restrict thread) {
	let res = thrd_detach(*thread);
	CHECK_ERROR_POSIX(res);
}

void cnx_thread_free(void* thread) {
	ignore(cnx_thread_join(static_cast(CnxThread*)(thread)));
}

void cnx_this_thread_yield(void) {
	thrd_yield();
}

void cnx_this_thread_sleep_for(CnxDuration duration) {
	let as_nanoseconds = cnx_duration_cast(duration, cnx_nanoseconds_period);
	let seconds = as_nanoseconds.count / as_nanoseconds.period.den;
	let nanoseconds = as_nanoseconds.count % as_nanoseconds.period.den;
	let spec = (struct timespec){.tv_sec = seconds, .tv_nsec = nanoseconds};

	ignore(thrd_sleep(&spec, nullptr));
}

CnxThreadID cnx_this_thread_get_id(void) {
	return thrd_current();
}

CnxResult(CnxTLSKey) cnx_tls_new(void* data, void (*destructor)(void*)) {
	CnxTLSKey key = {0};
	let_mut res = cnx_tls_init(&key, data, destructor);
	return cnx_result_and(res, Ok(CnxTLSKey, key));
}

CnxResult cnx_tls_init(CnxTLSKey* restrict key, void* data, void (*destructor)(void*)) {
	let res = tss_create(key, destructor);

	if(res != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return cnx_tls_set(*key, data);
}

void* cnx_tls_get(CnxTLSKey key) {
	return tss_get(key);
}

CnxResult cnx_tls_set(CnxTLSKey key, void* data) {
	let res = tss_set(key, data);
	CHECK_ERROR_POSIX(res);
}

#elif ___CNX_HAS_PTHREADS

CnxResult(CnxBasicMutex) cnx_basic_mutex_new(void) {
	CnxBasicMutex mutex = {0};
	let_mut res = cnx_basic_mutex_init(&mutex);
	return cnx_result_and(res, Ok(CnxBasicMutex, mutex));
}

CnxResult cnx_basic_mutex_init(CnxBasicMutex* mutex) {
	pthread_mutexattr_t attribute = {0};
	let_mut res = pthread_mutexattr_init(&attribute);
	if(res != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutexattr_settype(&attribute, PTHREAD_MUTEX_NORMAL);
	if(res != 0) {
		pthread_mutexattr_destroy(&attribute);
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutex_init(mutex, &attribute);
	if(res != 0) {
		pthread_mutexattr_destroy(&attribute);
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutexattr_destroy(&attribute);
	if(res != 0) {
		pthread_mutex_destroy(mutex);
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

CnxResult cnx_basic_mutex_lock(CnxBasicMutex* restrict mutex) {
	let res = pthread_mutex_lock(mutex);
	CHECK_ERROR_POSIX(res);
}

bool cnx_basic_mutex_try_lock(CnxBasicMutex* restrict mutex) {
	return pthread_mutex_trylock(mutex) == 0;
}

CnxResult cnx_basic_mutex_unlock(CnxBasicMutex* restrict mutex) {
	let res = pthread_mutex_unlock(mutex);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_basic_mutex_free(CnxBasicMutex* restrict mutex) {
	let res = pthread_mutex_destroy(mutex);
	CHECK_ERROR_POSIX(res);
}

CnxResult(CnxRecursiveBasicMutex) cnx_recursive_basic_mutex_new(void) {
	CnxRecursiveBasicMutex mutex = {0};
	let_mut res = cnx_recursive_basic_mutex_init(&mutex);
	return cnx_result_and(res, Ok(CnxRecursiveBasicMutex, mutex));
}

CnxResult cnx_recursive_basic_mutex_init(CnxRecursiveBasicMutex* mutex) {
	pthread_mutexattr_t attribute = {0};
	let_mut res = pthread_mutexattr_init(&attribute);
	if(res != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutexattr_settype(&attribute, PTHREAD_MUTEX_RECURSIVE);
	if(res != 0) {
		pthread_mutexattr_destroy(&attribute);
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutex_init(mutex, &attribute);
	if(res != 0) {
		pthread_mutexattr_destroy(&attribute);
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutexattr_destroy(&attribute);
	if(res != 0) {
		pthread_mutex_destroy(mutex);
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

CnxResult cnx_recursive_basic_mutex_lock(CnxRecursiveBasicMutex* restrict mutex) {
	let res = pthread_mutex_lock(mutex);
	CHECK_ERROR_POSIX(res);
}

bool cnx_recursive_basic_mutex_try_lock(CnxRecursiveBasicMutex* restrict mutex) {
	return pthread_mutex_trylock(mutex) == 0;
}

CnxResult cnx_recursive_basic_mutex_unlock(CnxRecursiveBasicMutex* restrict mutex) {
	let res = pthread_mutex_unlock(mutex);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_recursive_basic_mutex_free(CnxRecursiveBasicMutex* restrict mutex) {
	let res = pthread_mutex_destroy(mutex);
	CHECK_ERROR_POSIX(res);
}

CnxResult(CnxCondvar) cnx_condvar_new(void) {
	CnxCondvar condvar = __CNX_CONDVAR_INITIALIZER;
	return Ok(CnxCondvar, condvar);
}

CnxResult cnx_condvar_init(CnxCondvar* restrict condvar) {
	*condvar = __CNX_CONDVAR_INITIALIZER;
	return Ok(i32, 0);
}

CnxResult cnx_condvar_signal(CnxCondvar* restrict condvar) {
	let res = pthread_cond_signal(condvar);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_condvar_broadcast(CnxCondvar* restrict condvar) {
	let res = pthread_cond_broadcast(condvar);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_condvar_wait(CnxCondvar* restrict condvar, CnxBasicMutex* restrict mutex) {
	let res = pthread_cond_wait(condvar, mutex);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_condvar_wait_for(CnxCondvar* restrict condvar,
							   CnxBasicMutex* restrict mutex,
							   CnxDuration to_wait) {
	let spec = duration_to_timespec(to_wait);

	let res = pthread_cond_timedwait(condvar, mutex, &spec);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_condvar_free(CnxCondvar* restrict condvar) {
	let res = pthread_cond_destroy(condvar);
	CHECK_ERROR_POSIX(res);
}

CnxResult cnx_execute_once(CnxOnceFlag* restrict flag, void (*function)(void)) {
	let res = pthread_once(flag, function);
	CHECK_ERROR_POSIX(res);
}

CnxCompare cnx_thread_id_compare(CnxThreadID lhs, CnxThreadID rhs) {
	return pthread_equal(lhs, rhs) != 0 ? CNX_EQUAL :
										  (lhs < rhs ? CNX_LESS_THAN : CNX_GREATER_THAN);
}

bool cnx_thread_id_equal(CnxThreadID lhs, CnxThreadID rhs) {
	return pthread_equal(lhs, rhs) != 0;
}

bool cnx_thread_id_less_than(CnxThreadID lhs, CnxThreadID rhs) {
	return lhs < rhs;
}

bool cnx_thread_id_less_than_or_equal(CnxThreadID lhs, CnxThreadID rhs) {
	return cnx_thread_id_equal(lhs, rhs) || cnx_thread_id_less_than(lhs, rhs);
}

bool cnx_thread_id_greater_than(CnxThreadID lhs, CnxThreadID rhs) {
	return lhs > rhs;
}

bool cnx_thread_id_greater_than_or_equal(CnxThreadID lhs, CnxThreadID rhs) {
	return cnx_thread_id_equal(lhs, rhs) || cnx_thread_id_greater_than(lhs, rhs);
}

[[always_inline]] [[not_null(1)]] static inline void* thread_invoke(void* lambda) {
	lambda_scoped _lambda = static_cast(CnxThreadLambda)(lambda);
	lambda_call(_lambda);
	return 0;
}

CnxResult(CnxThread) cnx_thread_new(CnxThreadLambda lambda) {
	CnxThread thread = {0};
	let_mut res = cnx_thread_init(&thread, lambda);
	return cnx_result_and(res, Ok(CnxThread, thread));
}

CnxResult cnx_thread_init(CnxThread* restrict thread, CnxThreadLambda lambda) {
	let res = pthread_create(thread, nullptr, thread_invoke, lambda);
	CHECK_ERROR_POSIX(res);
}

bool cnx_thread_is_null(const CnxThread* restrict thread) {
	return cnx_thread_get_id(thread) == 0;
}

CnxThreadID cnx_thread_get_id(const CnxThread* restrict thread) {
	return *thread;
}

// NOLINTNEXTLINE(readability-non-const-parameter)
CnxResult cnx_thread_join(CnxThread* restrict thread) {
	let res = pthread_join(*thread, nullptr);
	CHECK_ERROR_POSIX(res);
}

// NOLINTNEXTLINE(readability-non-const-parameter)
CnxResult cnx_thread_detach(CnxThread* restrict thread) {
	let res = pthread_detach(*thread);
	CHECK_ERROR_POSIX(res);
}

void cnx_thread_free(void* thread) {
	ignore(cnx_thread_join(static_cast(CnxThread*)(thread)));
}

void cnx_this_thread_yield(void) {
	sched_yield();
}

void cnx_this_thread_sleep_for(CnxDuration duration) {
	let_mut spec = duration_to_timespec(duration);
	while(nanosleep(&spec, &spec) == -1 && errno == EINTR) {
	}
}

CnxThreadID cnx_this_thread_get_id(void) {
	let thread = pthread_self();
	return cnx_thread_get_id(&thread);
}

CnxResult(CnxTLSKey) cnx_tls_new(void* data, void (*destructor)(void*)) {
	CnxTLSKey key = {0};
	let_mut res = cnx_tls_init(&key, data, destructor);
	return cnx_result_and(res, Ok(CnxTLSKey, key));
}

CnxResult cnx_tls_init(CnxTLSKey* restrict key, void* data, void (*destructor)(void*)) {
	let_mut res = pthread_key_create(key, destructor);
	if(res != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return cnx_tls_set(*key, data);
}

void* cnx_tls_get(CnxTLSKey key) {
	return pthread_getspecific(key);
}

CnxResult cnx_tls_set(CnxTLSKey key, void* data) {
	let res = pthread_setspecific(key, data);
	CHECK_ERROR_POSIX(res);
}

#elif CNX_PLATFORM_WINDOWS

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

CnxResult(CnxBasicMutex) cnx_basic_mutex_new(void) {
	CnxBasicMutex mutex;
	InitializeSRWLock(&mutex);
	return Ok(CnxBasicMutex, mutex);
}

CnxResult cnx_basic_mutex_init(CnxBasicMutex* restrict mutex) {
	InitializeSRWLock(mutex);
	return Ok(i32, 0);
}

CnxResult cnx_basic_mutex_lock(CnxBasicMutex* restrict mutex) {
	AcquireSRWLockExclusive(mutex);
	return Ok(i32, 0);
}

bool cnx_basic_mutex_try_lock(CnxBasicMutex* restrict mutex) {
	return TryAcquireSRWLockExclusive(mutex) != 0;
}

CnxResult cnx_basic_mutex_unlock(CnxBasicMutex* restrict mutex) {
	ReleaseSRWLockExclusive(mutex);
	return Ok(i32, 0);
}

CnxResult cnx_basic_mutex_free(CnxBasicMutex* restrict mutex) {
	ignore(mutex);
	return Ok(i32, 0);
}

CnxResult(CnxRecursiveBasicMutex) cnx_recursive_basic_mutex_new(void) {
	CnxRecursiveBasicMutex mutex;
	InitializeCriticalSection(&mutex);
	return Ok(CnxRecursiveBasicMutex, mutex);
}

CnxResult cnx_recursive_basic_mutex_init(CnxRecursiveBasicMutex* restrict mutex) {
	InitializeCriticalSection(mutex);
	return Ok(i32, 0);
}

CnxResult cnx_recursive_basic_mutex_lock(CnxRecursiveBasicMutex* restrict mutex) {
	EnterCriticalSection(mutex);
	return Ok(i32, 0);
}

bool cnx_recursive_basic_mutex_try_lock(CnxRecursiveBasicMutex* restrict mutex) {
	return TryEnterCriticalSection(mutex) != 0;
}

CnxResult cnx_recursive_basic_mutex_unlock(CnxRecursiveBasicMutex* restrict mutex) {
	LeaveCriticalSection(mutex);
	return Ok(i32, 0);
}

CnxResult cnx_recursive_basic_mutex_free(CnxRecursiveBasicMutex* restrict mutex) {
	DeleteCriticalSection(mutex);
	return Ok(i32, 0);
}

CnxResult(CnxCondvar) cnx_condvar_new(void) {
	CnxCondvar condvar;
	InitializeConditionVariable(&condvar);
	return Ok(CnxCondvar, condvar);
}

CnxResult cnx_condvar_init(CnxCondvar* restrict condvar) {
	InitializeConditionVariable(condvar);
	return Ok(i32, 0);
}

CnxResult cnx_condvar_signal(CnxCondvar* restrict condvar) {
	WakeConditionVariable(condvar);
	return Ok(i32, 0);
}

CnxResult cnx_condvar_broadcast(CnxCondvar* restrict condvar) {
	WakeAllConditionVariable(condvar);
	return Ok(i32, 0);
}

CnxResult cnx_condvar_wait(CnxCondvar* restrict condvar, CnxBasicMutex* restrict mutex) {
	SleepConditionVariableSRW(condvar, mutex, INFINITE, 0);
	return Ok(i32, 0);
}

CnxResult cnx_condvar_wait_for(CnxCondvar* restrict condvar,
							   CnxBasicMutex* restrict mutex,
							   CnxDuration to_wait) {
	let milliseconds = cnx_duration_cast(to_wait, cnx_milliseconds_period);

	if(!SleepConditionVariableSRW(condvar,
								  mutex,
								  milliseconds.count > 0 ? narrow_cast(DWORD)(milliseconds.count) :
														   0,
								  0))
	{
		let error = cnx_error_category_get_last_error(CNX_WIN32_ERROR_CATEGORY);
		return Err(i32, cnx_error_new(error, CNX_WIN32_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

CnxResult cnx_condvar_free(CnxCondvar* restrict condvar) {
	ignore(condvar);
	return Ok(i32, 0);
}

[[always_inline]] [[nodiscard]] static inline BOOL CALLBACK execute_once_thunk(CnxOnceFlag* flag,
																			   void* function,
																			   void** context) {
	ignore(flag, context);

	void (*to_exec)(void) = static_cast(void (*)(void))(function);
	to_exec();
	return (TRUE);
}

CnxResult cnx_execute_once(CnxOnceFlag* flag, void (*function)(void)) {
	if(!InitOnceExecuteOnce(flag, execute_once_thunk, static_cast(void*)(function), nullptr)) {
		let error = cnx_error_category_get_last_error(CNX_WIN32_ERROR_CATEGORY);
		return Err(i32, cnx_error_new(error, CNX_WIN32_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

CnxCompare cnx_thread_id_compare(CnxThreadID lhs, CnxThreadID rhs) {
	return lhs == rhs ? CNX_EQUAL : (lhs < rhs ? CNX_LESS_THAN : CNX_GREATER_THAN);
}

bool cnx_thread_id_equal(CnxThreadID lhs, CnxThreadID rhs) {
	return lhs == rhs;
}

bool cnx_thread_id_less_than(CnxThreadID lhs, CnxThreadID rhs) {
	return lhs < rhs;
}

bool cnx_thread_id_less_than_or_equal(CnxThreadID lhs, CnxThreadID rhs) {
	return cnx_thread_id_equal(lhs, rhs) || cnx_thread_id_less_than(lhs, rhs);
}

bool cnx_thread_id_greater_than(CnxThreadID lhs, CnxThreadID rhs) {
	return lhs > rhs;
}

bool cnx_thread_id_greater_than_or_equal(CnxThreadID lhs, CnxThreadID rhs) {
	return cnx_thread_id_equal(lhs, rhs) || cnx_thread_id_greater_than(lhs, rhs);
}

[[always_inline]] [[not_null(1)]] static inline unsigned long WINAPI thread_invoke(void* lambda) {
	lambda_scoped _lambda = static_cast(CnxThreadLambda)(lambda);
	lambda_call(_lambda);
	return 0;
}

CnxResult(CnxThread) cnx_thread_new(CnxThreadLambda lambda) {
	CnxThread thread = {0};
	let_mut res = cnx_thread_init(&thread, lambda);
	return cnx_result_and(res, Ok(CnxThread, thread));
}

CnxResult cnx_thread_init(CnxThread* restrict thread, CnxThreadLambda lambda) {
	*thread = CreateThread(nullptr, 0, thread_invoke, lambda, 0, nullptr);
	if(*thread == nullptr) {
		let error = cnx_error_category_get_last_error(CNX_WIN32_ERROR_CATEGORY);
		return Err(i32, cnx_error_new(error, CNX_WIN32_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

bool cnx_thread_is_null(const CnxThread* restrict thread) {
	return *thread == nullptr;
}

CnxThreadID cnx_thread_get_id(const CnxThread* restrict thread) {
	return GetThreadId(*thread);
}

CnxResult cnx_thread_join(CnxThread* restrict thread) {
	if(WaitForSingleObjectEx(*thread, INFINITE, false) == WAIT_FAILED) {
		let error = cnx_error_category_get_last_error(CNX_WIN32_ERROR_CATEGORY);
		return Err(i32, cnx_error_new(error, CNX_WIN32_ERROR_CATEGORY));
	}

	if(!CloseHandle(*thread)) {
		let error = cnx_error_category_get_last_error(CNX_WIN32_ERROR_CATEGORY);
		return Err(i32, cnx_error_new(error, CNX_WIN32_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

CnxResult cnx_thread_detach(CnxThread* restrict thread) {
	if(!CloseHandle(*thread)) {
		let error = cnx_error_category_get_last_error(CNX_WIN32_ERROR_CATEGORY);
		return Err(i32, cnx_error_new(error, CNX_WIN32_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

void cnx_thread_free(void* thread) {
	ignore(cnx_thread_join(static_cast(CnxThread*)(thread)));
}

void cnx_this_thread_yield(void) {
	SwitchToThread();
}

void cnx_this_thread_sleep_for(CnxDuration duration) {
	let milliseconds = cnx_duration_cast(duration, cnx_milliseconds_period);
	Sleep(narrow_cast(DWORD)(milliseconds.count));
}

CnxThreadID cnx_this_thread_get_id(void) {
	return GetCurrentThreadId();
}

CnxResult(CnxTLSKey) cnx_tls_new(void* data, void(__CNX_TLS_DESTRUCTOR_TAG* destructor)(void*)) {
	CnxTLSKey key = {0};
	let_mut res = cnx_tls_init(&key, data, destructor);
	return cnx_result_and(res, Ok(CnxTLSKey, key));
}

CnxResult
cnx_tls_init(CnxTLSKey* key, void* data, void(__CNX_TLS_DESTRUCTOR_TAG* destructor)(void*)) {
	DWORD index = FlsAlloc(destructor);
	if(index == FLS_OUT_OF_INDEXES) {
		let error = cnx_error_category_get_last_error(CNX_WIN32_ERROR_CATEGORY);
		return Err(i32, cnx_error_new(error, CNX_WIN32_ERROR_CATEGORY));
	}

	*key = index;

	return cnx_tls_set(*key, data);
}

void* cnx_tls_get(CnxTLSKey key) {
	return FlsGetValue(key);
}

CnxResult cnx_tls_set(CnxTLSKey key, void* data) {
	if(!FlsSetValue(key, data)) {
		let error = cnx_error_category_get_last_error(CNX_WIN32_ERROR_CATEGORY);
		return Err(i32, cnx_error_new(error, CNX_WIN32_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

#endif // ___CNX_HAS_C11_THREADS

void cnx_stop_token_request_stop(CnxStopToken* restrict token) {
	atomic_store(token, true);
}

bool cnx_stop_token_stop_requested(const CnxStopToken* restrict token) {
	return atomic_load(const_cast(CnxStopToken*)(token));
}

void LambdaFunction(jthread_invoke) {
	let_mut binding = lambda_binding(CnxJThreadLambda, CnxStopToken*);
	lambda_scoped lambda = binding._1;
	let token_ptr = binding._2;
	lambda_call(lambda, token_ptr);
	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, static_cast(void*)(token_ptr));
}

CnxResult(CnxJThread) cnx_jthread_new(CnxJThreadLambda lambda) {
	CnxJThread thread = {0};
	let_mut res = cnx_jthread_init(&thread, lambda);
	return cnx_result_and(res, Ok(CnxJThread, thread));
}

CnxResult cnx_jthread_init(CnxJThread* restrict thread, CnxJThreadLambda lambda) {
	let_mut token_ptr = cnx_allocator_allocate_t(CnxStopToken, DEFAULT_ALLOCATOR);
	if(token_ptr == nullptr) {
		return Err(i32, cnx_error_new(ENOMEM, CNX_POSIX_ERROR_CATEGORY));
	}

	atomic_store(token_ptr, false);

	let_mut res
		= cnx_thread_init(&(thread->handle),
						  lambda_cast(lambda(jthread_invoke, lambda, token_ptr), CnxThreadLambda));
	match(res) {
		variant(Ok) {
			thread->stop_token = token_ptr;
			return Ok(i32, 0);
		}
		variant(Err, err) {
			cnx_allocator_deallocate(DEFAULT_ALLOCATOR, static_cast(void*)(token_ptr));
			return Err(i32, err);
		}
	}

	unreachable();
}

CnxResult cnx_jthread_join(CnxJThread* restrict thread) {
	cnx_stop_token_request_stop(thread->stop_token);
	return cnx_thread_join(&(thread->handle));
}

void cnx_jthread_free(void* thread) {
	let_mut _thread = static_cast(CnxJThread*)(thread);
	ignore(cnx_jthread_join(_thread));
}
