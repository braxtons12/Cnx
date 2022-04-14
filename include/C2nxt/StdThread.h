/// @file StdThread.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Type definitions and function declarations for threading functionality
/// @version 0.2.0
/// @date 2022-04-13
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

/// @defgroup std_thread Threading and Synchronization
/// C2nxt provides a set of cross-platform multithreading and synchronization facilities to make
/// working with concurrent code as simple and straightforward as possible. The API is similar to
/// C++'s and that added by the C11 standard, but is supported on platforms and compilers where
/// implementations of C11's threading may not be available (like Windows, for example).
///
/// Example:
/// @code {.c}
/// #include <C2nxt/StdThread.h>
/// #include <C2nxt/StdIO.h>
/// #include <C2nxt/StdAtomic.h>
/// #include <C2nxt/StdAssert.h>
///
/// // A lambda that captures a `StdBasicMutex*`, two `i32*`s and an `atomic_bool`*
/// void LambdaFunction(example_print, void) {
/// 	let bindings = LambdaBinding(StdBasicMutex*, i32*, i32*, atomic_bool*);
///
/// 	// run until the exit flag is set
/// 	let exit_flag_ptr = bindings._4;
/// 	while(!atomic_load(exit_flag_ptr)) {
/// 		let_mut mutex_ptr = bindings._1;
/// 		let locked = std_basic_mutex_lock(mutex_ptr);
///
/// 		std_assert(std_result_is_ok(locked), "Failed to lock the mutex");
/// 		if(std_result_is_ok(locked)) {
/// 			let num1 = *(bindings._2);
/// 			let num2 = *(bindings._3);
/// 			println("Values: [{}, {}]", num1, num2);
/// 		}
/// 	}
/// }
///
/// // A lambda that captures a `StdBasicMutex*`, two `i32*`s and an `atomic_bool`*
/// void LambdaFunction(example_add, void) {
/// 	let bindings = LambdaBinding(StdBasicMutex*, i32*, i32*, atomic_bool*);
///
/// 	// run until the exit flag is set
/// 	let exit_flag_ptr = bindings._4;
/// 	while(!atomic_load(exit_flag_ptr)) {
/// 		let_mut mutex_ptr = bindings._1;
/// 		let locked = std_basic_mutex_lock(mutex_ptr);
//
/// 		std_assert(std_result_is_ok(locked), "Failed to lock the mutex");
/// 		if(std_result_is_ok(locked)) {
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
/// 	let_mut maybe_mutex = std_basic_mutex_new();
///
/// 	std_assert(std_result_is_ok(maybe_mutex), "Failed to create a new mutex");
/// 	let_mut mutex = std_result_unwrap(maybe_mutex);
///
/// 	// creates a new thread whose startup routine is the `LambdaFunction` `example_print`
/// 	// the lambda will be automatically freed when the thread completes execution
/// 	let_mut maybe_print_thread = std_thread_new(lambda(example_print, &mutex,
/// 																	  &val1,
/// 																	  &val2,
/// 																	  &exit_flag));
/// 	std_assert(std_result_is_ok(maybe_print_thread), "Failed to create print thread");
/// 	let_mut print_thread = std_result_unwrap(maybe_print_thread);
///
/// 	// creates a new thread whose startup routine is the `LambdaFunction` `example_add`
/// 	// the lambda will be automatically freed when the thread completes execution
/// 	let_mut maybe_add_thread = std_thread_new(lambda(example_add, &mutex,
/// 																  &val1,
/// 																  &val2,
/// 																  &exit_flag));
/// 	std_assert(std_result_is_ok(maybe_add_thread), "Failed to create add thread");
/// 	let_mut add_thread = std_result_unwrap(maybe_add_thread);
///
/// 	std_this_thread_sleep_for(std_seconds(60));
///
/// 	atomic_store(&exit_flag, true);
/// 	let print_joined = std_thread_join(&print_thread);
/// 	let add_joined = std_thread_join(&add_thread);
/// 	std_assert(std_result_is_ok(print_joined), "Print thread failed to join");
/// 	std_assert(std_result_is_ok(add_joined), "Adding thread failed to join");
///
/// 	std_basic_mutex_free(&mutex);
///
/// 	println("Finshed printing the numbers!");
/// }
/// @endcode

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

