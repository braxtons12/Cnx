/// @file StdAtomicImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides an equivalent implementation (on Windows) of the
/// atomic operations dictated by the C11 standard
///
/// Implementation is based on Microsoft's implementation of C++'s `<atomic>` and mingw GCC's
/// `<stdatomic.h>`, but simplified and altered to fit the restrictions faced when implementing it
/// as a third party, and (slightly) extended to take advantage of our own fixed-width typedefs
/// @version 0.2.0
/// @date 2022-04-11
///
/// MIT License
/// @copyright Copyright (c) 2022 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.

#ifndef STD_ATOMIC_IMPL
#define STD_ATOMIC_IMPL

#include <C2nxt/StdPlatform.h>

#if STD_PLATFORM_WINDOWS && !STD_PLATFORM_COMPILER_GCC

	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/StdDef.h>
	#include <uchar.h>

typedef enum {
	memory_order_relaxed,
	memory_order_consume,
	memory_order_acquire,
	memory_order_release,
	memory_order_acq_rel,
	memory_order_seq_cst
} memory_order;

	#define _Atomic volatile

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
typedef _Atomic float atomic_float;
typedef _Atomic double atomic_double;
typedef _Atomic intptr_t atomic_intptr_t;
typedef _Atomic uintptr_t atomic_uintptr_t;
typedef _Atomic size_t atomic_size_t;
typedef _Atomic isize atomic_ssize_t;
typedef _Atomic ptrdiff_t atomic_ptrdiff_t;
typedef _Atomic intmax_t atomic_intmax_t;
typedef _Atomic uintmax_t atomic_uintmax_t;
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
typedef _Atomic f32 atomic_f32;
typedef _Atomic f64 atomic_f64;
typedef void* _Atomic atomic_ptr;

IGNORE_RESERVED_IDENTIFIER_WARNING_START

typedef struct {
	bool _value;
} __atomic_flag;

	#define __atomic_init(object, value) (*(object) = (value))

	#define __ATOMIC_VAR_INIT(value) \
		{                            \
			(value)                  \
		}

	#define __kill_dependency(object)                \
		({                                           \
			IGNORE_RESERVED_IDENTIFIER_WARNING_START \
			let __kill = (object);                   \
			__kill;                                  \
			IGNORE_RESERVED_IDENTIFIER_WARNING_STOP  \
		})

void __atomic_store_explicit8(volatile atomic_i8* object, i8 value, memory_order order);
void __atomic_store_explicit16(volatile atomic_i16* object, i16 value, memory_order order);
void __atomic_store_explicit32(volatile atomic_i32* object, i32 value, memory_order order);
void __atomic_store_explicit64(volatile atomic_i64* object, i64 value, memory_order order);

void __atomic_store8(volatile atomic_i8* object, i8 value);
void __atomic_store16(volatile atomic_i16* object, i16 value);
void __atomic_store32(volatile atomic_i32* object, i32 value);
void __atomic_store64(volatile atomic_i64* object, i64 value);

i8 __atomic_load_explicit8(volatile atomic_i8* object, memory_order order);
i16 __atomic_load_explicit16(volatile atomic_i16* object, memory_order order);
i32 __atomic_load_explicit32(volatile atomic_i32* object, memory_order order);
i64 __atomic_load_explicit64(volatile atomic_i64* object, memory_order order);

