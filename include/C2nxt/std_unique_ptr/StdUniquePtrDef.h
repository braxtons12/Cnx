/// @file StdUniquePtrDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the macro definitions for a struct template for representing a
/// uniquely owned pointer
/// @version 0.2.0
/// @date 2022-03-26
///
/// MIT License
/// @copyright Copyright (c) 2021 Braxton Salyer <braxtonsalyer@gmail.com>
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

#ifndef STD_UNIQUE_PTR_DEF
#define STD_UNIQUE_PTR_DEF

#include <C2nxt/StdAssert.h>
#include <C2nxt/StdDef.h>

#define StdUniquePtr(T)						  CONCAT2(StdUniquePtr, T)
#define StdUniquePtrIdentifier(T, Identifier) CONCAT2(std_unique_ptr, CONCAT3(T, _, Identifier))

#define std_unique_ptr_new(T)                                                         \
	({                                                                                \
		std_static_assert(                                                            \
			!__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                        \
			"std_unique_ptr_new is only available if UNIQUE_T is NOT an array type"); \
		StdUniquePtrIdentifier(T, new)();                                             \
	})

#define std_unique_ptr_new_with_deleter(T, deleter)                                                \
	({                                                                                             \
		std_static_assert(                                                                         \
			!__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                                     \
			"std_unique_ptr_new_with_deleter is only available if UNIQUE_T is NOT an array type"); \
		StdUniquePtrIdentifier(T, new_with_deleter)(deleter);                                      \
	})

#define std_unique_ptr_new_with_allocator(T, allocator)                                         \
	({                                                                                          \
		std_static_assert(!__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                    \
						  "std_unique_ptr_new_with_allocator is only available if UNIQUE_T is " \
						  "NOT an array type");                                                 \
		StdUniquePtrIdentifier(T, new_with_allocator)(allocator);                               \
	})
#define std_unique_ptr_new_with_allocator_and_deleter(T, allocator, deleter)                    \
	({                                                                                          \
		std_static_assert(!__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                    \
						  "std_unique_ptr_new_with_allocator_and_deleter is only available if " \
						  "UNIQUE_T is NOT an array type");                                     \
		StdUniquePtrIdentifier(T, new_with_allocator_and_deleter)(allocator, deleter);          \
	})
#define std_unique_ptr_new_with_capacity(T, capacity)                                       \
	({                                                                                      \
		std_static_assert(__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                 \
						  "std_unique_ptr_new_with_capacity is only available if UNIQUE_T " \
						  "is an array type");                                              \
		StdUniquePtrIdentifier(T, new_with_capacity)(capacity);                             \
	})
#define std_unique_ptr_new_with_capacity_and_deleter(T, capacity, deleter)                \
	({                                                                                    \
		std_static_assert(                                                                \
			__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                             \
			"std_unique_ptr_new_with_capacity_and_deleter is only available if UNIQUE_T " \
			"is an array type");                                                          \
		StdUniquePtrIdentifier(T, new_with_capacity_and_deleter)(capacity, deleter);      \
	})
#define std_unique_ptr_new_with_capacity_and_allocator(T, capacity, allocator)              \
	({                                                                                      \
		std_static_assert(                                                                  \
			__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                               \
			"std_unique_ptr_new_with_capacity_and_allocator is only available if UNIQUE_T " \
			"is an array type");                                                            \
		StdUniquePtrIdentifier(T, new_with_capacity_and_allocator)(capacity, allocator);    \
	})
#define std_unique_ptr_new_with_capacity_allocator_and_deleter(T, capacity, allocator, deleter) \
	({                                                                                          \
		std_static_assert(__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                     \
						  "std_unique_ptr_new_with_capacity_allocator_and_deleter is only "     \
						  "available if UNIQUE_T "                                              \
						  "is an array type");                                                  \
		StdUniquePtrIdentifier(T, new_with_capacity_allocator_and_deleter)(capacity,            \
																		   allocator,           \
																		   deleter);            \
	})
