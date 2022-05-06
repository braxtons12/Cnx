/// @file Mutex.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxMutex` provides several higher-level mutex types similar to those provided in C++'s
/// `<mutex>`
/// @version 0.2.0
/// @date 2022-05-06
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
/// @defgroup cnx_mutex CnxMutex
/// `CnxMutex` provides several high-level mutex types similar to those provided in C++'s `<mutex>`
///@}

#ifndef CNX_MUTEX
#define CNX_MUTEX

#include <Cnx/Def.h>
#include <Cnx/Thread.h>
#include <Cnx/sync/Condvar.h>
#include <Cnx/time/TimePoint.h>

typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxBasicMutex __mutex;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxMutex;

typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxRecursiveBasicMutex __mutex;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxRecursiveMutex;

typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxMutex __mutex;
	CnxCondvar __timed_guard;
	bool __locked;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxTimedMutex;

typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxMutex __mutex;
	CnxCondvar __timed_guard;
	usize __times_locked;
	CnxThreadID __locked_thread_id;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxRecursiveTimedMutex;

typedef struct {
} cnx_defer_lock_t;

static let cnx_defer_lock = (cnx_defer_lock_t){};

typedef struct {
} cnx_adopt_lock_t;

static let cnx_adopt_lock = (cnx_adopt_lock_t){};

typedef struct {
} cnx_try_lock_t;

static let cnx_try_lock = (cnx_try_lock_t){};

// clang-format off

Trait(
	/// @brief `CnxMutexInterface` is a uniform interface and Trait implementation that all
	/// higher-level mutexes (ie not the basic primitives provided in `<Cnx/Thread.h>) meeting
	/// Cnx's requirements provide.
	///
	/// @note While all Cnx mutexes provide this interface, only `lock`, `try_lock`, and
	/// `unlock` are mandatory. `try_lock_for` and `try_lock_until` may not be provided if they
	/// are not supported by a particular mutex type, in which case they will be initialized to
	/// `nullptr`
	/// @ingroup cnx_mutex
	CnxMutexInterface,
	__attr(not_null(1)) void (*const lock)(CnxMutexInterface* restrict mutex);
	__attr(nodiscard) __attr(not_null(1)) bool (*const try_lock)(CnxMutexInterface* restrict mutex);
	__attr(nodiscard)
	__attr(not_null(1)) bool (*const try_lock_for)(CnxMutexInterface* restrict mutex,
											 	   CnxDuration duration);
	__attr(nodiscard)
	__attr(not_null(1)) bool (*const try_lock_until)(CnxMutexInterface* restrict mutex,
													 CnxTimePoint stop_point);
	__attr(not_null(1)) void (*const unlock)(CnxMutexInterface* restrict mutex););

// clang-format on

#define __DISABLE_IF_NULL(mutex) \
	cnx_disable_if(!(mutex), "Can't perform a mutex operation on a nullptr")

