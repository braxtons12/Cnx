/// @file __std_thread.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Type definitions for threading functionality
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

#ifndef __STD_THREAD
#define __STD_THREAD

#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdDef.h>
#include <C2nxt/StdPlatform.h>

IGNORE_RESERVED_IDENTIFIER_WARNING_START

#if defined(__has_include)

	#if __has_include(<threads.h>)
		#define ___STD_HAS_C11_THREADS TRUE
	#else
		#define ___STD_HAS____STD_HAS_C11_THREADS FALSE
	#endif // __has_include(<threads.h>)

	#if __has_include(<pthread.h>)
		#define ___STD_HAS_PTHREADS TRUE
	#else
		#define ___STD_HAS_PTHREADS FALSE
	#endif // __has_include(<pthread.h>)

	#if !___STD_HAS_C11_THREADS && !___STD_HAS_PTHREADS && !STD_PLATFORM_WINDOWS
		#define ___STD_HAS_NO_THREADS TRUE
	#elif STD_PLATFORM_WINDOWS
		#define ___STD_HAS_NO_THREADS FALSE
	#endif // !___STD_HAS_C11_THREADS && !___STD_HAS_PTHREADS && !STD_PLATFORM_WINDOWS

#elif STD_PLATFORM_LINUX || STD_PLATFORM_BSD || STD_PLATFORM_ANDROID

	#define ___STD_HAS_C11_THREADS TRUE
	#define ___STD_HAS_PTHREADS	   TRUE

	#define ___STD_HAS_NO_THREADS FALSE

#elif STD_PLATFORM_WINDOWS

	#define ___STD_HAS_C11_THREADS FALSE
	#define ___STD_HAS_PTHREADS	   FALSE
	#define ___STD_HAS_NO_THREADS  FALSE

#else

	#define ___STD_HAS_NO_THREADS TRUE

#endif // defined(__has_include)

#if ___STD_HAS_NO_THREADS
	#error Threads not supported on the target platform
#endif

#if ___STD_HAS_C11_THREADS

	#include <threads.h>

typedef mtx_t __std_basic_mutex;

typedef mtx_t __std_recursive_basic_mutex;

	#define __STD_MUTEX_INITIALIZER ((__std_basic_mutex){0})

typedef cnd_t __std_condvar;

	#define __STD_CONDVAR_INITIALIZER ((__std_condvar_){0})

typedef once_flag __std_exec_once_flag;

	#define __STD_EXEC_ONCE_INITIALIZER ((__std_exe__std_exec_once_flag)ONCE_FLAG_INIT)

typedef thrd_t __std_thread_id;

typedef thrd_t __std_thread;

	#define __STD_NULL_THREAD static_cast(__std_thread_id)(0)

typedef tss_t __std_tls_key;

	#define __STD_TLS_DESTRUCTOR_TAG

#elif ___STD_HAS_PTHREADS

	#include <pthread.h>

typedef pthread_mutex_t __std_basic_mutex;

typedef pthread_mutex_t __std_recursive_basic_mutex;

	#define __STD_MUTEX_INITIALIZER ((__std_basic_mutex)PTHREAD_MUTEX_INITIALIZER)

typedef pthread_cond_t __std_condvar;

	#define __STD_CONDVAR_INITIALIZER ((__std_condvar)PTHREAD_COND_INITIALIZER)

typedef pthread_once_t __std_exec_once_flag;

	#define __STD_EXEC_ONCE_INITIALIZER PTHREAD_ONCE_INIT

typedef pthread_t __std_thread_id;

typedef pthread_t __std_thread;

	#define __STD_NULL_THREAD ((__std_thread){0})

typedef pthread_key_t __std_tls_key;

	#define __STD_TLS_DESTRUCTOR_TAG

#elif STD_PLATFORM_WINDOWS

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

typedef SRWLOCK __std_basic_mutex;

// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
typedef CRITICAL_SECTION __std_recursive_basic_mutex;

	#define __STD_MUTEX_INITIALIZER ((__std_basic_mutex) nullptr)

typedef CONDITION_VARIABLE __std_condvar;

	#define __STD_CONDVAR_INITIALIZER ((__std_condvar) nullptr)

typedef INIT_ONCE __std_exec_once_flag;

	#define __STD_EXEC_ONCE_INITIALIZER ((__std_exec_once_flag) nullptr)

typedef DWORD __std_thread_id;

typedef HANDLE __std_thread;

	#define __STD_NULL_THREAD static_cast(__std_thread_id)(0)

typedef DWORD __std_tls_key;

	#define __STD_TLS_DESTRUCTOR_TAG __stdcall

#endif // ___STD_HAS_C11_THREADS elif ___STD_HAS_PTHREADS elif ___STD_PLATFORM_WINDOWS

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#endif // __STD_THREAD
