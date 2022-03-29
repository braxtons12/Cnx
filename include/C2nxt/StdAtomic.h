/// @file StdAtomic.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides atomic operations dictated by the C11 standard,
/// either directly through those provided by the system libc (on NOT Windows) or through an
/// equivalent implementation (on Windows)
/// @version 0.2.0
/// @date 2022-03-22
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

#ifndef STD_ATOMIC
#define STD_ATOMIC

#include <C2nxt/StdPlatform.h>

#if !STD_PLATFORM_WINDOWS || STD_PLATFORM_COMPILER_GCC
	#include <stdatomic.h>
#else
	#include <C2nxt/std_atomic/StdAtomicImpl.h>

	#define atomic _Atomic

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Typedefs required by the standard ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

typedef _Atomic bool atomic_bool;
typedef _Atomic char atomic_char;
typedef _Atomic unsigned char atomic_uchar;
typedef _Atomic signed char atomic_schar;
typedef _Atomic short atomic_short;
typedef _Atomic unsigned short atomic_ushort;
typedef _Atomic int atomic_int;
typedef _Atomic unsigned int atomic_uint;
typedef _Atomic long atomic_long;
typedef _Atomic unsigned long atomic_ulong;
typedef _Atomic long long atomic_llong;
typedef _Atomic unsigned long long atomic_ullong;
typedef _Atomic char16_t atomic_char16_t;
typedef _Atomic char32_t atomic_char32_t;
typedef _Atomic wchar_t atomic_wchar_t;
typedef _Atomic int_least8_t atomic_int_least8_t;
typedef _Atomic int_least16_t atomic_int_least16_t;
typedef _Atomic int_least32_t atomic_int_least32_t;
typedef _Atomic int_least64_t atomic_int_least64_t;
typedef _Atomic uint_least8_t atomic_uint_least8_t;
typedef _Atomic uint_least16_t atomic_uint_least16_t;
typedef _Atomic uint_least32_t atomic_uint_least32_t;
typedef _Atomic uint_least64_t atomic_uint_least64_t;
typedef _Atomic int_fast8_t atomic_int_fast8_t;
typedef _Atomic int_fast16_t atomic_int_fast16_t;
typedef _Atomic int_fast32_t atomic_int_fast32_t;
typedef _Atomic int_fast64_t atomic_int_fast64_t;
typedef _Atomic uint_fast8_t atomic_uint_fast8_t;
typedef _Atomic uint_fast16_t atomic_uint_fast16_t;
typedef _Atomic uint_fast32_t atomic_uint_fast32_t;
typedef _Atomic uint_fast64_t atomic_uint_fast64_t;
typedef _Atomic intptr_t atomic_intptr_t;
typedef _Atomic uintptr_t atomic_uintptr_t;
typedef _Atomic size_t atomic_size_t;
typedef _Atomic isize atomic_ssize_t;
typedef _Atomic ptrdiff_t atomic_ptrdiff_t;
typedef _Atomic intmax_t atomic_intmax_t;
typedef _Atomic uintmax_t atomic_uintmax_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Extension typedefs for our library ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

typedef _Atomic i8 atomic_i8;
typedef _Atomic i16 atomic_i16;
typedef _Atomic i32 atomic_i32;
typedef _Atomic i64 atomic_i64;
typedef _Atomic isize atomic_isize;
typedef _Atomic byte atomic_byte;
typedef _Atomic u8 atomic_u8;
typedef _Atomic u16 atomic_u16;
typedef _Atomic u32 atomic_u32;
typedef _Atomic u64 atomic_u64;
typedef _Atomic usize atomic_usize;
typedef void* _Atomic atomic_ptr;

	////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////// Lock free indicators required by the standard ////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////

	#define ATOMIC_BOOL_LOCK_FREE	   2
	#define ATOMIC_CHAR_LOCK_FREE	   2
	#define ATOMIC_CHAR16_T_LOCK_FREE  2
	#define ATOMIC_CHAR32_T_LOCK_FREE  2
	#define ATOMIC_WCHAR_T_LOCK_FREE   2
	#define ATOMIC_SHORT_LOCK_FREE	   2
	#define ATOMIC_INT_LOCK_FREE	   2
	#define ATOMIC_LONG_LOCK_FREE	   2
	#define ATOMIC_LLONG_LOCK_FREE	   2
	#define ATOMIC_LONG_LONG_LOCK_FREE 2
	#define ATOMIC_POINTER_LOCK_FREE   2

	////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////// Extension lock free indicators for our library////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////

	#define ATOMIC_I8_LOCK_FREE	   2
	#define ATOMIC_I16_LOCK_FREE   2
	#define ATOMIC_I32_LOCK_FREE   2
	#define ATOMIC_I64_LOCK_FREE   2
	#define ATOMIC_ISIZE_LOCK_FREE 2
	#define ATOMIC_U8_LOCK_FREE	   2
	#define ATOMIC_U16_LOCK_FREE   2
	#define ATOMIC_U32_LOCK_FREE   2
	#define ATOMIC_U64_LOCK_FREE   2
	#define ATOMIC_USIZE_LOCK_FREE 2

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Atomic operations required by the standard ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