/// @brief Creates a new mutex
///
/// @return A `CnxMutex`
/// @ingroup cnx_mutex
__attr(nodiscard) CnxMutex cnx_mutex_new(void);
/// @brief Unconditionally locks the mutex pointed to by `mutex`
///
/// @param mutex - The mutex to lock
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_mutex_lock(CnxMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex`
///
/// If locking the mutex is successful, return `true`.
///
/// @param mutex - The mutex to lock
///
/// @return `true` if successful
/// @ingroup cnx_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_mutex_try_lock(CnxMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unlocks the locked mutex pointed to by `mutex`
///
/// @param mutex - The mutex to unlock
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_mutex_unlock(CnxMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Destroys the mutex pointed to by `mutex`
///
/// @param mutex - The mutex to free
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_mutex_free(CnxMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT

/// @brief Creates a new recursive mutex
///
/// @return A `CnxRecursiveMutex`
/// @ingroup cnx_mutex
__attr(nodiscard) CnxRecursiveMutex cnx_recursive_mutex_new(void);
/// @brief Unconditionally locks the mutex pointed to by `mutex`
///
/// @param mutex - The mutex to lock
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_recursive_mutex_lock(CnxRecursiveMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex`
///
/// If locking the mutex is successful, return `true`.
///
/// @param mutex - The mutex to lock
///
/// @return `true` if successful
/// @ingroup cnx_mutex
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_recursive_mutex_try_lock(CnxRecursiveMutex* restrict mutex)
		__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unlocks the locked mutex pointed to by `mutex`
///
/// @param mutex - The mutex to unlock
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_recursive_mutex_unlock(CnxRecursiveMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Destroys the mutex pointed to by `mutex`
///
/// @param mutex - The mutex to free
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_recursive_mutex_free(CnxRecursiveMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT

/// @brief Creates a new timed mutex
///
/// @return A `CnxTimedMutex`
/// @ingroup cnx_mutex
__attr(nodiscard) CnxTimedMutex cnx_timed_mutex_new(void);
/// @brief Unconditionally locks the mutex pointed to by `mutex`
///
/// @param mutex - The mutex to lock
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_timed_mutex_lock(CnxTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex`
///
/// If locking the mutex is successful, return `true`.
///
/// @param mutex - The mutex to lock
///
/// @return `true` if successful
/// @ingroup cnx_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_timed_mutex_try_lock(CnxTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex`
///
/// Tries to lock the `mutex` until the amount of time specified by `duration` has passed. If
/// locking is successful before `duration` has elapsed, returns `true`. Otherwise stops trying to
/// acquire the lock and returns `false`.
///
/// @param mutex - The mutex to lock
/// @param duration - The amount to time spend trying to lock the mutex
///
/// @return `true` if successful
/// @ingroup cnx_mutex
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_timed_mutex_try_lock_for(CnxTimedMutex* restrict mutex,
														  CnxDuration duration)
		__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex`
///
/// Tries to lock the `mutex` until the the point in time specified by `stop_point`. If
/// locking is successful before `stop_point` has occurred, returns `true`. Otherwise stops trying
/// to acquire the lock and returns `false`.
///
/// @param mutex - The mutex to lock
/// @param stop_point - The point in time at which to quit trying to lock the mutex
///
/// @return `true` if successful
/// @ingroup cnx_mutex
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_timed_mutex_try_lock_until(CnxTimedMutex* restrict mutex,
															CnxTimePoint stop_point)
		__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unlocks the locked mutex pointed to by `mutex`
///
/// @param mutex - The mutex to unlock
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_timed_mutex_unlock(CnxTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Destroys the mutex pointed to by `mutex`
///
/// @param mutex - The mutex to free
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_timed_mutex_free(CnxTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT

/// @brief Creates a new recursive timed mutex
///
/// @return A `CnxRecursiveTimedMutex`
/// @ingroup cnx_mutex
__attr(nodiscard) CnxRecursiveTimedMutex cnx_recursive_timed_mutex_new(void);
/// @brief Unconditionally locks the mutex pointed to by `mutex`
///
/// @param mutex - The mutex to lock
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_recursive_timed_mutex_lock(CnxRecursiveTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex`
///
/// If locking the mutex is successful, return `true`.
///
/// @param mutex - The mutex to lock
///
/// @return `true` if successful
/// @ingroup cnx_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_recursive_timed_mutex_try_lock(
	CnxRecursiveTimedMutex* restrict mutex) __DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex`
///
/// Tries to lock the `mutex` until the amount of time specified by `duration` has passed. If
/// locking is successful before `duration` has elapsed, returns `true`. Otherwise stops trying to
/// acquire the lock and returns `false`.
///
/// @param mutex - The mutex to lock
/// @param duration - The amount to time spend trying to lock the mutex
///
/// @return `true` if successful
/// @ingroup cnx_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_recursive_timed_mutex_try_lock_for(
	CnxRecursiveTimedMutex* restrict mutex,
	CnxDuration duration) __DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex`
///
/// Tries to lock the `mutex` until the the point in time specified by `stop_point`. If
/// locking is successful before `stop_point` has occurred, returns `true`. Otherwise stops trying
/// to acquire the lock and returns `false`.
///
/// @param mutex - The mutex to lock
/// @param stop_point - The point in time at which to quit trying to lock the mutex
///
/// @return `true` if successful
/// @ingroup cnx_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_recursive_timed_mutex_try_lock_until(
	CnxRecursiveTimedMutex* restrict mutex,
	CnxTimePoint stop_point) __DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unlocks the locked mutex pointed to by `mutex`
///
/// @param mutex - The mutex to unlock
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_recursive_timed_mutex_unlock(CnxRecursiveTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Destroys the mutex pointed to by `mutex`
///
/// @param mutex - The mutex to free
///
/// @ingroup cnx_mutex
__attr(not_null(1)) void cnx_recursive_timed_mutex_free(CnxRecursiveTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT

IGNORE_RESERVED_IDENTIFIER_WARNING_START
__attr(not_null(1)) void __cnx_mutex_lock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_mutex_try_lock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_mutex_unlock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);

__attr(not_null(1)) void __cnx_recursive_mutex_lock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(nodiscard)
	__attr(not_null(1)) bool __cnx_recursive_mutex_try_lock(CnxMutexInterface* restrict mutex)
		__DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_recursive_mutex_unlock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);

__attr(not_null(1)) void __cnx_timed_mutex_lock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(nodiscard)
	__attr(not_null(1)) bool __cnx_timed_mutex_try_lock(CnxMutexInterface* restrict mutex)
		__DISABLE_IF_NULL(mutex);
__attr(nodiscard)
	__attr(not_null(1)) bool __cnx_timed_mutex_try_lock_for(CnxMutexInterface* restrict mutex,
															CnxDuration duration)
		__DISABLE_IF_NULL(mutex);
__attr(nodiscard)
	__attr(not_null(1)) bool __cnx_timed_mutex_try_lock_until(CnxMutexInterface* restrict mutex,
															  CnxTimePoint stop_point)
		__DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_timed_mutex_unlock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);

__attr(not_null(1)) void __cnx_recursive_timed_mutex_lock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(nodiscard)
	__attr(not_null(1)) bool __cnx_recursive_timed_mutex_try_lock(CnxMutexInterface* restrict mutex)
		__DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_recursive_timed_mutex_try_lock_for(
	CnxMutexInterface* restrict mutex,
	CnxDuration duration) __DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_recursive_timed_mutex_try_lock_until(
	CnxMutexInterface* restrict mutex,
	CnxTimePoint stop_point) __DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_recursive_timed_mutex_unlock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);

