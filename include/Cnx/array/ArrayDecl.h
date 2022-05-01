/// @file ArrayDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the type and function declarations for a template instantiation of
/// `CnxArray(ARRAY_T, ARRAY_N)`
///
/// Requires that the following have been met prior to use:
/// 1. a `typedef` of your type to provide an alphanumeric name for it as `T`
/// 2. a `typedef` for pointer to your type as `Ref(YourType)`
/// 3. a `typedef` for pointer to const your type as `ConstRef(YourType)`
/// 4. Instantiations for Cnx iterators for the typedefs provided in (2) and (3)
/// 5. Instantiation of `CnxOption(YourType)`, via provided macros (See `CnxOption(T)`)
///
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

#if defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_DECL

	#define COLLECTION_DATA_ELEMENT	   ARRAY_T
	#define COLLECTION_DATA_COLLECTION CnxArray(ARRAY_T, ARRAY_N)
	#include <Cnx/CollectionData.h>
	#undef COLLECTION_DATA_COLLECTION
	#undef COLLECTION_DATA_ELEMENT

	#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Allocators.h>
	#include <Cnx/BasicTypes.h>
	#include <Cnx/Iterator.h>
	#include <Cnx/Platform.h>
	#include <Cnx/array/ArrayDef.h>
	#include <Cnx/option/OptionDef.h>
	#include <Cnx/Format.h>
	#include <Cnx/String.h>

typedef struct CnxArrayIdentifier(ARRAY_T, ARRAY_N, vtable)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, vtable);
typedef struct CnxArray(ARRAY_T, ARRAY_N) {
	ARRAY_T m_array[ARRAY_N];
	usize m_size;
	CnxAllocator m_allocator;
	const CnxArrayIdentifier(ARRAY_T, ARRAY_N, vtable) * m_vtable;
	const CnxCollectionData(CnxArray(ARRAY_T, ARRAY_N)) * m_data;
}
CnxArray(ARRAY_T, ARRAY_N);

typedef struct CnxArrayIterator(ARRAY_T, ARRAY_N) {
	isize m_index;
	CnxArray(ARRAY_T, ARRAY_N) * m_array;
}
CnxArrayIterator(ARRAY_T, ARRAY_N);

typedef struct CnxArrayConstIterator(ARRAY_T, ARRAY_N) {
	isize m_index;
	const CnxArray(ARRAY_T, ARRAY_N) * m_array;
}
CnxArrayConstIterator(ARRAY_T, ARRAY_N);

__attr(nodiscard) CnxArray(ARRAY_T, ARRAY_N) CnxArrayIdentifier(ARRAY_T, ARRAY_N, new)(void);
__attr(nodiscard) CnxArray(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, new_with_allocator)(CnxAllocator allocator);
__attr(nodiscard) __attr(not_null(1)) CnxArray(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, new_with_collection_data)(
		const CnxCollectionData(CnxArray(ARRAY_T, ARRAY_N)) * restrict data)
		cnx_disable_if(!data,
					   "Can't create a CnxArray(T, N) with null CnxCollectionData. To create a "
					   "CnxArray(T, N) with defaulted CnxCollectionData, use cnx_array_new()");
__attr(nodiscard) __attr(not_null(2)) CnxArray(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, new_with_allocator_and_collection_data)(
		CnxAllocator allocator,
		const CnxCollectionData(CnxArray(ARRAY_T, ARRAY_N)) * restrict data)
		cnx_disable_if(!data,
					   "Can't create a CnxArray(T, N) with null CnxCollectionData. To create a "
					   "CnxArray(T, N) with custom allocator and defaulted CnxCollectionData, use "
					   "cnx_array_new_with_allocator()");

	#define ___DISABLE_IF_NULL(self) \
		cnx_disable_if(!self, "Can't perform an operator on a null array")

__attr(nodiscard) __attr(not_null(1)) CnxArray(ARRAY_T, ARRAY_N)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, clone)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self) cnx_disable_if(
			!(self->m_data->m_copy_constructor),
			"Can't clone a CnxArray(ARRAY_T, ARRAY_N) with elements that aren't copyable (no "
			"element copy constructor defined)");
