/// @file Condvar.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxCondvar` provides a higher-level condition variable type similar to that provided in
/// C++'s `<condition_variable>`
/// @version 0.2.0
/// @date 2022-05-08
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

/// @ingroup cnx_sync Synchronization
/// @{
/// @defgroup cnx_condvar CnxCondvar
/// `CnxCondvar` provides a higher-level condition variable type, directly comparable to C++'s
/// `std::condition_variable`, that helps to make writing efficient multi-threaded code simpler and
/// easier
///
/// Example:
/// @code {.c}
/// #include <Cnx/sync/Mutex.h>
/// #include <Cnx/sync/UniqueLock.h>
/// #include <Cnx/sync/Condvar.h>
/// bool flag;
/// CnxCondvar condvar;
/// CnxMutex mutex;
///
/// // thread1:
/// UniqueLock lock = cnx_unique_lock(mutex);
/// while(!flag) {
/// 	cnx_condvar_wait(&condvar, &lock);
/// }
/// // do something now that we've received the flag
///
/// // thread2:
/// // do some stuff
/// {
/// 	UniqueLock lock = cnx_unique_lock(muted);
/// 	flag = true;
/// 	// signal to thread1 that we're done and it can do its work now
/// 	cnx_condvar_notify_one(&condvar);
/// }
/// @endcode
/// @}
#include <Cnx/__thread/__thread.h>

#if !___CNX_HAS_NO_THREADS

	#ifndef CNX_CONDVAR
		#define CNX_CONDVAR

		#include <Cnx/Def.h>
		#include <Cnx/Thread.h>
		#include <Cnx/time/TimePoint.h>

/// @brief `CnxCondvar` provides a higher-level condition variable type, directly comparable to
/// C++'s `std::condition_variable`, that helps to make writing efficient multi-threaded code
/// simpler and easier.
///
/// Example:
/// @code {.c}
/// #include <Cnx/sync/Mutex.h>
/// #include <Cnx/sync/UniqueLock.h>
/// #include <Cnx/sync/Condvar.h>
/// bool flag;
/// CnxCondvar condvar;
/// CnxMutex mutex;
///
/// // thread1:
/// UniqueLock lock = cnx_unique_lock(mutex);
/// while(!flag) {
/// 	cnx_condvar_wait(&condvar, &lock);
/// }
/// // do something now that we've received the flag
///
/// // thread2:
/// // do some stuff
/// {
/// 	UniqueLock lock = cnx_unique_lock(muted);
/// 	flag = true;
/// 	// signal to thread1 that we're done and it can do its work now
/// 	cnx_condvar_notify_one(&condvar);
/// }
/// @endcode
/// @ingroup cnx_condvar
typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxBasicCondvar __condvar;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxCondvar;

/// @brief Used to communicate the timeout status for `cnx_condvar_wait_for` and
/// `cnx_condvar_wait_until`. If a timeout occurs during either of these functions, then
/// `CnxCondvarTimeout` will be returned. Otherwise `CnxCondvarTimeout` will be returned.
/// @ingroup cnx_condvar
typedef enum {
	CnxCondvarNoTimeout = 0,
	CnxCondvarTimeout
} CnxCondvarStatus;

struct CnxUniqueLock;

		#define __DISABLE_IF_NULL(val) \
			cnx_disable_if(!(val), "Can't perform a CnxCondvar operation with a nullptr")

/// @brief Creates a new `CnxCondvar`
/// @return a new `CnxCondvar`
/// @ingroup cnx_condvar
__attr(nodiscard) CnxCondvar cnx_condvar_new(void);
/// @brief Immediately unlocks `lock` and waits on the given `CnxCondvar`, `condvar` until it has
/// been notified by another thread. `lock` will be automatically relocked before this function
/// returns.
///
/// @param condvar - The condvar to wait on
/// @param lock - The exclusive lock associated with the condition
/// @ingroup cnx_condvar
__attr(not_null(1, 2)) void cnx_condvar_wait(CnxCondvar* restrict condvar,
											 struct CnxUniqueLock* restrict lock)
	__DISABLE_IF_NULL(condvar) __DISABLE_IF_NULL(lock);
/// @brief Immediately unlocks `lock` and waits on the given `CnxCondvar`, `condvar` until it has
/// been notified by another thread, or the amount of time specified by `duration` has passed.
/// `lock` will be automatically relocked before this function returns.
///
/// @param condvar - The condvar to wait on
/// @param lock - The exclusive lock associated with the condition
/// @param duration - The maximum amount of time to wait on the condition
///
/// @return The `CnxCondvarStatus` indicating whether the thread was notified
/// (`CnxCondvarNoTimeout`), or, a timeout occurred and `duration` passed before a notification was
/// received (`CnxCondvarTimeout`).
/// @ingroup cnx_condvar
__attr(nodiscard) __attr(not_null(1, 2)) CnxCondvarStatus
	cnx_condvar_wait_for(CnxCondvar* restrict condvar,
						 struct CnxUniqueLock* restrict lock,
						 CnxDuration duration) __DISABLE_IF_NULL(condvar) __DISABLE_IF_NULL(lock);
/// @brief Immediately unlocks `lock` and waits on the given `CnxCondvar`, `condvar` until it has
/// been notified by another thread, or point in time specified by `stop_point` has occurred.
/// `lock` will be automatically relocked before this function returns.
///
/// @param condvar - The condvar to wait on
/// @param lock - The exclusive lock associated with the condition
/// @param stop_point - The point in time at which to stop waiting on the condition
///
/// @return The `CnxCondvarStatus` indicating whether the thread was notified
/// (`CnxCondvarNoTimeout`), or, a timeout occurred and `stop_point` occurred before a notification
/// was received (`CnxCondvarTimeout`).
/// @ingroup cnx_condvar
__attr(nodiscard) __attr(not_null(1, 2)) CnxCondvarStatus
	cnx_condvar_wait_until(CnxCondvar* restrict condvar,
						   struct CnxUniqueLock* restrict lock,
						   CnxTimePoint stop_point) __DISABLE_IF_NULL(condvar)
		__DISABLE_IF_NULL(lock);
/// @brief Notifies exactly __one__ thread waiting on the given `CnxCondvar` to stop waiting and
/// continue execution
///
/// @param condvar - The condvar to notify a waiting thread to continue
/// @ingroup cnx_condvar
__attr(not_null(1)) void cnx_condvar_notify_one(CnxCondvar* restrict condvar)
	__DISABLE_IF_NULL(condvar);
/// @brief Notifies all threads waiting on the given `CnxCondvar` to stop waiting and
/// continue execution
///
/// @param condvar - The condvar to notify waiting threads to continue
/// @ingroup cnx_condvar
__attr(not_null(1)) void cnx_condvar_notify_all(CnxCondvar* restrict condvar)
	__DISABLE_IF_NULL(condvar);
/// @brief Destroys the given `CnxCondvar`, freeing any associated resources
///
/// @param condvar - The condvar to destroy
/// @ingroup cnx_condvar
__attr(not_null(1)) void cnx_condvar_free(CnxCondvar* restrict condvar) __DISABLE_IF_NULL(condvar);

		#undef __DISABLE_IF_NULL

	#endif // CNX_CONDVAR

#endif //!___CNX_HAS_NO_THREADS