/// @brief `StdBasicMutex` is the basic mutual exclusion object type provided by C2nxt's threading
/// and synchronization API. Using a `StdBasicMutex` to control access to shared resources will
/// ensure that all access is synchronized and mutually exclusive between threads.
/// @ingroup std_thread
typedef __std_basic_mutex StdBasicMutex;
/// @brief `StdRecursiveBasicMutex` is the basic reentrant mutual exclusion object type provided by
/// C2nxt's threading and synchronization API. Using a `StdRecursiveBasicMutex` to control access to
/// shared resources will ensure that all access is synchronized and mutually exclusive between
/// threads, and allows for rentrant ownership of the lock on that access.
/// @ingroup std_thread
typedef __std_recursive_basic_mutex StdRecursiveBasicMutex;
/// @brief `StdCondvar` is the Condition Variable object type provided by C2nxt's threading and
/// synchronization API. Using a `StdCondvar` allows to block one or more threads until an event is
/// triggered, without wasting CPU resources.
/// @ingroup std_thread
typedef __std_condvar StdCondvar;
/// @brief A `StdThreadID` uniquely identifies an indiviual thread
/// @ingroup std_thread
typedef __std_thread_id StdThreadID;
/// @brief A `StdThread` is the handle type for basic OS-level threads provided by C2nxt's threading
/// and synchronization API. `StdThread` provides the facilities for creating multiple threads and
/// ending their execution, enabling concurrent tasks and operations to be performed.
/// @ingroup std_thread
typedef __std_thread StdThread;
/// @brief A `StdStopToken` is associated with a `StdJThread` and is used to signal to it when it
/// should end its execution.
/// @ingroup std_thread
typedef atomic_bool* StdStopToken;
/// @brief A `StdJThread` is the handle type for OS-level threads which are automatically joined
/// when their handle goes out of scope and use a dedicated stop token to signal to the thread when
/// its execution should halt.
/// @ingroup std_thread
typedef struct {
	StdThread m_handle;
	StdStopToken stop_token;
} StdJThread;
/// @brief A `StdTLSKey` is the key type for creating, accessing, and modifying a thread-local
/// storage object
/// @ingroup std_thread
typedef __std_tls_key StdTLSKey;
/// @brief A `StdOnceFlag` is a synchronization flag type for use with `std_execute_once` to ensure
/// a given function is executed exactly once, regardless of how many threads attempt to call it,
/// as long as all threads do so through the same `StdOnceFlag` and `std_execute_once`
/// @ingroup std_thread
typedef __std_exec_once_flag StdOnceFlag;

#ifndef thread_local
	/// @brief A variable declared as `thread_local` is local to the current thread and guaranteed
	/// to not be accessible by other threads
	/// @ingroup std_thread
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

