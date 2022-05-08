/// @file Mutex.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxMutex` provides several higher-level mutex types similar to those provided in C++'s
/// `<mutex>`
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
/// @defgroup cnx_mutex CnxMutex
/// `CnxMutex` provides several high-level mutex types similar to those provided in C++'s `<mutex>`,
/// to make working with multi-threaded code simpler and easier.
///
/// Example:
/// @code {.c}
/// // MyThing.h
/// #include <Cnx/sync/Mutex.h>
/// static MyType my_very_important_thing;
/// static CnxMutex* my_thing_mutex;
///
/// void init_my_thing(void);
/// void update_my_thing(u64 value);
/// u64 get_value_from_my_thing(void);
///
/// // MyThing.c
/// #include <Cnx/Allocators.h>
/// #include "MyThing.h"
/// void init_my_thing(void) {
/// 	if(my_thing_mutex == nullptr) {
/// 		my_thing_mutex = cnx_allocator_allocate_t(CnxMutex, DEFAULT_ALLOCATOR);
/// 		*my_thing_mutex = cnx_mutex_new();
///
/// 		cnx_mutex_lock(my_thing_mutex);
/// 		my_very_important_thing = {
/// 		// important intialization
/// 		};
/// 		cnx_mutex_unlock(my_thing_mutex);
/// 	}
/// }
///
/// void update_my_thing(u64 value) {
/// 	cnx_mutex_lock(my_thing_mutex);
/// 	my_very_important_thing.value = value;
/// 	cnx_mutex_unlock(my_thing_mutex);
/// }
///
/// u64 get_value_from_my_thing(void) {
/// 	cnx_mutex_lock(my_thing_mutex);
/// 	let val = my_very_important_thing.value;
/// 	cnx_mutex_unlock(my_thing_mutex);
/// 	return val;
/// }
///
/// /// elsewhere...
/// /// in thread1-related code
///
/// // do some compute intensive task...
/// // update the value
/// update_my_thing(new_value);
///
/// /// in thread2-related code
/// my_val = get_value_from_my_thing();
/// // do something with my_val
///
/// let_mut newval = get_value_from_my_thing();
/// while(newval == my_val) {
/// 	cnx_this_thread_sleep_for(cnx_milliseconds(100));
/// 	newval = get_value_from_my_thing();
/// }
///
/// my_val = newval;
/// // do something with new value
/// @endcode
///@}

#ifndef CNX_MUTEX
#define CNX_MUTEX

#include <Cnx/Def.h>
#include <Cnx/Thread.h>
#include <Cnx/sync/Condvar.h>
#include <Cnx/time/TimePoint.h>

/// @brief `CnxMutex` is the generic higher-level mutex type provided by Cnx. It is a simple mutex
/// type suitable for general purpose use (ie, it is not timed, recursive, etc), and is directly
/// comparable to C++'s `std::mutex`
///
/// Example:
/// @code {.c}
/// // MyThing.h
/// #include <Cnx/sync/Mutex.h>
/// static MyType my_very_important_thing;
/// static CnxMutex* my_thing_mutex;
///
/// void init_my_thing(void);
/// void update_my_thing(u64 value);
/// u64 get_value_from_my_thing(void);
///
/// // MyThing.c
/// #include <Cnx/Allocators.h>
/// #include "MyThing.h"
/// void init_my_thing(void) {
/// 	if(my_thing_mutex == nullptr) {
/// 		my_thing_mutex = cnx_allocator_allocate_t(CnxMutex, DEFAULT_ALLOCATOR);
/// 		*my_thing_mutex = cnx_mutex_new();
///
/// 		cnx_mutex_lock(my_thing_mutex);
/// 		my_very_important_thing = {
/// 		// important intialization
/// 		};
/// 		cnx_mutex_unlock(my_thing_mutex);
/// 	}
/// }
///
/// void update_my_thing(u64 value) {
/// 	cnx_mutex_lock(my_thing_mutex);
/// 	my_very_important_thing.value = value;
/// 	cnx_mutex_unlock(my_thing_mutex);
/// }
///
/// u64 get_value_from_my_thing(void) {
/// 	cnx_mutex_lock(my_thing_mutex);
/// 	let val = my_very_important_thing.value;
/// 	cnx_mutex_unlock(my_thing_mutex);
/// 	return val;
/// }
///
/// /// elsewhere...
/// /// in thread1-related code
///
/// // do some compute intensive task...
/// // update the value
/// update_my_thing(new_value);
///
/// /// in thread2-related code
/// my_val = get_value_from_my_thing();
/// // do something with my_val
///
/// let_mut newval = get_value_from_my_thing();
/// while(newval == my_val) {
/// 	cnx_this_thread_sleep_for(cnx_milliseconds(100));
/// 	newval = get_value_from_my_thing();
/// }
///
/// my_val = newval;
/// // do something with new value
/// @endcode
/// @ingroup cnx_mutex
typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxBasicMutex __mutex;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxMutex;

