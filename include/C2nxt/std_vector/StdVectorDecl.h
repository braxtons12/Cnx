/// @file StdVectorDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the type and function declarations for a template instantiation of
/// `StdVector(T)`
/// @version 0.2.2
/// @date 2022-03-09
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

#if defined(T) && defined(SMALL_OPT_CAPACITY) && STD_TEMPLATE_DECL

	#define COLLECTION_DATA_ELEMENT T
	#define COLLECTION_DATA_COLLECTION StdVector(T)
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

typedef struct StdVectorIdentifier(T, vtable) StdVectorIdentifier(T, vtable);
typedef struct StdVector(T) {
	union {
		T m_short[SMALL_OPT_CAPACITY];
		T* m_long;
	};
	usize m_size;
	usize m_capacity;
	StdAllocator m_allocator;
	const StdCollectionData(StdVector(T)) * m_data;
	const StdVectorIdentifier(T, vtable) * m_vtable;
}
StdVector(T);

typedef struct StdVectorIterator(T) {
	isize m_index;
	StdVector(T) * m_vector;
}
StdVectorIterator(T);

typedef struct StdVectorConstIterator(T) {
	isize m_index;
	const StdVector(T) * m_vector;
}
StdVectorConstIterator(T);

[[nodiscard]] StdVector(T) StdVectorIdentifier(T, new)(void);
[[nodiscard]] StdVector(T) StdVectorIdentifier(T, new_with_allocator)(StdAllocator allocator);
[[nodiscard]] [[not_null(1)]] StdVector(T) StdVectorIdentifier(T, new_with_collection_data)(
	const StdCollectionData(StdVector(T)) * restrict data)
	std_disable_if(!data,
				   "Can't create a StdVector(T) with null StdCollectionData. To create a "
				   "StdVector(T) with defaulted StdCollectionData, use std_vector_new()");
[[nodiscard]] [[not_null(2)]] StdVector(T)
	StdVectorIdentifier(T, new_with_allocator_and_collection_data)(
		StdAllocator allocator,
		const StdCollectionData(StdVector(T)) * restrict data)
		std_disable_if(!data,
					   "Can't create a StdVector(T) with null StdCollectionData. To create a "
					   "StdVector(T) with a custom allocator and defaulted StdCollectionData, use "
					   "std_vector_new_with_allocator()");
[[nodiscard]] StdVector(T) StdVectorIdentifier(T, new_with_capacity)(usize capacity);
[[nodiscard]] StdVector(T)
	StdVectorIdentifier(T, new_with_capacity_and_allocator)(usize capacity, StdAllocator allocator);
[[nodiscard]] [[not_null(2)]] StdVector(T)
	StdVectorIdentifier(T, new_with_capacity_and_collection_data)(
		usize capacity,
		const StdCollectionData(StdVector(T)) * restrict data)
		std_disable_if(!data,
					   "Can't create a StdVector(T) with null StdCollectionData. To create a "
					   "StdVector(T) with an initial capacity and defaulted StdCollectionData, use "
					   "std_vector_new_with_capacity()");
[[nodiscard]] [[not_null(3)]] StdVector(T)
	StdVectorIdentifier(T, new_with_capacity_allocator_and_collection_data)(
		usize capacity,
		StdAllocator allocator,
		const StdCollectionData(StdVector(T)) * restrict data)
		std_disable_if(!data,
					   "Can't create a StdVector(T) with null StdCollectionData. To create a "
					   "StdVector(T) with an initial capacity, custom allocator and defaulted "
					   "StdCollectionData, use "
					   "std_vector_new_with_capacity_and_allocator()");
[[nodiscard]] [[not_null(1)]] StdVector(T)
	StdVectorIdentifier(T, clone)(const StdVector(T) * restrict self)
		std_disable_if(!(self->m_data->m_copy_constructor),
					   "Can't clone a StdVector(T) with elements that aren't copyable (no "
					   "element copy constructor defined)");

	#define ___DISABLE_IF_NULL(self) \
		std_disable_if(!self, "Can't perform an operator on a null vector")