/// @brief Creates a new mutex
///
/// Creating a mutex can fail, depending on memory and operating system level constraints.
/// If creating the mutex is successful, returns a `StdBascicMutex`, otherwise returns an error.
///
/// @return A `StdBasicMutex` if successful
/// @ingroup std_thread
[[nodiscard]] StdResult(StdBasicMutex) std_basic_mutex_new(void);
/// @brief Initializes the mutex pointed to by `mutex`
///
/// Initializing a mutex can fail, depending on memory and operating system level constraints.
/// If initializing the mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_basic_mutex_init(StdBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unconditionally locks the mutex pointed to by `mutex`
///
/// Locking a mutex can fail (for example, if it's already locked on the calling thread).
/// If locking the mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_basic_mutex_lock(StdBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Attempts to lock the mutex pointed to by `mutex`
///
/// If locking the mutex is successful, return `true`.
//
/// @return `true` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] bool
std_basic_mutex_try_lock(StdBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Unlocks the mutex pointed to by `mutex`
///
/// Unlocking a mutex can fail (for example, if it's already unlocked).
/// If unlocking the mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_basic_mutex_unlock(StdBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex); // NOLINT
/// @brief Destroys the mutex pointed to by `mutex`
///
/// Destroying a mutex can fail (for example, it it's still in use by other threads).
/// If destroying the mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_basic_mutex_free(StdBasicMutex* restrict mutex) ___DISABLE_IF_NULL(mutex); // NOLINT

/// @brief Creates a new recursive mutex
///
/// Creating a mutex can fail, depending on memory and operating system level constraints.
/// If creating the recursive mutex is successful, returns a `StdRecursiveBascicMutex`, otherwise
/// returns an error.
//
/// @return A `StdRecursiveBasicMutex` if successful
/// @ingroup std_thread
[[nodiscard]] StdResult(StdRecursiveBasicMutex) std_recursive_basic_mutex_new(void);
/// @brief Initializes the recursive mutex pointed to by `mutex`
///
/// Initializing a mutex can fail, depending on memory and operating system level constraints.
/// If initializing the mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_recursive_basic_mutex_init(StdRecursiveBasicMutex* restrict mutex) // NOLINT
	___DISABLE_IF_NULL(mutex);
/// @brief Unconditionally locks the recursive mutex pointed to by `mutex`
///
/// Locking a mutex can fail.
/// If locking the recursive mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_recursive_basic_mutex_lock(StdRecursiveBasicMutex* restrict mutex) // NOLINT
	___DISABLE_IF_NULL(mutex);
/// @brief Attempts to lock the recursive mutex pointed to by `mutex`
///
/// If locking the recursive mutex is successful, return `true`.
///
/// @return `true` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] bool
std_recursive_basic_mutex_try_lock(StdRecursiveBasicMutex* restrict mutex) // NOLINT
	___DISABLE_IF_NULL(mutex);
/// @brief Unlocks the recursive mutex pointed to by `mutex`
///
/// Unlocking a mutex can fail (for example, if it's already unlocked).
/// If unlocking the recursive mutex is successful, returns `Ok`, otherwise returns an error.
//
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_recursive_basic_mutex_unlock(StdRecursiveBasicMutex* restrict mutex) // NOLINT
	___DISABLE_IF_NULL(mutex);
/// @brief Destroys the recursive mutex pointed to by `mutex`
///
/// Destroying a mutex can fail (for example, it it's still in use by other threads).
/// If destroying the recursive mutex is successful, returns `Ok`, otherwise returns an error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_recursive_basic_mutex_free(StdRecursiveBasicMutex* restrict mutex) // NOLINT
	___DISABLE_IF_NULL(mutex);

#undef ___DISABLE_IF_NULL

#define ___DISABLE_IF_NULL(condvar) \
	std_disable_if(!(condvar), "Can't perform a condvar operation on a nullptr")

/// @brief Creates a new condition variable, returning the result in a `StdResult`
///
/// Creating a condition variable can fail, depending on memory and operating system level
/// constraints.
/// If creating the condition variable is successful, returns a `StdCondvar`, otherwise returns an
/// error
///
/// @return A `StdRecursiveBasicMutex` if successful
/// @ingroup std_thread
[[nodiscard]] StdResult(StdCondvar) std_condvar_new(void);
/// @brief Initializes the condition variable pointed to by `condvar`
///
/// Initializing a condition variable can fail, depending on memory and operating system level
/// constraints.
/// If initializing the condition variable is successful, returns `Ok`, otherwise returns an error
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[not_null(1)]] StdResult
std_condvar_init(StdCondvar* restrict condvar) ___DISABLE_IF_NULL(condvar); // NOLINT
/// @brief Signals to the first thread waiting on the condition variable pointed to by `condvar` to
/// wake and continue execution.
///
/// Signalling with a condition variable can fail.
/// If signalling with the condition variable is successful, returns `Ok`, otherwise returns an
/// error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_condvar_signal(StdCondvar* restrict condvar) ___DISABLE_IF_NULL(condvar); // NOLINT
/// @brief Signals to every thread waiting on the condition variable pointed to by `condvar` to
/// wake and continue execution.
///
/// Signalling with a condition variable can fail.
/// If signalling with the condition variable is successful, returns `Ok`, otherwise returns an
/// error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_condvar_broadcast(StdCondvar* restrict condvar) ___DISABLE_IF_NULL(condvar); // NOLINT
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
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_condvar_wait(StdCondvar* restrict condvar, StdBasicMutex* restrict mutex) // NOLINT
	___DISABLE_IF_NULL(condvar);
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
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1, 2)]] StdResult
std_condvar_wait_for(StdCondvar* restrict condvar,
					 // NOLINTNEXTLINE(readability-non-const-parameter)
					 StdBasicMutex* restrict mutex,
					 StdDuration to_wait) ___DISABLE_IF_NULL(condvar)
	std_disable_if(!mutex, "Can't do a timed wait with a null mutex");
/// @brief Destroys the condition variable pointed to by `condvar`
///
/// Destroying a condition variable can fail (for example, if its still in use by another thread).
/// If destroying the condition variable is successful, returns `Ok`, otherwise, returns an error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_condvar_free(StdCondvar* restrict condvar) ___DISABLE_IF_NULL(condvar); // NOLINT

