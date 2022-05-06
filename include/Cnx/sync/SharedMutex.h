/// @file SharedMutex.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxSharedMutex` provides several higher-level reader-writer mutex types similar to those
/// provided in C++'s `<shared_mutex>`
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
/// @defgroup cnx_shared_mutex CnxSharedMutex
/// `CnxSharedMutex` provides several higher-level reader-writer mutex types comparable to those
/// provided in C++'s `<shared_mutex>`
///@}

#ifndef CNX_SHARED_MUTEX
#define CNX_SHARED_MUTEX

#include <Cnx/Def.h>
#include <Cnx/Thread.h>
#include <Cnx/sync/Mutex.h>

typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxMutex __mutex;
	CnxCondvar __exclusive_guard;
	CnxCondvar __shared_guard;
	u8 __state;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxSharedMutex;

typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxSharedMutex __mutex;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxSharedTimedMutex;

// clang-format off

Trait(
	/// @brief `CnxSharedMutexInterface` is a uniform interface and Trait implementation that all
	/// higher-level reader-writer mutexes (ie shared mutexes) meeting Cnx's requirements provide.
	///
	/// @note While all Cnx shared mutexes provide this interface, only `lock`, `try_lock`, 
	/// `unlock`, `lock_shared`, `try_lock_shared`, and `unlock_shared` are mandatory.
	/// `try_lock_for`, `try_lock_until`, `try_lock_shared_for`, and `try_lock_shared_until` may not
	/// be provided if they are not supported by a particular mutex type, in which case they will be
	/// initialized to `nullptr`
	/// @ingroup cnx_shared_mutex
	CnxSharedMutexInterface,
	__attr(not_null(1)) void (*const lock)(CnxSharedMutexInterface* restrict mutex);
	__attr(nodiscard) __attr(not_null(1)) bool (*const try_lock)(
													CnxSharedMutexInterface* restrict mutex);
	__attr(nodiscard)
	__attr(not_null(1)) bool (*const try_lock_for)(CnxSharedMutexInterface* restrict mutex,
											 	   CnxDuration duration);
	__attr(nodiscard)
	__attr(not_null(1)) bool (*const try_lock_until)(CnxSharedMutexInterface* restrict mutex,
													 CnxTimePoint stop_point);
	__attr(not_null(1)) void (*const unlock)(CnxSharedMutexInterface* restrict mutex);
	__attr(not_null(1)) void (*const lock_shared)(CnxSharedMutexInterface* restrict mutex);
	__attr(nodiscard) __attr(not_null(1)) bool (*const try_lock_shared)(
													CnxSharedMutexInterface* restrict mutex);
	__attr(nodiscard)
	__attr(not_null(1)) bool (*const try_lock_shared_for)(CnxSharedMutexInterface* restrict mutex,
											 	   		  CnxDuration duration);
	__attr(nodiscard)
	__attr(not_null(1)) bool (*const try_lock_shared_until)(CnxSharedMutexInterface* restrict mutex,
													 		CnxTimePoint stop_point);
	__attr(not_null(1)) void (*const unlock_shared)(CnxSharedMutexInterface* restrict mutex);
);

// clang-format on

#define __DISABLE_IF_NULL(mutex) \
	cnx_disable_if(!(mutex), "Can't perform a mutex operation on a nullptr")

