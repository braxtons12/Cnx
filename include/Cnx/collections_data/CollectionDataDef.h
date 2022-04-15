/// @file CollectionsDataDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Macro definitions for `CnxCollectionData`
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

#ifndef CNX_COLLECTION_DATA_DEF
#define CNX_COLLECTION_DATA_DEF

#include <Cnx/Def.h>

/// @brief Macro alias for an element default-constructor for type `T`
///
/// Used to generate and refer to a unique typedef name for the constructor
///
/// @param T - The type returned by the constructor
/// @ingroup collections
#define cnx_collection_element_default_constructor(T) \
	CONCAT3(cnx_collection_, T, _element_default_constructor)

/// @brief Macro alias for an element copy-constructor for type `T`
///
/// Used to generate and refer to a unique typedef name for the constructor
///
/// @param T - The type returned by the constructor
/// @ingroup collections
#define cnx_collection_element_copy_constructor(T) \
	CONCAT3(cnx_collection_, T, _element_copy_constructor)

/// @brief Macro alias for an element destructor for type `T`
///
/// Used to generate and refer to a unique typedef name for the destructor
///
/// @param T - The type destroyed by the destructor
/// @ingroup collections
#define cnx_collection_element_destructor(T) CONCAT3(cnx_collection_, T, _element_destructor)

/// @brief Macro alias for a `CnxCollectionData` for a collection of type `CollectionType`
///
/// Used to generate and refer to a unique typedef name for a particular collection instantiation's
/// associated `CnxCollectionData` type.
/// @param CollectionType - The type of the collection associated with this `CnxCollectionData`
/// @ingroup collections
#define CnxCollectionData(CollectionType) CONCAT2(CnxCollectionData, CollectionType)

#endif // CNX_COLLECTION_DATA_DEF
