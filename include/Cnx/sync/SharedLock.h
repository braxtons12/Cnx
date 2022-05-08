/// @file SharedLock.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief `CnxSharedLock` provides a shared-ownership scoped mutex locking mechanism similar to
/// C++'s `std::shared_lock`
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
/// @defgroup cnx_shared_lock CnxSharedLock
/// `CnxSharedLock` provides scoped non-exclusive (shared) locking of any higher-level mutexes
/// (eg `CnxMutex`, `CnxSharedMutex`, etc.) provided by Cnx. It allows for a simple, concise way to
/// acquire the shared lock on a mutex and ensure that lock is released appropriately.
///
/// Example:
/// @code {.c}
/// // MyThing.h
/// #include <Cnx/sync/SharedMutex.h>
/// static MyType my_very_important_thing;
/// static CnxSharedMutex* my_thing_mutex;
///
/// void init_my_thing(void);
/// void update_my_thing(u64 value);
/// u64 get_value_from_my_thing(void);
///
/// // MyThing.c
/// #include <Cnx/Allocators.h>
/// #include <Cnx/sync/SharedLock.h>
/// #include <Cnx/sync/UniqueLock.h>
/// #include "MyThing.h"
/// void init_my_thing(void) {
/// 	if(my_thing_mutex == nullptr) {
/// 		my_thing_mutex = cnx_allocator_allocate_t(CnxSharedMutex, DEFAULT_ALLOCATOR);
/// 		*my_thing_mutex = cnx_shared_mutex_new();
///
/// 		{
/// 			// use a `CnxUniqueLock` to exclusively lock `my_thing_mutex`.
/// 			// mark `lock` as `maybe_unused` so we don't get warnings about an unused variable
/// 			__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(*my_thing_mutex);
/// 			my_very_important_thing = {
/// 				// important intialization
/// 			};
/// 			// `lock` automatically destroyed at scope end, releasing the lock on
/// 			// `my_thing_mutex`
/// 		}
/// 	}
/// }
///
/// void update_my_thing(u64 value) {
/// 	// lock `my_thing_mutex` exclusively so we can write to what it's protecting
///  	__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(*my_thing_mutex);
/// 	my_very_important_thing.value = value;
/// 	// `lock` automatically destroyed at scope end, releasing the lock on `my_thing_mutex`
/// }
///
/// u64 get_value_from_my_thing(void) {
/// 	// lock `my_thing_mutex` non-exclusively (shared) so that we can read from what it's
/// 	// protecting
///  	__attr(maybe_unused) SharedLock lock = cnx_shared_lock(*my_thing_mutex);
/// 	let val = my_very_important_thing.value;
/// 	return val;
/// 	// `lock` automatically destroyed at scope end, releasing the lock on `my_thing_mutex`
/// }
///
/// /// elsewhere...
/// /// in thread1-related code
///
/// // do some compute intensive task...
/// // update the value
/// update_my_thing(new_value);
///
/// /// in thread2,3,4,...-related code
/// // multiple threads can all call `get_value_from_my_thing` without blocking eachother, because
/// // `my_thing_mutex` is a shared mutex and `get_value_from_my_thing` uses a shared lock
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
/// @}

#ifndef CNX_SHARED_LOCK
#define CNX_SHARED_LOCK
#include <Cnx/Def.h>
#include <Cnx/sync/SharedMutex.h>