#define std_unique_ptr_from(T, ptr) StdUniquePtrIdentifier(T, from)(ptr)
#define std_unique_ptr_from_with_deleter(T, ptr, deleter) \
	StdUniquePtrIdentifier(T, from_with_deleter)(ptr, deleter)
#define std_unique_ptr_from_with_allocator(T, ptr, allocator) \
	StdUniquePtrIdentifier(T, from_with_allocator)(ptr, allocator)
#define std_unique_ptr_from_with_allocator_and_deleter(T, ptr, allocator, deleter) \
	StdUniquePtrIdentifier(T, from_with_allocator_and_deleter)(ptr, allocator, deleter)

#define std_unique_ptr_free(T, self) StdUniquePtrIdentifier(T, free)(static_cast(void*)(&(self)))

#define std_unique_ptr_release(self)		((self).m_vtable->release(&(self)))
#define std_unique_ptr_reset(self, new_ptr) ((self).m_vtable->reset(&(self), (new_ptr)))
#define std_unique_ptr_swap(self, other)	((self).m_vtable->swap(&(self), (other)))
#define std_unique_ptr_get_deleter(self)	((self).m_vtable->get_deleter(&(self)))
#define std_unique_ptr_as_bool(self)		((self).m_vtable->as_bool(&(self)))

#define std_unique_ptr_get_const(self) ((self).m_vtable->get_const(&(self)))
#define std_unique_ptr_get_mut(self)   ((self).m_vtable->get(&(self)))

#define std_unique_ptr_get(self) ((__std_unique_ptr_get(self)))
#define __std_unique_ptr_get(self)                                \
	({                                                            \
		IGNORE_DISCARDED_QUALIFIERS_START                         \
		let UNIQUE_VAR(ptr) = &(self);                            \
		_Generic((UNIQUE_VAR(ptr)),           	                 \
			const typeof(self)* : std_unique_ptr_get_const(self), \
			default 			: std_unique_ptr_get_mut(self)); \
		IGNORE_DISCARDED_QUALIFIERS_STOP                          \
	})

#define std_unique_ptr_at(self, index) (*(__std_unique_ptr_at(self, index)))

#define __std_unique_ptr_at(self, index)                                   \
	({                                                                     \
		let UNIQUE_VAR(ptr) = &(self);                                     \
		_Generic(                     	                                  \
			(UNIQUE_VAR(ptr)),                                       	   \
			const typeof(self)* : &(std_unique_ptr_at_const(self, index)), \
			default 			: &std_unique_ptr_at_mut(self, index)); \
	})
#define std_unique_ptr_at_const(self, index) \
	(*(static_cast(const typeof(*((self).m_ptr))*)(__std_unique_ptr_at_const(self, index))))

#define __std_unique_ptr_at_const(self, index)                                     \
	({                                                                             \
		std_static_assert(__UNIQUE_PTR_CHECK_IS_ARRAY(self),                       \
						  "std_unique_ptr_at_const is only available if UNIQUE_T " \
						  "is an array type");                                     \
		&(((self).m_ptr)[index]);                                                  \
	})

#define std_unique_ptr_at_mut(self, index) (*(__std_unique_ptr_at_mut(self, index)))

#define __std_unique_ptr_at_mut(self, index)                                 \
	({                                                                       \
		std_static_assert(__UNIQUE_PTR_CHECK_IS_ARRAY(self),                 \
						  "std_unique_ptr_at is only available if UNIQUE_T " \
						  "is an array type");                               \
		&(((self).m_ptr)[index]);                                            \
	})

#define ptr(self) ((self).m_ptr)

#define std_make_unique(T, ...)                                                                 \
	({                                                                                          \
		std_static_assert(!__UNIQUE_PTR_CHECK_IS_ARRAY((StdUniquePtr(T)){0}),                   \
						  "std_make_unique is only available for Ts that are not array types"); \
		let_mut UNIQUE_VAR(ptr)                                                                 \
			= static_cast(T*)(std_allocator_allocate_t(T, DEFAULT_ALLOCATOR).m_memory);         \
		*UNIQUE_VAR(ptr) = (T){__VA_ARGS__};                                                    \
		std_unique_ptr_from(T, UNIQUE_VAR(ptr));                                                \
	})
