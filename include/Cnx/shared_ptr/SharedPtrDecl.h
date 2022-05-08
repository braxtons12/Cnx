/// @file SharedPtrDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the type definitions and function declarations for a struct template
/// for representing a sharedly owned pointer
/// @version 0.2.1
/// @date 2022-04-30
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

#if defined(SHARED_T) && defined(SHARED_DELETER) && SHARED_DECL

IGNORE_MISSING_FIELD_INITIALIZERS_WARNING_START

	#define CNX_TEMPlATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Allocators.h>
	#include <Cnx/Assert.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/TypeTraits.h>
	#include <Cnx/shared_ptr/SharedPtrDef.h>
	#include <Cnx/Atomic.h>

typedef struct CnxSharedPtr(SHARED_T) CnxSharedPtr(SHARED_T);

typedef struct CnxSharedPtrIdentifier(SHARED_T, vtable) CnxSharedPtrIdentifier(SHARED_T, vtable);
typedef CnxDeleter(SHARED_T, CnxSharedPtrIdentifier(SHARED_T, Deleter));

typedef struct CnxSharedPtr(SHARED_T) {
	union {
		__SHARED_PTR_ELEMENT_PTR m_ptr;
		SHARED_T* m_type;
	};
	atomic_usize* m_ref_count;
	CnxAllocator m_allocator;
	const CnxSharedPtrIdentifier(SHARED_T, vtable) * m_vtable;
}
CnxSharedPtr(SHARED_T);

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!(self), "Can't perform an operator on a null shared_ptr")

__attr(nodiscard) SHARED_STATIC SHARED_INLINE CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, default)(void);

__attr(nodiscard) SHARED_STATIC SHARED_INLINE CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, default_with_allocator)(CnxAllocator allocator);

__attr(nodiscard) SHARED_STATIC SHARED_INLINE CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, new)(void)
		cnx_disable_if(__SHARED_PTR_IS_ARRAY,
					   "cnx_shared_ptr_new is only available if "
					   "SHARED_T is NOT an array type");

__attr(nodiscard) SHARED_STATIC SHARED_INLINE CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, new_with_allocator)(CnxAllocator allocator)
		cnx_disable_if(__SHARED_PTR_IS_ARRAY,
					   "cnx_shared_ptr_new_with_allocator is only available if "
					   "SHARED_T is NOT an array type");

__attr(nodiscard) SHARED_STATIC SHARED_INLINE CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, new_with_capacity)(usize capacity)
		cnx_disable_if(!__SHARED_PTR_IS_ARRAY,
					   "cnx_shared_ptr_new_with_capacity is only available if "
					   "SHARED_T is an array type");

__attr(nodiscard) SHARED_STATIC SHARED_INLINE CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, new_with_capacity_and_allocator)(usize capacity,
																	  CnxAllocator allocator)
		cnx_disable_if(!__SHARED_PTR_IS_ARRAY,
					   "cnx_shared_ptr_new_with_capacity_and_allocator is only available if "
					   "SHARED_T is an array type");

__attr(nodiscard) SHARED_STATIC SHARED_INLINE CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, from)(__SHARED_PTR_ELEMENT_PTR restrict ptr);

__attr(nodiscard) SHARED_STATIC SHARED_INLINE CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, from_with_allocator)(__SHARED_PTR_ELEMENT_PTR restrict ptr,
														  CnxAllocator allocator);

__attr(nodiscard) __attr(not_null(1)) SHARED_STATIC SHARED_INLINE __SHARED_PTR_ELEMENT_PTR
	CnxSharedPtrIdentifier(SHARED_T, release)(CnxSharedPtr(SHARED_T) * restrict self)
		___DISABLE_IF_NULL(self);

__attr(not_null(1)) SHARED_STATIC SHARED_INLINE
	void CnxSharedPtrIdentifier(SHARED_T, reset)(CnxSharedPtr(SHARED_T) * restrict self,
												 __SHARED_PTR_ELEMENT_PTR restrict new_ptr)
		___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) SHARED_STATIC SHARED_INLINE CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, clone)(const CnxSharedPtr(SHARED_T) * restrict self)
		___DISABLE_IF_NULL(self);

__attr(not_null(1, 2)) SHARED_STATIC SHARED_INLINE
	void CnxSharedPtrIdentifier(SHARED_T, swap)(CnxSharedPtr(SHARED_T) * restrict self,
												CnxSharedPtr(SHARED_T) * restrict other)
		___DISABLE_IF_NULL(self) ___DISABLE_IF_NULL(other);