i8 __atomic_load8(volatile atomic_i8* object);
i16 __atomic_load16(volatile atomic_i16* object);
i32 __atomic_load32(volatile atomic_i32* object);
i64 __atomic_load64(volatile atomic_i64* object);

	#define __atomic_store_explicit(object, desired, order)                          \
		({                                                                           \
			if(sizeof(*(object)) == 1) {                                             \
				__atomic_store_explicit8(static_cast(volatile atomic_i8*)(object),   \
										 static_cast(i8)(desired),                   \
										 order);                                     \
			}                                                                        \
			else if(sizeof(*(object)) == 2) {                                        \
				__atomic_store_explicit16(static_cast(volatile atomic_i16*)(object), \
										  static_cast(i16)(desired),                 \
										  order);                                    \
			}                                                                        \
			else if(sizeof(*(object)) == 4) {                                        \
				__atomic_store_explicit32(static_cast(volatile atomic_i32*)(object), \
										  static_cast(i32)(desired),                 \
										  order);                                    \
			}                                                                        \
			else if(sizeof(*(object)) == 8) {                                        \
				__atomic_store_explicit64(static_cast(volatile atomic_i64*)(object), \
										  static_cast(i64)(desired),                 \
										  order);                                    \
			}                                                                        \
		})

	#define __atomic_store(object, desired)                                 \
		({                                                                  \
			if(sizeof(*(object)) == 1) {                                    \
				__atomic_store8(static_cast(volatile atomic_i8*)(object),   \
								static_cast(i8)(desired));                  \
			}                                                               \
			else if(sizeof(*(object)) == 2) {                               \
				__atomic_store16(static_cast(volatile atomic_i16*)(object), \
								 static_cast(i16)(desired));                \
			}                                                               \
			else if(sizeof(*(object)) == 4) {                               \
				__atomic_store32(static_cast(volatile atomic_i32*)(object), \
								 static_cast(i32)(desired));                \
			}                                                               \
			else if(sizeof(*(object)) == 8) {                               \
				__atomic_store64(static_cast(volatile atomic_i64*)(object), \
								 static_cast(i64)(desired));                \
			}                                                               \
		})

	#define __atomic_load_explicit(object, order)                                                 \
		({                                                                                        \
			IGNORE_RESERVED_IDENTIFIER_WARNING_START                                              \
			typeof(*(object)) __ret;                                                              \
			if(sizeof(*(object)) == 1) {                                                          \
				let __ret1                                                                        \
					= __atomic_load_explicit8(static_cast(volatile atomic_i8*)(object), order);   \
				__ret = *static_cast(typeof(object))(&__ret1);                                    \
			}                                                                                     \
			else if(sizeof(*(object)) == 2) {                                                     \
				let __ret1                                                                        \
					= __atomic_load_explicit16(static_cast(volatile atomic_i16*)(object), order); \
				__ret = *static_cast(typeof(object))(&__ret1);                                    \
			}                                                                                     \
			else if(sizeof(*(object)) == 4) {                                                     \
				let __ret1                                                                        \
					= __atomic_load_explicit32(static_cast(volatile atomic_i32*)(object), order); \
				__ret = *static_cast(typeof(object))(&__ret1);                                    \
			}                                                                                     \
			else if(sizeof(*(object)) == 8) {                                                     \
				let __ret1                                                                        \
					= __atomic_load_explicit64(static_cast(volatile atomic_i64*)(object), order); \
				__ret = *static_cast(typeof(object))(&__ret1);                                    \
			}                                                                                     \
			else {                                                                                \
				unreachable();                                                                    \
			}                                                                                     \
			IGNORE_RESERVED_IDENTIFIER_WARNING_STOP                                               \
			__ret;                                                                                \
		})

	#define __atomic_load(object)                                                        \
		({                                                                               \
			IGNORE_RESERVED_IDENTIFIER_WARNING_START                                     \
			typeof(*(object)) __ret;                                                     \
			if(sizeof(*(object)) == 1) {                                                 \
				let __ret1 = __atomic_load8(static_cast(volatile atomic_i8*)(object));   \
				__ret = *static_cast(typeof(object))(&__ret1);                           \
			}                                                                            \
			else if(sizeof(*(object)) == 2) {                                            \
				let __ret1 = __atomic_load16(static_cast(volatile atomic_i16*)(object)); \
				__ret = *static_cast(typeof(object))(&__ret1);                           \
			}                                                                            \
			else if(sizeof(*(object)) == 4) {                                            \
				let __ret1 = __atomic_load32(static_cast(volatile atomic_i32*)(object)); \
				__ret = *static_cast(typeof(object))(&__ret1);                           \
			}                                                                            \
			else if(sizeof(*(object)) == 8) {                                            \
				let __ret1 = __atomic_load64(static_cast(volatile atomic_i64*)(object)); \
				__ret = *static_cast(typeof(object))(&__ret1);                           \
			}                                                                            \
			else {                                                                       \
				unreachable();                                                           \
			}                                                                            \
			IGNORE_RESERVED_IDENTIFIER_WARNING_STOP                                      \
			__ret;                                                                       \
		})

