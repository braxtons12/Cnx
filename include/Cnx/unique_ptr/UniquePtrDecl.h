/// @file UniquePtrDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the type definitions and function declarations for a struct template
/// for representing a uniquely owned pointer
/// @version 0.2.1
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

#include <Cnx/Def.h>

#if defined(UNIQUE_T) && defined(UNIQUE_DELETER) && UNIQUE_DECL

IGNORE_MISSING_FIELD_INITIALIZERS_WARNING_START

	#define CNX_TEMPlATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Allocators.h>
	#include <Cnx/Assert.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/TypeTraits.h>
	#include <Cnx/unique_ptr/UniquePtrDef.h>

typedef struct CnxUniquePtr(UNIQUE_T) CnxUniquePtr(UNIQUE_T);

typedef struct CnxUniquePtrIdentifier(UNIQUE_T, vtable) CnxUniquePtrIdentifier(UNIQUE_T, vtable);
typedef CnxDeleter(UNIQUE_T, CnxUniquePtrIdentifier(UNIQUE_T, Deleter));

typedef struct CnxUniquePtr(UNIQUE_T) {
	union {
		__UNIQUE_PTR_ELEMENT_PTR m_ptr;
		UNIQUE_T* m_type;
	};
	CnxAllocator m_allocator;
	const CnxUniquePtrIdentifier(UNIQUE_T, vtable) * m_vtable;
}
CnxUniquePtr(UNIQUE_T);

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!(self), "Can't perform an operator on a null unique_ptr")

__attr(nodiscard) UNIQUE_STATIC UNIQUE_INLINE CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, default)(void);

__attr(nodiscard) UNIQUE_STATIC UNIQUE_INLINE CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, default_with_allocator)(CnxAllocator allocator);

__attr(nodiscard) UNIQUE_STATIC UNIQUE_INLINE CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, new)(void)
		cnx_disable_if(__UNIQUE_PTR_IS_ARRAY,
					   "cnx_unique_ptr_new is only available if "
					   "UNIQUE_T is NOT an array type");

__attr(nodiscard) UNIQUE_STATIC UNIQUE_INLINE CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, new_with_allocator)(CnxAllocator allocator)
		cnx_disable_if(__UNIQUE_PTR_IS_ARRAY,
					   "cnx_unique_ptr_new_with_allocator is only available if "
					   "UNIQUE_T is NOT an array type");

__attr(nodiscard) UNIQUE_STATIC UNIQUE_INLINE CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, new_with_capacity)(usize capacity)
		cnx_disable_if(!__UNIQUE_PTR_IS_ARRAY,
					   "cnx_unique_ptr_new_with_capacity is only available if "
					   "UNIQUE_T is an array type");

__attr(nodiscard) UNIQUE_STATIC UNIQUE_INLINE CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, new_with_capacity_and_allocator)(usize capacity,
																	  CnxAllocator allocator)
		cnx_disable_if(!__UNIQUE_PTR_IS_ARRAY,
					   "cnx_unique_ptr_new_with_capacity_and_allocator is only available if "
					   "UNIQUE_T is an array type");

__attr(nodiscard) UNIQUE_STATIC UNIQUE_INLINE CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, from)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr);

__attr(nodiscard) UNIQUE_STATIC UNIQUE_INLINE CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, from_with_allocator)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr,
														  CnxAllocator allocator);

__attr(nodiscard) __attr(not_null(1)) UNIQUE_STATIC UNIQUE_INLINE __UNIQUE_PTR_ELEMENT_PTR
	CnxUniquePtrIdentifier(UNIQUE_T, release)(CnxUniquePtr(UNIQUE_T) * restrict self)
		___DISABLE_IF_NULL(self);

__attr(not_null(1)) UNIQUE_STATIC UNIQUE_INLINE
	void CnxUniquePtrIdentifier(UNIQUE_T, reset)(CnxUniquePtr(UNIQUE_T) * restrict self,
												 __UNIQUE_PTR_ELEMENT_PTR restrict new_ptr)
		___DISABLE_IF_NULL(self);

__attr(not_null(1, 2)) UNIQUE_STATIC UNIQUE_INLINE
	void CnxUniquePtrIdentifier(UNIQUE_T, swap)(CnxUniquePtr(UNIQUE_T) * restrict self,
												CnxUniquePtr(UNIQUE_T) * restrict other)
		___DISABLE_IF_NULL(self) ___DISABLE_IF_NULL(other);

