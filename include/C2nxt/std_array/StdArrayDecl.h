/// @file StdArrayDecl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the type and function declarations for a template instantiation of
/// `StdArray(ARRAY_T, ARRAY_N)`
///
/// Requires that the following have been met prior to use:
/// 1. a `typedef` of your type to provide an alphanumeric name for it as `T`
/// 2. a `typedef` for pointer to your type as `Ref(YourType)`
/// 3. a `typedef` for pointer to const your type as `ConstRef(YourType)`
/// 4. Instantiations for C2nxt iterators for the typedefs provided in (2) and (3)
/// 5. Instantiation of `StdOption(YourType)`, via provided macros (See `StdOption(T)`)
///
/// @version 0.2.0
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

#if defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_DECL

	#define COLLECTION_DATA_ELEMENT	   ARRAY_T
	#define COLLECTION_DATA_COLLECTION StdArray(ARRAY_T, ARRAY_N)
	#include <C2nxt/StdCollectionData.h>
	#undef COLLECTION_DATA_COLLECTION
	#undef COLLECTION_DATA_ELEMENT

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdBasicTypes.h>
	#include <C2nxt/StdIterator.h>
	#include <C2nxt/StdPlatform.h>
	#include <C2nxt/std_array/StdArrayDef.h>
	#include <C2nxt/std_option/StdOptionDef.h>
	#include <C2nxt/StdFormat.h>
	#include <C2nxt/StdString.h>

typedef struct StdArrayIdentifier(ARRAY_T, ARRAY_N, vtable)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, vtable);
typedef struct StdArray(ARRAY_T, ARRAY_N) {
	ARRAY_T m_array[ARRAY_N];
	usize m_size;
	StdAllocator m_allocator;
	const StdArrayIdentifier(ARRAY_T, ARRAY_N, vtable) * m_vtable;
	const StdCollectionData(StdArray(ARRAY_T, ARRAY_N)) * m_data;
}
StdArray(ARRAY_T, ARRAY_N);

typedef struct StdArrayIterator(ARRAY_T, ARRAY_N) {
	isize m_index;
	StdArray(ARRAY_T, ARRAY_N) * m_array;
}
StdArrayIterator(ARRAY_T, ARRAY_N);

typedef struct StdArrayConstIterator(ARRAY_T, ARRAY_N) {
	isize m_index;
	const StdArray(ARRAY_T, ARRAY_N) * m_array;
}
StdArrayConstIterator(ARRAY_T, ARRAY_N);

[[nodiscard]] StdArray(ARRAY_T, ARRAY_N) StdArrayIdentifier(ARRAY_T, ARRAY_N, new)(void);
[[nodiscard]] StdArray(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, new_with_allocator)(StdAllocator allocator);
[[nodiscard]] [[not_null(1)]] StdArray(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, new_with_collection_data)(
		const StdCollectionData(StdArray(ARRAY_T, ARRAY_N)) * restrict data)
		std_disable_if(!data,
					   "Can't create a StdArray(T, N) with null StdCollectionData. To create a "
					   "StdArray(T, N) with defaulted StdCollectionData, use std_array_new()");
[[nodiscard]] [[not_null(2)]] StdArray(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, new_with_allocator_and_collection_data)(
		StdAllocator allocator,
		const StdCollectionData(StdArray(ARRAY_T, ARRAY_N)) * restrict data)
		std_disable_if(!data,
					   "Can't create a StdArray(T, N) with null StdCollectionData. To create a "
					   "StdArray(T, N) with custom allocator and defaulted StdCollectionData, use "
					   "std_array_new_with_allocator()");

	#define ___DISABLE_IF_NULL(self) \
		std_disable_if(!self, "Can't perform an operator on a null array")

