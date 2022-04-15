/// @file CollectionsDataDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Macro definitions for `CnxCollectionData`
/// @version 0.2.1
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

#if defined(COLLECTION_DATA_ELEMENT) && defined(COLLECTION_DATA_COLLECTION) \
	&& !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#define CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS TRUE

	#include <Cnx/Allocators.h>
	#include <Cnx/BasicTypes.h>

typedef COLLECTION_DATA_ELEMENT (*const cnx_collection_element_default_constructor(
	COLLECTION_DATA_ELEMENT))(CnxAllocator allocator);
typedef COLLECTION_DATA_ELEMENT (*const cnx_collection_element_copy_constructor(
	COLLECTION_DATA_ELEMENT))(const COLLECTION_DATA_ELEMENT* restrict element,
							  CnxAllocator allocator);
typedef void (*const cnx_collection_element_destructor(
	COLLECTION_DATA_ELEMENT))(COLLECTION_DATA_ELEMENT* restrict element, CnxAllocator allocator);

typedef struct CnxCollectionData(COLLECTION_DATA_COLLECTION) {
	cnx_collection_element_default_constructor(COLLECTION_DATA_ELEMENT) m_constructor;
	cnx_collection_element_copy_constructor(COLLECTION_DATA_ELEMENT) m_copy_constructor;
	cnx_collection_element_destructor(COLLECTION_DATA_ELEMENT) m_destructor;
}
CnxCollectionData(COLLECTION_DATA_COLLECTION);

	#undef CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#endif