__attr(nodiscard) __attr(not_null(1)) UNIQUE_STATIC UNIQUE_INLINE __UNIQUE_PTR_CONST_ELEMENT_PTR
	CnxUniquePtrIdentifier(UNIQUE_T, get_const)(const CnxUniquePtr(UNIQUE_T) * restrict self)
		___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) UNIQUE_STATIC UNIQUE_INLINE __UNIQUE_PTR_ELEMENT_PTR
	CnxUniquePtrIdentifier(UNIQUE_T, get)(CnxUniquePtr(UNIQUE_T) * restrict self)
		___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) UNIQUE_STATIC UNIQUE_INLINE
	CnxUniquePtrIdentifier(UNIQUE_T, Deleter)
		CnxUniquePtrIdentifier(UNIQUE_T, get_deleter)(const CnxUniquePtr(UNIQUE_T) * restrict self)
			___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) UNIQUE_STATIC UNIQUE_INLINE
	bool CnxUniquePtrIdentifier(UNIQUE_T, as_bool)(const CnxUniquePtr(UNIQUE_T) * restrict self)
		___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) UNIQUE_STATIC UNIQUE_INLINE __UNIQUE_PTR_CONST_ELEMENT_PTR
	CnxUniquePtrIdentifier(UNIQUE_T, at_const)(const CnxUniquePtr(UNIQUE_T) * restrict self,
											   usize index) ___DISABLE_IF_NULL(self)
		cnx_disable_if(!__UNIQUE_PTR_IS_ARRAY,
					   "cnx_unique_ptr_at_const is only available if "
					   "UNIQUE_T is an array type");

__attr(nodiscard) __attr(not_null(1)) UNIQUE_STATIC UNIQUE_INLINE __UNIQUE_PTR_ELEMENT_PTR
	CnxUniquePtrIdentifier(UNIQUE_T, at)(CnxUniquePtr(UNIQUE_T) * restrict self, usize index)
		___DISABLE_IF_NULL(self) cnx_disable_if(!__UNIQUE_PTR_IS_ARRAY,
												"cnx_unique_ptr_at is only available if "
												"UNIQUE_T is an array type");

__attr(not_null(1)) UNIQUE_STATIC UNIQUE_INLINE
	void CnxUniquePtrIdentifier(UNIQUE_T, free)(void* self);

typedef struct CnxUniquePtrIdentifier(UNIQUE_T, vtable) {
	__UNIQUE_PTR_ELEMENT_PTR(*const release)
	(CnxUniquePtr(UNIQUE_T)* restrict self);
	void (*const reset)(CnxUniquePtr(UNIQUE_T)* restrict self,
						__UNIQUE_PTR_ELEMENT_PTR restrict new_ptr);
	void (*const swap)(CnxUniquePtr(UNIQUE_T)* restrict self,
					   CnxUniquePtr(UNIQUE_T)* restrict other);
	__UNIQUE_PTR_CONST_ELEMENT_PTR(*const get_const)
	(const CnxUniquePtr(UNIQUE_T)* restrict self);
	__UNIQUE_PTR_ELEMENT_PTR (*const get)(CnxUniquePtr(UNIQUE_T)* restrict self);
	CnxUniquePtrIdentifier(UNIQUE_T, Deleter) (*const get_deleter)(
		const CnxUniquePtr(UNIQUE_T)* restrict self);
	bool (*const as_bool)(const CnxUniquePtr(UNIQUE_T)* restrict self);
}
CnxUniquePtrIdentifier(UNIQUE_T, vtable);

__attr(maybe_unused) static let CnxUniquePtrIdentifier(UNIQUE_T, vtable_impl)
	= (CnxUniquePtrIdentifier(UNIQUE_T, vtable)){
		.release = CnxUniquePtrIdentifier(UNIQUE_T, release),
		.reset = CnxUniquePtrIdentifier(UNIQUE_T, reset),
		.swap = CnxUniquePtrIdentifier(UNIQUE_T, swap),
		.get_const = CnxUniquePtrIdentifier(UNIQUE_T, get_const),
		.get = CnxUniquePtrIdentifier(UNIQUE_T, get),
		.get_deleter = CnxUniquePtrIdentifier(UNIQUE_T, get_deleter),
		.as_bool = CnxUniquePtrIdentifier(UNIQUE_T, as_bool),
	};

	#undef ___DISABLE_IF_NULL
	#undef CNX_TEMPlATE_SUPPRESS_INSTANTIATIONS

IGNORE_MISSING_FIELD_INITIALIZERS_WARNING_STOP
#endif // defined(UNIQUE_T) && UNIQUE_DECL