bool __atomic_flag_test_and_set(volatile __atomic_flag* object);
bool __atomic_flag_test_and_set_explicit(volatile __atomic_flag* object, memory_order order);

void __atomic_flag_clear(volatile __atomic_flag* object);
void __atomic_flag_clear_explicit(volatile __atomic_flag* object, memory_order order);

	#define __atomic_is_lock_free(object) (static_cast(bool)(sizeof(*(object)) <= 8))

i8 __atomic_fetch_add_explicit8(volatile atomic_i8* addend, i8 value, memory_order order);
i16 __atomic_fetch_add_explicit16(volatile atomic_i16* addend, i16 value, memory_order order);
i32 __atomic_fetch_add_explicit32(volatile atomic_i32* addend, i32 value, memory_order order);
i64 __atomic_fetch_add_explicit64(volatile atomic_i64* addend, i64 value, memory_order order);

	#define __atomic_fetch_add_explicit(addend, value, order)                                  \
		({                                                                                     \
			IGNORE_RESERVED_IDENTIFIER_WARNING_START                                           \
			typeof(*(addend)) __ret;                                                           \
			if(sizeof(*(addend)) == 1) {                                                       \
				let __ret1                                                                     \
					= __atomic_fetch_add_explicit8(static_cast(volatile atomic_i8*)(addend),   \
												   static_cast(i8)(value),                     \
												   order);                                     \
				__ret = *static_cast(typeof(addend))(&__ret1);                                 \
			}                                                                                  \
			else if(sizeof(*(addend)) == 2) {                                                  \
				let __ret1                                                                     \
					= __atomic_fetch_add_explicit16(static_cast(volatile atomic_i16*)(addend), \
													static_cast(i16)(value),                   \
													order);                                    \
				__ret = *static_cast(typeof(addend))(&__ret1);                                 \
			}                                                                                  \
			else if(sizeof(*(addend)) == 4) {                                                  \
				let __ret1                                                                     \
					= __atomic_fetch_add_explicit32(static_cast(volatile atomic_i32*)(addend), \
													static_cast(i32)(value),                   \
													order);                                    \
				__ret = *static_cast(typeof(addend))(&__ret1);                                 \
			}                                                                                  \
			else if(sizeof(*(addend)) == 8) {                                                  \
				let __ret1                                                                     \
					= __atomic_fetch_add_explicit64(static_cast(volatile atomic_i64*)(addend), \
													static_cast(i64)(value),                   \
													order);                                    \
				__ret = *static_cast(typeof(addend))(&__ret1);                                 \
			}                                                                                  \
			else {                                                                             \
				unreachable();                                                                 \
			}                                                                                  \
			IGNORE_RESERVED_IDENTIFIER_WARNING_STOP                                            \
			__ret;                                                                             \
		})

	#define __atomic_fetch_add(addend, value) \
		__atomic_fetch_add_explicit(addend, value, memory_order_seq_cst)

	#define __atomic_fetch_sub_explicit(minuend, subtrahend, order) \
		__atomic_fetch_add_explicit(minuend, -(subtrahend), order)
	#define __atomic_fetch_sub(minuend, subtrahend) __atomic_fetch_add(minuend, -(subtrahend))

