/// @file StdThread.c
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

#include <C2nxt/StdAssert.h>
#include <C2nxt/StdThread.h>
#include <time.h>

#define RESULT_T	StdBasicMutex
#define RESULT_IMPL TRUE
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_IMPL

#define RESULT_T	StdRecursiveBasicMutex
#define RESULT_IMPL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_IMPL

#define RESULT_T	StdCondvar
#define RESULT_IMPL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_IMPL

#define RESULT_T	StdThread
#define RESULT_IMPL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_IMPL

#define RESULT_T	StdTLSKey
#define RESULT_IMPL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_IMPL

struct timespec duration_to_timespec(StdDuration duration) {
	let as_nanoseconds = std_duration_cast(duration, std_nanoseconds_period);
	let seconds = as_nanoseconds.count / as_nanoseconds.period.den;
	let nanoseconds = as_nanoseconds.count % as_nanoseconds.period.den;
	return (struct timespec){.tv_sec = seconds, .tv_nsec = static_cast(long)(nanoseconds)};
}

#define CHECK_ERROR_POSIX(res)                                           \
	if((res) != 0) {                                                     \
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY)); \
	}                                                                    \
                                                                         \
	return Ok(i32, 0)

#if ___STD_HAS_C11_THREADS
	#include <threads.h>

StdResult(StdBasicMutex) std_basic_mutex_new(void) {
	StdBasicMutex mutex;
	let_mut res = std_basic_mutex_init(&mutex);
	return std_result_and(res, Ok(StdBasicMutex, mutex));
}

StdResult std_basic_mutex_init(StdBasicMutex* restrict mutex) {
	let res = mtx_init(mutex, mtx_plain);
	CHECK_ERROR_POSIX(res);
}

StdResult std_basic_mutex_lock(StdBasicMutex* restrict mutex) {
	let res = mtx_lock(mutex);
	CHECK_ERROR_POSIX(res);
}

bool std_basic_mutex_try_lock(StdBasicMutex* restrict mutex) {
	return mtx_trylock(mutex) == thrd_success;
}

StdResult std_basic_mutex_unlock(StdBasicMutex* restrict mutex) {
	let res = mtx_unlock(mutex);
	CHECK_ERROR_POSIX(res);
}

StdResult std_basic_mutex_free(StdBasicMutex* restrict mutex) {
	mtx_destroy(mutex);
	return Ok(i32, 0);
}

StdResult(StdRecursiveBasicMutex) std_recursive_basic_mutex_new(void) {
	StdRecursiveBasicMutex mutex;
	let_mut res = std_recursive_basic_mutex_init(&mutex);
	return std_result_and(res, Ok(StdRecursiveBasicMutex, mutex));
}

StdResult std_recursive_basic_mutex_init(StdRecursiveBasicMutex* restrict mutex) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	let res = mtx_init(mutex, mtx_plain | mtx_recursive);
	CHECK_ERROR_POSIX(res);
}

StdResult std_recursive_basic_mutex_lock(StdRecursiveBasicMutex* restrict mutex) {
	let res = mtx_lock(mutex);
	CHECK_ERROR_POSIX(res);
}

bool std_recursive_basic_mutex_try_lock(StdRecursiveBasicMutex* restrict mutex) {
	return mtx_lock(mutex) == thrd_success;
}

StdResult std_recursive_basic_mutex_unlock(StdRecursiveBasicMutex* restrict mutex) {
	let res = mtx_unlock(mutex);
	CHECK_ERROR_POSIX(res);
}

StdResult std_recursive_basic_mutex_free(StdRecursiveBasicMutex* restrict mutex) {
	mtx_destroy(mutex);
	return Ok(i32, 0);
}

StdResult(StdCondvar) std_condvar_new(void) {
	StdCondvar condvar;
	let_mut res = std_condvar_init(&condvar);
	return std_result_and(res, Ok(StdCondvar, condvar));
}

StdResult std_condvar_init(StdCondvar* restrict condvar) {
	let res = cnd_init(condvar);
	CHECK_ERROR_POSIX(res);
}

StdResult std_condvar_signal(StdCondvar* restrict condvar) {
	let res = cnd_signal(condvar);
	CHECK_ERROR_POSIX(res);
}

StdResult std_condvar_broadcast(StdCondvar* restrict condvar) {
	let res = cnd_broadcast(condvar);
	CHECK_ERROR_POSIX(res);
}