[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const T*
	StdVectorIdentifier(T, at_const)(const StdVector(T) * restrict self, usize index)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] T*
	StdVectorIdentifier(T, at_mut)(StdVector(T) * restrict self, usize index)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const T*
	StdVectorIdentifier(T, front_const)(const StdVector(T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] T*
	StdVectorIdentifier(T, front_mut)(StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const T*
	StdVectorIdentifier(T, back_const)(const StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] T*
	StdVectorIdentifier(T, back_mut)(StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const T*
	StdVectorIdentifier(T, data_const)(const StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] T*
	StdVectorIdentifier(T, data_mut)(StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] bool
	StdVectorIdentifier(T, is_empty)(const StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] bool
	StdVectorIdentifier(T, is_full)(const StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] usize
	StdVectorIdentifier(T, size)(const StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] usize StdVectorIdentifier(T, max_size)(void);
[[nodiscard]] [[not_null(1)]] usize
	StdVectorIdentifier(T, capacity)(const StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void StdVectorIdentifier(T, reserve)(StdVector(T) * restrict self,
													 usize new_capacity) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void StdVectorIdentifier(T, resize)(StdVector(T) * restrict self, usize new_size)
	___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdVectorIdentifier(T, shrink_to_fit)(StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdVectorIdentifier(T, clear)(StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void StdVectorIdentifier(T, push_back)(StdVector(T) * restrict self, T element)
	___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdOption(T)
	StdVectorIdentifier(T, pop_back)(StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdVectorIdentifier(T, insert)(StdVector(T) * restrict self, T element, usize index)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void StdVectorIdentifier(T, erase)(StdVector(T) * restrict self, usize index)
	___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdVectorIdentifier(T, erase_n)(StdVector(T) * restrict self, usize index, usize num_elements)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void StdVectorIdentifier(T, free)(void* restrict self) ___DISABLE_IF_NULL(self);

DeclIntoStdRandomAccessIterator(StdVector(T), Ref(T), StdVectorIdentifier(T, into_iter), into);
DeclIntoStdRandomAccessIterator(StdVector(T),
								Ref(T),
								StdVectorIdentifier(T, into_reverse_iter),
								into_reverse);
DeclIntoStdRandomAccessIterator(StdVector(T),
								ConstRef(T),
								StdVectorIdentifier(T, into_const_iter),
								into);
DeclIntoStdRandomAccessIterator(StdVector(T),
								ConstRef(T),
								StdVectorIdentifier(T, into_reverse_const_iter),
								into_reverse);

[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(T))
	CONCAT3(std_vector_, T, _begin)(StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(T))
	CONCAT3(std_vector_, T, _end)(StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(T))
	CONCAT3(std_vector_, T, _rbegin)(StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(T))
	CONCAT3(std_vector_, T, _rend)(StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(T))
	StdVectorIdentifier(T, cbegin)(const StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(T))
	StdVectorIdentifier(T, cend)(const StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(T))
	StdVectorIdentifier(T, crbegin)(const StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(T))
	StdVectorIdentifier(T, crend)(const StdVector(T) * restrict self) ___DISABLE_IF_NULL(self);

typedef struct StdVectorIdentifier(T, vtable) {
	StdVector(T) (*const clone)(const StdVector(T)* restrict self);
	const T* (*const at_const)(const StdVector(T)* restrict self, usize index);
	T* (*const at_mut)(StdVector(T)* restrict self, usize index);
	const T* (*const front_const)(const StdVector(T)* restrict self);
	T* (*const front_mut)(StdVector(T)* restrict self);
	const T* (*const back_const)(const StdVector(T)* restrict self);
	T* (*const back_mut)(StdVector(T)* restrict self);
	const T* (*const data_const)(const StdVector(T)* restrict self);
	T* (*const data_mut)(StdVector(T)* restrict self);
	bool (*const is_empty)(const StdVector(T)* restrict self);
	bool (*const is_full)(const StdVector(T)* restrict self);
	usize (*const size)(const StdVector(T)* restrict self);
	usize (*const capacity)(const StdVector(T)* restrict self);
	void (*const reserve)(StdVector(T)* restrict self, usize new_capacity);
	void (*const resize)(StdVector(T)* restrict self, usize new_size);
	void (*const shrink_to_fit)(StdVector(T)* restrict self);
	void (*const clear)(StdVector(T)* restrict self);
	void (*const push_back)(StdVector(T)* restrict self, T element);
	StdOption(T) (*const pop_back)(StdVector(T)* restrict self);
	void (*const insert)(StdVector(T)* restrict self, T element, usize index);
	void (*const erase)(StdVector(T)* restrict self, usize index);
	void (*const erase_n)(StdVector(T)* restrict self, usize index, usize num_elements);
	void (*const free)(void* restrict self);
	StdRandomAccessIterator(Ref(T)) (*const into_iter)(const StdVector(T)* restrict self);
	StdRandomAccessIterator(Ref(T)) (*const into_reverse_iter)(const StdVector(T)* restrict self);
	StdRandomAccessIterator(ConstRef(T)) (*const into_const_iter)(
		const StdVector(T)* restrict self);
	StdRandomAccessIterator(ConstRef(T)) (*const into_reverse_const_iter)(
		const StdVector(T)* restrict self);
	StdRandomAccessIterator(Ref(T)) (*const begin)(StdVector(T)* restrict self);
	StdRandomAccessIterator(Ref(T)) (*const end)(StdVector(T)* restrict self);
	StdRandomAccessIterator(Ref(T)) (*const rbegin)(StdVector(T)* restrict self);
	StdRandomAccessIterator(Ref(T)) (*const rend)(StdVector(T)* restrict self);
	StdRandomAccessIterator(ConstRef(T)) (*const cbegin)(const StdVector(T)* restrict self);
	StdRandomAccessIterator(ConstRef(T)) (*const cend)(const StdVector(T)* restrict self);
	StdRandomAccessIterator(ConstRef(T)) (*const crbegin)(const StdVector(T)* restrict self);
	StdRandomAccessIterator(ConstRef(T)) (*const crend)(const StdVector(T)* restrict self);
}
StdVectorIdentifier(T, vtable);

	#undef ___DISABLE_IF_NULL
	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(T) && defined(SMALL_OPT_CAPACITY) && STD_TEMPLATE_DECL
