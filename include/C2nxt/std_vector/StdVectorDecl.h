/// @file StdVectorDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the type and function declarations for a template instantiation of
/// `StdVector(VECTOR_T)`
/// @version 0.2.3
/// @date 2022-03-21
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

#if defined(VECTOR_T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_DECL

	#define COLLECTION_DATA_ELEMENT	   VECTOR_T
	#define COLLECTION_DATA_COLLECTION StdVector(VECTOR_T)
	#include <C2nxt/StdCollectionData.h>
	#undef COLLECTION_DATA_COLLECTION
	#undef COLLECTION_DATA_ELEMENT

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/StdIterator.h>
	#include <C2nxt/StdPlatform.h>
	#include <C2nxt/std_vector/StdVectorDef.h>
	#include <C2nxt/std_option/StdOptionDef.h>

typedef struct StdVectorIdentifier(VECTOR_T, vtable) StdVectorIdentifier(VECTOR_T, vtable);
typedef struct StdVector(VECTOR_T) {
	union {
		VECTOR_T m_short[VECTOR_SMALL_OPT_CAPACITY];
		VECTOR_T* m_long;
	};
	usize m_size;
	usize m_capacity;
	StdAllocator m_allocator;
	const StdCollectionData(StdVector(VECTOR_T)) * m_data;
	const StdVectorIdentifier(VECTOR_T, vtable) * m_vtable;
}
StdVector(VECTOR_T);

typedef struct StdVectorIterator(VECTOR_T) {
	isize m_index;
	StdVector(VECTOR_T) * m_vector;
}
StdVectorIterator(VECTOR_T);

typedef struct StdVectorConstIterator(VECTOR_T) {
	isize m_index;
	const StdVector(VECTOR_T) * m_vector;
}
StdVectorConstIterator(VECTOR_T);

[[nodiscard]] StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T, new)(void);
[[nodiscard]] StdVector(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, new_with_allocator)(StdAllocator allocator);
[[nodiscard]] [[not_null(1)]] StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T,
																	  new_with_collection_data)(
	const StdCollectionData(StdVector(VECTOR_T)) * restrict data)
	std_disable_if(!data,
				   "Can't create a StdVector(VECTOR_T) with null StdCollectionData. To create a "
				   "StdVector(VECTOR_T) with defaulted StdCollectionData, use std_vector_new()");
[[nodiscard]] [[not_null(2)]] StdVector(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, new_with_allocator_and_collection_data)(
		StdAllocator allocator,
		const StdCollectionData(StdVector(VECTOR_T)) * restrict data)
		std_disable_if(
			!data,
			"Can't create a StdVector(VECTOR_T) with null StdCollectionData. To create a "
			"StdVector(VECTOR_T) with a custom allocator and defaulted StdCollectionData, use "
			"std_vector_new_with_allocator()");
[[nodiscard]] StdVector(VECTOR_T) StdVectorIdentifier(VECTOR_T, new_with_capacity)(usize capacity);
[[nodiscard]] StdVector(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, new_with_capacity_and_allocator)(usize capacity,
																   StdAllocator allocator);
[[nodiscard]] [[not_null(2)]] StdVector(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, new_with_capacity_and_collection_data)(
		usize capacity,
		const StdCollectionData(StdVector(VECTOR_T)) * restrict data)
		std_disable_if(
			!data,
			"Can't create a StdVector(VECTOR_T) with null StdCollectionData. To create a "
			"StdVector(VECTOR_T) with an initial capacity and defaulted StdCollectionData, use "
			"std_vector_new_with_capacity()");
[[nodiscard]] [[not_null(3)]] StdVector(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, new_with_capacity_allocator_and_collection_data)(
		usize capacity,
		StdAllocator allocator,
		const StdCollectionData(StdVector(VECTOR_T)) * restrict data)
		std_disable_if(!data,
					   "Can't create a StdVector(T) with null StdCollectionData. To create a "
					   "StdVector(T) with an initial capacity, custom allocator and defaulted "
					   "StdCollectionData, use "
					   "std_vector_new_with_capacity_and_allocator()");
[[nodiscard]] [[not_null(1)]] StdVector(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, clone)(const StdVector(VECTOR_T) * restrict self)
		std_disable_if(!(self->m_data->m_copy_constructor),
					   "Can't clone a StdVector(T) with elements that aren't copyable (no "
					   "element copy constructor defined)");

	#define ___DISABLE_IF_NULL(self) \
		std_disable_if(!self, "Can't perform an operator on a null vector")

[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const VECTOR_T*
	StdVectorIdentifier(VECTOR_T, at_const)(const StdVector(VECTOR_T) * restrict self, usize index)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] VECTOR_T*
	StdVectorIdentifier(VECTOR_T, at_mut)(StdVector(VECTOR_T) * restrict self, usize index)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const VECTOR_T*
	StdVectorIdentifier(VECTOR_T, front_const)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] VECTOR_T*
	StdVectorIdentifier(VECTOR_T, front_mut)(StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const VECTOR_T*
	StdVectorIdentifier(VECTOR_T, back_const)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] VECTOR_T*
	StdVectorIdentifier(VECTOR_T, back_mut)(StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const VECTOR_T*
	StdVectorIdentifier(VECTOR_T, data_const)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] VECTOR_T*
	StdVectorIdentifier(VECTOR_T, data_mut)(StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] bool
	StdVectorIdentifier(VECTOR_T, is_empty)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] bool
	StdVectorIdentifier(VECTOR_T, is_full)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] usize
	StdVectorIdentifier(VECTOR_T, size)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] usize StdVectorIdentifier(VECTOR_T, max_size)(void);
