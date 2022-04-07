/// @file StdThread.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Function implementations for threading functionality
/// @version 0.1.2
/// @date 2022-04-06
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

#include <C2nxt/StdThread.h>
#include <threads.h>
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

#if ___STD_HAS_C11_THREADS

StdResult(StdBasicMutex) std_basic_mutex_new(void) {
	StdBasicMutex mutex;
	let ret = mtx_init(&mutex, mtx_plain);
	if(ret != 0) {
		return Err(StdBasicMutex, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(StdBasicMutex, mutex);
}

StdResult std_basic_mutex_init(StdBasicMutex* restrict mutex) {
	let ret = mtx_init(mutex, mtx_plain);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_basic_mutex_lock(StdBasicMutex* restrict mutex) {
	let ret = mtx_lock(mutex);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

bool std_basic_mutex_try_lock(StdBasicMutex* restrict mutex) {
	return mtx_trylock(mutex) == thrd_success;
}

StdResult std_basic_mutex_unlock(StdBasicMutex* restrict mutex) {
	let ret = mtx_unlock(mutex);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_basic_mutex_free(StdBasicMutex* restrict mutex) {
	mtx_destroy(mutex);
	return Ok(i32, 0);
}

StdResult(StdRecursiveBasicMutex) std_recursive_basic_mutex_new(void) {
	StdRecursiveBasicMutex mutex;
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	let ret = mtx_init(&mutex, mtx_plain | mtx_recursive);
	if(ret != 0) {
		return Err(StdRecursiveBasicMutex, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(StdRecursiveBasicMutex, mutex);
}

StdResult std_recursive_basic_mutex_init(StdRecursiveBasicMutex* restrict mutex) {
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	let ret = mtx_init(mutex, mtx_plain | mtx_recursive);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_recursive_basic_mutex_lock(StdRecursiveBasicMutex* restrict mutex) {
	let ret = mtx_lock(mutex);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

bool std_recursive_basic_mutex_try_lock(StdRecursiveBasicMutex* restrict mutex) {
	return mtx_lock(mutex) == thrd_success;
}

StdResult std_recursive_basic_mutex_unlock(StdRecursiveBasicMutex* restrict mutex) {
	let ret = mtx_unlock(mutex);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_recursive_basic_mutex_free(StdRecursiveBasicMutex* restrict mutex) {
	mtx_destroy(mutex);
	return Ok(i32, 0);
}

StdResult(StdCondvar) std_condvar_new(void) {
	StdCondvar condvar;
	let ret = cnd_init(&condvar);
	if(ret != 0) {
		return Err(StdCondvar, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(StdCondvar, condvar);
}

StdResult std_condvar_init(StdCondvar* restrict condvar) {
	let ret = cnd_init(condvar);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_condvar_signal(StdCondvar* restrict condvar) {
	let ret = cnd_signal(condvar);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_condvar_broadcast(StdCondvar* restrict condvar) {
	let ret = cnd_broadcast(condvar);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_condvar_wait(StdCondvar* restrict condvar, StdBasicMutex* restrict mutex) {
	let ret = cnd_wait(condvar, mutex);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_condvar_timedwait(StdCondvar* restrict condvar,
								StdBasicMutex* restrict mutex,
								StdDuration to_wait) {
	let as_nanoseconds = std_duration_cast(to_wait, std_nanoseconds_period);
	let seconds = as_nanoseconds.count / as_nanoseconds.period.den;
	let nanoseconds = as_nanoseconds.count % as_nanoseconds.period.den;
	let spec = (struct timespec){.tv_sec = seconds, .tv_nsec = nanoseconds};

	let ret = cnd_timedwait(condvar, mutex, &spec);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

StdResult std_condvar_free(StdCondvar* restrict condvar) {
	cnd_destroy(condvar);

	return Ok(i32, 0);
}

StdResult std_execute_once(StdOnceFlag* restrict flag, void (*function)(void)) {
	call_once(flag, function);
	return Ok(i32, 0);
}

[[always_inline]] [[not_null(1)]] static inline int thread_invoke(void* lambda) {
	let _lambda = static_cast(StdThreadLambda*)(lambda);
	lambda_call(*_lambda);
	return 0;
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

StdResult(StdThread) std_thread_new(StdThreadLambda lambda) {
	StdThread thread = {0};
	let ret = thrd_create(&thread, thread_invoke, &lambda);

	if(ret != 0) {
		return Err(StdThread, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(StdThread, thread);
}

StdResult std_thread_init(StdThread* thread, StdThreadLambda lambda) {
	let ret = thrd_create(thread, thread_invoke, &lambda);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
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
	let ret = thrd_join(*thread, &res);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, res);
}

// NOLINTNEXTLINE(readability-non-const-parameter)
StdResult std_thread_detach(StdThread* restrict thread) {
	let ret = thrd_detach(*thread);
	if(ret != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
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
	let_mut res = tss_create(&key, destructor);
	if(res != 0) {
		return Err(StdTLSKey, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	res = tss_set(key, data);

	if(res != 0) {
		return Err(StdTLSKey, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(StdTLSKey, key);
}

StdResult std_tls_init(StdTLSKey* restrict key, void* data, void (*destructor)(void*)) {
	let_mut res = tss_create(key, destructor);

	if(res != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	res = tss_set(*key, data);

	if(res != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

void* std_tls_get(StdTLSKey key) {
	return tss_get(key);
}

StdResult std_tls_set(StdTLSKey key, void* data) {
	let res = tss_set(key, data);
	if(res != 0) {
		return Err(i32, std_error_new(errno, STD_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

#elif ___STD_HAS_PTHREADS

#elif STD_PLATFORM_WINDOWS

#endif // ___STD_HAS_C11_THREADS