__attr(nodiscard) __attr(not_null(1)) SHARED_STATIC SHARED_INLINE __SHARED_PTR_CONST_ELEMENT_PTR
	CnxSharedPtrIdentifier(SHARED_T, get_const)(const CnxSharedPtr(SHARED_T) * restrict self)
		___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) SHARED_STATIC SHARED_INLINE __SHARED_PTR_ELEMENT_PTR
	CnxSharedPtrIdentifier(SHARED_T, get)(CnxSharedPtr(SHARED_T) * restrict self)
		___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) __attr(returns_not_null) SHARED_STATIC SHARED_INLINE
	CnxSharedPtrIdentifier(SHARED_T, Deleter)
		CnxSharedPtrIdentifier(SHARED_T, get_deleter)(const CnxSharedPtr(SHARED_T) * restrict self)
			___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) SHARED_STATIC SHARED_INLINE
	bool CnxSharedPtrIdentifier(SHARED_T, as_bool)(const CnxSharedPtr(SHARED_T) * restrict self)
		___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) SHARED_STATIC SHARED_INLINE __SHARED_PTR_CONST_ELEMENT_PTR
	CnxSharedPtrIdentifier(SHARED_T, at_const)(const CnxSharedPtr(SHARED_T) * restrict self,
											   usize index) ___DISABLE_IF_NULL(self)
		cnx_disable_if(!__SHARED_PTR_IS_ARRAY,
					   "cnx_shared_ptr_at_const is only available if "
					   "SHARED_T is an array type");

__attr(nodiscard) __attr(not_null(1)) SHARED_STATIC SHARED_INLINE __SHARED_PTR_ELEMENT_PTR
	CnxSharedPtrIdentifier(SHARED_T, at)(CnxSharedPtr(SHARED_T) * restrict self, usize index)
		___DISABLE_IF_NULL(self) cnx_disable_if(!__SHARED_PTR_IS_ARRAY,
												"cnx_shared_ptr_at is only available if "
												"SHARED_T is an array type");

__attr(not_null(1)) SHARED_STATIC SHARED_INLINE
	void CnxSharedPtrIdentifier(SHARED_T, free)(void* self);

typedef struct CnxSharedPtrIdentifier(SHARED_T, vtable) {
	__SHARED_PTR_ELEMENT_PTR(*const release)
	(CnxSharedPtr(SHARED_T)* restrict self);
	void (*const reset)(CnxSharedPtr(SHARED_T)* restrict self,
						__SHARED_PTR_ELEMENT_PTR restrict new_ptr);
	CnxSharedPtr(SHARED_T) (*const clone)(const CnxSharedPtr(SHARED_T)* restrict self);
	void (*const swap)(CnxSharedPtr(SHARED_T)* restrict self,
					   CnxSharedPtr(SHARED_T)* restrict other);
	__SHARED_PTR_CONST_ELEMENT_PTR(*const get_const)
	(const CnxSharedPtr(SHARED_T)* restrict self);
	__SHARED_PTR_ELEMENT_PTR (*const get)(CnxSharedPtr(SHARED_T)* restrict self);
	CnxSharedPtrIdentifier(SHARED_T, Deleter) (*const get_deleter)(
		const CnxSharedPtr(SHARED_T)* restrict self);
	bool (*const as_bool)(const CnxSharedPtr(SHARED_T)* restrict self);
}
CnxSharedPtrIdentifier(SHARED_T, vtable);

__attr(maybe_unused) static let CnxSharedPtrIdentifier(SHARED_T, vtable_impl)
	= (CnxSharedPtrIdentifier(SHARED_T, vtable)){
		.release = CnxSharedPtrIdentifier(SHARED_T, release),
		.reset = CnxSharedPtrIdentifier(SHARED_T, reset),
		.clone = CnxSharedPtrIdentifier(SHARED_T, clone),
		.swap = CnxSharedPtrIdentifier(SHARED_T, swap),
		.get_const = CnxSharedPtrIdentifier(SHARED_T, get_const),
		.get = CnxSharedPtrIdentifier(SHARED_T, get),
		.get_deleter = CnxSharedPtrIdentifier(SHARED_T, get_deleter),
		.as_bool = CnxSharedPtrIdentifier(SHARED_T, as_bool),
	};

	#undef ___DISABLE_IF_NULL
	#undef CNX_TEMPlATE_SUPPRESS_INSTANTIATIONS

IGNORE_MISSING_FIELD_INITIALIZERS_WARNING_STOP
#endif // defined(SHARED_T) && SHARED_DECL
