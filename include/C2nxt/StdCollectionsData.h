/// @file StdCollectionsData.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdCollectionData is a simple wrapper for an allocator, element default-constructor, and
/// element destructor to be used within a C2nxt collection.
/// @version 0.1
/// @date 2021-07-20
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

#include "StdAllocators.h"
#include "StdBasicTypes.h"

/// @brief Macro alias for an element default-constructor for type `T`
///
/// Used to generate and refer to a unique typedef name for the constructor
///
/// @param T - The type returned by the constructor
/// @ingroup collections
#define std_collection_element_default_constructor(T) \
	CONCAT3(std_collection_, T, _element_default_constructor)

/// @brief Macro alias for an element copy-constructor for type `T`
///
/// Used to generate and refer to a unique typedef name for the constructor
///
/// @param T - The type returned by the constructor
/// @ingroup collections
#define std_collection_element_copy_constructor(T) \
	CONCAT3(std_collection_, T, _element_copy_constructor)

/// @brief Macro alias for an element destructor for type `T`
///
/// Used to generate and refer to a unique typdef name for the destructor
///
/// @param T - The type destroyed by the destructor
/// @ingroup collections
#define std_collection_element_destructor(T) CONCAT3(std_collection_, T, _element_destructor)

/// @brief Macro alias for a `StdCollectionData` for a collection of type `CollectionType`
///
/// Used to generate and refer to a unique typedef name for a particular collection instantiation's
/// associated `StdCollectionData` type.
/// @param CollectionType - The type of the collection associated with this `StdCollectionData`
/// @ingroup collections
#define StdCollectionData(CollectionType) CONCAT2(StdCollectionData, CollectionType)

/// @brief Instantaties the necessary declarations and definitions for a unique `StdCollectionData`
/// type.
///
/// Instaniates a `StdCollectionData` type associated with a collection of type `CollectionType`
/// which holds `T`s.
///
/// @param T - The type held by the associated collection
/// @param CollectionType - The collection type associated with this `StdCollectionData`
/// @ingroup collections
#define DeclStdCollectionData(T, CollectionType)                                        \
	typedef T (*std_collection_element_default_constructor(T))(StdAllocator allocator); \
	typedef T (*std_collection_element_copy_constructor(T))(const T* restrict element,  \
															StdAllocator allocator);    \
	typedef void (*std_collection_element_destructor(T))(T* restrict element,           \
														 StdAllocator allocator);       \
                                                                                        \
	typedef struct StdCollectionData(CollectionType) {                                  \
		std_collection_element_default_constructor(T) m_constructor;                    \
		std_collection_element_copy_constructor(T) m_copy_constructor;                  \
		std_collection_element_destructor(T) m_destructor;                              \
		StdAllocator m_allocator;                                                       \
	}                                                                                   \
	StdCollectionData(CollectionType)