/// @brief `CnxRecursiveMutex` is a higher-level mutex type, directly comparable to C++'s
/// std::recursive_mutex`, for use when an algorithm requires that a thread be able to lock the same
/// mutex multiple times in its control flow. While such an algorithm would cause the thread to
/// deadlock itself when used with a normal mutex, this is the intended task for a recursive mutex.
/// For example, if a recursive algorithm requires synchronization using the same mutex at multiple
/// levels of recursion, and one level of the algorithm can't release the lock before calling the
/// next, a `CnxRecursiveMutex` would make this type of algorithm possible and prevent the thread
/// from deadlocking itself.
///
/// Example:
/// @code {.c}
/// // MyThing.h
/// #include <Cnx/sync/Mutex.h>
/// static MyType my_very_important_thing;
/// static CnxRecursiveMutex* my_thing_mutex;
///
/// void init_my_thing(void);
/// void update_my_thing(u64 value);
/// u64 get_value_from_my_thing(void);
///
/// // MyThing.c
/// #include <Cnx/Allocators.h>
/// #include "MyThing.h"
/// void init_my_thing(void) {
/// 	if(my_thing_mutex == nullptr) {
/// 		my_thing_mutex = cnx_allocator_allocate_t(CnxRecursiveMutex, DEFAULT_ALLOCATOR);
/// 		*my_thing_mutex = cnx_recursive_mutex_new();
///
/// 		cnx_recursive_mutex_lock(my_thing_mutex);
/// 		my_very_important_thing = {
/// 		// important intialization
/// 		};
/// 		cnx_recursive_mutex_unlock(my_thing_mutex);
/// 	}
/// }
///
/// void update_my_thing(u64 value) {
/// 	cnx_mutex_lock(my_thing_mutex);
/// 	let adjusted_value = static_cast(u64)(4 * value);
/// 	if(adjusted_value < 46) {
/// 		// even though we've already acquired the lock on `my_thing_mutex`, and this will try to
/// 		// acquire it again, that's OK because we're using a recursive mutex
/// 		update_my_thing(adjusted_value);
/// 	}
/// 	else {
/// 		my_very_important_thing.value = adjusted_value;
/// 	}
/// 	cnx_mutex_unlock(my_thing_mutex);
/// }
///
/// u64 get_value_from_my_thing(void) {
/// 	cnx_mutex_lock(my_thing_mutex);
/// 	let val = my_very_important_thing.value;
/// 	cnx_mutex_unlock(my_thing_mutex);
/// 	return val;
/// }
///
/// /// elsewhere...
/// /// in thread1-related code
///
/// // do some compute intensive task...
/// // update the value
/// update_my_thing(new_value);
///
/// /// in thread2-related code
/// my_val = get_value_from_my_thing();
/// // do something with my_val
///
/// let_mut newval = get_value_from_my_thing();
/// while(newval == my_val) {
/// 	cnx_this_thread_sleep_for(cnx_milliseconds(100));
/// 	newval = get_value_from_my_thing();
/// }
///
/// my_val = newval;
/// // do something with new value
/// @endcode
/// @ingroup cnx_mutex
typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxRecursiveBasicMutex __mutex;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxRecursiveMutex;