/// @brief Executes the given function exactly once.
///
/// Executes the given function exactly once, regardless how many threads attempt to execute
/// it, or if they attempt to execute it multiple times, as long as all attempts to call it go
/// through `std_execute_once` using the same `StdOnceFlag`
///
/// Executing the function can fail (for example, on Windows).
/// If execution is successful, return `Ok`, otherwise returns an error.
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[not_null(1, 2)]] StdResult
std_execute_once(StdOnceFlag* restrict flag, void (*function)(void)) // NOLINT
	std_disable_if(!flag, "Can't perform an execute once with a null `StdOnceFlag`")
		std_disable_if(!function, "Can't execute a nullptr");

/// @brief Compares the two `StdThreadID`s
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return A `StdCompare` indicating how `lhs` compares to `rhs`
/// @ingroup std_thread
[[nodiscard]] StdCompare std_thread_id_compare(StdThreadID lhs, StdThreadID rhs);
/// @brief Returns whether the two `StdThreadID`s are equal
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return whether the two IDs are equal
/// @ingroup std_thread
[[nodiscard]] bool std_thread_id_equal(StdThreadID lhs, StdThreadID rhs);
/// @brief Returns whether the first `StdThreadID` is less than the second
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return whether `lhs` is less than `rhs`
/// @ingroup std_thread
[[nodiscard]] bool std_thread_id_less_than(StdThreadID lhs, StdThreadID rhs);
/// @brief Returns whether the first `StdThreadID` is less than or equal to the second
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return whether `lhs` is less than or equal to `rhs`
/// @ingroup std_thread
[[nodiscard]] bool std_thread_id_less_than_or_equal(StdThreadID lhs, StdThreadID rhs);
/// @brief Returns whether the first `StdThreadID` is greater than the second
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return whether `lhs` is greater than `rhs`
/// @ingroup std_thread
[[nodiscard]] bool std_thread_id_greater_than(StdThreadID lhs, StdThreadID rhs);
/// @brief Returns whether the first `StdThreadID` is greater than or equal to the second
///
/// @param lhs - The id to compare
/// @param rhs - The id to compare `lhs` to
///
/// @return whether `lhs` is greater than or equal to `rhs`
/// @ingroup std_thread
[[nodiscard]] bool std_thread_id_greater_than_or_equal(StdThreadID lhs, StdThreadID rhs);

/// @brief When spawning a new thread, a `Lambda(void)` is used as its startup routine. This typedef
/// allows for receiving that startup routine as a function parameter.
/// @ingroup std_thread
typedef Lambda(void) StdThreadLambda;

#undef ___DISABLE_IF_NULL

#define ___DISABLE_IF_NULL(thread) \
	std_disable_if(!(thread), "Can't perform a thread operation on a nullptr")

/// @brief Spawns a new thread, with the given `StdThreadLambda` as its startup routine.
///
/// Spawns a new thread, with the given `StdThreadLambda` as its startup routine. The given
/// lambda will be automatically freed after the thread's execution completes.
///
/// Spawning a thread can fail due to memory or operating system level constraints.
/// If spawning the thread is successful, returns a handle to the spawned thread.
/// Otherwise, returns an error.
///
/// @param lambda - The lambda to invoke as the spawned thread's startup routine
///
/// @return A `StdThread` on success
/// @ingroup std_thread
[[nodiscard]] StdResult(StdThread) std_thread_new(StdThreadLambda lambda);
/// @brief Spawns a new thread, with the given `StdThreadLambda` as its startup routine.
///
/// Spawns a new thread, with the given `StdThreadLambda` as its startup routine. The given
/// lambda will be automatically freed after the thread's execution completes.
///
/// Spawning a thread can fail due to memory or operating system level constraints.
/// If spawning the thread is successful, the `StdThread` pointed to by `thread` will be initialized
/// with the handle to the spawned thread, and `Ok` will be returned. Otherwise, returns an error.
///
/// @param thread - The pointer to thread handle to set as the handle to the spawned thread
/// @param lambda - The lambda to invoke as the spawned thread's startup routine
///
/// @return `Ok` on success
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_thread_init(StdThread* restrict thread, StdThreadLambda lambda) // NOLINT
	___DISABLE_IF_NULL(thread);