__attr(nodiscard) __attr(not_null(1))
	__attr(returns_not_null) const ARRAY_T* CnxArrayIdentifier(ARRAY_T, ARRAY_N, at_const)(
		const CnxArray(ARRAY_T, ARRAY_N) * restrict self,
		usize index) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1))
	__attr(returns_not_null) ARRAY_T* CnxArrayIdentifier(ARRAY_T, ARRAY_N, at_mut)(
		CnxArray(ARRAY_T, ARRAY_N) * restrict self,
		usize index) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1))
	__attr(returns_not_null) const ARRAY_T* CnxArrayIdentifier(ARRAY_T, ARRAY_N, front_const)(
		const CnxArray(ARRAY_T, ARRAY_N) * restrict self) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1))
	__attr(returns_not_null) ARRAY_T* CnxArrayIdentifier(ARRAY_T, ARRAY_N, front_mut)(
		CnxArray(ARRAY_T, ARRAY_N) * restrict self) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1))
	__attr(returns_not_null) const ARRAY_T* CnxArrayIdentifier(ARRAY_T, ARRAY_N, back_const)(
		const CnxArray(ARRAY_T, ARRAY_N) * restrict self) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1))
	__attr(returns_not_null) ARRAY_T* CnxArrayIdentifier(ARRAY_T, ARRAY_N, back_mut)(
		CnxArray(ARRAY_T, ARRAY_N) * restrict self) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1))
	__attr(returns_not_null) const ARRAY_T* CnxArrayIdentifier(ARRAY_T, ARRAY_N, data_const)(
		const CnxArray(ARRAY_T, ARRAY_N) * restrict self) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1))
	__attr(returns_not_null) ARRAY_T* CnxArrayIdentifier(ARRAY_T, ARRAY_N, data_mut)(
		CnxArray(ARRAY_T, ARRAY_N) * restrict self) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) bool CnxArrayIdentifier(ARRAY_T, ARRAY_N, is_empty)(
	const CnxArray(ARRAY_T, ARRAY_N) * restrict self) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) bool CnxArrayIdentifier(ARRAY_T, ARRAY_N, is_full)(
	const CnxArray(ARRAY_T, ARRAY_N) * restrict self) ___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) usize
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, size)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) usize
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, capacity)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
__attr(not_null(1)) void CnxArrayIdentifier(ARRAY_T,
											ARRAY_N,
											resize)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
													usize new_size) ___DISABLE_IF_NULL(self);
__attr(not_null(1)) void CnxArrayIdentifier(ARRAY_T,
											ARRAY_N,
											clear)(CnxArray(ARRAY_T, ARRAY_N) * restrict self)
	___DISABLE_IF_NULL(self);
__attr(not_null(1)) void CnxArrayIdentifier(ARRAY_T,
											ARRAY_N,
											push_back)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
													   ARRAY_T element) ___DISABLE_IF_NULL(self);
__attr(not_null(1)) CnxOption(ARRAY_T)
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, pop_back)(CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
__attr(not_null(1)) void CnxArrayIdentifier(ARRAY_T,
											ARRAY_N,
											insert)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
													ARRAY_T element,
													usize index) ___DISABLE_IF_NULL(self);
__attr(not_null(1)) void CnxArrayIdentifier(ARRAY_T,
											ARRAY_N,
											erase)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
												   usize index) ___DISABLE_IF_NULL(self);
__attr(not_null(1)) void CnxArrayIdentifier(ARRAY_T,
											ARRAY_N,
											erase_n)(CnxArray(ARRAY_T, ARRAY_N) * restrict self,
													 usize index,
													 usize length) ___DISABLE_IF_NULL(self);
__attr(not_null(1)) void CnxArrayIdentifier(ARRAY_T, ARRAY_N, free)(void* restrict self)
	___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) CnxString
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, format)(const CnxFormat* restrict self,
												 CnxFormatSpecifier specifier)
		___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) CnxString
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, format_with_allocator)(const CnxFormat* restrict self,
																CnxFormatSpecifier specifier,
																CnxAllocator allocator)
		___DISABLE_IF_NULL(self);

