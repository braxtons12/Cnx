/// @file SharedPtrImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a struct template
/// for representing a sharedly owned pointer
/// @version 0.2.0
/// @date 2022-04-15
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

#if defined(SHARED_T) && SHARED_DECL

IGNORE_MISSING_FIELD_INITIALIZERS_WARNING_START

	#define CNX_TEMPlATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Allocators.h>
	#include <Cnx/Assert.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/TypeTraits.h>
	#include <Cnx/shared_ptr/SharedPtrDef.h>

[[always_inline]] static inline void
CnxSharedPtrIdentifier(SHARED_T, default_deleter)(__SHARED_PTR_ELEMENT_PTR restrict ptr,
												  CnxAllocator allocator) {
	cnx_allocator_deallocate(allocator, ptr);
}

CnxSharedPtr(SHARED_T) CnxSharedPtrIdentifier(SHARED_T, default)(void) {
	return CnxSharedPtrIdentifier(SHARED_T, default_with_allocator)(DEFAULT_ALLOCATOR);
}

CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, default_with_allocator)(CnxAllocator allocator) {

	let_mut ref_count = cnx_allocator_allocate_t(atomic_usize, DEFAULT_ALLOCATOR);
	atomic_store(ref_count, 1);
	return (CnxSharedPtr(SHARED_T)){.m_ptr = nullptr,
									.m_ref_count = ref_count,
									.m_allocator = allocator,
									.m_vtable = &CnxSharedPtrIdentifier(SHARED_T, vtable_impl)};
}

CnxSharedPtr(SHARED_T) CnxSharedPtrIdentifier(SHARED_T, new)(void) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on an array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(!__SHARED_PTR_IS_ARRAY,
			   "cnx_shared_ptr_new is not available when SHARED_T is an array type");
	#endif
	let_mut ptr = cnx_allocator_allocate_t(__SHARED_PTR_ELEMENT, DEFAULT_ALLOCATOR);
	let_mut ref_count = cnx_allocator_allocate_t(atomic_usize, DEFAULT_ALLOCATOR);
	atomic_store(ref_count, 1);
	return (CnxSharedPtr(SHARED_T)){.m_ptr = ptr,
									.m_ref_count = ref_count,
									.m_allocator = DEFAULT_ALLOCATOR,
									.m_vtable = &CnxSharedPtrIdentifier(SHARED_T, vtable_impl)};
}

CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, new_with_allocator)(CnxAllocator allocator) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on an array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(!__SHARED_PTR_IS_ARRAY,
			   "cnx_shared_ptr_new_with_allocator is not available when SHARED_T is an array type");
	#endif
	let_mut ptr = cnx_allocator_allocate_t(__SHARED_PTR_ELEMENT, allocator);
	let_mut ref_count = cnx_allocator_allocate_t(atomic_usize, DEFAULT_ALLOCATOR);
	atomic_store(ref_count, 1);
	return (CnxSharedPtr(SHARED_T)){.m_ptr = ptr,
									.m_ref_count = ref_count,
									.m_allocator = allocator,
									.m_vtable = &CnxSharedPtrIdentifier(SHARED_T, vtable_impl)};
}

CnxSharedPtr(SHARED_T) CnxSharedPtrIdentifier(SHARED_T, new_with_capacity)(usize capacity) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(
		__SHARED_PTR_IS_ARRAY,
		"cnx_shared_ptr_new_with_capacity is not available when SHARED_T is NOT an array type");
	#endif
	let_mut ptr = cnx_allocator_allocate_array_t(__SHARED_PTR_ELEMENT, DEFAULT_ALLOCATOR, capacity);
	let_mut ref_count = cnx_allocator_allocate_t(atomic_usize, DEFAULT_ALLOCATOR);
	atomic_store(ref_count, 1);
	return (CnxSharedPtr(SHARED_T)){.m_ptr = ptr,
									.m_ref_count = ref_count,
									.m_allocator = DEFAULT_ALLOCATOR,
									.m_vtable = &CnxSharedPtrIdentifier(SHARED_T, vtable_impl)};
}

CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, new_with_capacity_and_allocator)(usize capacity,
																	  CnxAllocator allocator) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(__SHARED_PTR_IS_ARRAY,
			   "cnx_shared_ptr_new_with_capacity_and_allocator is not available when SHARED_T is "
			   "NOT an array type");
	#endif
	let_mut ptr = cnx_allocator_allocate_array_t(__SHARED_PTR_ELEMENT, allocator, capacity);
	let_mut ref_count = cnx_allocator_allocate_t(atomic_usize, DEFAULT_ALLOCATOR);
	atomic_store(ref_count, 1);
	return (CnxSharedPtr(SHARED_T)){.m_ptr = ptr,
									.m_ref_count = ref_count,
									.m_allocator = allocator,
									.m_vtable = &CnxSharedPtrIdentifier(SHARED_T, vtable_impl)};
}

CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, from)(__SHARED_PTR_ELEMENT_PTR restrict ptr) {
	return CnxSharedPtrIdentifier(SHARED_T, from_with_allocator)(ptr, DEFAULT_ALLOCATOR);
}

CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, from_with_allocator)(__SHARED_PTR_ELEMENT_PTR restrict ptr,
														  CnxAllocator allocator) {
	let_mut ref_count = cnx_allocator_allocate_t(atomic_usize, DEFAULT_ALLOCATOR);
	atomic_store(ref_count, 1);
	return (CnxSharedPtr(SHARED_T)){.m_ptr = ptr,
									.m_ref_count = ref_count,
									.m_allocator = allocator,
									.m_vtable = &CnxSharedPtrIdentifier(SHARED_T, vtable_impl)};
}

__SHARED_PTR_ELEMENT_PTR
CnxSharedPtrIdentifier(SHARED_T, release)(CnxSharedPtr(SHARED_T) * restrict self) {
	let_mut ptr = self->m_ptr;
	self->m_ptr = nullptr;
	return ptr;
}

void CnxSharedPtrIdentifier(SHARED_T, reset)(CnxSharedPtr(SHARED_T) * restrict self,
											 __SHARED_PTR_ELEMENT_PTR restrict new_ptr) {
	let_mut ptr = self->m_ptr;
	let_mut count = self->m_ref_count;
	self->m_ref_count = cnx_allocator_allocate_t(atomic_usize, self->m_allocator);
	atomic_store(self->m_ref_count, 1);
	self->m_ptr = new_ptr;
	if(ptr != nullptr && atomic_load(count) == 1) {
		cnx_allocator_deallocate(self->m_allocator, count);
		SHARED_DELETER(ptr, self->m_allocator);
	}
}

CnxSharedPtr(SHARED_T)
	CnxSharedPtrIdentifier(SHARED_T, clone)(const CnxSharedPtr(SHARED_T) * restrict self) {

	atomic_fetch_add(self->m_ref_count, 1);
	return *self;
}

void CnxSharedPtrIdentifier(SHARED_T, swap)(CnxSharedPtr(SHARED_T) * restrict self,
											CnxSharedPtr(SHARED_T) * restrict other) {
	let_mut _self = self->m_ptr;
	self->m_ptr = other->m_ptr;
	other->m_ptr = _self;
}

__SHARED_PTR_CONST_ELEMENT_PTR
CnxSharedPtrIdentifier(SHARED_T, get_const)(const CnxSharedPtr(SHARED_T) * restrict self) {
	return self->m_ptr;
}

__SHARED_PTR_ELEMENT_PTR
CnxSharedPtrIdentifier(SHARED_T, get)(CnxSharedPtr(SHARED_T) * restrict self) {
	return self->m_ptr;
}

CnxSharedPtrIdentifier(SHARED_T, Deleter) CnxSharedPtrIdentifier(SHARED_T, get_deleter)(
	[[maybe_unused]] const CnxSharedPtr(SHARED_T) * restrict self) {
	return SHARED_DELETER;
}

bool CnxSharedPtrIdentifier(SHARED_T, as_bool)(const CnxSharedPtr(SHARED_T) * restrict self) {
	return self->m_ptr != nullptr;
}

__SHARED_PTR_CONST_ELEMENT_PTR
CnxSharedPtrIdentifier(SHARED_T, at_const)(const CnxSharedPtr(SHARED_T) * restrict self,
										   usize index) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(__SHARED_PTR_IS_ARRAY,
			   "cnx_shared_ptr_at_const is not available when SHARED_T is not an array type");
	#endif
	return &((self->m_ptr)[index]);
}

__SHARED_PTR_ELEMENT_PTR
CnxSharedPtrIdentifier(SHARED_T, at)(CnxSharedPtr(SHARED_T) * restrict self, usize index) {
	// if compiling with clang, we'll trigger a cnx_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !CNX_PLATFORM_COMPILER_CLANG
	cnx_assert(__SHARED_PTR_IS_ARRAY,
			   "cnx_shared_ptr_at is not available when SHARED_T is not an array type");
	#endif
	return &((self->m_ptr)[index]);
}

void CnxSharedPtrIdentifier(SHARED_T, free)(void* self) {
	let_mut _self = static_cast(CnxSharedPtr(SHARED_T)*)(self);
	if(_self->m_ptr != nullptr && _self->m_ref_count != nullptr) {
		if(atomic_load(_self->m_ref_count) == 1) {
			let_mut ptr = _self->m_ptr;
			_self->m_ptr = nullptr;
			let_mut count = _self->m_ref_count;
			_self->m_ref_count = nullptr;
			cnx_allocator_deallocate(_self->m_allocator, count);
			SHARED_DELETER(ptr, _self->m_allocator);
		}
		else {
			atomic_fetch_sub(_self->m_ref_count, 1);
		}
	}
}

	#undef CNX_TEMPlATE_SUPPRESS_INSTANTIATIONS

IGNORE_MISSING_FIELD_INITIALIZERS_WARNING_STOP

#endif // defined(SHARED_T) && SHARED_DECL
