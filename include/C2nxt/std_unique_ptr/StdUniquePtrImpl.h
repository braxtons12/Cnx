/// @file StdUniquePtrImpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the function definitions for a struct template
/// for representing a uniquely owned pointer
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

#include <C2nxt/StdDef.h>

#if defined(UNIQUE_T) && UNIQUE_DECL

	#define STD_TEMPlATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdAssert.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/StdTypeTraits.h>
	#include <C2nxt/std_unique_ptr/StdUniquePtrDef.h>

	#if STD_PLATFORM_COMPILER_GCC
		#define IGNORE_NONNULL_COMPARE_WARNING_START \
			_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonnull-compare\"")
		#define IGNORE_NONNULL_COMPARE_WARNING_STOP _Pragma("GCC diagnostic pop")
	#else
		#define IGNORE_NONNULL_COMPARE_WARNING_START
		#define IGNORE_NONNULL_COMPARE_WARNING_STOP
	#endif

[[always_inline]] static inline void
StdUniquePtrIdentifier(UNIQUE_T, default_deleter)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr,
												  StdAllocator allocator) {
	std_allocator_deallocate_t(__UNIQUE_PTR_ELEMENT, allocator, ptr);
}

StdUniquePtr(UNIQUE_T) StdUniquePtrIdentifier(UNIQUE_T, default)(void) {
	return StdUniquePtrIdentifier(UNIQUE_T, default_with_allocator)(DEFAULT_ALLOCATOR);
}

StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, default_with_allocator)(StdAllocator allocator) {
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = nullptr,
									.m_allocator = allocator,
									.m_deleter = StdUniquePtrIdentifier(UNIQUE_T, default_deleter),
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

StdUniquePtr(UNIQUE_T) StdUniquePtrIdentifier(UNIQUE_T, new)(void) {
	// if compiling with clang, we'll trigger a std_disable_if clause at compile time if this is
	// ever used on an array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !STD_PLATFORM_COMPILER_CLANG
	std_assert(!__UNIQUE_PTR_IS_ARRAY,
			   "std_unique_ptr_new is not available when UNIQUE_T is an array type");
	#endif
	let_mut ptr = static_cast(__UNIQUE_PTR_ELEMENT_PTR)(
		std_allocator_allocate_t(__UNIQUE_PTR_ELEMENT, DEFAULT_ALLOCATOR).m_memory);
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = DEFAULT_ALLOCATOR,
									.m_deleter = StdUniquePtrIdentifier(UNIQUE_T, default_deleter),
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, new_with_allocator)(StdAllocator allocator) {
	// if compiling with clang, we'll trigger a std_disable_if clause at compile time if this is
	// ever used on an array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !STD_PLATFORM_COMPILER_CLANG
	std_assert(!__UNIQUE_PTR_IS_ARRAY,
			   "std_unique_ptr_new_with_allocator is not available when UNIQUE_T is an array type");
	#endif
	let_mut ptr = static_cast(__UNIQUE_PTR_ELEMENT_PTR)(
		std_allocator_allocate_t(__UNIQUE_PTR_ELEMENT, allocator).m_memory);
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = allocator,
									.m_deleter = StdUniquePtrIdentifier(UNIQUE_T, default_deleter),
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T,
						   new_with_deleter)(const StdUniquePtrIdentifier(UNIQUE_T, Deleter)
												 deleter) {
	// if compiling with clang, we'll trigger a std_disable_if clause at compile time if this is
	// ever used on an array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !STD_PLATFORM_COMPILER_CLANG
	std_assert(!__UNIQUE_PTR_IS_ARRAY,
			   "std_unique_ptr_new_with_deleter is not available when UNIQUE_T is an array type");
	#endif
	IGNORE_NONNULL_COMPARE_WARNING_START
	std_assert(deleter, "std_unique_ptr_new_with_deleter called with a null deleter");
	IGNORE_NONNULL_COMPARE_WARNING_STOP

	let_mut ptr = static_cast(__UNIQUE_PTR_ELEMENT_PTR)(
		std_allocator_allocate_t(__UNIQUE_PTR_ELEMENT, DEFAULT_ALLOCATOR).m_memory);
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = DEFAULT_ALLOCATOR,
									.m_deleter = deleter,
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

StdUniquePtr(UNIQUE_T) StdUniquePtrIdentifier(UNIQUE_T, new_with_allocator_and_deleter)(
	StdAllocator allocator,
	const StdUniquePtrIdentifier(UNIQUE_T, Deleter) deleter) {
	// if compiling with clang, we'll trigger a std_disable_if clause at compile time if this is
	// ever used on an array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !STD_PLATFORM_COMPILER_CLANG
	std_assert(!__UNIQUE_PTR_IS_ARRAY,
			   "std_unique_ptr_new_with_allocator_and_deleter is not available when UNIQUE_T is an "
			   "array type");
	#endif
	IGNORE_NONNULL_COMPARE_WARNING_START
	std_assert(deleter, "std_unique_ptr_new_with_allocator_and_deleter called with a null deleter");
	IGNORE_NONNULL_COMPARE_WARNING_STOP
	let_mut ptr = static_cast(__UNIQUE_PTR_ELEMENT_PTR)(
		std_allocator_allocate_t(__UNIQUE_PTR_ELEMENT, allocator).m_memory);
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = allocator,
									.m_deleter = deleter,
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

StdUniquePtr(UNIQUE_T) StdUniquePtrIdentifier(UNIQUE_T, new_with_capacity)(usize capacity) {
	// if compiling with clang, we'll trigger a std_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !STD_PLATFORM_COMPILER_CLANG
	std_assert(
		__UNIQUE_PTR_IS_ARRAY,
		"std_unique_ptr_new_with_capacity is not available when UNIQUE_T is NOT an array type");
	#endif
	let_mut ptr = static_cast(__UNIQUE_PTR_ELEMENT_PTR)(
		std_allocator_allocate_array_t(__UNIQUE_PTR_ELEMENT, DEFAULT_ALLOCATOR, capacity).m_memory);
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = DEFAULT_ALLOCATOR,
									.m_deleter = StdUniquePtrIdentifier(UNIQUE_T, default_deleter),
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, new_with_capacity_and_allocator)(usize capacity,
																	  StdAllocator allocator) {
	// if compiling with clang, we'll trigger a std_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !STD_PLATFORM_COMPILER_CLANG
	std_assert(__UNIQUE_PTR_IS_ARRAY,
			   "std_unique_ptr_new_with_capacity_and_allocator is not available when UNIQUE_T is "
			   "NOT an array type");
	#endif
	let_mut ptr = static_cast(__UNIQUE_PTR_ELEMENT_PTR)(
		std_allocator_allocate_array_t(__UNIQUE_PTR_ELEMENT, allocator, capacity).m_memory);
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = allocator,
									.m_deleter = StdUniquePtrIdentifier(UNIQUE_T, default_deleter),
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

StdUniquePtr(UNIQUE_T) StdUniquePtrIdentifier(UNIQUE_T, new_with_capacity_and_deleter)(
	usize capacity,
	const StdUniquePtrIdentifier(UNIQUE_T, Deleter) deleter) {
	// if compiling with clang, we'll trigger a std_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !STD_PLATFORM_COMPILER_CLANG
	std_assert(__UNIQUE_PTR_IS_ARRAY,
			   "std_unique_ptr_new_with_capacity_and_deleter is not available when UNIQUE_T is NOT "
			   "an array type");
	#endif
	IGNORE_NONNULL_COMPARE_WARNING_START
	std_assert(deleter, "std_unique_ptr_new_with_capacity_and_deleter called with a null deleter");
	IGNORE_NONNULL_COMPARE_WARNING_STOP
	let_mut ptr = static_cast(__UNIQUE_PTR_ELEMENT_PTR)(
		std_allocator_allocate_array_t(__UNIQUE_PTR_ELEMENT, DEFAULT_ALLOCATOR, capacity).m_memory);
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = DEFAULT_ALLOCATOR,
									.m_deleter = deleter,
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

StdUniquePtr(UNIQUE_T) StdUniquePtrIdentifier(UNIQUE_T, new_with_capacity_allocator_and_deleter)(
	usize capacity,
	StdAllocator allocator,
	const StdUniquePtrIdentifier(UNIQUE_T, Deleter) deleter) {
	// if compiling with clang, we'll trigger a std_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !STD_PLATFORM_COMPILER_CLANG
	std_assert(
		__UNIQUE_PTR_IS_ARRAY,
		"std_unique_ptr_new_with_capacity_allocator_and_deleter is not available when UNIQUE_T is "
		"NOT an array type");
	#endif
	IGNORE_NONNULL_COMPARE_WARNING_START
	std_assert(deleter,
			   "std_unique_ptr_new_with_capacity_allocator_and_deleter called with a null deleter");
	IGNORE_NONNULL_COMPARE_WARNING_STOP
	let_mut ptr = static_cast(__UNIQUE_PTR_ELEMENT_PTR)(
		std_allocator_allocate_array_t(__UNIQUE_PTR_ELEMENT, allocator, capacity).m_memory);
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = allocator,
									.m_deleter = deleter,
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, from)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr) {
	return StdUniquePtrIdentifier(UNIQUE_T, from_with_allocator)(ptr, DEFAULT_ALLOCATOR);
}

StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T, from_with_allocator)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr,
														  StdAllocator allocator) {
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = allocator,
									.m_deleter = StdUniquePtrIdentifier(UNIQUE_T, default_deleter),
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

StdUniquePtr(UNIQUE_T)
	StdUniquePtrIdentifier(UNIQUE_T,
						   from_with_deleter)(__UNIQUE_PTR_ELEMENT_PTR restrict ptr,
											  const StdUniquePtrIdentifier(UNIQUE_T, Deleter)
												  deleter) {
	IGNORE_NONNULL_COMPARE_WARNING_START
	std_assert(deleter, "std_unique_ptr_from_with_deleter called with a null deleter");
	IGNORE_NONNULL_COMPARE_WARNING_STOP
	return StdUniquePtrIdentifier(UNIQUE_T,
								  from_with_allocator_and_deleter)(ptr, DEFAULT_ALLOCATOR, deleter);
}

StdUniquePtr(UNIQUE_T) StdUniquePtrIdentifier(UNIQUE_T, from_with_allocator_and_deleter)(
	__UNIQUE_PTR_ELEMENT_PTR restrict ptr,
	StdAllocator allocator,
	const StdUniquePtrIdentifier(UNIQUE_T, Deleter) deleter) {
	IGNORE_NONNULL_COMPARE_WARNING_START
	std_assert(deleter,
			   "std_unique_ptr_from_with_allocator_and_deleter called with a null deleter");
	IGNORE_NONNULL_COMPARE_WARNING_STOP
	return (StdUniquePtr(UNIQUE_T)){.m_ptr = ptr,
									.m_allocator = allocator,
									.m_deleter = deleter,
									.m_vtable = &StdUniquePtrIdentifier(UNIQUE_T, vtable_impl)};
}

__UNIQUE_PTR_ELEMENT_PTR
StdUniquePtrIdentifier(UNIQUE_T, release)(StdUniquePtr(UNIQUE_T) * restrict self) {
	let_mut ptr = self->m_ptr;
	self->m_ptr = nullptr;
	return ptr;
}

void StdUniquePtrIdentifier(UNIQUE_T, reset)(StdUniquePtr(UNIQUE_T) * restrict self,
											 __UNIQUE_PTR_ELEMENT_PTR restrict new_ptr) {
	self->m_deleter(self->m_ptr, self->m_allocator);
	self->m_ptr = new_ptr;
}

void StdUniquePtrIdentifier(UNIQUE_T, swap)(StdUniquePtr(UNIQUE_T) * restrict self,
											StdUniquePtr(UNIQUE_T) * restrict other) {
	let_mut _self = self->m_ptr;
	self->m_ptr = other->m_ptr;
	other->m_ptr = _self;
}

__UNIQUE_PTR_CONST_ELEMENT_PTR
StdUniquePtrIdentifier(UNIQUE_T, get_const)(const StdUniquePtr(UNIQUE_T) * restrict self) {
	return self->m_ptr;
}

__UNIQUE_PTR_ELEMENT_PTR
StdUniquePtrIdentifier(UNIQUE_T, get)(StdUniquePtr(UNIQUE_T) * restrict self) {
	return self->m_ptr;
}

StdUniquePtrIdentifier(UNIQUE_T, Deleter)
	StdUniquePtrIdentifier(UNIQUE_T, get_deleter)(const StdUniquePtr(UNIQUE_T) * restrict self) {
	return self->m_deleter;
}

bool StdUniquePtrIdentifier(UNIQUE_T, as_bool)(const StdUniquePtr(UNIQUE_T) * restrict self) {
	return self->m_ptr != nullptr;
}

__UNIQUE_PTR_CONST_ELEMENT_PTR
StdUniquePtrIdentifier(UNIQUE_T, at_const)(const StdUniquePtr(UNIQUE_T) * restrict self,
										   usize index) {
	// if compiling with clang, we'll trigger a std_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !STD_PLATFORM_COMPILER_CLANG
	std_assert(__UNIQUE_PTR_IS_ARRAY,
			   "std_unique_ptr_at_const is not available when UNIQUE_T is not an array type");
	#endif
	return &((self->m_ptr)[index]);
}

__UNIQUE_PTR_ELEMENT_PTR
StdUniquePtrIdentifier(UNIQUE_T, at)(StdUniquePtr(UNIQUE_T) * restrict self, usize index) {
	// if compiling with clang, we'll trigger a std_disable_if clause at compile time if this is
	// ever used on a non-array type, and for GCC/others we'll trigger a static_assert when called
	// through the macro, but this could still be called if directly called instead of going through
	// the macro, so at least make it really annoying in debug builds
	// ‾‾\  (* *)  /‾‾
	//	  \ \ - / /
	//     \_| |_/
	#if !STD_PLATFORM_COMPILER_CLANG
	std_assert(__UNIQUE_PTR_IS_ARRAY,
			   "std_unique_ptr_at is not available when UNIQUE_T is not an array type");
	#endif
	return &((self->m_ptr)[index]);
}

void StdUniquePtrIdentifier(UNIQUE_T, free)(void* self) {
	let_mut _self = static_cast(StdUniquePtr(UNIQUE_T)*)(self);
	if(_self->m_ptr != nullptr) {
		_self->m_deleter(_self->m_ptr, _self->m_allocator);
	}
}

	#undef STD_TEMPlATE_SUPPRESS_INSTANTIATIONS
#endif // defined(UNIQUE_T) && UNIQUE_DECL
