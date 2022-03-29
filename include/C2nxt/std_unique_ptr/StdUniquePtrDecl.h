/// @file StdUniquePtrDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the type definitions and function declarations for a struct template
/// for representing a uniquely owned pointer
/// @version 0.2.0
/// @date 2022-03-28
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

#include <C2nxt/StdDef.h>

#if defined(UNIQUE_T) && defined(UNIQUE_DELETER) && UNIQUE_DECL

	#define STD_TEMPlATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdAssert.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/StdTypeTraits.h>
	#include <C2nxt/std_unique_ptr/StdUniquePtrDef.h>

typedef struct StdUniquePtr(UNIQUE_T) StdUniquePtr(UNIQUE_T);

typedef struct StdUniquePtrIdentifier(UNIQUE_T, vtable) StdUniquePtrIdentifier(UNIQUE_T, vtable);
typedef StdDeleter(UNIQUE_T, StdUniquePtrIdentifier(UNIQUE_T, Deleter));

typedef struct StdUniquePtr(UNIQUE_T) {
	union {
		__UNIQUE_PTR_ELEMENT_PTR m_ptr;
		UNIQUE_T* m_type;
	};
	StdAllocator m_allocator;
	const StdUniquePtrIdentifier(UNIQUE_T, vtable) * m_vtable;
}
StdUniquePtr(UNIQUE_T);

	#define ___DISABLE_IF_NULL(self) \
		std_disable_if(!(self), "Can't perform an operator on a null unique_ptr")

[[nodiscard]] StdUniquePtr(UNIQUE_T) StdUniquePtrIdentifier(UNIQUE_T, default)(void);

[[nodiscard]] StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, default_with_allocator)(StdAllocator allocator);

[[nodiscard]] StdUniquePtr(UNIQUE_T) StdUniquePtrIdentifier(UNIQUE_T, new)(void)
	std_disable_if(__UNIQUE_PTR_IS_ARRAY,
				   "std_unique_ptr_new is only available if "
				   "UNIQUE_T is NOT an array type");

[[nodiscard]] StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, new_with_allocator)(StdAllocator allocator)
		std_disable_if(__UNIQUE_PTR_IS_ARRAY,
					   "std_unique_ptr_new_with_allocator is only available if "
					   "UNIQUE_T is NOT an array type");

[[nodiscard]] StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, new_with_capacity)(usize capacity)
		std_disable_if(!__UNIQUE_PTR_IS_ARRAY,
					   "std_unique_ptr_new_with_capacity is only available if "
					   "UNIQUE_T is an array type");

[[nodiscard]] StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, new_with_capacity_and_allocator)(usize capacity,
																	  StdAllocator allocator)
		std_disable_if(!__UNIQUE_PTR_IS_ARRAY,
					   "std_unique_ptr_new_with_capacity_and_allocator is only available if "
					   "UNIQUE_T is an array type");

[[nodiscard]] StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, from)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr);

[[nodiscard]] StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, from_with_allocator)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr,
														  StdAllocator allocator);

[[nodiscard]] [[not_null(1)]] __UNIQUE_PTR_ELEMENT_PTR
	StdUniquePtrIdentifier(UNIQUE_T, release)(StdUniquePtr(UNIQUE_T) * restrict self)
		___DISABLE_IF_NULL(self);

[[not_null(1)]] void
	StdUniquePtrIdentifier(UNIQUE_T, reset)(StdUniquePtr(UNIQUE_T) * restrict self,
											__UNIQUE_PTR_ELEMENT_PTR restrict new_ptr)
		___DISABLE_IF_NULL(self);

[[not_null(1, 2)]] void
	StdUniquePtrIdentifier(UNIQUE_T, swap)(StdUniquePtr(UNIQUE_T) * restrict self,
										   StdUniquePtr(UNIQUE_T) * restrict other)
		___DISABLE_IF_NULL(self) ___DISABLE_IF_NULL(other);