i8 __atomic_fetch_or_explicit8(volatile atomic_i8* object, i8 operand, memory_order order);
i16 __atomic_fetch_or_explicit16(volatile atomic_i16* object, i16 operand, memory_order order);
i32 __atomic_fetch_or_explicit32(volatile atomic_i32* object, i32 operand, memory_order order);
i64 __atomic_fetch_or_explicit64(volatile atomic_i64* object, i64 operand, memory_order order);

	#define __atomic_fetch_or_explicit(object, value, order)                                       \
		({                                                                                         \
			IGNORE_RESERVED_IDENTIFIER_WARNING_START                                               \
			typeof(*(object)) __ret;                                                               \
			if(sizeof(*(object)) == 1) {                                                           \
				let __ret1 = __atomic_fetch_or_explicit8(static_cast(volatile atomic_i8*)(object), \
														 static_cast(i8)(value),                   \
														 order);                                   \
				__ret = *static_cast(typeof(object))(&__ret1);                                     \
			}                                                                                      \
			else if(sizeof(*(object)) == 2) {                                                      \
				let __ret1                                                                         \
					= __atomic_fetch_or_explicit16(static_cast(volatile atomic_i16*)(object),      \
												   static_cast(i16)(value),                        \
												   order);                                         \
				__ret = *static_cast(typeof(object))(&__ret1);                                     \
			}                                                                                      \
			else if(sizeof(*(object)) == 4) {                                                      \
				let __ret1                                                                         \
					= __atomic_fetch_or_explicit32(static_cast(volatile atomic_i32*)(object),      \
												   static_cast(i32)(value),                        \
												   order);                                         \
				__ret = *static_cast(typeof(object))(&__ret1);                                     \
			}                                                                                      \
			else if(sizeof(*(object)) == 8) {                                                      \
				let __ret1                                                                         \
					= __atomic_fetch_or_explicit64(static_cast(volatile atomic_i64*)(object),      \
												   static_cast(i64)(value),                        \
												   order);                                         \
				__ret = *static_cast(typeof(object))(&__ret1);                                     \
			}                                                                                      \
			else {                                                                                 \
				unreachable();                                                                     \
			}                                                                                      \
			IGNORE_RESERVED_IDENTIFIER_WARNING_STOP                                                \
			__ret;                                                                                 \
		})

	#define __atomic_fetch_or(object, value) \
		__atomic_fetch_or_explicit(object, value, memory_order_seq_cst)

i8 __atomic_fetch_xor_explicit8(volatile atomic_i8* object, i8 operand, memory_order order);
i16 __atomic_fetch_xor_explicit16(volatile atomic_i16* object, i16 operand, memory_order order);
i32 __atomic_fetch_xor_explicit32(volatile atomic_i32* object, i32 operand, memory_order order);
i64 __atomic_fetch_xor_explicit64(volatile atomic_i64* object, i64 operand, memory_order order);

	#define __atomic_fetch_xor_explicit(object, value, order)                                  \
		({                                                                                     \
			IGNORE_RESERVED_IDENTIFIER_WARNING_START                                           \
			typeof(*(object)) __ret;                                                           \
			if(sizeof(*(object)) == 1) {                                                       \
				let __ret1                                                                     \
					= __atomic_fetch_xor_explicit8(static_cast(volatile atomic_i8*)(object),   \
												   static_cast(i8)(value),                     \
												   order);                                     \
				__ret = *static_cast(typeof(object))(&__ret1);                                 \
			}                                                                                  \
			else if(sizeof(*(object)) == 2) {                                                  \
				let __ret1                                                                     \
					= __atomic_fetch_xor_explicit16(static_cast(volatile atomic_i16*)(object), \
													static_cast(i16)(value),                   \
													order);                                    \
				__ret = *static_cast(typeof(object))(&__ret1);                                 \
			}                                                                                  \
			else if(sizeof(*(object)) == 4) {                                                  \
				let __ret1                                                                     \
					= __atomic_fetch_xor_explicit32(static_cast(volatile atomic_i32*)(object), \
													static_cast(i32)(value),                   \
													order);                                    \
				__ret = *static_cast(typeof(object))(&__ret1);                                 \
			}                                                                                  \
			else if(sizeof(*(object)) == 8) {                                                  \
				let __ret1                                                                     \
					= __atomic_fetch_xor_explicit64(static_cast(volatile atomic_i64*)(object), \
													static_cast(i64)(value),                   \
													order);                                    \
				__ret = *static_cast(typeof(object))(&__ret1);                                 \
			}                                                                                  \
			else {                                                                             \
				unreachable();                                                                 \
			}                                                                                  \
			IGNORE_RESERVED_IDENTIFIER_WARNING_STOP                                            \
			__ret;                                                                             \
		})

	#define __atomic_fetch_xor(object, value) \
		__atomic_fetch_xor_explicit(object, value, memory_order_seq_cst)

