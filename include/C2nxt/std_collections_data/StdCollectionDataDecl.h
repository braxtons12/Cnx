/// @file StdCollectionsDataDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Macro definitions for `StdCollectionData`
/// @version 0.2.1
/// @date 2022-02-24
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

#if defined(T) && defined(CollectionType) && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#define STD_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <C2nxt/StdAllocators.h>
	#include <C2nxt/StdBasicTypes.h>

typedef T (*const std_collection_element_default_constructor(T))(StdAllocator allocator);
typedef T (*const std_collection_element_copy_constructor(T))(const T* restrict element,
															  StdAllocator allocator);
typedef void (*const std_collection_element_destructor(T))(T* restrict element,
														   StdAllocator allocator);

typedef struct StdCollectionData(CollectionType) {
	std_collection_element_default_constructor(T) m_constructor;
	std_collection_element_copy_constructor(T) m_copy_constructor;
	std_collection_element_destructor(T) m_destructor;
}
StdCollectionData(CollectionType);

	#undef STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#endif
