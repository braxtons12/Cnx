/// @file Thread.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Type definitions and function declarations for threading functionality
/// @version 0.2.2
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

/// @defgroup cnx_thread Threading and Synchronization Primitives
/// Cnx provides a set of cross-platform multithreading and synchronization facilities to make
/// working with concurrent code as simple and straightforward as possible. The API is similar to
/// C++'s and that added by the C11 standard, but is supported on platforms and compilers where
/// implementations of C11's threading may not be available (like Windows, for example).
///
/// Example:
/// @code {.c}
/// #include <Cnx/Thread.h>
/// #include <Cnx/IO.h>
/// #include <Cnx/Atomic.h>
/// #include <Cnx/Assert.h>
///
/// // A lambda that captures a `CnxBasicMutex*`, two `i32*`s and an `atomic_bool`*
/// void LambdaFunction(example_print, void) {
/// 	let bindings = LambdaBinding(CnxBasicMutex*, i32*, i32*, atomic_bool*);
///
/// 	// run until the exit flag is set
/// 	let exit_flag_ptr = bindings._4;
/// 	while(!atomic_load(exit_flag_ptr)) {
/// 		let_mut mutex_ptr = bindings._1;
/// 		let locked = cnx_basic_mutex_lock(mutex_ptr);
///
/// 		cnx_assert(cnx_result_is_ok(locked), "Failed to lock the mutex");
/// 		if(cnx_result_is_ok(locked)) {
/// 			let num1 = *(bindings._2);
/// 			let num2 = *(bindings._3);
/// 			println("Values: [{}, {}]", num1, num2);
/// 		}
/// 	}
/// }
///
/// // A lambda that captures a `CnxBasicMutex*`, two `i32*`s and an `atomic_bool`*
/// void LambdaFunction(example_add, void) {
/// 	let bindings = LambdaBinding(CnxBasicMutex*, i32*, i32*, atomic_bool*);
///
/// 	// run until the exit flag is set
/// 	let exit_flag_ptr = bindings._4;
/// 	while(!atomic_load(exit_flag_ptr)) {
/// 		let_mut mutex_ptr = bindings._1;
/// 		let locked = cnx_basic_mutex_lock(mutex_ptr);
//
/// 		cnx_assert(cnx_result_is_ok(locked), "Failed to lock the mutex");
/// 		if(cnx_result_is_ok(locked)) {
/// 			let_mut num1_ptr = bindings._2;
/// 			let_mut num2_ptr = bindings._3;
/// 			(*(num1_ptr))++;
/// 			(*(num2_ptr))++;
/// 		}
/// 	}
/// }
///
/// // Spawns one thread that continuosly prints two `i32`s for 60 seconds.
/// // Spawns a second thread that continuously increments said `i32`s.
/// // After 60 seconds, joins both threads.
/// void example(void) {
/// 	i32 val1 = 0;
/// 	i32 val2 = 0;
/// 	atomic_bool exit_flag = false;
///
/// 	let_mut maybe_mutex = cnx_basic_mutex_new();
///
/// 	cnx_assert(cnx_result_is_ok(maybe_mutex), "Failed to create a new mutex");
/// 	let_mut mutex = cnx_result_unwrap(maybe_mutex);
///
/// 	// creates a new thread whose startup routine is the `LambdaFunction` `example_print`
/// 	// the lambda will be automatically freed when the thread completes execution
/// 	let_mut maybe_print_thread = cnx_thread_new(lambda(example_print, &mutex,
/// 																	  &val1,
/// 																	  &val2,
/// 																	  &exit_flag));
/// 	cnx_assert(cnx_result_is_ok(maybe_print_thread), "Failed to create print thread");
/// 	let_mut print_thread = cnx_result_unwrap(maybe_print_thread);
///
/// 	// creates a new thread whose startup routine is the `LambdaFunction` `example_add`
/// 	// the lambda will be automatically freed when the thread completes execution
/// 	let_mut maybe_add_thread = cnx_thread_new(lambda(example_add, &mutex,
/// 																  &val1,
/// 																  &val2,
/// 																  &exit_flag));
/// 	cnx_assert(cnx_result_is_ok(maybe_add_thread), "Failed to create add thread");
/// 	let_mut add_thread = cnx_result_unwrap(maybe_add_thread);
///
/// 	cnx_this_thread_sleep_for(cnx_seconds(60));
///
/// 	atomic_store(&exit_flag, true);
/// 	let print_joined = cnx_thread_join(&print_thread);
/// 	let add_joined = cnx_thread_join(&add_thread);
/// 	cnx_assert(cnx_result_is_ok(print_joined), "Print thread failed to join");
/// 	cnx_assert(cnx_result_is_ok(add_joined), "Adding thread failed to join");
///
/// 	cnx_basic_mutex_free(&mutex);
///
/// 	println("Finshed printing the numbers!");
/// }
/// @endcode