/// @brief `CnxTimedMutex` is a higher-level mutex type for use when timed backoff of mutex locking
/// is required, and is directly comparable to C++'s `std::timed_mutex`. It allows for specifying a
/// timeout, which if reached will cause execution to stop attempting to acquire the lock and signal
/// failure to the caller, instead of blocking indefinitely until the lock was successfully acquired
/// like a tradition mutex. For example, if an algorithm needs synchronization, but blocking for
/// longer than X milliseconds when trying to acquire the lock would be problematic, a
/// `CnxTimedMutex` would be the appropriate mutex type to use.
///
/// Example:
/// @code {.c}
/// // MyThing.h
/// #include <Cnx/sync/Mutex.h>
/// static MyType my_very_important_thing;
/// static CnxTimedMutex* my_thing_mutex;
///
/// void init_my_thing(void);
/// bool update_my_thing(u64 value);
/// u64 get_value_from_my_thing(void);
///
/// // MyThing.c
/// #include <Cnx/Allocators.h>
/// #include "MyThing.h"
/// void init_my_thing(void) {
/// 	if(my_thing_mutex == nullptr) {
/// 		my_thing_mutex = cnx_allocator_allocate_t(CnxTimedMutex, DEFAULT_ALLOCATOR);
/// 		*my_thing_mutex = cnx_timed_mutex_new();
///
/// 		cnx_timed_mutex_lock(my_thing_mutex);
/// 		my_very_important_thing = {
/// 		// important intialization
/// 		};
/// 		cnx_timed_mutex_unlock(my_thing_mutex);
/// 	}
/// }
///
/// bool update_my_thing(u64 value) {
///  	// Try to acquire the lock for 50 milliseconds.
///  	// if we acquire it before the timeout, update the value.
///  	// Otherwise return `false` to communicate that updating failed
///  	if(cnx_timed_mutex_try_lock_for(my_thing_mutex), cnx_milliseconds(50)) {
/// 		my_very_important_thing.value = value;
/// 		return true;
/// 	}
///
/// 	return false;
/// }
///
/// u64 get_value_from_my_thing(void) {
/// 	cnx_mutex_lock(my_thing_mutex);
/// 	let val = my_very_important_thing.value;
/// 	cnx_mutex_unlock(my_thing_mutex);
/// 	return val;
/// }
///
/// /// elsewhere...
/// /// in thread1-related code
///
/// loop {
/// 	// do some compute intensive task...
///
/// 	// try to update the value
/// 	if(!update_my_thing(new_value)) {
/// 		fprintln(log_file, "INFO: Failed to update value");
/// 	}
/// }
///
/// /// in thread2-related code
/// my_val = get_value_from_my_thing();
/// // do something with my_val
///
/// let_mut newval = get_value_from_my_thing();
/// while(newval == my_val) {
/// 	cnx_this_thread_sleep_for(cnx_milliseconds(100));
/// 	newval = get_value_from_my_thing();
/// }
///
/// my_val = newval;
/// // do something with new value
/// @endcode
/// @ingroup cnx_mutex
typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxMutex __mutex;
	CnxCondvar __timed_guard;
	bool __locked;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxTimedMutex;