[[nodiscard]] [[not_null(1)]] StdArray(ARRAY_T, ARRAY_N)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, clone)(const StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self) std_disable_if(
			!(self->m_data->m_copy_constructor),
			"Can't clone a StdArray(ARRAY_T, ARRAY_N) with elements that aren't copyable (no "
			"element copy constructor defined)");
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const ARRAY_T*
	StdArrayIdentifier(ARRAY_T, ARRAY_N, at_const)(const StdArray(ARRAY_T, ARRAY_N) * restrict self,
												   usize index) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] ARRAY_T*
	StdArrayIdentifier(ARRAY_T, ARRAY_N, at_mut)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
												 usize index) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const ARRAY_T*
	StdArrayIdentifier(ARRAY_T, ARRAY_N, front_const)(const StdArray(ARRAY_T,
																	 ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] ARRAY_T*
	StdArrayIdentifier(ARRAY_T, ARRAY_N, front_mut)(StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const ARRAY_T*
	StdArrayIdentifier(ARRAY_T, ARRAY_N, back_const)(const StdArray(ARRAY_T,
																	ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] ARRAY_T*
	StdArrayIdentifier(ARRAY_T, ARRAY_N, back_mut)(StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] const ARRAY_T*
	StdArrayIdentifier(ARRAY_T, ARRAY_N, data_const)(const StdArray(ARRAY_T,
																	ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] [[returns_not_null]] ARRAY_T*
	StdArrayIdentifier(ARRAY_T, ARRAY_N, data_mut)(StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] bool
	StdArrayIdentifier(ARRAY_T, ARRAY_N, is_empty)(const StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] bool
	StdArrayIdentifier(ARRAY_T, ARRAY_N, is_full)(const StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] usize
	StdArrayIdentifier(ARRAY_T, ARRAY_N, size)(const StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] usize
	StdArrayIdentifier(ARRAY_T, ARRAY_N, capacity)(const StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdArrayIdentifier(ARRAY_T, ARRAY_N, resize)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
												 usize new_size) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdArrayIdentifier(ARRAY_T, ARRAY_N, clear)(StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdArrayIdentifier(ARRAY_T, ARRAY_N, push_back)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
													ARRAY_T element) ___DISABLE_IF_NULL(self);
[[not_null(1)]] StdOption(ARRAY_T)
	StdArrayIdentifier(ARRAY_T, ARRAY_N, pop_back)(StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdArrayIdentifier(ARRAY_T, ARRAY_N, insert)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
												 ARRAY_T element,
												 usize index) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdArrayIdentifier(ARRAY_T, ARRAY_N, erase)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
												usize index) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdArrayIdentifier(ARRAY_T, ARRAY_N, erase_n)(StdArray(ARRAY_T, ARRAY_N) * restrict self,
												  usize index,
												  usize length) ___DISABLE_IF_NULL(self);
[[not_null(1)]] void
	StdArrayIdentifier(ARRAY_T, ARRAY_N, free)(void* restrict self) ___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdString
	StdArrayIdentifier(ARRAY_T, ARRAY_N, format)(const StdFormat* restrict self,
												 StdFormatSpecifier specifier)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdString
	StdArrayIdentifier(ARRAY_T, ARRAY_N, format_with_allocator)(const StdFormat* restrict self,
																StdFormatSpecifier specifier,
																StdAllocator allocator)
		___DISABLE_IF_NULL(self);

[[maybe_unused]] static ImplTraitFor(StdFormat,
									 StdArray(ARRAY_T, ARRAY_N),
									 StdArrayIdentifier(ARRAY_T, ARRAY_N, format),
									 StdArrayIdentifier(ARRAY_T, ARRAY_N, format_with_allocator));

DeclIntoStdRandomAccessIterator(StdArray(ARRAY_T, ARRAY_N),
								Ref(ARRAY_T),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, into_iter),
								into);
DeclIntoStdRandomAccessIterator(StdArray(ARRAY_T, ARRAY_N),
								Ref(ARRAY_T),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_iter),
								into_reverse);
DeclIntoStdRandomAccessIterator(StdArray(ARRAY_T, ARRAY_N),
								ConstRef(ARRAY_T),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, into_const_iter),
								into);