#define std_make_unique_with_deleter(T, deleter, ...)                                          \
	({                                                                                         \
		std_static_assert(                                                                     \
			!__UNIQUE_PTR_CHECK_IS_ARRAY((StdUniquePtr(T)){0}),                                \
			"std_make_unique_with_deleter is only available for Ts that are not array types"); \
		let_mut UNIQUE_VAR(ptr)                                                                \
			= static_cast(T*)(std_allocator_allocate_t(T, DEFAULT_ALLOCATOR).m_memory);        \
		*UNIQUE_VAR(ptr) = (T){__VA_ARGS__};                                                   \
		std_unique_ptr_from_with_deleter(T, UNIQUE_VAR(ptr), deleter);                         \
	})
#define std_make_unique_with_allocator(T, allocator, ...)                                        \
	({                                                                                           \
		std_static_assert(                                                                       \
			!__UNIQUE_PTR_CHECK_IS_ARRAY((StdUniquePtr(T)){0}),                                  \
			"std_make_unique_with_allocator is only available for Ts that are not array types"); \
		let_mut UNIQUE_VAR(ptr)                                                                  \
			= static_cast(T*)(std_allocator_allocate_t(T, allocator).m_memory);                  \
		*UNIQUE_VAR(ptr) = (T){__VA_ARGS__};                                                     \
		std_unique_ptr_from_with_allocator(T, UNIQUE_VAR(ptr), allocator);                       \
	})
#define std_make_unique_with_allocator_and_deleter(T, allocator, deleter, ...)                   \
	({                                                                                           \
		std_static_assert(                                                                       \
			!__UNIQUE_PTR_CHECK_IS_ARRAY((StdUniquePtr(T)){0}),                                  \
			"std_make_unique_with_allocator is only available for Ts that are not array types"); \
		let_mut UNIQUE_VAR(ptr)                                                                  \
			= static_cast(T*)(std_allocator_allocate_t(T, allocator).m_memory);                  \
		*UNIQUE_VAR(ptr) = (T){__VA_ARGS__};                                                     \
		std_unique_ptr_from_with_allocator_and_deleter(T, UNIQUE_VAR(ptr), allocator, deleter);  \
	})

#define unique_scoped(T) scoped(StdUniquePtrIdentifier(T, free))

#define __UNIQUE_PTR_CHECK_IS_ARRAY(self) \
	(!(std_types_equal_v(typeof(*((self).m_ptr)), (typeof(*((self).m_type))){0})))
#define __UNIQUE_PTR_IS_ARRAY				(!(std_types_equal_v(typeof((UNIQUE_T){0}), *(&(UNIQUE_T){0}))))
#define __UNIQUE_PTR_IS_ARRAY_GENERIC(T)	(!(std_types_equal_v(typeof((T){0}), *(&(T){0}))))
#define __UNIQUE_PTR_ELEMENT_PTR_BASE		((UNIQUE_T){0}, (UNIQUE_T){1})
#define __UNIQUE_PTR_CONST_ELEMENT_PTR_BASE ((const UNIQUE_T){0}, (const UNIQUE_T){1})

// clang-format off
#define __UNIQUE_PTR_ELEMENT_PTR 												\
		typeof(_Generic(((UNIQUE_T){0}),								\
			typeof(*(&(UNIQUE_T){0})) 	: &(UNIQUE_T){0},    			\
			default					  	: __UNIQUE_PTR_ELEMENT_PTR_BASE))
#define __UNIQUE_PTR_CONST_ELEMENT_PTR 												\
	typeof(_Generic(((UNIQUE_T){0}),								\
		typeof(*(&(UNIQUE_T){0})) 	: &(const UNIQUE_T){0},    			\
		default					  	: __UNIQUE_PTR_CONST_ELEMENT_PTR_BASE))
// clang-format on

#define __UNIQUE_PTR_ELEMENT typeof(*((__UNIQUE_PTR_ELEMENT_PTR){0}))
#endif // STD_UNIQUE_PTR_DEF