i8 __atomic_fetch_and_explicit8(volatile atomic_i8* object, i8 operand, memory_order order);
i16 __atomic_fetch_and_explicit16(volatile atomic_i16* object, i16 operand, memory_order order);
i32 __atomic_fetch_and_explicit32(volatile atomic_i32* object, i32 operand, memory_order order);
i64 __atomic_fetch_and_explicit64(volatile atomic_i64* object, i64 operand, memory_order order);

	#define __atomic_fetch_and_explicit(object, value, order)                                  \
		({                                                                                     \
			IGNORE_RESERVED_IDENTIFIER_WARNING_START                                           \
			typeof(*(object)) __ret;                                                           \
			if(sizeof(*(object)) == 1) {                                                       \
				let __ret1                                                                     \
					= __atomic_fetch_and_explicit8(static_cast(volatile atomic_i8*)(object),   \
												   static_cast(i8)(value),                     \
												   order);                                     \
				__ret = *static_cast(typeof(object))(&__ret1);                                 \
			}                                                                                  \
			else if(sizeof(*(object)) == 2) {                                                  \
				let __ret1                                                                     \
					= __atomic_fetch_and_explicit16(static_cast(volatile atomic_i16*)(object), \
													static_cast(i16)(value),                   \
													order);                                    \
				__ret = *static_cast(typeof(object))(&__ret1);                                 \
			}                                                                                  \
			else if(sizeof(*(object)) == 4) {                                                  \
				let __ret1                                                                     \
					= __atomic_fetch_and_explicit32(static_cast(volatile atomic_i32*)(object), \
													static_cast(i32)(value),                   \
													order);                                    \
				__ret = *static_cast(typeof(object))(&__ret1);                                 \
			}                                                                                  \
			else if(sizeof(*(object)) == 8) {                                                  \
				let __ret1                                                                     \
					= __atomic_fetch_and_explicit64(static_cast(volatile atomic_i64*)(object), \
													static_cast(i64)(value),                   \
													order);                                    \
				__ret = *static_cast(typeof(object))(&__ret1);                                 \
			}                                                                                  \
			else {                                                                             \
				unreachable();                                                                 \
			}                                                                                  \
			IGNORE_RESERVED_IDENTIFIER_WARNING_STOP                                            \
			__ret;                                                                             \
		})

	#define __atomic_fetch_and(object, value) \
		__atomic_fetch_and_explicit(object, value, memory_order_seq_cst)

i8 __atomic_exchange_explicit8(volatile atomic_i8* object, i8 desired, memory_order order);
i16 __atomic_exchange_explicit16(volatile atomic_i16* object, i16 desired, memory_order order);
i32 __atomic_exchange_explicit32(volatile atomic_i32* object, i32 desired, memory_order order);
i64 __atomic_exchange_explicit64(volatile atomic_i64* object, i64 desired, memory_order order);

	#define __atomic_exchange_explicit(object, desired, order)                                     \
		({                                                                                         \
			IGNORE_RESERVED_IDENTIFIER_WARNING_START                                               \
			typeof(*(object)) __ret;                                                               \
			if(sizeof(*(object)) == 1) {                                                           \
				let __ret1 = __atomic_exchange_explicit8(static_cast(volatile atomic_i8*)(object), \
														 static_cast(i8)(desired),                 \
														 order);                                   \
				__ret = *static_cast(typeof(object))(&__ret1);                                     \
			}                                                                                      \
			else if(sizeof(*(object)) == 2) {                                                      \
				let __ret1                                                                         \
					= __atomic_exchange_explicit16(static_cast(volatile atomic_i16*)(object),      \
												   static_cast(i16)(desired),                      \
												   order);                                         \
				__ret = *static_cast(typeof(object))(&__ret1);                                     \
			}                                                                                      \
			else if(sizeof(*(object)) == 4) {                                                      \
				let __ret1                                                                         \
					= __atomic_exchange_explicit32(static_cast(volatile atomic_i32*)(object),      \
												   static_cast(i32)(desired),                      \
												   order);                                         \
				__ret = *static_cast(typeof(object))(&__ret1);                                     \
			}                                                                                      \
			else if(sizeof(*(object)) == 8) {                                                      \
				let __ret1                                                                         \
					= __atomic_exchange_explicit64(static_cast(volatile atomic_i64*)(object),      \
												   static_cast(i64)(desired),                      \
												   order);                                         \
				__ret = *static_cast(typeof(object))(&__ret1);                                     \
			}                                                                                      \
			else {                                                                                 \
				unreachable();                                                                     \
			}                                                                                      \
			IGNORE_RESERVED_IDENTIFIER_WARNING_STOP                                                \
			__ret;                                                                                 \
		})

	#define __atomic_exchange(object, desired) \
		__atomic_exchange_explicit(object, desired, memory_order_seq_cst)