/// @brief `CnxSharedLock` provides scoped non-exclusive (shared) locking of higher-level mutexes
/// (eg `CnxSharedMutex`, `CnxSharedTimedMutex`) provided by Cnx. It allows for a simple, concise
/// way to acquire the shared lock on a mutex and ensure that lock is released appropriately when
/// the `CnxSharedLock` exits scope.
///
/// To ensure that a `CnxSharedLock` is appropriately destroyed when it leaves scope, releasing the
/// lock on its associated mutex, declare it as a `SharedLock`.
///
/// A `CnxSharedLock` should never be copied, to do so is undefined behavior. A `CnxSharedLock` may
/// be moved into a new scope with `move` (eg. passing it to a function as a parameter), however.
/// If this is done though, the receiving function will need to either move the parameter into a
/// local variable declared as a `SharedLock`, or manually unlock the mutex before it returns.
///
/// Example:
/// @code {.c}
/// // MyThing.h
/// #include <Cnx/sync/SharedMutex.h>
/// static MyType my_very_important_thing;
/// static CnxSharedMutex* my_thing_mutex;
///
/// void init_my_thing(void);
/// void update_my_thing(u64 value);
/// u64 get_value_from_my_thing(void);
///
/// // MyThing.c
/// #include <Cnx/Allocators.h>
/// #include <Cnx/sync/UniqueLock.h>
/// #include <Cnx/sync/SharedLock.h>
/// #include "MyThing.h"
/// void init_my_thing(void) {
/// 	if(my_thing_mutex == nullptr) {
/// 		my_thing_mutex = cnx_allocator_allocate_t(CnxSharedMutex, DEFAULT_ALLOCATOR);
/// 		*my_thing_mutex = cnx_shared_mutex_new();
///
/// 		{
/// 			// use a `CnxUniqueLock` to lock `my_thing_mutex`.
/// 			// mark `lock` as `maybe_unused` so we don't get warnings about an unused variable
/// 			__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(*my_thing_mutex);
/// 			my_very_important_thing = {
/// 				// important intialization
/// 			};
/// 			// `lock` automatically destroyed at scope end, releasing the lock on
/// `my_thing_mutex`
/// 		}
/// 	}
/// }
///
/// void update_my_thing(u64 value) {
///  	__attr(maybe_unused) UniqueLock lock = cnx_unique_lock(*my_thing_mutex);
/// 	my_very_important_thing.value = value;
/// 	// `lock` automatically destroyed at scope end, releasing the lock on `my_thing_mutex`
/// }
///
/// u64 get_value_from_my_thing(CnxSharedLock lock) {
///  	// move lock into lock2 so that the mutex will be unlocked at scope exit
///  	__attr(maybe_unused) SharedLock lock2 = move(lock);
/// 	let val = my_very_important_thing.value;
/// 	return val;
/// 	// `lock2` automatically destroyed at scope end, releasing the lock on `my_thing_mutex`
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
/// SharedLock lock1 = cnx_shared_lock(*my_thing_mutex);
/// my_val = get_value_from_my_thing(move(lock1));
/// // do something with my_val
///
/// SharedLock lock2 = cnx_shared_lock(*my_thing_mutex);
/// let_mut newval = get_value_from_my_thing(move(lock2));
/// while(newval == my_val) {
/// 	cnx_this_thread_sleep_for(cnx_milliseconds(100));
/// 	SharedLock lock = cnx_shared_lock(*my_thing_mutex);
/// 	newval = get_value_from_my_thing(move(lock));
/// }
///
/// my_val = newval;
/// // do something with new value
/// @endcode
/// @ingroup cnx_shared_lock
typedef struct CnxSharedLock {
	IGNORE_RESERVED_IDENTIFIER_WARNING_START
	CnxSharedMutexInterface __mutex;
	bool __owned;
	IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
} CnxSharedLock;

/// @brief Declaring a `CnxSharedLock` as a `SharedLock` ensures that it will be destroyed when it
/// exits scope, releasing the lock on its associated mutex
/// @ingroup cnx_shared_lock
#define SharedLock scoped(cnx_shared_lock_free)

#define __DISABLE_IF_NULL(lock) \
	cnx_disable_if(!(lock), "Can't perform a CnxSharedLock operation on a nullptr")

