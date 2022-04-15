/// @file UniquePtrImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a struct template
/// for representing a uniquely owned pointer
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

#include <Cnx/Def.h>

#if defined(UNIQUE_T) && UNIQUE_DECL

IGNORE_MISSING_FIELD_INITIALIZERS_WARNING_START

	#define CNX_TEMPlATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Allocators.h>
	#include <Cnx/Assert.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/TypeTraits.h>
	#include <Cnx/unique_ptr/UniquePtrDef.h>

[[always_inline]] static inline void
CnxUniquePtrIdentifier(UNIQUE_T, default_deleter)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr,
												  CnxAllocator allocator) {
	cnx_allocator_deallocate(allocator, ptr);
}

CnxUniquePtr(UNIQUE_T) CnxUniquePtrIdentifier(UNIQUE_T, default)(void) {
	return CnxUniquePtrIdentifier(UNIQUE_T, default_with_allocator)(DEFAULT_ALLOCATOR);
}

CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, default_with_allocator)(CnxAllocator allocator) {
	return (CnxUniquePtr(UNIQUE_T)){.m_ptr = nullptr,
									.m_allocator = allocator,
									.m_vtable = &CnxUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

CnxUniquePtr(UNIQUE_T) CnxUniquePtrIdentifier(UNIQUE_T, new)(void) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on an array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(!__UNIQUE_PTR_IS_ARRAY,
			   "cnx_unique_ptr_new is not available when UNIQUE_T is an array type");
	#endif
	let_mut ptr = cnx_allocator_allocate_t(__UNIQUE_PTR_ELEMENT, DEFAULT_ALLOCATOR);
	return (CnxUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = DEFAULT_ALLOCATOR,
									.m_vtable = &CnxUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, new_with_allocator)(CnxAllocator allocator) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on an array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(!__UNIQUE_PTR_IS_ARRAY,
			   "cnx_unique_ptr_new_with_allocator is not available when UNIQUE_T is an array type");
	#endif
	let_mut ptr = cnx_allocator_allocate_t(__UNIQUE_PTR_ELEMENT, allocator);
	return (CnxUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = allocator,
									.m_vtable = &CnxUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

CnxUniquePtr(UNIQUE_T) CnxUniquePtrIdentifier(UNIQUE_T, new_with_capacity)(usize capacity) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(
		__UNIQUE_PTR_IS_ARRAY,
		"cnx_unique_ptr_new_with_capacity is not available when UNIQUE_T is NOT an array type");
	#endif
	let_mut ptr = cnx_allocator_allocate_array_t(__UNIQUE_PTR_ELEMENT, DEFAULT_ALLOCATOR, capacity);
	return (CnxUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = DEFAULT_ALLOCATOR,
									.m_vtable = &CnxUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, new_with_capacity_and_allocator)(usize capacity,
																	  CnxAllocator allocator) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(__UNIQUE_PTR_IS_ARRAY,
			   "cnx_unique_ptr_new_with_capacity_and_allocator is not available when UNIQUE_T is "
			   "NOT an array type");
	#endif
	let_mut ptr = cnx_allocator_allocate_array_t(__UNIQUE_PTR_ELEMENT, allocator, capacity);
	return (CnxUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = allocator,
									.m_vtable = &CnxUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, from)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr) {
	return CnxUniquePtrIdentifier(UNIQUE_T, from_with_allocator)(ptr, DEFAULT_ALLOCATOR);
}

CnxUniquePtr(UNIQUE_T)
	CnxUniquePtrIdentifier(UNIQUE_T, from_with_allocator)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr,
														  CnxAllocator allocator) {
	return (CnxUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = allocator,
									.m_vtable = &CnxUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

__UNIQUE_PTR_ELEMENT_PTR
CnxUniquePtrIdentifier(UNIQUE_T, release)(CnxUniquePtr(UNIQUE_T) * restrict self) {
	let_mut ptr = self->m_ptr;
	self->m_ptr = nullptr;
	return ptr;
}

void CnxUniquePtrIdentifier(UNIQUE_T, reset)(CnxUniquePtr(UNIQUE_T) * restrict self,
											 __UNIQUE_PTR_ELEMENT_PTR restrict new_ptr) {
	let_mut ptr = self->m_ptr;
	self->m_ptr = new_ptr;
	if(ptr != nullptr) {
		UNIQUE_DELETER(ptr, self->m_allocator);
	}
}

void CnxUniquePtrIdentifier(UNIQUE_T, swap)(CnxUniquePtr(UNIQUE_T) * restrict self,
											CnxUniquePtr(UNIQUE_T) * restrict other) {
	let_mut _self = self->m_ptr;
	self->m_ptr = other->m_ptr;
	other->m_ptr = _self;
}

__UNIQUE_PTR_CONST_ELEMENT_PTR
CnxUniquePtrIdentifier(UNIQUE_T, get_const)(const CnxUniquePtr(UNIQUE_T) * restrict self) {
	return self->m_ptr;
}

__UNIQUE_PTR_ELEMENT_PTR
CnxUniquePtrIdentifier(UNIQUE_T, get)(CnxUniquePtr(UNIQUE_T) * restrict self) {
	return self->m_ptr;
}

CnxUniquePtrIdentifier(UNIQUE_T, Deleter) CnxUniquePtrIdentifier(UNIQUE_T, get_deleter)(
	[[maybe_unused]] const CnxUniquePtr(UNIQUE_T) * restrict self) {
	return UNIQUE_DELETER;
}

bool CnxUniquePtrIdentifier(UNIQUE_T, as_bool)(const CnxUniquePtr(UNIQUE_T) * restrict self) {
	return self->m_ptr != nullptr;
}

__UNIQUE_PTR_CONST_ELEMENT_PTR
CnxUniquePtrIdentifier(UNIQUE_T, at_const)(const CnxUniquePtr(UNIQUE_T) * restrict self,
										   usize index) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(__UNIQUE_PTR_IS_ARRAY,
			   "cnx_unique_ptr_at_const is not available when UNIQUE_T is not an array type");
	#endif
	return &((self->m_ptr)[index]);
}

__UNIQUE_PTR_ELEMENT_PTR
CnxUniquePtrIdentifier(UNIQUE_T, at)(CnxUniquePtr(UNIQUE_T) * restrict self, usize index) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(__UNIQUE_PTR_IS_ARRAY,
			   "cnx_unique_ptr_at is not available when UNIQUE_T is not an array type");
	#endif
	return &((self->m_ptr)[index]);
}

void CnxUniquePtrIdentifier(UNIQUE_T, free)(void* self) {
	let_mut _self = static_cast(CnxUniquePtr(UNIQUE_T)*)(self);
	if(_self->m_ptr != nullptr) {
		let_mut ptr = _self->m_ptr;
		_self->m_ptr = nullptr;
		if(ptr != nullptr) {
			UNIQUE_DELETER(ptr, _self->m_allocator);
		}
	}
}

	#undef CNX_TEMPlATE_SUPPRESS_INSTANTIATIONS

IGNORE_MISSING_FIELD_INITIALIZERS_WARNING_STOP

#endif // defined(UNIQUE_T) && UNIQUE_DECL