bool __atomic_compare_exchange_strong_explicit8(volatile atomic_i8* object,
												i8* restrict expected,
												i8 desired,
												memory_order order);
bool __atomic_compare_exchange_strong_explicit16(volatile atomic_i16* object,
												 i16* restrict expected,
												 i16 desired,
												 memory_order order);
bool __atomic_compare_exchange_strong_explicit32(volatile atomic_i32* object,
												 i32* restrict expected,
												 i32 desired,
												 memory_order order);
bool __atomic_compare_exchange_strong_explicit64(volatile atomic_i64* object,
												 i64* restrict expected,
												 i64 desired,
												 memory_order order);

	#define __atomic_compare_exchange_strong_explicit(object, expected, desired, order) \
		({                                                                              \
			IGNORE_RESERVED_IDENTIFIER_WARNING_START                                    \
			typeof(*(object)) __ret;                                                    \
			if(sizeof(*(object)) == 1) {                                                \
				let __ret1 = __atomic_compare_exchange_strong_explicit8(                \
					static_cast(volatile atomic_i8*)(object),                           \
					static_cast(i8*)(expected),                                         \
					static_cast(i8)(desired),                                           \
					order);                                                             \
				__ret = *static_cast(typeof(object))(&__ret1);                          \
			}                                                                           \
			else if(sizeof(*(object)) == 2) {                                           \
				let __ret1 = __atomic_compare_exchange_strong_explicit16(               \
					static_cast(volatile atomic_i16*)(object),                          \
					static_cast(i16*)(expected),                                        \
					static_cast(i16)(desired),                                          \
					order);                                                             \
				__ret = *static_cast(typeof(object))(&__ret1);                          \
			}                                                                           \
			else if(sizeof(*(object)) == 4) {                                           \
				let __ret1 = __atomic_compare_exchange_strong_explicit32(               \
					static_cast(volatile atomic_i32*)(object),                          \
					static_cast(i32*)(expected),                                        \
					static_cast(i32)(desired),                                          \
					order);                                                             \
				__ret = *static_cast(typeof(object))(&__ret1);                          \
			}                                                                           \
			else if(sizeof(*(object)) == 8) {                                           \
				let __ret1 = __atomic_compare_exchange_strong_explicit64(               \
					static_cast(volatile atomic_i64*)(object),                          \
					static_cast(i64*)(expected),                                        \
					static_cast(i64)(desired),                                          \
					order);                                                             \
				__ret = *static_cast(typeof(object))(&__ret1);                          \
			}                                                                           \
			else {                                                                      \
				unreachable();                                                          \
			}                                                                           \
			IGNORE_RESERVED_IDENTIFIER_WARNING_STOP                                     \
			__ret;                                                                      \
		})

	#define __atomic_compare_exchange_strong(object, expected, desired) \
		__atomic_compare_exchange_strong_explicit(object, expected, desired, memory_order_seq_cst)

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#else
	#error Windows-only atomic implementation included on platform other than Windows
#endif // STD_PLATFORM_WINDOWS
#endif // STD_ATOMIC_IMPL