/// @brief Creates a new shared mutex
///
/// @return A `CnxSharedMutex`
/// @ingroup cnx_shared_mutex
__attr(nodiscard) CnxSharedMutex cnx_shared_mutex_new(void);
/// @brief Unconditionally locks the mutex pointed to by `mutex` exclusively (aka for writing)
///
/// @param mutex - The mutex to lock exclusively (aka for writing)
/// @ingroup cnx_shared_mutex
__attr(not_null(1)) void cnx_shared_mutex_lock(CnxSharedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex` exclusively (aka for writing)
///
/// If locking the mutex is successful, return `true`.
///
/// @param mutex - The mutex to lock exclusively (aka for writing)
///
/// @return `true` if successful
/// @ingroup cnx_shared_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_shared_mutex_try_lock(CnxSharedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unconditionally locks the mutex pointed to by `mutex` non-exclusively
/// (shared, aka for reading)
///
/// @param mutex - The mutex to lock non-exclusively (aka shared, for reading)
/// @ingroup cnx_shared_mutex
__attr(not_null(1)) void cnx_shared_mutex_lock_shared(CnxSharedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex` non-exclusively
/// (shared, aka for reading)
///
/// If locking the mutex is successful, return `true`.
///
/// @param mutex - The mutex to lock non-exclusively (aka shared, for reading)
///
/// @return `true` if successful
/// @ingroup cnx_shared_mutex
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_shared_mutex_try_lock_shared(CnxSharedMutex* restrict mutex)
		__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unlocks the exclusively locked mutex pointed to by `mutex`
///
/// @param mutex - The mutex to unlock exclusively
/// @ingroup cnx_shared_mutex
__attr(not_null(1)) void cnx_shared_mutex_unlock(CnxSharedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unlocks the non-exclusively (aka shared) locked mutex pointed to by `mutex`
///
/// @param mutex - The mutex to unlock non-exclusively
/// @ingroup cnx_shared_mutex
__attr(not_null(1)) void cnx_shared_mutex_unlock_shared(CnxSharedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Destroys the mutex pointed to by `mutex`
///
/// @param mutex - The mutex to free
///
/// @ingroup cnx_shared_mutex
__attr(not_null(1)) void cnx_shared_mutex_free(CnxSharedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT

/// @brief Creates a new shared timed mutex
///
/// @return A `CnxSharedTimedMutex`
/// @ingroup cnx_shared_mutex
__attr(nodiscard) CnxSharedTimedMutex cnx_shared_timed_mutex_new(void);
/// @brief Unconditionally locks the mutex pointed to by `mutex` exclusively (aka for writing)
///
/// @param mutex - The mutex to lock exclusively (aka for writing)
/// @ingroup cnx_shared_mutex
__attr(not_null(1)) void cnx_shared_timed_mutex_lock(CnxSharedTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex` exclusively (aka for writing)
///
/// If locking the mutex is successful, return `true`.
///
/// @param mutex - The mutex to lock exclusively (aka for writing)
///
/// @return `true` if successful
/// @ingroup cnx_shared_mutex
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_shared_timed_mutex_try_lock(CnxSharedTimedMutex* restrict mutex)
		__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex` exclusively (aka for writing)
///
/// Tries to lock the `mutex` until the amount of time specified by `duration` has passed. If
/// locking is successful before `duration` has elapsed, returns `true`. Otherwise stops trying to
/// acquire the lock and returns `false`.
///
/// @param mutex - The mutex to lock
/// @param duration - The amount to time spend trying to lock the mutex
///
/// @return `true` if successful
/// @ingroup cnx_shared_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_shared_timed_mutex_try_lock_for(
	CnxSharedTimedMutex* restrict mutex,
	CnxDuration duration) __DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex` exclusively (aka for writing)
///
/// Tries to lock the `mutex` until the the point in time specified by `stop_point`. If
/// locking is successful before `stop_point` has occurred, returns `true`. Otherwise stops trying
/// to acquire the lock and returns `false`.
///
/// @param mutex - The mutex to lock
/// @param stop_point - The point in time at which to quit trying to lock the mutex
///
/// @return `true` if successful
/// @ingroup cnx_shared_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_shared_timed_mutex_try_lock_until(
	CnxSharedTimedMutex* restrict mutex,
	CnxTimePoint stop_point) __DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unconditionally locks the mutex pointed to by `mutex` non-exclusively
/// (shared, aka for reading)
///
/// @param mutex - The mutex to lock non-exclusively (aka shared, for reading)
/// @ingroup cnx_shared_mutex
__attr(not_null(1)) void cnx_shared_timed_mutex_lock_shared(CnxSharedTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex` non-exclusively
/// (shared, aka for reading)
///
/// If locking the mutex is successful, return `true`.
///
/// @param mutex - The mutex to lock non-exclusively (aka shared, for reading)
///
/// @return `true` if successful
/// @ingroup cnx_shared_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_shared_timed_mutex_try_lock_shared(
	CnxSharedTimedMutex* restrict mutex) __DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex` non-exclusively
/// (aka shared, for reading)
///
/// Tries to lock the `mutex` until the amount of time specified by `duration` has passed. If
/// locking is successful before `duration` has elapsed, returns `true`. Otherwise stops trying to
/// acquire the lock and returns `false`.
///
/// @param mutex - The mutex to lock
/// @param duration - The amount to time spend trying to lock the mutex
///
/// @return `true` if successful
/// @ingroup cnx_shared_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_shared_timed_mutex_try_lock_shared_for(
	CnxSharedTimedMutex* restrict mutex,
	CnxDuration duration) __DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex` non-exclusively
/// (aka shared, for reading)
///
/// Tries to lock the `mutex` until the the point in time specified by `stop_point`. If
/// locking is successful before `stop_point` has occurred, returns `true`. Otherwise stops trying
/// to acquire the lock and returns `false`.
///
/// @param mutex - The mutex to lock
/// @param stop_point - The point in time at which to quit trying to lock the mutex
///
/// @return `true` if successful
/// @ingroup cnx_shared_mutex
__attr(nodiscard) __attr(not_null(1)) bool cnx_shared_timed_mutex_try_lock_shared_until(
	CnxSharedTimedMutex* restrict mutex,
	CnxTimePoint stop_point) __DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unlocks the exclusively locked mutex pointed to by `mutex`
///
/// @param mutex - The mutex to unlock exclusively
/// @ingroup cnx_shared_mutex
__attr(not_null(1)) void cnx_shared_timed_mutex_unlock(CnxSharedTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unlocks the non-exclusively (aka shared) locked mutex pointed to by `mutex`
///
/// @param mutex - The mutex to unlock non-exclusively
/// @ingroup cnx_shared_mutex
__attr(not_null(1)) void cnx_shared_timed_mutex_unlock_shared(CnxSharedTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Destroys the mutex pointed to by `mutex`
///
/// @param mutex - The mutex to free
///
/// @ingroup cnx_shared_mutex
__attr(not_null(1)) void cnx_shared_timed_mutex_free(CnxSharedTimedMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT

IGNORE_RESERVED_IDENTIFIER_WARNING_START

__attr(not_null(1)) void __cnx_shared_mutex_lock_imutex(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(nodiscard)
	__attr(not_null(1)) bool __cnx_shared_mutex_try_lock_imutex(CnxMutexInterface* restrict mutex)
		__DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_shared_mutex_unlock_imutex(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);

__attr(not_null(1)) void __cnx_shared_timed_mutex_lock_imutex(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_timed_mutex_try_lock_imutex(
	CnxMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_timed_mutex_try_lock_for_imutex(
	CnxMutexInterface* restrict mutex,
	CnxDuration duration) __DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_timed_mutex_try_lock_until_imutex(
	CnxMutexInterface* restrict mutex,
	CnxTimePoint stop_point) __DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_shared_timed_mutex_unlock_imutex(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);

__attr(maybe_unused) static ImplTraitFor(CnxMutexInterface,
										 CnxSharedMutex,
										 .lock = __cnx_shared_mutex_lock_imutex,
										 .try_lock = __cnx_shared_mutex_try_lock_imutex,
										 .try_lock_for = nullptr,
										 .try_lock_until = nullptr,
										 .unlock = __cnx_shared_mutex_unlock_imutex);
__attr(maybe_unused) static ImplTraitFor(CnxMutexInterface,
										 CnxSharedTimedMutex,
										 .lock = __cnx_shared_timed_mutex_lock_imutex,
										 .try_lock = __cnx_shared_timed_mutex_try_lock_imutex,
										 .try_lock_for
										 = __cnx_shared_timed_mutex_try_lock_for_imutex,
										 .try_lock_until
										 = __cnx_shared_timed_mutex_try_lock_until_imutex,
										 .unlock = __cnx_shared_timed_mutex_unlock_imutex);

__attr(not_null(1)) void __cnx_shared_mutex_lock_ishared(CnxSharedMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_mutex_try_lock_ishared(
	CnxSharedMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_shared_mutex_unlock_ishared(CnxSharedMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_shared_mutex_lock_shared_ishared(
	CnxSharedMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_mutex_try_lock_shared_ishared(
	CnxSharedMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_shared_mutex_unlock_shared_ishared(
	CnxSharedMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);

__attr(not_null(1)) void __cnx_shared_timed_mutex_lock_ishared(
	CnxSharedMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_timed_mutex_try_lock_ishared(
	CnxSharedMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_timed_mutex_try_lock_for_ishared(
	CnxSharedMutexInterface* restrict mutex,
	CnxDuration duration) __DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_timed_mutex_try_lock_until_ishared(
	CnxSharedMutexInterface* restrict mutex,
	CnxTimePoint stop_point) __DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_shared_timed_mutex_unlock_ishared(
	CnxSharedMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_shared_timed_mutex_lock_shared_ishared(
	CnxSharedMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_timed_mutex_try_lock_shared_ishared(
	CnxSharedMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_timed_mutex_try_lock_shared_for_ishared(
	CnxSharedMutexInterface* restrict mutex,
	CnxDuration duration) __DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) bool __cnx_shared_timed_mutex_try_lock_shared_until_ishared(
	CnxSharedMutexInterface* restrict mutex,
	CnxTimePoint stop_point) __DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_shared_timed_mutex_unlock_shared_ishared(
	CnxSharedMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);

__attr(maybe_unused) static ImplTraitFor(CnxSharedMutexInterface,
										 CnxSharedMutex,
										 .lock = __cnx_shared_mutex_lock_ishared,
										 .try_lock = __cnx_shared_mutex_try_lock_ishared,
										 .try_lock_for = nullptr,
										 .try_lock_until = nullptr,
										 .unlock = __cnx_shared_mutex_unlock_ishared,
										 .lock_shared = __cnx_shared_mutex_lock_shared_ishared,
										 .try_lock_shared
										 = __cnx_shared_mutex_try_lock_shared_ishared,
										 .try_lock_shared_for = nullptr,
										 .try_lock_shared_until = nullptr,
										 .unlock_shared = __cnx_shared_mutex_unlock_shared_ishared);
__attr(maybe_unused) static ImplTraitFor(
	CnxSharedMutexInterface,
	CnxSharedTimedMutex,
	.lock = __cnx_shared_timed_mutex_lock_ishared,
	.try_lock = __cnx_shared_timed_mutex_try_lock_ishared,
	.try_lock_for = __cnx_shared_timed_mutex_try_lock_for_ishared,
	.try_lock_until = __cnx_shared_timed_mutex_try_lock_until_ishared,
	.unlock = __cnx_shared_timed_mutex_unlock_ishared,
	.lock_shared = __cnx_shared_timed_mutex_lock_shared_ishared,
	.try_lock_shared = __cnx_shared_timed_mutex_try_lock_shared_ishared,
	.try_lock_shared_for = __cnx_shared_timed_mutex_try_lock_shared_for_ishared,
	.try_lock_shared_until = __cnx_shared_timed_mutex_try_lock_shared_until_ishared,
	.unlock_shared = __cnx_shared_timed_mutex_unlock_shared_ishared);
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#undef __DISABLE_IF_NULL
#endif // CNX_SHARED_MUTEX
