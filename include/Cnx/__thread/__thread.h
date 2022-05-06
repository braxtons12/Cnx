/// @file __thread.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Type definitions for threading functionality
/// @version 0.2.1
/// @date 2022-05-05
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

#ifndef __CNX_THREAD
#define __CNX_THREAD

#include <Cnx/BasicTypes.h>
#include <Cnx/Def.h>
#include <Cnx/Platform.h>

IGNORE_RESERVED_IDENTIFIER_WARNING_START

#if defined(__has_include) && !CNX_PLATFORM_WINDOWS

	#if __has_include(<threads.h>)
		#define ___CNX_HAS_C11_THREADS TRUE
	#else
		#define ___CNX_HAS_C11_THREADS FALSE
	#endif // __has_include(<threads.h>)

	#if __has_include(<pthread.h>)
		#define ___CNX_HAS_PTHREADS TRUE
	#else
		#define ___CNX_HAS_PTHREADS FALSE
	#endif // __has_include(<pthread.h>)

	#if !___CNX_HAS_C11_THREADS && !___CNX_HAS_PTHREADS
		#define ___CNX_HAS_NO_THREADS TRUE
	#endif // !___CNX_HAS_C11_THREADS && !___CNX_HAS_PTHREADS

#elif CNX_PLATFORM_LINUX || CNX_PLATFORM_BSD || CNX_PLATFORM_ANDROID

	#define ___CNX_HAS_C11_THREADS TRUE
	#define ___CNX_HAS_PTHREADS	   TRUE

	#define ___CNX_HAS_NO_THREADS FALSE

#elif CNX_PLATFORM_WINDOWS

	#define ___CNX_HAS_C11_THREADS FALSE
	#define ___CNX_HAS_PTHREADS	   FALSE
	#define ___CNX_HAS_NO_THREADS  FALSE

#else

	#define ___CNX_HAS_NO_THREADS TRUE

#endif // defined(__has_include) && !CNX_PLATFORM_WINDOWS

#if ___CNX_HAS_C11_THREADS

	#include <threads.h>

typedef mtx_t __cnx_basic_mutex;

typedef mtx_t __cnx_recursive_basic_mutex;

	#define __CNX_MUTEX_INITIALIZER ((__cnx_basic_mutex){0})

typedef cnd_t __cnx_condvar;

	#define __CNX_CONDVAR_INITIALIZER ((__cnx_condvar_){0})

typedef once_flag __cnx_exec_once_flag;

	#define __CNX_EXEC_ONCE_INITIALIZER ((__cnx_exe_once_flag)ONCE_FLAG_INIT)

typedef thrd_t __cnx_thread_id;

typedef thrd_t __cnx_thread;

	#define __CNX_NULL_THREAD static_cast(__cnx_thread_id)(0)

typedef tss_t __cnx_tls_key;

	#define __CNX_TLS_DESTRUCTOR_TAG

#elif ___CNX_HAS_PTHREADS

	#include <pthread.h>

typedef pthread_mutex_t __cnx_basic_mutex;

typedef pthread_mutex_t __cnx_recursive_basic_mutex;

	#define __CNX_MUTEX_INITIALIZER ((__cnx_basic_mutex)PTHREAD_MUTEX_INITIALIZER)

typedef pthread_cond_t __cnx_condvar;

	#define __CNX_CONDVAR_INITIALIZER ((__cnx_condvar)PTHREAD_COND_INITIALIZER)

typedef pthread_once_t __cnx_exec_once_flag;

	#define __CNX_EXEC_ONCE_INITIALIZER PTHREAD_ONCE_INIT

typedef pthread_t __cnx_thread_id;

typedef pthread_t __cnx_thread;

	#define __CNX_NULL_THREAD ((__cnx_thread){0})

typedef pthread_key_t __cnx_tls_key;

	#define __CNX_TLS_DESTRUCTOR_TAG

#elif CNX_PLATFORM_WINDOWS

	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>

typedef SRWLOCK __cnx_basic_mutex;

// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
typedef CRITICAL_SECTION __cnx_recursive_basic_mutex;

	#define __CNX_MUTEX_INITIALIZER ((__cnx_basic_mutex) nullptr)

typedef CONDITION_VARIABLE __cnx_condvar;

	#define __CNX_CONDVAR_INITIALIZER ((__cnx_condvar) nullptr)

typedef INIT_ONCE __cnx_exec_once_flag;

	#define __CNX_EXEC_ONCE_INITIALIZER ((__cnx_exec_once_flag) nullptr)

typedef DWORD __cnx_thread_id;

typedef HANDLE __cnx_thread;

	#define __CNX_NULL_THREAD static_cast(__cnx_thread_id)(0)

typedef DWORD __cnx_tls_key;

	#define __CNX_TLS_DESTRUCTOR_TAG __stdcall

#endif // ___CNX_HAS_C11_THREADS elif ___CNX_HAS_PTHREADS elif ___CNX_PLATFORM_WINDOWS

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#endif // __CNX_THREAD