/// @brief Creates a new `CnxSharedLock` associated with the given mutex
///
/// Creates a new `CnxSharedLock` associated with the given mutex. If an instance of a tag type is
/// __NOT__ given to specify construction behavior, this will block until the shared lock is
/// acquired on the given mutex. If an instance of a tag type specifying construction behavior
/// __IS__ given, however, then construction will behave according to the given tag type:
/// 	- A `cnx_defer_lock_t`: Construct the `CnxSharedLock` without acquiring the lock on the
/// 	given mutex. The mutex will need to be manually locked (via the shared lock) later
/// 	- A `cnx_adopt_lock_t`: The shared lock has already been acquired, so the `CnxSharedLock`
/// 	does not need to acquire it. Behavior will otherwise proceed as if no tag type instance
/// 	were given.
/// 	- A `cnx_try_lock_t`: Attempt to acquire the shared lock on the given mutex during
/// 	construction. The `CnxSharedLock` will need to be queried as to whether it successfully
/// 	acquired the lock with `cnx_shared_lock_owns_lock`, but otherwise behavior will proceed as
/// 	if no tag type instance were given.
///
/// @param ...
/// 	- mutex - The mutex to acquire a non-exclusive (shared) lock on. Can be any of
/// 	`CnxSharedMutex`, or `CnxSharedTimedMutex`
/// 	- tag - An instance of the tag type specifying construction behavior (see above).
///
/// @return a `CnxSharedLock` constructed with the specified behavior
/// @ingroup cnx_shared_lock
#define cnx_shared_lock(...) CONCAT2(__cnx_shared_lock_, PP_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

/// @brief Acquires a shared lock on the mutex associated with the given `CnxSharedLock`
///
/// @param lock - The `CnxSharedLock` associated with the mutex to lock non-exclusively
/// @ingroup cnx_shared_lock
__attr(not_null(1)) void cnx_shared_lock_lock(CnxSharedLock* restrict lock) __DISABLE_IF_NULL(lock);
/// @brief Attempts to acquire a shared lock on the mutex associated with the given `CnxSharedLock`
///
/// @param lock - The `CnxSharedLock` associated with the mutex to lock non-exclusively
///
/// @return whether locking the mutex was successful
/// @ingroup cnx_shared_lock
__attr(nodiscard) __attr(not_null(1)) bool cnx_shared_lock_try_lock(CnxSharedLock* restrict lock)
	__DISABLE_IF_NULL(lock);
/// @brief Attempts to acquire a shared lock on the mutex associated with the given
/// `CnxSharedLock`
///
/// Attempts to acquire a shared lock on the mutex associated with `lock`. If the amount of time
/// specified by `duration` has passed before successfully acquiring the lock, this will timeout and
/// cease acquisition.
///
/// @param lock - The `CnxSharedLock` associated with the mutex to lock
/// @param duration - The maximum amount of time to spend trying to acquire the lock, after which
/// acquisition will timeout and fail
///
/// @return whether locking the mutex was successful
/// @note This is unavailable if the mutex associated with `lock` is not a timed mutex. Calling this
/// on a `CnxSharedLock` associated with a non-timed mutex is undefined behavior.
/// @ingroup cnx_shared_lock
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_shared_lock_try_lock_for(CnxSharedLock* restrict lock,
														  CnxDuration duration)
		__DISABLE_IF_NULL(lock) __DISABLE_IF_NULL(lock->__mutex.m_vtable->try_lock_for);
/// @brief Attempts to acquire a shared lock on the mutex associated with the given
/// `CnxSharedLock`
///
/// Attempts to acquire a shared lock on the mutex associated with `lock`. If the point in time
/// specified by `stop_point` has passed before successfully acquiring the lock, this will timeout
/// and cease acquisition.
///
/// @param lock - The `CnxSharedLock` associated with the mutex to lock
/// @param stop_point - The point in time after which acquisition should timeout and fail, if the
/// lock has not yet been acquired
///
/// @return whether locking the mutex was successful
/// @note This is unavailable if the mutex associated with `lock` is not a timed mutex. Calling this
/// on a `CnxSharedLock` associated with a non-timed mutex is undefined behavior.
/// @ingroup cnx_shared_lock
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_shared_lock_try_lock_until(CnxSharedLock* restrict lock,
															CnxTimePoint stop_point)
		__DISABLE_IF_NULL(lock) __DISABLE_IF_NULL(lock->__mutex.m_vtable->try_lock_until);
/// @brief Unlocks the mutex associated with the given `CnxSharedLock`
///
/// @param lock - The `CnxSharedLock` associated with the mutex to unlock
/// @ingroup cnx_shared_lock
__attr(not_null(1)) void cnx_shared_lock_unlock(CnxSharedLock* restrict lock)
	__DISABLE_IF_NULL(lock);