[[nodiscard]] [[not_null(1)]] usize
	StdVectorIdentifier(VECTOR_T, capacity)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdVectorIdentifier(VECTOR_T, reserve)(StdVector(VECTOR_T) * restrict self, usize new_capacity)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void StdVectorIdentifier(VECTOR_T, resize)(StdVector(VECTOR_T) * restrict self,
														   usize new_size) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdVectorIdentifier(VECTOR_T, shrink_to_fit)(StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void StdVectorIdentifier(VECTOR_T, clear)(StdVector(VECTOR_T) * restrict self)
	___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdVectorIdentifier(VECTOR_T, push_back)(StdVector(VECTOR_T) * restrict self, VECTOR_T element)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] StdOption(VECTOR_T)
	StdVectorIdentifier(VECTOR_T, pop_back)(StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void StdVectorIdentifier(VECTOR_T, insert)(StdVector(VECTOR_T) * restrict self,
														   VECTOR_T element,
														   usize index) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void StdVectorIdentifier(VECTOR_T, erase)(StdVector(VECTOR_T) * restrict self,
														  usize index) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdVectorIdentifier(VECTOR_T, erase_n)(StdVector(VECTOR_T) * restrict self,
										   usize index,
										   usize num_elements) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdVectorIdentifier(VECTOR_T, free)(void* restrict self) ___DISABLE_IF_NULL(self);

DeclIntoStdRandomAccessIterator(StdVector(VECTOR_T),
								Ref(VECTOR_T),
								StdVectorIdentifier(VECTOR_T, into_iter),
								into);
DeclIntoStdRandomAccessIterator(StdVector(VECTOR_T),
								Ref(VECTOR_T),
								StdVectorIdentifier(VECTOR_T, into_reverse_iter),
								into_reverse);
DeclIntoStdRandomAccessIterator(StdVector(VECTOR_T),
								ConstRef(VECTOR_T),
								StdVectorIdentifier(VECTOR_T, into_const_iter),
								into);
DeclIntoStdRandomAccessIterator(StdVector(VECTOR_T),
								ConstRef(VECTOR_T),
								StdVectorIdentifier(VECTOR_T, into_reverse_const_iter),
								into_reverse);

[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, begin)(StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, end)(StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, rbegin)(StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, rend)(StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, cbegin)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, cend)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, crbegin)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(VECTOR_T))
	StdVectorIdentifier(VECTOR_T, crend)(const StdVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);

typedef struct StdVectorIdentifier(VECTOR_T, vtable) {
	StdVector(VECTOR_T) (*const clone)(const StdVector(VECTOR_T)* restrict self);
	const VECTOR_T* (*const at_const)(const StdVector(VECTOR_T)* restrict self, usize index);
	VECTOR_T* (*const at_mut)(StdVector(VECTOR_T)* restrict self, usize index);
	const VECTOR_T* (*const front_const)(const StdVector(VECTOR_T)* restrict self);
	VECTOR_T* (*const front_mut)(StdVector(VECTOR_T)* restrict self);
	const VECTOR_T* (*const back_const)(const StdVector(VECTOR_T)* restrict self);
	VECTOR_T* (*const back_mut)(StdVector(VECTOR_T)* restrict self);
	const VECTOR_T* (*const data_const)(const StdVector(VECTOR_T)* restrict self);
	VECTOR_T* (*const data_mut)(StdVector(VECTOR_T)* restrict self);
	bool (*const is_empty)(const StdVector(VECTOR_T)* restrict self);
	bool (*const is_full)(const StdVector(VECTOR_T)* restrict self);
	usize (*const size)(const StdVector(VECTOR_T)* restrict self);
	usize (*const capacity)(const StdVector(VECTOR_T)* restrict self);
	void (*const reserve)(StdVector(VECTOR_T)* restrict self, usize new_capacity);
	void (*const resize)(StdVector(VECTOR_T)* restrict self, usize new_size);
	void (*const shrink_to_fit)(StdVector(VECTOR_T)* restrict self);
	void (*const clear)(StdVector(VECTOR_T)* restrict self);
	void (*const push_back)(StdVector(VECTOR_T)* restrict self, VECTOR_T element);
	StdOption(VECTOR_T) (*const pop_back)(StdVector(VECTOR_T)* restrict self);
	void (*const insert)(StdVector(VECTOR_T)* restrict self, VECTOR_T element, usize index);
	void (*const erase)(StdVector(VECTOR_T)* restrict self, usize index);
	void (*const erase_n)(StdVector(VECTOR_T)* restrict self, usize index, usize num_elements);
	void (*const free)(void* restrict self);
	StdRandomAccessIterator(Ref(VECTOR_T)) (*const into_iter)(
		const StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(Ref(VECTOR_T)) (*const into_reverse_iter)(
		const StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(ConstRef(VECTOR_T)) (*const into_const_iter)(
		const StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(ConstRef(VECTOR_T)) (*const into_reverse_const_iter)(
		const StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(Ref(VECTOR_T)) (*const begin)(StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(Ref(VECTOR_T)) (*const end)(StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(Ref(VECTOR_T)) (*const rbegin)(StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(Ref(VECTOR_T)) (*const rend)(StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(ConstRef(VECTOR_T)) (*const cbegin)(
		const StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(ConstRef(VECTOR_T)) (*const cend)(
		const StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(ConstRef(VECTOR_T)) (*const crbegin)(
		const StdVector(VECTOR_T)* restrict self);
	StdRandomAccessIterator(ConstRef(VECTOR_T)) (*const crend)(
		const StdVector(VECTOR_T)* restrict self);
}
StdVectorIdentifier(VECTOR_T, vtable);

	#undef ___DISABLE_IF_NULL
	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(VECTOR_T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_DECL