// TODO(braxtons12): Add Clang Thread Safety Analysis attributes to these functions and types
#ifndef CNX_THREAD
#define CNX_THREAD

#include <Cnx/Atomic.h>
#include <Cnx/Def.h>
#include <Cnx/Lambda.h>
#include <Cnx/__thread/__thread.h>
#include <Cnx/time/Duration.h>
#include <Cnx/time/TimePoint.h>

#if ___CNX_HAS_NO_THREADS
	#error Threads not supported on the target platform
#endif

#define RESULT_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Result.h>
#undef RESULT_INCLUDE_DEFAULT_INSTANTIATIONS

/// @brief `CnxBasicMutex` is the basic mutual exclusion object type provided by Cnx's threading
/// and synchronization API. Using a `CnxBasicMutex` to control access to shared resources will
/// ensure that all access is synchronized and mutually exclusive between threads.
/// @ingroup cnx_thread
typedef __cnx_basic_mutex CnxBasicMutex;
/// @brief `CnxRecursiveBasicMutex` is the basic reentrant mutual exclusion object type provided by
/// Cnx's threading and synchronization API. Using a `CnxRecursiveBasicMutex` to control access to
/// shared resources will ensure that all access is synchronized and mutually exclusive between
/// threads, and allows for rentrant ownership of the lock on that access.
/// @ingroup cnx_thread
typedef __cnx_recursive_basic_mutex CnxRecursiveBasicMutex;
/// @brief `CnxBasicCondvar` is the Condition Variable object type provided by Cnx's threading and
/// synchronization API. Using a `CnxBasicCondvar` allows to block one or more threads until an
/// event is triggered, without wasting CPU resources.
/// @ingroup cnx_thread
typedef __cnx_condvar CnxBasicCondvar;
/// @brief A `CnxThreadID` uniquely identifies an indiviual thread
/// @ingroup cnx_thread
typedef __cnx_thread_id CnxThreadID;
/// @brief A `CnxThread` is the handle type for basic OS-level threads provided by Cnx's threading
/// and synchronization API. `CnxThread` provides the facilities for creating multiple threads and
/// ending their execution, enabling concurrent tasks and operations to be performed.
/// @ingroup cnx_thread
typedef __cnx_thread CnxThread;
/// @brief A `CnxStopToken` is associated with a `CnxJThread` and is used to signal to it when it
/// should end its execution.
/// @ingroup cnx_thread
typedef atomic_bool CnxStopToken;
/// @brief A `CnxJThread` is the handle type for OS-level threads which are automatically joined
/// when their handle goes out of scope and use a dedicated stop token to signal to the thread when
/// its execution should end.
/// @ingroup cnx_thread
typedef struct {
	/// @brief The handle to the OS-level thread the `CnxJThread` manages
	CnxThread handle;
	/// @brief The stop token used to signal to the thread to end execution
	CnxStopToken* stop_token;
} CnxJThread;
/// @brief A `CnxTLSKey` is the key type for creating, accessing, and modifying a thread-local
/// storage object
/// @ingroup cnx_thread
typedef __cnx_tls_key CnxTLSKey;
/// @brief A `CnxOnceFlag` is a synchronization flag type for use with `cnx_execute_once` to ensure
/// a given function is executed exactly once, regardless of how many threads attempt to call it,
/// as long as all threads do so through the same `CnxOnceFlag` and `cnx_execute_once`
/// @ingroup cnx_thread
typedef __cnx_exec_once_flag CnxOnceFlag;