/// @brief Returns the `CnxSharedMutexInterface` Trait object corresponding with the mutex
/// associated with the given `CnxSharedLock`
///
/// @param lock - The `CnxSharedLock` to get the associated mutex of
///
/// @return the `CnxSharedMutexInterface` Trait object of the mutex associated with the given lock
/// @ingroup cnx_shared_lock
__attr(nodiscard) __attr(not_null(1)) CnxSharedMutexInterface
	cnx_shared_lock_mutex(CnxSharedLock* restrict lock) __DISABLE_IF_NULL(lock);
/// @brief Returns whether the given `CnxSharedLock` currently owns a non-exclusive (shared) lock on
/// its associated mutex
///
/// @param lock - The `CnxSharedLock` to check if owns a shared lock on its associated mutex
///
/// @return whether `lock` owns a lock on its mutex
/// @ingroup cnx_shared_lock
__attr(nodiscard) __attr(not_null(1)) bool cnx_shared_lock_owns_lock(CnxSharedLock* restrict lock)
	__DISABLE_IF_NULL(lock);
/// @brief Destroys the given `CnxSharedLock`, unlocking its associated mutex if it owns a lock on
/// it
///
/// @param lock - The `CnxSharedLock` to destroy
/// @note In practice, this shouldn't be called manually, and instead the associated `CnxSharedLock`
/// should be declared as a `SharedLock` so that this is called on it implicitly when it exits scope
/// @ingroup cnx_shared_lock
__attr(not_null(1)) void cnx_shared_lock_free(void* lock) __DISABLE_IF_NULL(lock);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __cnx_shared_lock_cast(mutex) \
	_Generic((&(mutex)), 																   		   \
				const CnxSharedMutex* 				: as_trait(CnxSharedMutexInterface, 		   \
															   CnxSharedMutex, 					   \
															   mutex),     						   \
				CnxSharedMutex* 						: as_trait(CnxSharedMutexInterface, 	   \
																   CnxSharedMutex, 				   \
																   mutex),     					   \
				const CnxRecursiveMutex* 		: as_trait(CnxSharedMutexInterface, 					   \
														   CnxRecursiveMutex, 					   \
														   mutex), 								   \
				CnxRecursiveMutex* 				: as_trait(CnxSharedMutexInterface, 					   \
														   CnxRecursiveMutex, 					   \
														   mutex), 								   \
				const CnxSharedTimedMutex* 			: as_trait(CnxSharedMutexInterface, 		   \
														   CnxSharedTimedMutex, 				   \
														   mutex), 								   \
				CnxSharedTimedMutex* 					: as_trait(CnxSharedMutexInterface, 	   \
														   CnxSharedTimedMutex, 				   \
														   mutex))

#define __cnx_shared_lock_1(mutex) __cnx_shared_lock(__cnx_shared_lock_cast(mutex))

#define __cnx_shared_lock_2(mutex, lock_type) \
	_Generic((&(lock_type)), 																   \
				const cnx_defer_lock_t* 	: __cnx_shared_lock_deferred, 						   \
				cnx_defer_lock_t* 			: __cnx_shared_lock_deferred, 						   \
				const cnx_try_lock_t* 		: __cnx_shared_lock_try, 							   \
				cnx_try_lock_t* 			: __cnx_shared_lock_try, 							   \
				const cnx_adopt_lock_t* 	: __cnx_shared_lock_adopt, 							   \
				cnx_adopt_lock_t* 			: __cnx_shared_lock_adopt) 							   \
			(__cnx_shared_lock_cast(mutex), lock_type)

__attr(nodiscard) CnxSharedLock __cnx_shared_lock(CnxSharedMutexInterface mutex);
__attr(nodiscard) CnxSharedLock
	__cnx_shared_lock_deferred(CnxSharedMutexInterface mutex, cnx_defer_lock_t defer_lock);
__attr(nodiscard) CnxSharedLock
	__cnx_shared_lock_try(CnxSharedMutexInterface mutex, cnx_try_lock_t try_lock);
__attr(nodiscard) CnxSharedLock
	__cnx_shared_lock_adopt(CnxSharedMutexInterface mutex, cnx_adopt_lock_t adopt_lock);

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#undef __DISABLE_IF_NULL

#endif // CNX_SHARED_LOCK