__attr(maybe_unused) static ImplTraitFor(CnxMutexInterface,
										 CnxMutex,
										 __cnx_mutex_lock,
										 __cnx_mutex_try_lock,
										 nullptr,
										 nullptr,
										 __cnx_mutex_unlock);
__attr(maybe_unused) static ImplTraitFor(CnxMutexInterface,
										 CnxRecursiveMutex,
										 __cnx_recursive_mutex_lock,
										 __cnx_recursive_mutex_try_lock,
										 nullptr,
										 nullptr,
										 __cnx_recursive_mutex_unlock);
__attr(maybe_unused) static ImplTraitFor(CnxMutexInterface,
										 CnxTimedMutex,
										 __cnx_timed_mutex_lock,
										 __cnx_timed_mutex_try_lock,
										 __cnx_timed_mutex_try_lock_for,
										 __cnx_timed_mutex_try_lock_until,
										 __cnx_timed_mutex_unlock);
__attr(maybe_unused) static ImplTraitFor(CnxMutexInterface,
										 CnxRecursiveTimedMutex,
										 __cnx_recursive_timed_mutex_lock,
										 __cnx_recursive_timed_mutex_try_lock,
										 __cnx_recursive_timed_mutex_try_lock_for,
										 __cnx_recursive_timed_mutex_try_lock_until,
										 __cnx_recursive_mutex_unlock);

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#undef __DISABLE_IF_NULL

#endif // CNX_MUTEX