DeclIntoStdRandomAccessIterator(StdArray(ARRAY_T, ARRAY_N),
								ConstRef(ARRAY_T),
								StdArrayIdentifier(ARRAY_T, ARRAY_N, into_reverse_const_iter),
								into_reverse);

[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, begin)(StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, end)(StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, rbegin)(StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(Ref(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, rend)(StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);

[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, cbegin)(const StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, cend)(const StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, crbegin)(const StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);
[[nodiscard]] [[not_null(1)]] StdRandomAccessIterator(ConstRef(ARRAY_T))
	StdArrayIdentifier(ARRAY_T, ARRAY_N, crend)(const StdArray(ARRAY_T, ARRAY_N) * restrict self)
		___DISABLE_IF_NULL(self);

typedef struct StdArrayIdentifier(ARRAY_T, ARRAY_N, vtable) {
	StdArray(ARRAY_T, ARRAY_N) (*const clone)(const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	const ARRAY_T* (*const at_const)(const StdArray(ARRAY_T, ARRAY_N)* restrict self, usize index);
	ARRAY_T* (*const at_mut)(StdArray(ARRAY_T, ARRAY_N)* restrict self, usize index);
	const ARRAY_T* (*const front_const)(const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	ARRAY_T* (*const front_mut)(StdArray(ARRAY_T, ARRAY_N)* restrict self);
	const ARRAY_T* (*const back_const)(const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	ARRAY_T* (*const back_mut)(StdArray(ARRAY_T, ARRAY_N)* restrict self);
	const ARRAY_T* (*const data_const)(const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	ARRAY_T* (*const data_mut)(StdArray(ARRAY_T, ARRAY_N)* restrict self);
	bool (*const is_empty)(const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	bool (*const is_full)(const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	usize (*const size)(const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	usize (*const capacity)(const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	void (*const resize)(StdArray(ARRAY_T, ARRAY_N)* restrict self, usize new_size);
	void (*const clear)(StdArray(ARRAY_T, ARRAY_N)* restrict self);
	void (*const push_back)(StdArray(ARRAY_T, ARRAY_N)* restrict self, ARRAY_T element);
	StdOption(ARRAY_T) (*const pop_back)(StdArray(ARRAY_T, ARRAY_N)* restrict self);
	void (*const insert)(StdArray(ARRAY_T, ARRAY_N)* restrict self, ARRAY_T element, usize index);
	void (*const erase)(StdArray(ARRAY_T, ARRAY_N)* restrict self, usize index);
	void (*const erase_n)(StdArray(ARRAY_T, ARRAY_N)* restrict self, usize index, usize length);
	void (*const free)(void* restrict self);
	StdRandomAccessIterator(Ref(ARRAY_T)) (*const into_iter)(
		const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(Ref(ARRAY_T)) (*const into_reverse_iter)(
		const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(ConstRef(ARRAY_T)) (*const into_const_iter)(
		const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(ConstRef(ARRAY_T)) (*const into_reverse_const_iter)(
		const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(Ref(ARRAY_T)) (*const begin)(StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(Ref(ARRAY_T)) (*const end)(StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(Ref(ARRAY_T)) (*const rbegin)(
		StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(Ref(ARRAY_T)) (*const rend)(StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(ConstRef(ARRAY_T)) (*const cbegin)(
		const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(ConstRef(ARRAY_T)) (*const cend)(
		const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(ConstRef(ARRAY_T)) (*const crbegin)(
		const StdArray(ARRAY_T, ARRAY_N)* restrict self);
	StdRandomAccessIterator(ConstRef(ARRAY_T)) (*const crend)(
		const StdArray(ARRAY_T, ARRAY_N)* restrict self);
}
StdArrayIdentifier(ARRAY_T, ARRAY_N, vtable);

	#undef ___DISABLE_IF_NULL
	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
#endif // defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_DECL