/// @brief Checks if the given thread handle is null (if it has been initialized)
///
/// @param thread - The thread to check if is null
///
/// @return whether the thread is null
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] bool
std_thread_is_null(const StdThread* restrict thread) ___DISABLE_IF_NULL(thread);
/// @brief Gets the ID of the current thread
///
/// @param thread - The thread to get the ID of
///
/// @return The ID of the given thread
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdThreadID
std_thread_get_id(const StdThread* restrict thread) ___DISABLE_IF_NULL(thread);
/// @brief Joins the given thread, blocking until its execution has completed
///
/// Joining a thread can fail.
/// Returns `Ok` if joining the thread is successful. Otherwise, returns an error.
///
/// @param thread - The thread to join
///
/// @return `Ok` if successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult std_thread_join(StdThread* restrict thread) // NOLINT
	___DISABLE_IF_NULL(thread);
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
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult
std_thread_detach(StdThread* restrict thread) ___DISABLE_IF_NULL(thread); // NOLINT
/// @brief Frees the given thread, blocking until it's joined or joining fails.
///
/// @param thread - The thread to free
/// @ingroup std_thread
[[not_null(1)]] void std_thread_free(void* thread) ___DISABLE_IF_NULL(thread); // NOLINT

#define StdJThread scoped(std_thread_free)

/// @brief Yields execution of the current thread, allowing the operating system to execute other
/// threads until it decides to return execution to this one.
/// @ingroup std_thread
void std_this_thread_yield(void);
/// @brief Yields execution of the current thread until at least the given `duration` of time has
/// passed
///
/// @param duration - The minimum amount of time to yield execution for
/// @ingroup std_thread
void std_this_thread_sleep_for(StdDuration duration);
/// @brief Returns the ID of the current thread
///
/// @return The ID of the current thread
/// @ingroup std_thread
[[nodiscard]] StdThreadID std_this_thread_get_id(void);

#undef ___DISABLE_IF_NULL

#define ___DISABLE_IF_NULL(tls) \
	std_disable_if(!(tls), "Can't perform a thread-local storage operation on a nullptr")

/// @brief Creates a new thread-local storage
///
/// Creates a new thread-local storage that will be destroyed by the given `destructor` function at
/// thread exit.
///
/// Creating a thread-local storage can fail.
/// If creation is successful, the storage will be set to `data` and the `StdTLSKey` associated with
/// the storage will be returned. Otherwise, an error will be returned.
///
/// @param data - The value to initialize the TLS to
/// @param destructor - The destructor function to destroy the TLS with at thread exit
///
/// @return A `StdTLSKey` on success
/// @ingroup std_thread
[[nodiscard]] [[not_null(1)]] StdResult(StdTLSKey)
	std_tls_new(void* data, void(__STD_TLS_DESTRUCTOR_TAG* destructor)(void*))
		___DISABLE_IF_NULL(data);
/// @brief Initializes a thread-local storage and associates it with the key pointed to by `key`
///
/// Initializes a new thread-local storage that will be destroyed by the given `destructor` function
/// at thread exit, and associates the thread-local storage with the key pointed to by `key`.
///
/// Creating a thread-local storage can fail.
/// If creation is successful, the storage will be set to `data` and the `StdTLSKey` associated with
/// the storage will be returned. Otherwise, an error will be returned.
///
/// @param data - The value to initialize the TLS to
/// @param destructor - The destructor function to destroy the TLS with at thread exit
///
/// @return A `StdTLSKey` on success
/// @ingroup std_thread
[[nodiscard]] [[not_null(1, 2)]] StdResult
std_tls_init(StdTLSKey* restrict key, // NOLINT
			 void* data,
			 void(__STD_TLS_DESTRUCTOR_TAG* destructor)(void*)) ___DISABLE_IF_NULL(key)
	___DISABLE_IF_NULL(data);
/// @brief Retrieves the current value of the thread-local storage associated with `key`
///
/// If key is a key to a valid TLS, returns the current value of the  TLS, otherwise returns a
/// `nullptr`
///
/// @param key - The thread-local storage key to get the currently associated value of
///
/// @return The current value of the TLS, or `nullptr`
/// @ingroup std_thread
[[nodiscard]] [[returns_not_null]] void* std_tls_get(StdTLSKey key);
/// @brief Sets the value of the thread-local storage associated with `key` to `data`
///
/// Setting the value of a thread-local storage can fail.
/// If setting the thread-local storage is successful, returns `Ok`. Otherwise, an error is returned
///
/// @param key - The thread-local storage key to set the associated value of
/// @param data - The value to set the TLS to
///
/// @return `Ok` on successful
/// @ingroup std_thread
[[nodiscard]] [[not_null(2)]] StdResult
std_tls_set(StdTLSKey key, void* data) ___DISABLE_IF_NULL(data);

#undef ___DISABLE_IF_NULL

#endif // STD_THREAD