__attr(maybe_unused) static ImplTraitFor(CnxFormat,
										 CnxArray(ARRAY_T, ARRAY_N),
										 CnxArrayIdentifier(ARRAY_T, ARRAY_N, format),
										 CnxArrayIdentifier(ARRAY_T,
															ARRAY_N,
															format_with_allocator));

DeclIntoCnxRandomAccessIterator(CnxArray(ARRAY_T, ARRAY_N),
								Ref(ARRAY_T),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_iter),
								into);
DeclIntoCnxRandomAccessIterator(CnxArray(ARRAY_T, ARRAY_N),
								Ref(ARRAY_T),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_iter),
								into_reverse);
DeclIntoCnxRandomAccessIterator(CnxArray(ARRAY_T, ARRAY_N),
								ConstRef(ARRAY_T),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_const_iter),
								into);
DeclIntoCnxRandomAccessIterator(CnxArray(ARRAY_T, ARRAY_N),
								ConstRef(ARRAY_T),
								CnxArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_const_iter),
								into_reverse);

__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIterator(Ref(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, begin)(CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIterator(Ref(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, end)(CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIterator(Ref(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, rbegin)(CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIterator(Ref(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, rend)(CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);

__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIterator(ConstRef(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, cbegin)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIterator(ConstRef(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, cend)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIterator(ConstRef(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, crbegin)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
__attr(nodiscard) __attr(not_null(1)) CnxRandomAccessIterator(ConstRef(ARRAY_T))
	CnxArrayIdentifier(ARRAY_T, ARRAY_N, crend)(const CnxArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);

typedef struct CnxArrayIdentifier(ARRAY_T, ARRAY_N, vtable) {
	CnxArray(ARRAY_T, ARRAY_N) (*const clone)(const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	const ARRAY_T* (*const at_const)(const CnxArray(ARRAY_T, ARRAY_N)* restrict self, usize index);
	ARRAY_T* (*const at_mut)(CnxArray(ARRAY_T, ARRAY_N)* restrict self, usize index);
	const ARRAY_T* (*const front_const)(const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	ARRAY_T* (*const front_mut)(CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	const ARRAY_T* (*const back_const)(const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	ARRAY_T* (*const back_mut)(CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	const ARRAY_T* (*const data_const)(const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	ARRAY_T* (*const data_mut)(CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	bool (*const is_empty)(const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	bool (*const is_full)(const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	usize (*const size)(const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	usize (*const capacity)(const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	void (*const resize)(CnxArray(ARRAY_T, ARRAY_N)* restrict self, usize new_size);
	void (*const clear)(CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	void (*const push_back)(CnxArray(ARRAY_T, ARRAY_N)* restrict self, ARRAY_T element);
	CnxOption(ARRAY_T) (*const pop_back)(CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	void (*const insert)(CnxArray(ARRAY_T, ARRAY_N)* restrict self, ARRAY_T element, usize index);
	void (*const erase)(CnxArray(ARRAY_T, ARRAY_N)* restrict self, usize index);
	void (*const erase_n)(CnxArray(ARRAY_T, ARRAY_N)* restrict self, usize index, usize length);
	void (*const free)(void* restrict self);
	CnxRandomAccessIterator(Ref(ARRAY_T)) (*const into_iter)(
		const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(Ref(ARRAY_T)) (*const into_reverse_iter)(
		const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(ConstRef(ARRAY_T)) (*const into_const_iter)(
		const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(ConstRef(ARRAY_T)) (*const into_reverse_const_iter)(
		const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(Ref(ARRAY_T)) (*const begin)(CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(Ref(ARRAY_T)) (*const end)(CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(Ref(ARRAY_T)) (*const rbegin)(
		CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(Ref(ARRAY_T)) (*const rend)(CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(ConstRef(ARRAY_T)) (*const cbegin)(
		const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(ConstRef(ARRAY_T)) (*const cend)(
		const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(ConstRef(ARRAY_T)) (*const crbegin)(
		const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
	CnxRandomAccessIterator(ConstRef(ARRAY_T)) (*const crend)(
		const CnxArray(ARRAY_T, ARRAY_N)* restrict self);
}
CnxArrayIdentifier(ARRAY_T, ARRAY_N, vtable);

	#undef ___DISABLE_IF_NULL
	#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_DECL