StdResult std_condvar_wait(StdCondvar* restrict condvar, StdBasicMutex* restrict mutex) {
	let res = cnd_wait(condvar, mutex);
	CHECK_ERROR_POSIX(res);
}

StdResult std_condvar_timedwait(StdCondvar* restrict condvar,
								StdBasicMutex* restrict mutex,
								StdDuration to_wait) {
	let spec = duration_to_timespec(to_wait);

	let res = cnd_timedwait(condvar, mutex, &spec);
	CHECK_ERROR_POSIX(res);
}

StdResult std_condvar_free(StdCondvar* restrict condvar) {
	cnd_destroy(condvar);

	return Ok(i32, 0);
}

StdResult std_execute_once(StdOnceFlag* restrict flag, void (*function)(void)) {
	call_once(flag, function);
	return Ok(i32, 0);
}

StdCompare std_thread_id_compare(StdThreadID lhs, StdThreadID rhs) {
	return thrd_equal(lhs, rhs) != 0 ? STD_EQUAL : (lhs < rhs ? STD_LESS_THAN : STD_GREATER_THAN);
}

bool std_thread_id_equal(StdThreadID lhs, StdThreadID rhs) {
	return thrd_equal(lhs, rhs) != 0;
}

bool std_thread_id_less_than(StdThreadID lhs, StdThreadID rhs) {
	return lhs < rhs;
}

bool std_thread_id_less_than_or_equal(StdThreadID lhs, StdThreadID rhs) {
	return std_thread_id_equal(lhs, rhs) || std_thread_id_less_than(lhs, rhs);
}

bool std_thread_id_greater_than(StdThreadID lhs, StdThreadID rhs) {
	return lhs > rhs;
}

bool std_thread_id_greater_than_or_equal(StdThreadID lhs, StdThreadID rhs) {
	return std_thread_id_equal(lhs, rhs) || std_thread_id_greater_than(lhs, rhs);
}

[[always_inline]] [[not_null(1)]] static inline int thread_invoke(void* lambda) {
	lambda_scoped _lambda = static_cast(StdThreadLambda)(lambda);
	lambda_call(_lambda);
	return 0;
}

StdResult(StdThread) std_thread_new(StdThreadLambda lambda) {
	StdThread thread; // NOLINT
	let_mut res = std_thread_init(&thread, lambda);
	return std_result_and(res, Ok(StdThread, thread));
}

StdResult std_thread_init(StdThread* thread, StdThreadLambda lambda) {
	let res = thrd_create(thread, thread_invoke, lambda);
	CHECK_ERROR_POSIX(res);
}

bool std_thread_is_null(const StdThread* restrict thread) {
	return std_thread_get_id(thread) == 0;
}

StdThreadID std_thread_get_id(const StdThread* restrict thread) {
	return *thread;
}

// NOLINTNEXTLINE(readability-non-const-parameter)
StdResult std_thread_join(StdThread* restrict thread) {
	i32 res = 0;
	let result = thrd_join(*thread, &res);
	if(result != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, res);
}

// NOLINTNEXTLINE(readability-non-const-parameter)
StdResult std_thread_detach(StdThread* restrict thread) {
	let res = thrd_detach(*thread);
	CHECK_ERROR_POSIX(res);
}

void std_thread_free(void* thread) {
	ignore(std_thread_join(static_cast(StdThread*)(thread)));
}

void std_this_thread_yield(void) {
	thrd_yield();
}

void std_this_thread_sleep_for(StdDuration duration) {
	let as_nanoseconds = std_duration_cast(duration, std_nanoseconds_period);
	let seconds = as_nanoseconds.count / as_nanoseconds.period.den;
	let nanoseconds = as_nanoseconds.count % as_nanoseconds.period.den;
	let spec = (struct timespec){.tv_sec = seconds, .tv_nsec = nanoseconds};

	ignore(thrd_sleep(&spec, nullptr));
}

StdThreadID std_this_thread_get_id(void) {
	return thrd_current();
}

StdResult(StdTLSKey) std_tls_new(void* data, void (*destructor)(void*)) {
	StdTLSKey key = {0};
	let_mut res = std_tls_init(&key, data, destructor);
	return std_result_and(res, Ok(StdTLSKey, key));
}