#ifndef thread_local
	/// @brief A variable declared as `thread_local` is local to the current thread and guaranteed
	/// to not be accessible by other threads
	/// @ingroup cnx_thread
	#define thread_local _Thread_local
#endif

#define RESULT_T	CnxBasicMutex
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

#define RESULT_T	CnxRecursiveBasicMutex
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

#define RESULT_T	CnxBasicCondvar
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

#define RESULT_T	CnxThread
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

#define RESULT_T	CnxJThread
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

#define RESULT_T	CnxTLSKey
#define RESULT_DECL TRUE
// NOLINTNEXTLINE(readability-duplicate-include)
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

#define __DISABLE_IF_NULL(mutex) \
	cnx_disable_if(!(mutex), "Can't perform a mutex operation on a nullptr")

/// @brief Creates a new mutex
///
/// Creating a mutex can fail, depending on memory and operating system level constraints.
/// If creating the mutex is successful, returns a `CnxBasicMutex`, otherwise returns an error.
///
/// @return A `CnxBasicMutex` if successful
/// @ingroup cnx_thread
__attr(nodiscard) CnxResult(CnxBasicMutex) cnx_basic_mutex_new(void);
/// @brief Initializes the mutex pointed to by `mutex`
///
/// Initializing a mutex can fail, depending on memory and operating system level constraints.
/// If initializing the mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @param mutex - The mutex to initalize
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult cnx_basic_mutex_init(CnxBasicMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unconditionally locks the mutex pointed to by `mutex`
///
/// Locking a mutex can fail (for example, if it's already locked on the calling thread).
/// If locking the mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @param mutex - The mutex to lock
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult cnx_basic_mutex_lock(CnxBasicMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex`
///
/// If locking the mutex is successful, return `true`.
//
/// @param mutex - The mutex to lock
///
/// @return `true` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) bool cnx_basic_mutex_try_lock(CnxBasicMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unlocks the mutex pointed to by `mutex`
///
/// Unlocking a mutex can fail (for example, if it's already unlocked).
/// If unlocking the mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @param mutex - The mutex to unlock
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_basic_mutex_unlock(CnxBasicMutex* restrict mutex) __DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Destroys the mutex pointed to by `mutex`
///
/// Destroying a mutex can fail (for example, it it's still in use by other threads).
/// If destroying the mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @param mutex - The mutex to free
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult cnx_basic_mutex_free(CnxBasicMutex* restrict mutex)
	__DISABLE_IF_NULL(mutex); // NOLINT

/// @brief Creates a new recursive mutex
///
/// Creating a mutex can fail, depending on memory and operating system level constraints.
/// If creating the recursive mutex is successful, returns a `CnxRecursiveBasicMutex`, otherwise
/// returns an error.
//
/// @return A `CnxRecursiveBasicMutex` if successful
/// @ingroup cnx_thread
__attr(nodiscard) CnxResult(CnxRecursiveBasicMutex) cnx_recursive_basic_mutex_new(void);
/// @brief Initializes the recursive mutex pointed to by `mutex`
///
/// Initializing a mutex can fail, depending on memory and operating system level constraints.
/// If initializing the mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @param mutex - The mutex to initialize
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_recursive_basic_mutex_init(CnxRecursiveBasicMutex* restrict mutex) // NOLINT
	__DISABLE_IF_NULL(mutex);
/// @brief Unconditionally locks the recursive mutex pointed to by `mutex`
///
/// Locking a mutex can fail.
/// If locking the recursive mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @param mutex - The mutex to lock
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_recursive_basic_mutex_lock(CnxRecursiveBasicMutex* restrict mutex) // NOLINT
	__DISABLE_IF_NULL(mutex);
/// @brief Attempts to lock the recursive mutex pointed to by `mutex`
///
/// If locking the recursive mutex is successful, return `true`.
///
/// @param mutex - The mutex to lock
///
/// @return `true` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) bool cnx_recursive_basic_mutex_try_lock(
	CnxRecursiveBasicMutex* restrict mutex) // NOLINT
	__DISABLE_IF_NULL(mutex);
/// @brief Unlocks the recursive mutex pointed to by `mutex`
///
/// Unlocking a mutex can fail (for example, if it's already unlocked).
/// If unlocking the recursive mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @param mutex - The mutex to unlock
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_recursive_basic_mutex_unlock(CnxRecursiveBasicMutex* restrict mutex) // NOLINT
	__DISABLE_IF_NULL(mutex);
/// @brief Destroys the recursive mutex pointed to by `mutex`
///
/// Destroying a mutex can fail (for example, it it's still in use by other threads).
/// If destroying the recursive mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @param mutex - The mutex to free
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_recursive_basic_mutex_free(CnxRecursiveBasicMutex* restrict mutex) // NOLINT
	__DISABLE_IF_NULL(mutex);

#undef __DISABLE_IF_NULL

#define __DISABLE_IF_NULL(condvar) \
	cnx_disable_if(!(condvar), "Can't perform a condvar operation on a nullptr")

/// @brief Creates a new condition variable, returning the result in a `CnxResult`
///
/// Creating a condition variable can fail, depending on memory and operating system level
/// constraints.
/// If creating the condition variable is successful, returns a `CnxBasicCondvar`, otherwise returns
/// an error
///
/// @return A `CnxRecursiveBasicMutex` if successful
/// @ingroup cnx_thread
__attr(nodiscard) CnxResult(CnxBasicCondvar) cnx_basic_condvar_new(void);
/// @brief Initializes the condition variable pointed to by `condvar`
///
/// Initializing a condition variable can fail, depending on memory and operating system level
/// constraints.
/// If initializing the condition variable is successful, returns `Ok`, otherwise returns an error
///
/// @param condvar - The condvar to initialize
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(not_null(1)) CnxResult cnx_basic_condvar_init(CnxBasicCondvar* restrict condvar)
	__DISABLE_IF_NULL(condvar); // NOLINT
/// @brief Signals to the first thread waiting on the condition variable pointed to by `condvar` to
/// wake and continue execution.
///
/// Signalling with a condition variable can fail.
/// If signalling with the condition variable is successful, returns `Ok`, otherwise returns an
/// error.
///
/// @param condvar - The condvar to signal on
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_basic_condvar_signal(CnxBasicCondvar* restrict condvar)
		__DISABLE_IF_NULL(condvar); // NOLINT
/// @brief Signals to every thread waiting on the condition variable pointed to by `condvar` to
/// wake and continue execution.
///
/// Signalling with a condition variable can fail.
/// If signalling with the condition variable is successful, returns `Ok`, otherwise returns an
/// error.
///
/// @param condvar - The condvar to broadcast on
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_basic_condvar_broadcast(CnxBasicCondvar* restrict condvar)
		__DISABLE_IF_NULL(condvar); // NOLINT
/// @brief Blocks on the condition variable pointed to by `condvar` until the thread is signalled
/// by it.
///
/// Unlocks the mutex pointed to by `mutex` and blocks on the condition variable pointed to
/// by `condvar` until the thread is signalled by it. The mutex is re-locked again before the
/// function returns. The mutex must be locked by the calling thread prior to calling this.
///
/// Waiting on a condition variable can fail.
/// If waiting on the condition variable is successful, returns `Ok`, otherwise returns an
/// error.
///
/// @param condvar - The condvar to wait on
/// @param mutex - The mutex associated with the condvar
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_basic_condvar_wait(CnxBasicCondvar* restrict condvar,
						   CnxBasicMutex* restrict mutex) // NOLINT
	__DISABLE_IF_NULL(condvar);
/// @brief Blocks on the condition variable pointed to by `condvar` until the thread is signalled
/// by it, or `to_wait` time has elapsed.
///
/// Unlocks the mutex pointed to by `mutex` and blocks on the condition variable pointed to
/// by `condvar` until the thread is signalled by it, or `to_wait` time has elapsed. The mutex is
/// re-locked again before the function returns. The mutex must be locked by the calling thread
/// prior to calling this.
///
/// Waiting on a condition variable can fail.
/// Returns `Ok` if a signal is received from the condition variable before `to_wait` has elapsed,
/// otherwise returns an error.
///
/// @param condvar - The condvar to wait on
/// @param mutex - The mutex associated with the condvar
/// @param to_wait - The amount of time to wait on the condvar
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1, 2)) CnxResult
	cnx_basic_condvar_wait_for(CnxBasicCondvar* restrict condvar,
							   // NOLINTNEXTLINE(readability-non-const-parameter)
							   CnxBasicMutex* restrict mutex,
							   CnxDuration to_wait) __DISABLE_IF_NULL(condvar)
		cnx_disable_if(!mutex, "Can't do a timed wait with a null mutex");
/// @brief Blocks on the condition variable pointed to by `condvar` until the thread is signalled
/// by it, or the point in time indicated by `stop_point` time has been reached.
///
/// Unlocks the mutex pointed to by `mutex` and blocks on the condition variable pointed to
/// by `condvar` until the thread is signalled by it, or the time indicated by `stop_point` has
/// been reached. The mutex is re-locked again before the function returns. The mutex must be locked
/// by the calling thread prior to calling this.
///
/// Waiting on a condition variable can fail.
/// Returns `Ok` if a signal is received from the condition variable before `to_wait` has elapsed,
/// otherwise returns an error.
///
/// @param condvar - The condvar to wait on
/// @param mutex - The mutex associated with the condvar
/// @param stop_point - The point in time to stop waiting on the condvar
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1, 2)) CnxResult
	cnx_basic_condvar_wait_until(CnxBasicCondvar* restrict condvar,
								 // NOLINTNEXTLINE(readability-non-const-parameter)
								 CnxBasicMutex* restrict mutex,
								 CnxTimePoint stop_point) __DISABLE_IF_NULL(condvar)
		cnx_disable_if(!mutex, "Can't do a timed wait with a null mutex");
/// @brief Destroys the condition variable pointed to by `condvar`
///
/// Destroying a condition variable can fail (for example, if its still in use by another thread).
/// If destroying the condition variable is successful, returns `Ok`, otherwise, returns an error.
///
/// @param condvar - The condvar to free
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_basic_condvar_free(CnxBasicCondvar* restrict condvar) __DISABLE_IF_NULL(condvar); // NOLINT

/// @brief Executes the given function exactly once.
///
/// Executes the given function exactly once, regardless how many threads attempt to execute
/// it, or if they attempt to execute it multiple times, as long as all attempts to call it go
/// through `cnx_execute_once` using the same `CnxOnceFlag`
///
/// Executing the function can fail (for example, on Windows).
/// If execution is successful, return `Ok`, otherwise returns an error.
///
/// @param flag - The execute once flag associated with `function`'s execution
/// @param function - The function to execute exactly once
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(not_null(1, 2)) CnxResult
	cnx_execute_once(CnxOnceFlag* restrict flag, void (*function)(void)) // NOLINT
	cnx_disable_if(!flag, "Can't perform an execute once with a null `CnxOnceFlag`")
		cnx_disable_if(!function, "Can't execute a nullptr");

/// @brief Compares the two `CnxThreadID`s
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return A `CnxCompare` indicating how `lhs` compares to `rhs`
/// @ingroup cnx_thread
__attr(nodiscard) CnxCompare cnx_thread_id_compare(CnxThreadID lhs, CnxThreadID rhs);
/// @brief Returns whether the two `CnxThreadID`s are equal
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return whether the two IDs are equal
/// @ingroup cnx_thread
__attr(nodiscard) bool cnx_thread_id_equal(CnxThreadID lhs, CnxThreadID rhs);
/// @brief Returns whether the first `CnxThreadID` is less than the second
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return whether `lhs` is less than `rhs`
/// @ingroup cnx_thread
__attr(nodiscard) bool cnx_thread_id_less_than(CnxThreadID lhs, CnxThreadID rhs);
/// @brief Returns whether the first `CnxThreadID` is less than or equal to the second
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return whether `lhs` is less than or equal to `rhs`
/// @ingroup cnx_thread
__attr(nodiscard) bool cnx_thread_id_less_than_or_equal(CnxThreadID lhs, CnxThreadID rhs);
/// @brief Returns whether the first `CnxThreadID` is greater than the second
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return whether `lhs` is greater than `rhs`
/// @ingroup cnx_thread
__attr(nodiscard) bool cnx_thread_id_greater_than(CnxThreadID lhs, CnxThreadID rhs);
/// @brief Returns whether the first `CnxThreadID` is greater than or equal to the second
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return whether `lhs` is greater than or equal to `rhs`
/// @ingroup cnx_thread
__attr(nodiscard) bool cnx_thread_id_greater_than_or_equal(CnxThreadID lhs, CnxThreadID rhs);

/// @brief When spawning a new thread, a `Lambda(void)` is used as its startup routine. This typedef
/// allows for receiving that startup routine as a function parameter.
/// @ingroup cnx_thread
typedef Lambda(void) CnxThreadLambda;

#undef __DISABLE_IF_NULL

#define __DISABLE_IF_NULL(thread) \
	cnx_disable_if(!(thread), "Can't perform a thread operation on a nullptr")

/// @brief Spawns a new thread, with the given `CnxThreadLambda` as its startup routine.
///
/// Spawns a new thread, with the given `CnxThreadLambda` as its startup routine. The given
/// lambda will be automatically freed after the thread's execution completes.
///
/// Spawning a thread can fail due to memory or operating system level constraints.
/// If spawning the thread is successful, returns a handle to the spawned thread.
/// Otherwise, returns an error.
///
/// @param lambda - The lambda to invoke as the spawned thread's startup routine
///
/// @return A `CnxThread` on success
/// @ingroup cnx_thread
__attr(nodiscard) CnxResult(CnxThread) cnx_thread_new(CnxThreadLambda lambda);
/// @brief Spawns a new thread, with the given `CnxThreadLambda` as its startup routine.
///
/// Spawns a new thread, with the given `CnxThreadLambda` as its startup routine. The given
/// lambda will be automatically freed after the thread's execution completes.
///
/// Spawning a thread can fail due to memory or operating system level constraints.
/// If spawning the thread is successful, the `CnxThread` pointed to by `thread` will be initialized
/// with the handle to the spawned thread, and `Ok` will be returned. Otherwise, returns an error.
///
/// @param thread - The pointer to thread handle to set as the handle to the spawned thread
/// @param lambda - The lambda to invoke as the spawned thread's startup routine
///
/// @return `Ok` on success
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_thread_init(CnxThread* restrict thread, CnxThreadLambda lambda) // NOLINT
	__DISABLE_IF_NULL(thread);
/// @brief Checks if the given thread handle is null (if it has been initialized)
///
/// @param thread - The thread to check if is null
///
/// @return whether the thread is null
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) bool cnx_thread_is_null(const CnxThread* restrict thread)
	__DISABLE_IF_NULL(thread);
/// @brief Gets the ID of the current thread
///
/// @param thread - The thread to get the ID of
///
/// @return The ID of the given thread
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxThreadID
	cnx_thread_get_id(const CnxThread* restrict thread) __DISABLE_IF_NULL(thread);
/// @brief Joins the given thread, blocking until its execution has completed
///
/// Joining a thread can fail.
/// Returns `Ok` if joining the thread is successful. Otherwise, returns an error.
///
/// @param thread - The thread to join
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_thread_join(CnxThread* restrict thread) // NOLINT
	__DISABLE_IF_NULL(thread);
/// @brief Separates execution of the thread associated with the given thread handle from the handle
///
/// Separates execution of the thread associated with the given handle from that handle. When
/// successfully detached, the associated thread will continue execution independently and will no
/// longer be associated with any handle.
///
/// Detaching a thread can fail.
/// Returns `Ok` if detaching the thread is successful. Otherwise, returns an error.
///
/// @param thread - The thread to detach
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_thread_detach(CnxThread* restrict thread) // NOLINT(readability-non-const-parameter)
	__DISABLE_IF_NULL(thread);
/// @brief Frees the given thread, blocking until it's joined or joining fails.
///
/// @param thread - The thread to free
/// @ingroup cnx_thread
__attr(not_null(1)) void cnx_thread_free(void* thread) __DISABLE_IF_NULL(thread); // NOLINT

#define CnxScopedThread scoped(cnx_thread_free)

/// @brief Yields execution of the current thread, allowing the operating system to execute other
/// threads until it decides to return execution to this one.
/// @ingroup cnx_thread
void cnx_this_thread_yield(void);
/// @brief Yields execution of the current thread until at least the given `duration` of time has
/// passed
///
/// @param duration - The minimum amount of time to yield execution for
/// @ingroup cnx_thread
void cnx_this_thread_sleep_for(CnxDuration duration);
/// @brief Returns the ID of the current thread
///
/// @return The ID of the current thread
/// @ingroup cnx_thread
__attr(nodiscard) CnxThreadID cnx_this_thread_get_id(void);

#undef __DISABLE_IF_NULL

#define __DISABLE_IF_NULL(token) \
	cnx_disable_if(!(token), "Can't perform a CnxStopToken operation operation on a nullptr")

/// @brief Requests the thread associated with the given stop token to end execution
///
/// @param token - The token associated with the thread to end
/// @ingroup cnx_thread
__attr(not_null(1)) void cnx_stop_token_request_stop(CnxStopToken* restrict token)
	__DISABLE_IF_NULL(token);
/// @brief Returns whether the thread associated with the given stop token has been requested to end
/// execution
///
/// @param token - The token associated with the thread to end
///
/// @return whether the thread has been requested to end
/// @ingroup cnx_thread
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_stop_token_stop_requested(const CnxStopToken* restrict token)
		__DISABLE_IF_NULL(token);

/// @brief When spawning a new `CnxJThread`, a `Lambda(void, const CnxStopToken*)` is used as its
/// startup routine. This typedef allows for receiving that startup routine as a function parameter.
/// @ingroup cnx_thread
typedef Lambda(void, const CnxStopToken*) CnxJThreadLambda;

#undef __DISABLE_IF_NULL

#define __DISABLE_IF_NULL(thread) \
	cnx_disable_if(!(thread), "Can't perform a CnxJThread operation operation on a nullptr")

/// @brief Spawns a new `CnxJThread`, with the given `CnxJThreadLambda` as its startup routine.
///
/// Spawns a new thread, with the given `CnxJThreadLambda` as its startup routine. The given
/// lambda will be automatically freed after the thread's execution completes.
///
/// Spawning a thread can fail due to memory or operating system level constraints.
/// If spawning the thread is successful, returns a handle to the spawned thread.
/// Otherwise, returns an error.
///
/// @param lambda - The lambda to invoke as the spawned thread's startup routine
///
/// @return A `CnxJThread` on success
/// @ingroup cnx_thread
__attr(nodiscard) CnxResult(CnxJThread) cnx_jthread_new(CnxJThreadLambda lambda);
/// @brief Spawns a new `CnxJThread`, with the given `CnxJThreadLambda` as its startup routine.
///
/// Spawns a new thread, with the given `CnxJThreadLambda` as its startup routine. The given
/// lambda will be automatically freed after the thread's execution completes.
///
/// Spawning a thread can fail due to memory or operating system level constraints.
/// If spawning the thread is successful, the `CnxThread` pointed to by `thread` will be initialized
/// with the handle to the spawned thread, and `Ok` will be returned. Otherwise, returns an error.
///
/// @param thread - The pointer to thread handle to set as the handle to the spawned thread
/// @param lambda - The lambda to invoke as the spawned thread's startup routine
///
/// @return `Ok` on success
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_jthread_init(CnxJThread* restrict thread, CnxJThreadLambda lambda) // NOLINT
	__DISABLE_IF_NULL(thread);
/// @brief Checks if the given thread handle is null (if it has been initialized)
///
/// @param thread - The thread to check if is null
///
/// @return whether the thread is null
/// @ingroup cnx_thread
#define cnx_jthread_is_null(thread) (cnx_thread_is_null(&((thread)->handle)))
/// @brief Gets the ID of the current thread
///
/// @param thread - The thread to get the ID of
///
/// @return The ID of the given thread
/// @ingroup cnx_thread
#define cnx_jthread_get_id(thread) (cnx_thread_get_id(&((thread)->handle)))
/// @brief Joins the given `CnxJThread`, blocking until its execution has completed
///
/// Joining a thread can fail.
/// Returns `Ok` if joining the thread is successful. Otherwise, returns an error.
///
/// @param thread - The thread to join
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult cnx_jthread_join(CnxJThread* restrict thread)
	__DISABLE_IF_NULL(thread);
/// @brief Separates execution of the thread associated with the `CnxJThread` thread handle from the
/// handle
///
/// Separates execution of the thread associated with the given handle from that handle. When
/// successfully detached, the associated thread will continue execution independently and will no
/// longer be associated with any handle.
///
/// Detaching a thread can fail.
/// Returns `Ok` if detaching the thread is successful. Otherwise, returns an error.
///
/// @param thread - The thread to detach
///
/// @return `Ok` if successful
/// @ingroup cnx_thread
#define cnx_jthread_detach(thread) (cnx_thread_detach(&((thread)->handle)))
/// @brief Frees the given `CnxJThread`, blocking until it's joined or joining fails.
///
/// @param thread - The thread to free
/// @ingroup cnx_thread
__attr(not_null(1)) void cnx_jthread_free(void* thread) __DISABLE_IF_NULL(thread);

#define CnxScopedJThread scoped(cnx_jthread_free)

#undef __DISABLE_IF_NULL

#define __DISABLE_IF_NULL(tls) \
	cnx_disable_if(!(tls), "Can't perform a thread-local storage operation on a nullptr")

/// @brief Creates a new thread-local storage
///
/// Creates a new thread-local storage that will be destroyed by the given `destructor` function at
/// thread exit.
///
/// Creating a thread-local storage can fail.
/// If creation is successful, the storage will be set to `data` and the `CnxTLSKey` associated with
/// the storage will be returned. Otherwise, an error will be returned.
///
/// @param data - The value to initialize the TLS to
/// @param destructor - The destructor function to destroy the TLS with at thread exit
///
/// @return A `CnxTLSKey` on success
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxTLSKey)
	cnx_tls_new(void* data, void(__CNX_TLS_DESTRUCTOR_TAG* destructor)(void*))
		__DISABLE_IF_NULL(data);
/// @brief Initializes a thread-local storage and associates it with the key pointed to by `key`
///
/// Initializes a new thread-local storage that will be destroyed by the given `destructor` function
/// at thread exit, and associates the thread-local storage with the key pointed to by `key`.
///
/// Creating a thread-local storage can fail.
/// If creation is successful, the storage will be set to `data` and the `CnxTLSKey` associated with
/// the storage will be returned. Otherwise, an error will be returned.
///
/// @param data - The value to initialize the TLS to
/// @param destructor - The destructor function to destroy the TLS with at thread exit
///
/// @return A `CnxTLSKey` on success
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(1, 2)) CnxResult
	cnx_tls_init(CnxTLSKey* restrict key, // NOLINT
				 void* data,
				 void(__CNX_TLS_DESTRUCTOR_TAG* destructor)(void*)) __DISABLE_IF_NULL(key)
		__DISABLE_IF_NULL(data);
/// @brief Retrieves the current value of the thread-local storage associated with `key`
///
/// If key is a key to a valid TLS, returns the current value of the  TLS, otherwise returns a
/// `nullptr`
///
/// @param key - The thread-local storage key to get the currently associated value of
///
/// @return The current value of the TLS, or `nullptr`
/// @ingroup cnx_thread
__attr(nodiscard) __attr(returns_not_null) void* cnx_tls_get(CnxTLSKey key);
/// @brief Sets the value of the thread-local storage associated with `key` to `data`
///
/// Setting the value of a thread-local storage can fail.
/// If setting the thread-local storage is successful, returns `Ok`. Otherwise, an error is returned
///
/// @param key - The thread-local storage key to set the associated value of
/// @param data - The value to set the TLS to
///
/// @return `Ok` on successful
/// @ingroup cnx_thread
__attr(nodiscard) __attr(not_null(2)) CnxResult cnx_tls_set(CnxTLSKey key, void* data)
	__DISABLE_IF_NULL(data);

#undef __DISABLE_IF_NULL

#endif // CNX_THREAD