[[nodiscard]] [[not_null(1)]] __UNIQUE_PTR_CONST_ELEMENT_PTR
	StdUniquePtrIdentifier(UNIQUE_T, get_const)(const StdUniquePtr(UNIQUE_T) * restrict self)
		___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] __UNIQUE_PTR_ELEMENT_PTR
	StdUniquePtrIdentifier(UNIQUE_T, get)(StdUniquePtr(UNIQUE_T) * restrict self)
		___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] [[returns_not_null]] StdUniquePtrIdentifier(UNIQUE_T, Deleter)
	StdUniquePtrIdentifier(UNIQUE_T, get_deleter)(const StdUniquePtr(UNIQUE_T) * restrict self)
		___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] bool
	StdUniquePtrIdentifier(UNIQUE_T, as_bool)(const StdUniquePtr(UNIQUE_T) * restrict self)
		___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] __UNIQUE_PTR_CONST_ELEMENT_PTR
	StdUniquePtrIdentifier(UNIQUE_T, at_const)(const StdUniquePtr(UNIQUE_T) * restrict self,
											   usize index) ___DISABLE_IF_NULL(self)
		std_disable_if(!__UNIQUE_PTR_IS_ARRAY,
					   "std_unique_ptr_at_const is only available if "
					   "UNIQUE_T is an array type");

[[nodiscard]] [[not_null(1)]] __UNIQUE_PTR_ELEMENT_PTR
	StdUniquePtrIdentifier(UNIQUE_T, at)(StdUniquePtr(UNIQUE_T) * restrict self, usize index)
		___DISABLE_IF_NULL(self) std_disable_if(!__UNIQUE_PTR_IS_ARRAY,
												"std_unique_ptr_at is only available if "
												"UNIQUE_T is an array type");

[[not_null(1)]] void StdUniquePtrIdentifier(UNIQUE_T, free)(void* self);

typedef struct StdUniquePtrIdentifier(UNIQUE_T, vtable) {
	__UNIQUE_PTR_ELEMENT_PTR(*const release)
	(StdUniquePtr(UNIQUE_T)* restrict self);
	void (*const reset)(StdUniquePtr(UNIQUE_T)* restrict self,
						__UNIQUE_PTR_ELEMENT_PTR restrict new_ptr);
	void (*const swap)(StdUniquePtr(UNIQUE_T)* restrict self,
					   StdUniquePtr(UNIQUE_T)* restrict other);
	__UNIQUE_PTR_CONST_ELEMENT_PTR(*const get_const)
	(const StdUniquePtr(UNIQUE_T)* restrict self);
	__UNIQUE_PTR_ELEMENT_PTR (*const get)(StdUniquePtr(UNIQUE_T)* restrict self);
	StdUniquePtrIdentifier(UNIQUE_T, Deleter) (*const get_deleter)(
		const StdUniquePtr(UNIQUE_T)* restrict self);
	bool (*const as_bool)(const StdUniquePtr(UNIQUE_T)* restrict self);
}
StdUniquePtrIdentifier(UNIQUE_T, vtable);

[[maybe_unused]] static let StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)
	= (StdUniquePtrIdentifier(UNIQUE_T, vtable)){
		.release = StdUniquePtrIdentifier(UNIQUE_T, release),
		.reset = StdUniquePtrIdentifier(UNIQUE_T, reset),
		.swap = StdUniquePtrIdentifier(UNIQUE_T, swap),
		.get_const = StdUniquePtrIdentifier(UNIQUE_T, get_const),
		.get = StdUniquePtrIdentifier(UNIQUE_T, get),
		.get_deleter = StdUniquePtrIdentifier(UNIQUE_T, get_deleter),
		.as_bool = StdUniquePtrIdentifier(UNIQUE_T, as_bool),
	};

	#undef ___DISABLE_IF_NULL
	#undef STD_TEMPlATE_SUPPRESS_INSTANTIATIONS
#endif // defined(UNIQUE_T) && UNIQUE_DECL