StdResult std_tls_init(StdTLSKey* restrict key, void* data, void (*destructor)(void*)) {
	let res = tss_create(key, destructor);

	if(res != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return std_tls_set(*key, data);
}

void* std_tls_get(StdTLSKey key) {
	return tss_get(key);
}

StdResult std_tls_set(StdTLSKey key, void* data) {
	let res = tss_set(key, data);
	CHECK_ERROR_POSIX(res);
}

#elif ___STD_HAS_PTHREADS

StdResult(StdBasicMutex) std_basic_mutex_new(void) {
	StdBasicMutex mutex = {0};
	let_mut res = std_basic_mutex_init(&mutex);
	return std_result_and(res, Ok(StdBasicMutex, mutex));
}

StdResult std_basic_mutex_init(StdBasicMutex* mutex) {
	pthread_mutexattr_t attribute = {0};
	let_mut res = pthread_mutexattr_init(&attribute);
	if(res != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutexattr_settype(&attribute, PTHREAD_MUTEX_NORMAL);
	if(res != 0) {
		pthread_mutexattr_destroy(&attribute);
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutex_init(mutex, &attribute);
	if(res != 0) {
		pthread_mutexattr_destroy(&attribute);
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutexattr_destroy(&attribute);
	if(res != 0) {
		pthread_mutex_destroy(mutex);
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_basic_mutex_lock(StdBasicMutex* restrict mutex) {
	let res = pthread_mutex_lock(mutex);
	CHECK_ERROR_POSIX(res);
}

bool std_basic_mutex_try_lock(StdBasicMutex* restrict mutex) {
	return pthread_mutex_trylock(mutex) == 0;
}

StdResult std_basic_mutex_unlock(StdBasicMutex* restrict mutex) {
	let res = pthread_mutex_unlock(mutex);
	CHECK_ERROR_POSIX(res);
}

StdResult std_basic_mutex_free(StdBasicMutex* restrict mutex) {
	let res = pthread_mutex_destroy(mutex);
	CHECK_ERROR_POSIX(res);
}

StdResult(StdRecursiveBasicMutex) std_recursive_basic_mutex_new(void) {
	StdRecursiveBasicMutex mutex = {0};
	let_mut res = std_recursive_basic_mutex_init(&mutex);
	return std_result_and(res, Ok(StdRecursiveBasicMutex, mutex));
}

StdResult std_recursive_basic_mutex_init(StdRecursiveBasicMutex* mutex) {
	pthread_mutexattr_t attribute = {0};
	let_mut res = pthread_mutexattr_init(&attribute);
	if(res != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutexattr_settype(&attribute, PTHREAD_MUTEX_RECURSIVE);
	if(res != 0) {
		pthread_mutexattr_destroy(&attribute);
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutex_init(mutex, &attribute);
	if(res != 0) {
		pthread_mutexattr_destroy(&attribute);
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	res = pthread_mutexattr_destroy(&attribute);
	if(res != 0) {
		pthread_mutex_destroy(mutex);
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_recursive_basic_mutex_lock(StdRecursiveBasicMutex* restrict mutex) {
	let res = pthread_mutex_lock(mutex);
	CHECK_ERROR_POSIX(res);
}

bool std_recursive_basic_mutex_try_lock(StdRecursiveBasicMutex* restrict mutex) {
	return pthread_mutex_trylock(mutex) == 0;
}

StdResult std_recursive_basic_mutex_unlock(StdRecursiveBasicMutex* restrict mutex) {
	let res = pthread_mutex_unlock(mutex);
	CHECK_ERROR_POSIX(res);
}

StdResult std_recursive_basic_mutex_free(StdRecursiveBasicMutex* restrict mutex) {
	let res = pthread_mutex_destroy(mutex);
	CHECK_ERROR_POSIX(res);
}

StdResult(StdCondvar) std_condvar_new(void) {
	StdCondvar condvar = __STD_CONDVAR_INITIALIZER;
	return Ok(StdCondvar, condvar);
}

StdResult std_condvar_init(StdCondvar* restrict condvar) {
	*condvar = __STD_CONDVAR_INITIALIZER;
	return Ok(i32, 0);
}

StdResult std_condvar_signal(StdCondvar* restrict condvar) {
	let res = pthread_cond_signal(condvar);
	CHECK_ERROR_POSIX(res);
}

StdResult std_condvar_broadcast(StdCondvar* restrict condvar) {
	let res = pthread_cond_broadcast(condvar);
	CHECK_ERROR_POSIX(res);
}

StdResult std_condvar_wait(StdCondvar* restrict condvar, StdBasicMutex* restrict mutex) {
	let res = pthread_cond_wait(condvar, mutex);
	CHECK_ERROR_POSIX(res);
}

StdResult std_condvar_timedwait(StdCondvar* restrict condvar,
								StdBasicMutex* restrict mutex,
								StdDuration to_wait) {
	let spec = duration_to_timespec(to_wait);

	let res = pthread_cond_timedwait(condvar, mutex, &spec);
	CHECK_ERROR_POSIX(res);
}

StdResult std_condvar_free(StdCondvar* restrict condvar) {
	let res = pthread_cond_destroy(condvar);
	CHECK_ERROR_POSIX(res);
}

StdResult std_execute_once(StdOnceFlag* restrict flag, void (*function)(void)) {
	let res = pthread_once(flag, function);
	CHECK_ERROR_POSIX(res);
}

StdCompare std_thread_id_compare(StdThreadID lhs, StdThreadID rhs) {
	return pthread_equal(lhs, rhs) != 0 ? STD_EQUAL :
										  (lhs < rhs ? STD_LESS_THAN : STD_GREATER_THAN);
}

bool std_thread_id_equal(StdThreadID lhs, StdThreadID rhs) {
	return pthread_equal(lhs, rhs) != 0;
}

bool std_thread_id_less_than(StdThreadID lhs, StdThreadID rhs) {
	return lhs < rhs;
}

bool std_thread_id_less_than_or_equal(StdThreadID lhs, StdThreadID rhs) {
	return std_thread_id_equal(lhs, rhs) || std_thread_id_less_than(lhs, rhs);
}

bool std_thread_id_greater_than(StdThreadID lhs, StdThreadID rhs) {
	return lhs > rhs;
}

bool std_thread_id_greater_than_or_equal(StdThreadID lhs, StdThreadID rhs) {
	return std_thread_id_equal(lhs, rhs) || std_thread_id_greater_than(lhs, rhs);
}

[[always_inline]] [[not_null(1)]] static inline void* thread_invoke(void* lambda) {
	lambda_scoped _lambda = static_cast(StdThreadLambda)(lambda);
	lambda_call(_lambda);
	return 0;
}

StdResult(StdThread) std_thread_new(StdThreadLambda lambda) {
	StdThread thread = {0};
	let_mut res = std_thread_init(&thread, lambda);
	return std_result_and(res, Ok(StdThread, thread));
}

StdResult std_thread_init(StdThread* restrict thread, StdThreadLambda lambda) {
	let res = pthread_create(thread, nullptr, thread_invoke, lambda);
	CHECK_ERROR_POSIX(res);
}

bool std_thread_is_null(const StdThread* restrict thread) {
	return std_thread_get_id(thread) == 0;
}

StdThreadID std_thread_get_id(const StdThread* restrict thread) {
	return *thread;
}

// NOLINTNEXTLINE(readability-non-const-parameter)
StdResult std_thread_join(StdThread* restrict thread) {
	let res = pthread_join(*thread, nullptr);
	CHECK_ERROR_POSIX(res);
}

// NOLINTNEXTLINE(readability-non-const-parameter)
StdResult std_thread_detach(StdThread* restrict thread) {
	let res = pthread_detach(*thread);
	CHECK_ERROR_POSIX(res);
}

void std_thread_free(void* thread) {
	ignore(std_thread_join(static_cast(StdThread*)(thread)));
}

void std_this_thread_yield(void) {
	sched_yield();
}

void std_this_thread_sleep_for(StdDuration duration) {
	let_mut spec = duration_to_timespec(duration);
	while(nanosleep(&spec, &spec) == -1 && errno == EINTR) {
	}
}

StdThreadID std_this_thread_get_id(void) {
	let thread = pthread_self();
	return std_thread_get_id(&thread);
}

StdResult(StdTLSKey) std_tls_new(void* data, void (*destructor)(void*)) {
	StdTLSKey key = {0};
	let_mut res = std_tls_init(&key, data, destructor);
	return std_result_and(res, Ok(StdTLSKey, key));
}

StdResult std_tls_init(StdTLSKey* restrict key, void* data, void (*destructor)(void*)) {
	let_mut res = pthread_key_create(key, destructor);
	if(res != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return std_tls_set(*key, data);
}

void* std_tls_get(StdTLSKey key) {
	return pthread_getspecific(key);
}

StdResult std_tls_set(StdTLSKey key, void* data) {
	let res = pthread_setspecific(key, data);
	CHECK_ERROR_POSIX(res);
}

#elif STD_PLATFORM_WINDOWS

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

StdResult(StdBasicMutex) std_basic_mutex_new(void) {
	StdBasicMutex mutex;
	InitializeSRWLock(&mutex);
	return Ok(StdBasicMutex, mutex);
}

StdResult std_basic_mutex_init(StdBasicMutex* restrict mutex) {
	InitializeSRWLock(mutex);
	return Ok(i32, 0);
}

StdResult std_basic_mutex_lock(StdBasicMutex* restrict mutex) {
	AcquireSRWLockExclusive(mutex);
	return Ok(i32, 0);
}

bool std_basic_mutex_try_lock(StdBasicMutex* restrict mutex) {
	return TryAcquireSRWLockExclusive(mutex) != 0;
}

StdResult std_basic_mutex_unlock(StdBasicMutex* restrict mutex) {
	ReleaseSRWLockExclusive(mutex);
	return Ok(i32, 0);
}

StdResult std_basic_mutex_free(StdBasicMutex* restrict mutex) {
	ignore(mutex);
	return Ok(i32, 0);
}

StdResult(StdRecursiveBasicMutex) std_recursive_basic_mutex_new(void) {
	StdRecursiveBasicMutex mutex;
	InitializeCriticalSection(&mutex);
	return Ok(StdRecursiveBasicMutex, mutex);
}

StdResult std_recursive_basic_mutex_init(StdRecursiveBasicMutex* restrict mutex) {
	InitializeCriticalSection(mutex);
	return Ok(i32, 0);
}

StdResult std_recursive_basic_mutex_lock(StdRecursiveBasicMutex* restrict mutex) {
	EnterCriticalSection(mutex);
	return Ok(i32, 0);
}

bool std_recursive_basic_mutex_try_lock(StdRecursiveBasicMutex* restrict mutex) {
	return TryEnterCriticalSection(mutex) != 0;
}

StdResult std_recursive_basic_mutex_unlock(StdRecursiveBasicMutex* restrict mutex) {
	LeaveCriticalSection(mutex);
	return Ok(i32, 0);
}

StdResult std_recursive_basic_mutex_free(StdRecursiveBasicMutex* restrict mutex) {
	DeleteCriticalSection(mutex);
	return Ok(i32, 0);
}

StdResult(StdCondvar) std_condvar_new(void) {
	StdCondvar condvar;
	InitializeConditionVariable(&condvar);
	return Ok(StdCondvar, condvar);
}

StdResult std_condvar_init(StdCondvar* restrict condvar) {
	InitializeConditionVariable(condvar);
	return Ok(i32, 0);
}

StdResult std_condvar_signal(StdCondvar* restrict condvar) {
	WakeConditionVariable(condvar);
	return Ok(i32, 0);
}

StdResult std_condvar_broadcast(StdCondvar* restrict condvar) {
	WakeAllConditionVariable(condvar);
	return Ok(i32, 0);
}

StdResult std_condvar_wait(StdCondvar* restrict condvar, StdBasicMutex* restrict mutex) {
	SleepConditionVariableSRW(condvar, mutex, INFINITE, 0);
	return Ok(i32, 0);
}

StdResult std_condvar_timedwait(StdCondvar* restrict condvar,
								StdBasicMutex* restrict mutex,
								StdDuration to_wait) {
	let milliseconds = std_duration_cast(to_wait, std_milliseconds_period);

	if(!SleepConditionVariableSRW(condvar,
								  mutex,
								  milliseconds.count > 0 ? narrow_cast(DWORD)(milliseconds.count) :
														   0,
								  0))
	{
		let error = GetLastError();
		// TODO(braxtons12) Implement STD_WIN32_ERROR_CATEGORY
		return Err(i32, std_error_new(error, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_condvar_free(StdCondvar* restrict condvar) {
	ignore(condvar);
	return Ok(i32, 0);
}

[[always_inline]] [[nodiscard]] static inline BOOL CALLBACK execute_once_thunk(StdOnceFlag* flag,
																			   void* function,
																			   void** context) {
	ignore(flag, context);

	void (*to_exec)(void) = static_cast(void (*)(void))(function);
	to_exec();
	return (TRUE);
}

StdResult std_execute_once(StdOnceFlag* flag, void (*function)(void)) {
	if(!InitOnceExecuteOnce(flag, execute_once_thunk, static_cast(void*)(function), nullptr)) {
		let error = GetLastError();
		// TODO(braxtons12) Implement STD_WIN32_ERROR_CATEGORY
		return Err(i32, std_error_new(error, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdCompare std_thread_id_compare(StdThreadID lhs, StdThreadID rhs) {
	return lhs == rhs ? STD_EQUAL : (lhs < rhs ? STD_LESS_THAN : STD_GREATER_THAN);
}

bool std_thread_id_equal(StdThreadID lhs, StdThreadID rhs) {
	return lhs == rhs;
}

bool std_thread_id_less_than(StdThreadID lhs, StdThreadID rhs) {
	return lhs < rhs;
}

bool std_thread_id_less_than_or_equal(StdThreadID lhs, StdThreadID rhs) {
	return std_thread_id_equal(lhs, rhs) || std_thread_id_less_than(lhs, rhs);
}

bool std_thread_id_greater_than(StdThreadID lhs, StdThreadID rhs) {
	return lhs > rhs;
}

bool std_thread_id_greater_than_or_equal(StdThreadID lhs, StdThreadID rhs) {
	return std_thread_id_equal(lhs, rhs) || std_thread_id_greater_than(lhs, rhs);
}

[[always_inline]] [[not_null(1)]] static inline unsigned long WINAPI thread_invoke(void* lambda) {
	lambda_scoped _lambda = static_cast(StdThreadLambda)(lambda);
	lambda_call(_lambda);
	return 0;
}

StdResult(StdThread) std_thread_new(StdThreadLambda lambda) {
	StdThread thread = {0};
	let_mut res = std_thread_init(&thread, lambda);
	return std_result_and(res, Ok(StdThread, thread));
}

StdResult std_thread_init(StdThread* restrict thread, StdThreadLambda lambda) {
	*thread = CreateThread(nullptr, 0, thread_invoke, lambda, 0, nullptr);
	if(*thread == nullptr) {
		let error = GetLastError();
		// TODO(braxtons12) Implement STD_WIN32_ERROR_CATEGORY
		return Err(i32, std_error_new(error, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

bool std_thread_is_null(const StdThread* restrict thread) {
	return *thread == nullptr;
}

StdThreadID std_thread_get_id(const StdThread* restrict thread) {
	return GetThreadId(*thread);
}

StdResult std_thread_join(StdThread* restrict thread) {
	if(WaitForSingleObjectEx(*thread, INFINITE, false) == WAIT_FAILED) {
		let error = GetLastError();
		// TODO(braxtons12) Implement STD_WIN32_ERROR_CATEGORY
		return Err(i32, std_error_new(error, STD_POSIX_ERROR_CATEGORY));
	}

	if(!CloseHandle(*thread)) {
		let error = GetLastError();
		// TODO(braxtons12) Implement STD_WIN32_ERROR_CATEGORY
		return Err(i32, std_error_new(error, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_thread_detach(StdThread* restrict thread) {
	if(!CloseHandle(*thread)) {
		let error = GetLastError();
		// TODO(braxtons12) Implement STD_WIN32_ERROR_CATEGORY
		return Err(i32, std_error_new(error, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

void std_thread_free(void* thread) {
	ignore(std_thread_join(static_cast(StdThread*)(thread)));
}

void std_this_thread_yield(void) {
	SwitchToThread();
}

void std_this_thread_sleep_for(StdDuration duration) {
	let milliseconds = std_duration_cast(duration, std_milliseconds_period);
	Sleep(narrow_cast(DWORD)(milliseconds.count));
}

StdThreadID std_this_thread_get_id(void) {
	return GetCurrentThreadId();
}

StdResult(StdTLSKey) std_tls_new(void* data, void(__STD_TLS_DESTRUCTOR_TAG* destructor)(void*)) {
	StdTLSKey key = {0};
	let_mut res = std_tls_init(&key, data, destructor);
	return std_result_and(res, Ok(StdTLSKey, key));
}

StdResult
std_tls_init(StdTLSKey* key, void* data, void(__STD_TLS_DESTRUCTOR_TAG* destructor)(void*)) {
	DWORD index = FlsAlloc(destructor);
	if(index == FLS_OUT_OF_INDEXES) {
		let error = GetLastError();
		// TODO(braxtons12) Implement STD_WIN32_ERROR_CATEGORY
		return Err(i32, std_error_new(error, STD_POSIX_ERROR_CATEGORY));
	}

	*key = index;

	return std_tls_set(*key, data);
}

void* std_tls_get(StdTLSKey key) {
	return FlsGetValue(key);
}

StdResult std_tls_set(StdTLSKey key, void* data) {
	if(!FlsSetValue(key, data)) {
		let error = GetLastError();
		// TODO(braxtons12) Implement STD_WIN32_ERROR_CATEGORY
		return Err(i32, std_error_new(error, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

#endif // ___STD_HAS_C11_THREADS
