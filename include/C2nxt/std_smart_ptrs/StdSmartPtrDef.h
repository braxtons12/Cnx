/// @file StdSmartPtrDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the macro definitions for working with smart pointer templates
/// @version 0.2.0
/// @date 2022-03-29
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

#ifndef STD_SMART_PTRS
#define STD_SMART_PTRS

#include <C2nxt/StdDef.h>

/// @ingroup memory
/// @{
/// @defgroup std_smart_ptrs StdSmartPtrs
/// StdSmartPtrs provides utilities for implementing and working with C2nxt smart pointers
/// @}

/// @brief Returns a pointer to the managed object of a C2nxt smart pointer
///
/// This is const correct:
/// 		- If `self` is const, this will return a pointer-to-const of the managed object.
/// 		- Otherwise, this will return a "normal" pointer to the managed object.
/// If you want to explicitly get a pointer-to-const or pointer-to-not-const, use
/// `ptr_const` or `ptr_mut`, respectively.
///
/// @param self - The smart pointer to get the pointer from
///
/// @return const correct pointer to the object managed by `self`
/// @ingroup std_smart_ptrs
#define ptr(self) (*__ptr(self))

/// @brief implementation of `ptr`
#define __ptr(self)                                  \
	({                                               \
		IGNORE_DISCARDED_QUALIFIERS_START            \
		let UNIQUE_VAR(self_check) = &(self);        \
		_Generic((UNIQUE_VAR(self_check)),         \
			const typeof(self)* : &ptr_const(self), \
			default 			: &ptr_mut(self)); \
		IGNORE_DISCARDED_QUALIFIERS_STOP             \
	})

/// @brief Returns a pointer-to-const to the managed object of a C2nxt smart pointer
///
/// @param self - The smart pointer to get the pointer from
///
/// @return pointer-to-const to the object managed by `self`
/// @ingroup std_smart_ptrs
#define ptr_const(self) (*(static_cast(const typeof(*((self).m_ptr))**)(&((self).m_ptr))))
/// @brief Returns a pointer-to-non-const to the managed object of a C2nxt smart pointer
///
/// @param self - The smart pointer to get the pointer from
///
/// @return pointer-to-non-const to the object managed by `self`
/// @ingroup std_smart_ptrs
#define ptr_mut(self) (*(static_cast(typeof(*((self).m_ptr))**)(&((self).m_ptr))))

/// @brief Moves `self` into the assigned-to or bound-to variable/parameter/etc.
///
/// This is almost equivalent to `move` provided in `std_def`, but ensures that the pointer to the
/// managed object in the moved-from value is set to the correct `nullptr` constant for the
/// platform, for cases where the platform's `nullptr` (`NULL`) constant is not zero. If your
/// target platform(s)'s `nullptr` constant is guaranteed to be zero, you can safely use `move`
/// instead.
///
/// @param self - The smart pointer to move
///
/// @return the moved value
/// @ingroup std_smart_ptrs
#define ptr_move(self)                                                    \
	({                                                                    \
		let_mut UNIQUE_VAR(ptr) = move(self);                             \
		ptr(self) = nullptr; /** In case NULL isn't 0, assign nullptr **/ \
		UNIQUE_VAR(ptr);                                                  \
	})

/// @brief The function type (with optional name, for use in typedefs) of a deleter function used in
/// smart pointer templates.
///
/// Given an instance of `T`, `t`,
/// the signature is: ``` void (*name)(Y* restrict to_deleter, StdAllocator allocator);```
/// where `Y` is either:
/// 1. `typeof(t)` if `T` is __NOT__ an array type (e.g. `int`, `usize`, `StdVector(i32)`)
/// 2. `typeof(t[0]) if `T` __IS__ an array type (e.g. a typedef to `int[]` or `usize[]`)
///
/// @ingroup std_smart_ptrs
#define StdDeleter(T, ...) \
	void (*__VA_ARGS__)(__SMART_PTR_ELEMENT_PTR(T) restrict self, StdAllocator allocator)

/// @brief checks if the given smart pointer instance manages an array type
#define __SMART_PTR_CHECK_INSTANCE_IS_ARRAY(self) \
	(!(std_types_equal_v(typeof(*((self).m_ptr)), (typeof(*((self).m_type))){0})))
/// @brief checks if the given type is an array type
#define __SMART_PTR_IS_ARRAY(T) (!(std_types_equal_v(typeof((T){0}), *(&(T){0}))))
/// @brief gets the element pointer type for the given array type.
/// e.g if `T` evaluates to `int[]`, returns `int*`.
#define __SMART_PTR_ELEMENT_PTR_BASE(T) ((T){0}, (T){1})
/// @brief gets the const element pointer type for the given array type.
/// e.g if `T` evaluates to `int[]`, returns `const int*`.
#define __SMART_PTR_CONST_ELEMENT_PTR_BASE(T) ((const T){0}, (const T){1})

// clang-format off
/// @brief gets the pointer type/element pointer type for the given type used in a
/// smart pointer template.
/// e.g. if `T` evalutes to `int`, returns `int*` as the pointer-to-int type is `int*`,
/// but if `T` evaluates to `int[]`, also returns `int*`, as the pointer-to-element type of
/// an array of `int`s is `int*`.
#define __SMART_PTR_ELEMENT_PTR(T) 											   \
			typeof(_Generic(((T){0}),								           \
				typeof(*(&(T){0})) 	: &(T){0},    			                   \
				default				: __SMART_PTR_ELEMENT_PTR_BASE(T)) 		   \
			)
/// @brief gets the pointer to const type/const element pointer type for the given type used in a
/// smart pointer template.
/// e.g. if `T` evalutes to `int`, returns `const int*` as the pointer-to-const-int type is
/// `const int*`,
/// but if `T` evaluates to `int[]`, also returns `const int*`, as the pointer-to-const-element
/// type of an array of `int`s is `const int*`.
#define __SMART_PTR_CONST_ELEMENT_PTR(T) 									   \
			typeof(_Generic(((T){0}),								           \
				typeof(*(&(T){0})) 	: &(const T){0},    			           \
				default				: __SMART_PTR_CONST_ELEMENT_PTR_BASE(T))   \
			)
// clang-format on

/// @brief gets the element type for the given type `T` used in a smart pointer template.
/// e.g. it `T` evaluates to `int` returns `int`, as a smart pointer managing an `int` only contains
/// one `int`.
/// But if `T` evaluates to `int[]`, also returns `int` because a smart pointer managing `int`s
/// contains several `int` elements
#define __SMART_PTR_ELEMENT(T) typeof(*((__SMART_PTR_ELEMENT_PTR(T)){0}))

#endif // STD_SMART_PTRS