typedef __atomic_flag atomic_flag;

	#define ATOMIC_FLAG_INIT \
		(atomic_flag) { ._value = false }
	#define ATOMIC_VAR_INIT(value) __ATOMIC_VAR_INIT(value)

	#define kill_dependency(object) __kill_dependency(object)

	#define atomic_flag_test_and_set(flag) __atomic_flag_test_and_set(flag)
	#define atomic_flag_test_and_set_explicit(flag, order) \
		__atomic_flag_test_and_set_explicit(flag, order)

	#define atomic_flag_clear(flag)					__atomic_flag_clear(flag)
	#define atomic_flag_clear_explicit(flag, order) __atomic_flag_clear_explicit(flag, order)

	#define atomic_init(object_ptr, value) __atomic_init(object_ptr, value)

	#define atomic_is_lock_free(object_ptr) __atomic_is_lock_free(object_ptr)

	#define atomic_store(object_ptr, value) __atomic_store(object_ptr, value)
	#define atomic_store_explicit(object_ptr, value, order) \
		__atomic_store_explicit(object_ptr, value, order)

	#define atomic_load(object_ptr)					__atomic_load(object_ptr)
	#define atomic_load_explicit(object_ptr, order) __atomic_load_explicit(object_ptr, order)

	#define atomic_exchange(object_ptr, desired) __atomic_exchange(object_ptr, desired)
	#define atomic_exchange_explicit(object_ptr, desired, order) \
		__atomic_exchange_explicit(object_ptr, desired, order)

	#define atomic_compare_exchange_strong(object_ptr, expected_ptr, desired) \
		__atomic_compare_exchange_strong(object_ptr, expected_ptr, desired)
	#define atomic_compare_exchange_strong_explicit(object_ptr, expected_ptr, desired, order) \
		__atomic_compare_exchange_strong_explicit(object_ptr, expected_ptr, desired, order)

	#define atomic_fetch_add(addend_ptr, value) __atomic_fetch_add(addend_ptr, value)
	#define atomic_fetch_add_explicit(addend_ptr, value, order) \
		__atomic_fetch_add_explicit(addend_ptr, value, order)

	#define atomic_fetch_sub(minuend_ptr, subtrahend) __atomic_fetch_sub(minuend_ptr, subtrahend)
	#define atomic_fetch_sub_explicit(minued_ptr, subtrahend, order) \
		__atomic_fetch_sub_explicit(minuend_ptr, subtrahend, order)

	#define atomic_fetch_or(object_ptr, value) __atomic_fetch_or(object_ptr, value)
	#define atomic_fetch_or_explicit(object_ptr, value, order) \
		__atomic_fetch_or_explicit(object_ptr, value, order)

	#define atomic_fetch_xor(object_ptr, value) __atomic_fetch_xor(object_ptr, value)
	#define atomic_fetch_xor_explicit(object_ptr, value, order) \
		__atomic_fetch_xor_explicit(object_ptr, value, order)

	#define atomic_fetch_and(object_ptr, value) __atomic_fetch_and(object_ptr, value)
	#define atomic_fetch_and_explicit(object_ptr, value, order) \
		__atomic_fetch_and_explicit(object_ptr, value, order)

	#define atomic_thread_fence(order) __atomic_thread_fence(order)
	#define atomic_signal_fence(order) __atomic_signal_fence(order)

#endif // !STD_PLATFORM_WINDOWS
#endif // STD_ATOMIC
