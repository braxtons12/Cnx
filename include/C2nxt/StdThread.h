/// @file StdThread.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Type definitions and function declarations for threading functionality
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

#ifndef STD_THREAD
#define STD_THREAD

#include <C2nxt/StdAtomic.h>
#include <C2nxt/StdDef.h>
#include <C2nxt/StdLambda.h>
#include <C2nxt/__std_thread/__std_thread.h>
#include <C2nxt/time/StdDuration.h>

#define RESULT_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <C2nxt/StdResult.h>
#undef RESULT_INCLUDE_DEFAULT_INSTANTIATIONS

typedef __std_basic_mutex StdBasicMutex;
typedef __std_recursive_basic_mutex StdRecursiveBasicMutex;
typedef __std_condvar StdCondvar;
typedef __std_thread_id StdThreadID;
typedef __std_thread StdThread;
typedef struct {
	StdThread m_handle;
	atomic_bool join_flag;
} StdJThread;
typedef __std_tls_key StdTLSKey;
typedef __std_exec_once_flag StdOnceFlag;

#ifndef thread_local
	#define thread_local _Thread_local
#endif

#define RESULT_T	StdBasicMutex
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_DECL

#define RESULT_T	StdRecursiveBasicMutex
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_DECL

#define RESULT_T	StdCondvar
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_DECL

#define RESULT_T	StdThread
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_DECL

#define RESULT_T	StdTLSKey
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <C2nxt/StdResult.h>
#undef RESULT_T
#undef RESULT_DECL

#define ___DISABLE_IF_NULL(mutex) \
	std_disable_if(!(mutex), "Can't perform a mutex operation on a nullptr")

[[nodiscard]] StdResult(StdBasicMutex) std_basic_mutex_new(void);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_basic_mutex_init(StdBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_basic_mutex_lock(StdBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex);
[[nodiscard]] [[not_null(1)]] bool
// NOLINTNEXTLINE(readability-non-const-parameter)
std_basic_mutex_try_lock(StdBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_basic_mutex_unlock(StdBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_basic_mutex_free(StdBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex);

[[nodiscard]] StdResult(StdRecursiveBasicMutex) std_recursive_basic_mutex_new(void);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_recursive_basic_mutex_init(StdRecursiveBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_recursive_basic_mutex_lock(StdRecursiveBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex);
[[nodiscard]] [[not_null(1)]] bool
// NOLINTNEXTLINE(readability-non-const-parameter)
std_recursive_basic_mutex_try_lock(StdRecursiveBasicMutex* restrict mutex)
	___DISABLE_IF_NULL(mutex);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_recursive_basic_mutex_unlock(StdRecursiveBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_recursive_basic_mutex_free(StdRecursiveBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex);

#undef ___DISABLE_IF_NULL

#define ___DISABLE_IF_NULL(condvar) \
	std_disable_if(!(condvar), "Can't perform a condvar operation on a nullptr")

[[nodiscard]] StdResult(StdCondvar) std_condvar_new(void);
[[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_condvar_init(StdCondvar* restrict condvar) ___DISABLE_IF_NULL(condvar);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_condvar_signal(StdCondvar* restrict condvar) ___DISABLE_IF_NULL(condvar);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_condvar_broadcast(StdCondvar* restrict condvar) ___DISABLE_IF_NULL(condvar);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_condvar_wait(StdCondvar* restrict condvar, StdBasicMutex* restrict mutex)
	___DISABLE_IF_NULL(condvar);
// NOLINTNEXTLINE(readability-non-const-parameter)
[[nodiscard]] [[not_null(1, 2)]] StdResult
std_condvar_timedwait(StdCondvar* restrict condvar,
					  // NOLINTNEXTLINE(readability-non-const-parameter)
					  StdBasicMutex* restrict mutex,
					  StdDuration to_wait) ___DISABLE_IF_NULL(condvar)
	std_disable_if(!mutex, "Can't do a timed wait with a null mutex");
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_condvar_free(StdCondvar* restrict condvar) ___DISABLE_IF_NULL(condvar);

[[nodiscard]] [[not_null(1, 2)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_execute_once(StdOnceFlag* restrict flag, void (*function)(void))
	std_disable_if(!flag, "Can't perform an execute once with a null `StdOnceFlag`")
		std_disable_if(!function, "Can't execute a nullptr");

[[nodiscard]] StdCompare std_thread_id_compare(StdThreadID lhs, StdThreadID rhs);
[[nodiscard]] bool std_thread_id_equal(StdThreadID lhs, StdThreadID rhs);
[[nodiscard]] bool std_thread_id_less_than(StdThreadID lhs, StdThreadID rhs);
[[nodiscard]] bool std_thread_id_less_than_or_equal(StdThreadID lhs, StdThreadID rhs);
[[nodiscard]] bool std_thread_id_greater_than(StdThreadID lhs, StdThreadID rhs);
[[nodiscard]] bool std_thread_id_greater_than_or_equal(StdThreadID lhs, StdThreadID rhs);

typedef Lambda(void) StdThreadLambda;

#undef ___DISABLE_IF_NULL

#define ___DISABLE_IF_NULL(thread) \
	std_disable_if(!(thread), "Can't perform a thread operation on a nullptr")

[[nodiscard]] StdResult(StdThread) std_thread_new(StdThreadLambda lambda);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_thread_init(StdThread* restrict thread, StdThreadLambda lambda) ___DISABLE_IF_NULL(thread);
[[nodiscard]] [[not_null(1)]] bool
std_thread_is_null(const StdThread* restrict thread) ___DISABLE_IF_NULL(thread);
[[nodiscard]] [[not_null(1)]] StdThreadID
std_thread_get_id(const StdThread* restrict thread) ___DISABLE_IF_NULL(thread);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_thread_join(StdThread* restrict thread) ___DISABLE_IF_NULL(thread);
[[nodiscard]] [[not_null(1)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_thread_detach(StdThread* restrict thread) ___DISABLE_IF_NULL(thread);
// NOLINTNEXTLINE(readability-non-const-parameter)
[[not_null(1)]] void std_thread_free(void* thread) ___DISABLE_IF_NULL(thread);

#define StdJThread scoped(std_thread_free)

void std_this_thread_yield(void);
void std_this_thread_sleep_for(StdDuration duration);
[[nodiscard]] StdThreadID std_this_thread_get_id(void);

#undef ___DISABLE_IF_NULL

#define ___DISABLE_IF_NULL(tls) \
	std_disable_if(!(tls), "Can't perform a thread-local storage operation on a nullptr")

[[nodiscard]] [[not_null(1)]] StdResult(StdTLSKey)
	std_tls_new(void* data, void(__STD_TLS_DESTRUCTOR_TAG* destructor)(void*))
		___DISABLE_IF_NULL(data);
[[nodiscard]] [[not_null(1, 2)]] StdResult
// NOLINTNEXTLINE(readability-non-const-parameter)
std_tls_init(StdTLSKey* restrict key, void* data, void(__STD_TLS_DESTRUCTOR_TAG* destructor)(void*))
	___DISABLE_IF_NULL(key) ___DISABLE_IF_NULL(data);
[[nodiscard]] [[returns_not_null]] void* std_tls_get(StdTLSKey key);
[[nodiscard]] [[not_null(2)]] StdResult
std_tls_set(StdTLSKey key, void* data) ___DISABLE_IF_NULL(data);

#undef ___DISABLE_IF_NULL

#endif // STD_THREAD
