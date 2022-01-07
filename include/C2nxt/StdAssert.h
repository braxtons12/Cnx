/// @file StdAssert.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides wrappers for standard C compile-time and runtime asserts and other
/// custom asserts and assert-like facilities
/// @version 0.1.1
/// @date 2022-01-07
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

/// @ingroup error_handling
/// @{
/// @defgroup asserts Asserts
/// Asserts provides various compile-time and runtime asserts and similar facilities, some of which
/// are wrappers for the standard C asserts, for enforcing invariants, run-time
/// constraints/contracts, and compile-time requirements
///
/// Example
/// @code {.c}
///
/// // i must be less than 10
/// void func(i32 i) {
/// 	// enforce that i is less than 10
/// 	std_assert(i < 10, "func called with i >= 10 (i must be less than 10");
/// 	// use i somehow...
/// }
///
/// void* my_alloc(usize size) {
/// 	// allocation code...
/// 	if(allocation_failed) {
/// 		// runtime abort with error message
///			std_panic("my_alloc failed to allocate memory!");
/// 	}
/// }
///
/// #ifndef THING
/// 	// THING must be less than 20
/// 	#define THING 12
/// #endif
/// // enforce THING < 20 at compile-time
/// std_static_assert(THING < 20, "THING defined as >= 20 (THING must be less than 20)");
///
/// #ifndef THING
/// 	// THING must be a usize
/// 	#define THING static_cast(usize)(12)
/// #endif
/// // enforce THING is a usize at compile-time
/// std_type_assert_v(usize, THING);
/// @endcode
/// @}
#include <stdio.h>
#include <stdlib.h>

#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdTypeTraits.h>

#ifndef STD_ASSERT
	/// @brief Declarations and definitions for C2nxt asserts and assert-like facilities
	#define STD_ASSERT

	/// @brief Invokes a panic with the given error message.
	///
	/// A panic is an immediate abort with error message. Panics should only be used in extreme
	/// circumstances of absolute program failure (for example, when an invariant is irrecoverably
	/// broken or on OOM).
	///
	/// @param error_message - The error message to print to `stderr` before aborting.
	/// @ingroup asserts
	#define std_panic(error_message)                                                     \
		({                                                                                \
			fprintf(stderr, "Panic at %s:%d : %s\n", __FILE__, __LINE__, error_message); \
			abort();                                                                     \
		})

	#if STD_PLATFORM_DEBUG && !defined(STD_DISABLE_ASSERTIONS)

// clang-format off

	/// @brief Asserts that the given condition is `true`. If `false`, prints the given file, line,
	/// and error message and aborts
	///
	/// @param condition - The condition that must be `true`
	/// @param error_message - The associated error message if `condition` is `false`
	/// @param file - The file the error occurred in
	/// @param line - The line number the error occurred at
	void std_assert_cstring(bool condition, const_cstring error_message, const_cstring file, i64 line);

		
	/// @brief Asserts that the given condition is `true`.
	///
	/// If `condition` is false, this will print the file and line at which the error occurred,
	/// along with the given error message, to `stderr`, then abort.
	///
	/// @param condition - The condition that must be `true`
	/// @param error_message - The associated error message if `condition` is `false`
	/// @ingroup asserts
	#define std_assert(condition, error_message) _Generic((error_message), 						   \
		const_cstring 						: 	std_assert_cstring, 						   	   \
		cstring 							: 	std_assert_cstring, 						   	   \
		const char[sizeof(error_message)] 	: 	std_assert_cstring, 						   	   \
		char[sizeof(error_message)] 		: 	std_assert_cstring) 						   	   \
		(condition, error_message, __FILE__, __LINE__)

	// clang-format on

	#else
		/// @brief Asserts that the given condition is `true`.
		///
		/// If `condition` is false, this will print the file and line at which the error occurred,
		/// along with the given error message, to `stderr`, then abort.
		///
		/// @param condition - The condition that must be `true`
		/// @param error_message - The associated error message if `condition` is `false`
		/// @ingroup asserts
		#define std_assert(condition, error_message)
	#endif // STD_PLATFORM_DEBUG && !defined(STD_DISABLE_ASSERTIONS)

	#if !defined(STD_DISABLE_ASSERTIONS)
		/// @brief Asserts that the given compile-time condition is `true`.
		///
		/// If `condition` is `false`, this will print the given error message and abort
		/// compilation.
		///
		/// @param condition - The condition that must be true at compile time
		/// @param error_message - The associated error message if `condition` is `false`
		/// @ingroup asserts
		#define std_static_assert(condition, error_message) _Static_assert(condition, error_message)

		/// @brief Asserts that the two types are equivalent at compile-time.
		///
		/// If `type1` and `type2` are not **strictly** equivalent, this will abort compilation with
		/// an error message.
		///
		/// @param type1 - The first type to compare
		/// @param type2 - The second type to compare
		/// @ingroup asserts
		#define std_type_assert(type1, type2)                \
			std_static_assert(std_types_equal(type1, type2), \
							  "The type `type1` must be the same as `type2`")

		/// @brief Asserts that the type `type` and the type of `value` are equivalent at
		/// compile-time.
		///
		/// If `type` and the type of `value` are not **strictly** equivalent, this will abort
		/// compilation with an error message.
		///
		/// @param type - The type to compare
		/// @param value - The value to compare the type of
		/// @ingroup asserts
		#define std_type_assert_v(type, value)                \
			std_static_assert(std_types_equal_v(type, value), \
							  "The type of `value` must be the same as the given `type`")

		/// @brief Asserts that the types of the two given values are equivalent at compile-time.
		///
		/// If the type of `value1` and the type of `value2` are not **strictly** equivalent, this
		/// will abort compilation with an error message.
		///
		/// @param value1 - The first value to compare the type of
		/// @param value2 - The second value to compare the type of
		/// @ingroup asserts
		#define std_type_assert_vs(value1, value2)                \
			std_static_assert(std_types_equal_vs(value1, value2), \
							  "The type of `value` must be the same as the given `type`")

	#else
		/// @brief Asserts that the given compile-time condition is `true`.
		///
		/// If `condition` is `false`, this will print the given error message and abort
		/// compilation.
		///
		/// @param condition - The condition that must be true at compile time
		/// @param error_message - The associated error message if `condition` is `false`
		/// @ingroup asserts
		#define std_static_assert(condition, error_message)
		/// @brief Asserts that the two types are equivalent at compile-time.
		///
		/// If `type1` and `type2` are not **strictly** equivalent, this will abort compilation with
		/// an error message.
		///
		/// @param type1 - The first type to compare
		/// @param type2 - The second type to compare
		/// @ingroup asserts
		#define std_type_assert(type, value)
		/// @brief Asserts that the type `type` and the type of `value` are equivalent at
		/// compile-time.
		///
		/// If `type` and the type of `value` are not **strictly** equivalent, this will abort
		/// compilation with an error message.
		///
		/// @param type - The type to compare
		/// @param value - The value to compare the type of
		/// @ingroup asserts
		#define std_type_assert_v(type, value)
		/// @brief Asserts that the types of the two given values are equivalent at compile-time.
		///
		/// If the type of `value1` and the type of `value2` are not **strictly** equivalent, this
		/// will abort compilation with an error message.
		///
		/// @param value1 - The first value to compare the type of
		/// @param value2 - The second value to compare the type of
		/// @ingroup asserts
		#define std_type_assert_vs(value1, value2)
	#endif // !defined(STD_DISABLE_ASSERTIONS)
#endif	   // STD_ASSERT
