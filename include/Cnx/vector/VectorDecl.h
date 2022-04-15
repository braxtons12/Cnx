/// @file VectorDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the type and function declarations for a template instantiation of
/// `CnxVector(VECTOR_T)`
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
	#define COLLECTION_DATA_COLLECTION CnxVector(VECTOR_T)
	#include <Cnx/CollectionData.h>
	#undef COLLECTION_DATA_COLLECTION
	#undef COLLECTION_DATA_ELEMENT

	#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Allocators.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/Iterator.h>
	#include <Cnx/Platform.h>
	#include <Cnx/Format.h>
	#include <Cnx/vector/VectorDef.h>
	#include <Cnx/option/OptionDef.h>

typedef struct CnxVectorIdentifier(VECTOR_T, vtable) CnxVectorIdentifier(VECTOR_T, vtable);
typedef struct CnxVector(VECTOR_T) {
	union {
		VECTOR_T m_short[VECTOR_SMALL_OPT_CAPACITY];
		VECTOR_T* m_long;
	};
	usize m_size;
	usize m_capacity;
	CnxAllocator m_allocator;
	const CnxCollectionData(CnxVector(VECTOR_T)) * m_data;
	const CnxVectorIdentifier(VECTOR_T, vtable) * m_vtable;
}
CnxVector(VECTOR_T);

typedef struct CnxVectorIterator(VECTOR_T) {
	isize m_index;
	CnxVector(VECTOR_T) * m_vector;
}
CnxVectorIterator(VECTOR_T);

typedef struct CnxVectorConstIterator(VECTOR_T) {
	isize m_index;
	const CnxVector(VECTOR_T) * m_vector;
}
CnxVectorConstIterator(VECTOR_T);

[[nodiscard]] CnxVector(VECTOR_T) CnxVectorIdentifier(VECTOR_T, new)(void);
[[nodiscard]] CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_allocator)(CnxAllocator allocator);
[[nodiscard]] [[not_null(1)]] CnxVector(VECTOR_T) CnxVectorIdentifier(VECTOR_T,
																	  new_with_collection_data)(
	const CnxCollectionData(CnxVector(VECTOR_T)) * restrict data)
	cnx_disable_if(!data,
				   "Can't create a CnxVector(VECTOR_T) with null CnxCollectionData. To create a "
				   "CnxVector(VECTOR_T) with defaulted CnxCollectionData, use cnx_vector_new()");
[[nodiscard]] [[not_null(2)]] CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_allocator_and_collection_data)(
		CnxAllocator allocator,
		const CnxCollectionData(CnxVector(VECTOR_T)) * restrict data)
		cnx_disable_if(
			!data,
			"Can't create a CnxVector(VECTOR_T) with null CnxCollectionData. To create a "
			"CnxVector(VECTOR_T) with a custom allocator and defaulted CnxCollectionData, use "
			"cnx_vector_new_with_allocator()");
[[nodiscard]] CnxVector(VECTOR_T) CnxVectorIdentifier(VECTOR_T, new_with_capacity)(usize capacity);
[[nodiscard]] CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_capacity_and_allocator)(usize capacity,
																   CnxAllocator allocator);
[[nodiscard]] [[not_null(2)]] CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_capacity_and_collection_data)(
		usize capacity,
		const CnxCollectionData(CnxVector(VECTOR_T)) * restrict data)
		cnx_disable_if(
			!data,
			"Can't create a CnxVector(VECTOR_T) with null CnxCollectionData. To create a "
			"CnxVector(VECTOR_T) with an initial capacity and defaulted CnxCollectionData, use "
			"cnx_vector_new_with_capacity()");
[[nodiscard]] [[not_null(3)]] CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, new_with_capacity_allocator_and_collection_data)(
		usize capacity,
		CnxAllocator allocator,
		const CnxCollectionData(CnxVector(VECTOR_T)) * restrict data)
		cnx_disable_if(!data,
					   "Can't create a CnxVector(T) with null CnxCollectionData. To create a "
					   "CnxVector(T) with an initial capacity, custom allocator and defaulted "
					   "CnxCollectionData, use "
					   "cnx_vector_new_with_capacity_and_allocator()");