/// @brief `CnxRecursiveTimedMutex` is a higher-level mutex type for use when the properties of both
/// a recursive and a timed mutex are necessary for the task at hand. It's directly comparable to
/// C++'s `std::recursive_timed_mutex`. It allows for both specifying a timeout for lock acquisition
/// as well as recursive lock acquisition on the same thread.
/// @ingroup cnx_mutex
typedef struct {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxMutex __mutex;
	CnxCondvar __timed_guard;
	usize __times_locked;
	CnxThreadID __locked_thread_id;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxRecursiveTimedMutex;

/// @brief `cnx_defer_lock_t` is a tag type intended for use with scoped lock guards such as
/// `CnxUniqueLock` and `CnxSharedLock`, to communicate that the guard should defer locking the
/// given mutex until requested to do so. This is in contrast to the default behavior, where a
/// lock guard will acquire the lock upon its construction.
/// @ingroup cnx_mutex
typedef struct {
} cnx_defer_lock_t;

/// @brief A global instantiation of `cnx_defer_lock_t`, provided for convenience
/// @ingroup cnx_mutex
static let cnx_defer_lock = (cnx_defer_lock_t){};

/// @brief `cnx_adopt_lock_t` is a tag type intended for use with scoped lock guards such as
/// `CnxUniqueLock` and `CnxSharedLock`, to communicate that the given mutex has already been locked
/// and thus the guard does not need to acquire the lock itself. This is in contrast to the default
/// behavior, where a lock guard will acquire the lock upon its construction.
/// @ingroup cnx_mutex
typedef struct {
} cnx_adopt_lock_t;

/// @brief A global instantiation of `cnx_adopt_lock_t`, provided for convenience
/// @ingroup cnx_mutex
static let cnx_adopt_lock = (cnx_adopt_lock_t){};

/// @brief `cnx_try_lock_t` is a tag type intended for use with scoped lock guards such as
/// `CnxUniqueLock` and `CnxSharedLock`, to communicate that the guard should only attempt to lock
/// the given mutex (via `try_lock`), and not block if lock acquisition was unsuccessful. This is in
/// contrast to the default behavior, where a lock guard will unconditionally acquire the lock upon
/// its construction.
/// @ingroup cnx_mutex
typedef struct {
} cnx_try_lock_t;

/// @brief A global instantiation of `cnx_try_lock_t`, provided for convenience
/// @ingroup cnx_mutex
static let cnx_try_lock = (cnx_try_lock_t){};

// clang-format off

IGNORE_RESERVED_IDENTIFIER_WARNING_START
typedef enum {
	__CNX_MUTEX_ID_MUTEX = 0,
	__CNX_MUTEX_ID_RECURSIVE_MUTEX,
	__CNX_MUTEX_ID_TIMED_MUTEX,
	__CNX_MUTEX_ID_RECURSIVE_TIMED_MUTEX,
	__CNX_MUTEX_ID_SHARED_MUTEX,
	__CNX_MUTEX_ID_SHARED_TIMED_MUTEX
} __CnxMutexId;
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

Trait(
	/// @struct CnxMutexInterface
	/// @brief `CnxMutexInterface` is a uniform interface and Trait implementation that all
	/// higher-level mutexes (ie not the basic primitives provided in `<Cnx/Thread.h>`) meeting
	/// Cnx's requirements provide. This interface allows for using the various mutex types provided
	/// by Cnx with other facilities like `CnxUniqueLock` without having to special case on the
	/// type(s) of the mutex(es) used.
	/// 
	///
	/// Functions:
	/// 	- 
	/// 	@code {.c}
	/// 	void (*const lock)(CnxMutexInterface* restrict mutex)
	/// 	@endcode
	///
	/// 		Exclusively lock the mutex
	/// 	- 
	/// 	@code {.c}
	/// 	bool (*const try_lock)(CnxMutexInterface* restrict mutex)
	/// 	@endcode
	///
	/// 		Attempt to exclusively lock the mutex
	/// 	- 
	/// 	@code {.c}
	/// 	bool (*const try_lock_for)(CnxMutexInterface* restrict mutex, CnxDuration duration)
	/// 	@endcode
	///
	/// 		Attempt to exclusively lock the mutex, timeout after `duration` amount of time has
	/// 		passed. Only available if `mutex` is a timed mutex (otherwise this will be
	/// 		`nullptr`).
	/// 	- 
	/// 	@code {.c}
	/// 	bool (*const try_lock_until)(CnxMutexInterface* restrict mutex, CnxTimePoint stop_point)
	/// 	@endcode
	///
	/// 		Attempt to exclusively lock the mutex, timeout once the time `stop_point` has
	/// 		occurred. Only available if `mutex` is a timed mutex (otherwise this will be
	/// 		`nullptr`).
	/// 	- 
	/// 	@code {.c}
	/// 	void (*const unlock)(CnxMutexInterface* restrict mutex)
	/// 	@endcode
	///
	/// 		Unlocks the mutex
	/// 	- 
	/// 	@code {.c}
	/// 	__CnxMutexId (*const type_id)(CnxMutexInterface* restrict mutex)
	/// 	@endcode
	///
	/// 		Returns the mutex type ID for the mutex. Used internally by scoped lock guards
	/// 		like `CnxUniqueLock` to identify the concrete type of the mutex
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
	__attr(not_null(1)) void (*const unlock)(CnxMutexInterface* restrict mutex);
	__attr(nodiscard)
	__attr(not_null(1)) __CnxMutexId (*const type_id)(CnxMutexInterface* restrict mutex));

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
__attr(nodiscard) __attr(not_null(1)) __CnxMutexId __cnx_mutex_id(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);

__attr(not_null(1)) void __cnx_recursive_mutex_lock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(nodiscard)
	__attr(not_null(1)) bool __cnx_recursive_mutex_try_lock(CnxMutexInterface* restrict mutex)
		__DISABLE_IF_NULL(mutex);
__attr(not_null(1)) void __cnx_recursive_mutex_unlock(CnxMutexInterface* restrict mutex)
	__DISABLE_IF_NULL(mutex);
__attr(nodiscard) __attr(not_null(1)) __CnxMutexId
	__cnx_recursive_mutex_id(CnxMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);

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
__attr(nodiscard) __attr(not_null(1)) __CnxMutexId
	__cnx_timed_mutex_id(CnxMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);

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
__attr(nodiscard) __attr(not_null(1)) __CnxMutexId
	__cnx_recursive_timed_mutex_id(CnxMutexInterface* restrict mutex) __DISABLE_IF_NULL(mutex);

__attr(maybe_unused) static ImplTraitFor(CnxMutexInterface,
										 CnxMutex,
										 .lock = __cnx_mutex_lock,
										 .try_lock = __cnx_mutex_try_lock,
										 .try_lock_for = nullptr,
										 .try_lock_until = nullptr,
										 .unlock = __cnx_mutex_unlock,
										 .type_id = __cnx_mutex_id);
__attr(maybe_unused) static ImplTraitFor(CnxMutexInterface,
										 CnxRecursiveMutex,
										 .lock = __cnx_recursive_mutex_lock,
										 .try_lock = __cnx_recursive_mutex_try_lock,
										 .try_lock_for = nullptr,
										 .try_lock_until = nullptr,
										 .unlock = __cnx_recursive_mutex_unlock,
										 .type_id = __cnx_recursive_mutex_id);
__attr(maybe_unused) static ImplTraitFor(CnxMutexInterface,
										 CnxTimedMutex,
										 .lock = __cnx_timed_mutex_lock,
										 .try_lock = __cnx_timed_mutex_try_lock,
										 .try_lock_for = __cnx_timed_mutex_try_lock_for,
										 .try_lock_until = __cnx_timed_mutex_try_lock_until,
										 .unlock = __cnx_timed_mutex_unlock,
										 .type_id = __cnx_timed_mutex_id);
__attr(maybe_unused) static ImplTraitFor(CnxMutexInterface,
										 CnxRecursiveTimedMutex,
										 .lock = __cnx_recursive_timed_mutex_lock,
										 .try_lock = __cnx_recursive_timed_mutex_try_lock,
										 .try_lock_for = __cnx_recursive_timed_mutex_try_lock_for,
										 .try_lock_until
										 = __cnx_recursive_timed_mutex_try_lock_until,
										 .unlock = __cnx_recursive_mutex_unlock,
										 .type_id = __cnx_recursive_timed_mutex_id);
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#undef __DISABLE_IF_NULL

#endif // CNX_MUTEX