[[nodiscard]] [[not_null(1)]] CnxVector(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, clone)(const CnxVector(VECTOR_T) * restrict self)
		cnx_disable_if(!(self->m_data->m_copy_constructor),
					   "Can't clone a CnxVector(T) with elements that aren't copyable (no "
					   "element copy constructor defined)");

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!self, "Can't perform an operator on a null vector")

[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const VECTOR_T*
	CnxVectorIdentifier(VECTOR_T, at_const)(const CnxVector(VECTOR_T) * restrict self, usize index)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] VECTOR_T*
	CnxVectorIdentifier(VECTOR_T, at_mut)(CnxVector(VECTOR_T) * restrict self, usize index)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const VECTOR_T*
	CnxVectorIdentifier(VECTOR_T, front_const)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] VECTOR_T*
	CnxVectorIdentifier(VECTOR_T, front_mut)(CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const VECTOR_T*
	CnxVectorIdentifier(VECTOR_T, back_const)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] VECTOR_T*
	CnxVectorIdentifier(VECTOR_T, back_mut)(CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const VECTOR_T*
	CnxVectorIdentifier(VECTOR_T, data_const)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] VECTOR_T*
	CnxVectorIdentifier(VECTOR_T, data_mut)(CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] bool
	CnxVectorIdentifier(VECTOR_T, is_empty)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] bool
	CnxVectorIdentifier(VECTOR_T, is_full)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] usize
	CnxVectorIdentifier(VECTOR_T, size)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] usize CnxVectorIdentifier(VECTOR_T, max_size)(void);
[[nodiscard]] [[not_null(1)]] usize
	CnxVectorIdentifier(VECTOR_T, capacity)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	CnxVectorIdentifier(VECTOR_T, reserve)(CnxVector(VECTOR_T) * restrict self, usize new_capacity)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void CnxVectorIdentifier(VECTOR_T, resize)(CnxVector(VECTOR_T) * restrict self,
														   usize new_size) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	CnxVectorIdentifier(VECTOR_T, shrink_to_fit)(CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void CnxVectorIdentifier(VECTOR_T, clear)(CnxVector(VECTOR_T) * restrict self)
	___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	CnxVectorIdentifier(VECTOR_T, push_back)(CnxVector(VECTOR_T) * restrict self, VECTOR_T element)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] CnxOption(VECTOR_T)
	CnxVectorIdentifier(VECTOR_T, pop_back)(CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void CnxVectorIdentifier(VECTOR_T, insert)(CnxVector(VECTOR_T) * restrict self,
														   VECTOR_T element,
														   usize index) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void CnxVectorIdentifier(VECTOR_T, erase)(CnxVector(VECTOR_T) * restrict self,
														  usize index) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	CnxVectorIdentifier(VECTOR_T, erase_n)(CnxVector(VECTOR_T) * restrict self,
										   usize index,
										   usize num_elements) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	CnxVectorIdentifier(VECTOR_T, free)(void* restrict self) ___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] CnxString
	CnxVectorIdentifier(VECTOR_T, format)(const CnxFormat* restrict self,
										  CnxFormatSpecifier specifier) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxString
	CnxVectorIdentifier(VECTOR_T, format_with_allocator)(const CnxFormat* restrict self,
														 CnxFormatSpecifier specifier,
														 CnxAllocator allocator)
		___DISABLE_IF_NULL(self);

[[maybe_unused]] static ImplTraitFor(CnxFormat,
									 CnxVector(VECTOR_T),
									 CnxVectorIdentifier(VECTOR_T, format),
									 CnxVectorIdentifier(VECTOR_T, format_with_allocator));

DeclIntoCnxRandomAccessIterator(CnxVector(VECTOR_T),
								Ref(VECTOR_T),
								CnxVectorIdentifier(VECTOR_T, into_iter),
								into);
DeclIntoCnxRandomAccessIterator(CnxVector(VECTOR_T),
								Ref(VECTOR_T),
								CnxVectorIdentifier(VECTOR_T, into_reverse_iter),
								into_reverse);
DeclIntoCnxRandomAccessIterator(CnxVector(VECTOR_T),
								ConstRef(VECTOR_T),
								CnxVectorIdentifier(VECTOR_T, into_const_iter),
								into);
DeclIntoCnxRandomAccessIterator(CnxVector(VECTOR_T),
								ConstRef(VECTOR_T),
								CnxVectorIdentifier(VECTOR_T, into_reverse_const_iter),
								into_reverse);

[[nodiscard]] [[not_null(1)]] CnxRandomAccessIterator(Ref(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, begin)(CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxRandomAccessIterator(Ref(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, end)(CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxRandomAccessIterator(Ref(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, rbegin)(CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxRandomAccessIterator(Ref(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, rend)(CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] CnxRandomAccessIterator(ConstRef(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, cbegin)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxRandomAccessIterator(ConstRef(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, cend)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxRandomAccessIterator(ConstRef(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, crbegin)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] CnxRandomAccessIterator(ConstRef(VECTOR_T))
	CnxVectorIdentifier(VECTOR_T, crend)(const CnxVector(VECTOR_T) * restrict self)
		___DISABLE_IF_NULL(self);

typedef struct CnxVectorIdentifier(VECTOR_T, vtable) {
	CnxVector(VECTOR_T) (*const clone)(const CnxVector(VECTOR_T)* restrict self);
	const VECTOR_T* (*const at_const)(const CnxVector(VECTOR_T)* restrict self, usize index);
	VECTOR_T* (*const at_mut)(CnxVector(VECTOR_T)* restrict self, usize index);
	const VECTOR_T* (*const front_const)(const CnxVector(VECTOR_T)* restrict self);
	VECTOR_T* (*const front_mut)(CnxVector(VECTOR_T)* restrict self);
	const VECTOR_T* (*const back_const)(const CnxVector(VECTOR_T)* restrict self);
	VECTOR_T* (*const back_mut)(CnxVector(VECTOR_T)* restrict self);
	const VECTOR_T* (*const data_const)(const CnxVector(VECTOR_T)* restrict self);
	VECTOR_T* (*const data_mut)(CnxVector(VECTOR_T)* restrict self);
	bool (*const is_empty)(const CnxVector(VECTOR_T)* restrict self);
	bool (*const is_full)(const CnxVector(VECTOR_T)* restrict self);
	usize (*const size)(const CnxVector(VECTOR_T)* restrict self);
	usize (*const capacity)(const CnxVector(VECTOR_T)* restrict self);
	void (*const reserve)(CnxVector(VECTOR_T)* restrict self, usize new_capacity);
	void (*const resize)(CnxVector(VECTOR_T)* restrict self, usize new_size);
	void (*const shrink_to_fit)(CnxVector(VECTOR_T)* restrict self);
	void (*const clear)(CnxVector(VECTOR_T)* restrict self);
	void (*const push_back)(CnxVector(VECTOR_T)* restrict self, VECTOR_T element);
	CnxOption(VECTOR_T) (*const pop_back)(CnxVector(VECTOR_T)* restrict self);
	void (*const insert)(CnxVector(VECTOR_T)* restrict self, VECTOR_T element, usize index);
	void (*const erase)(CnxVector(VECTOR_T)* restrict self, usize index);
	void (*const erase_n)(CnxVector(VECTOR_T)* restrict self, usize index, usize num_elements);
	void (*const free)(void* restrict self);
	CnxRandomAccessIterator(Ref(VECTOR_T)) (*const into_iter)(
		const CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(Ref(VECTOR_T)) (*const into_reverse_iter)(
		const CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(ConstRef(VECTOR_T)) (*const into_const_iter)(
		const CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(ConstRef(VECTOR_T)) (*const into_reverse_const_iter)(
		const CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(Ref(VECTOR_T)) (*const begin)(CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(Ref(VECTOR_T)) (*const end)(CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(Ref(VECTOR_T)) (*const rbegin)(CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(Ref(VECTOR_T)) (*const rend)(CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(ConstRef(VECTOR_T)) (*const cbegin)(
		const CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(ConstRef(VECTOR_T)) (*const cend)(
		const CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(ConstRef(VECTOR_T)) (*const crbegin)(
		const CnxVector(VECTOR_T)* restrict self);
	CnxRandomAccessIterator(ConstRef(VECTOR_T)) (*const crend)(
		const CnxVector(VECTOR_T)* restrict self);
}
CnxVectorIdentifier(VECTOR_T, vtable);

	#undef ___DISABLE_IF_NULL
	#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(VECTOR_T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_DECL
