/// @file Iterator.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides iterator Trait templates similar to the different categories of
/// iterators in C++ for Cnx.
/// @version 0.1
/// @date 2022-01-02
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

#include <Cnx/BasicTypes.h>
#include <Cnx/Trait.h>
#include <memory.h>

/// @defgroup iterators Iterators
/// Cnx defines its own set of iterator SizedTrait templates that allow for simple,
/// collection-agnostic processing of a collection of elements in a way similar to C++ or Rust.
///
/// Iterators provide a uniform interface for accessing the elements of a collection in contained
/// order. They act as reference-wrappers for the element associated with their current position,
/// providing reference-like semantics for accessing values, and because of this are designed to
/// work with pointer-to-T (`typedef`ed as `Ref(T)` or `ConstRef(T)`) as their generic parameter as
/// opposed to T.
///
/// Cnx defines four categories of iterators, each with different requirements and functionality.
/// 1. Forward Iterators (`CnxForwardIterator(T)`) provide forward-only (increment-only)
/// access to the elements in an iteration. This is provided through the associated `next` function.
/// 2. Reverse Iterators (`CnxReverseIterator(T)`) provide reverse-on (decrement-only)
/// access to the elements in an iteration. This is provided through the associated `next` function.
/// 3. Bidirectional Iterators (`CnxBidirectionalIterator(T)`) provide forward and reverse
/// access to the elements in an iteration. This is provided through the associated `next` and
/// `previous` functions.
/// 4. Random Access Iterators (`CnxRandomAccessIterator(T)`) provide bidirectional and
/// random (indexed) access to the elements in an iteration. This is provided through the
/// associated `next`, `previous`, and `at` functions.
///
/// All iterators also provide access to the current value through `current` and equality comparison
/// through `equals`.
///
/// Using iterators are very simple.
///
/// @code {.c}
/// let_mut str = cnx_string_from("this is a string");
/// // We can use them by hand like so:
/// let_mut begin = cnx_string_begin(str);
/// let end = cnx_string_end(str);
///	// for each `char` in `str`, do something
/// for(let_mut character = cnx_iterator_current(begin);
/// 	!cnx_iterator_equals(begin, end);
///		character = cnx_iterator_next(begin)) {
///		// do something with character
/// }
///
/// // or we can use `foreach(element, collection)` to wrap most of that for us:
///	// for each `char` in `str`, do something
/// foreach(character, str) {
/// 	// do something with character
/// }
/// @endcode
///
/// `foreach(element, collection)` accesses elements by value, but there are also versions to access
/// by pointer-to-const and pointer as well:
///
/// @code {.c}
/// foreach_ref(character, str) {
/// 	// character is `const char*`
/// }
/// foreach_ref_mut(character, str) {
///	 	// character is `char*`
/// }
/// @endcode
///
/// Most Cnx collections will require const and non-const iterators for their associated type to
/// be instantiated prior to their own instantiation. This is very simple, to provide instantiations
/// of iterators for a collection of type `T`, just:
///
/// @code {.c}
/// typedef T* Ref(T);
/// typedef const T* ConstRef(T);
/// DeclCnxIterators(Ref(T));
/// DeclCnxIterators(ConstRef(T));
/// @endcode
///
/// The above will instantiate iterators of all four categories for the type `T`, but you could also
/// only instantiate forward iterators or reverse iterators, depending on the collection you intend
/// to use. When unsure, instantiating all is the simplest solution.
///
/// To provide Cnx iterators for your own collections, the process is a little more involved.
/// First, you'll need a concrete type to back your iterator (it will implement the desired iterator
/// Traits). By design, these should contain, **exactly**, an `isize` index, `m_index`, and a
/// pointer to associated collection, in **exactly** that order. If you deviate from this size, you
/// will cause undefined behavior and **should** get a compiler error. If you deviate from this
/// layout, you'll break other abstractions built on top of iterators, like `CnxRange(T)`, for your
/// type.
///
/// For example, the pseudo-code for the concrete type for `CnxVector(T)`'s non-const iterator is:
///
/// @code {.c}
/// typedef CnxVector(T)Iterator {
///		CnxVector(T)* m_vector;
/// 	isize m_index;
/// } CnxVector(T)Iterator;
/// @endcode
///
/// On top of that, you'll need a factory function for your concrete type of the signature:
///
/// @code {.c}
/// ConcreteIterator (*const collection_iterator_new)(const CollectionType* restrict collection);
/// @endcode
///
/// and the functions for the specific iterator category you intend to provide. None of these need
/// be part of your public API, they can remain implementation details.
/// You'll then need to instantiate a declaration and implementation for an `into_iter` function.
/// For example, for `CnxForwardIterator(T)`, this would be:
///
/// @code {.c}
/// // in "YourCollection.h"
/// DeclIntoCnxForwardIterator(YourCollection, T, YourDesiredIntoIterFuncName);
/// // in "YourCollection.c"
/// ImplIntoCnxForwardIterator(YourCollection,
/// 						   T,
/// 						   YourDesiredIntoIterFuncName,
/// 						   YourConcreteIteratorTypeFactoryFunction,
/// 						   YourIteratorNextFunction,
/// 						   YourIteratorCurrentFunction,
/// 						   YourIteratorEqualsFunction);
/// @endcode
///
/// From here, you can wrap the `into_iter` function to provide iterators initialized to specific
/// points in the iteration. For more detailed examples of providing each iterator category, see the
/// documentation for the specific category in question. For a reference implementation of how to
/// provide iterators for a user-defined collection, consider viewing the implementations for
/// `CnxString`, `CnxVector(T)`, or `CnxArray(T)`
///
/// @note Remember that iterators have reference-like semantics, so if you provide iterator
/// implementations for your own collections, keep in mind that if your collection stores `T`, your
/// iterators should wrap and return `Ref(T)` or `ConstRef(T)` (`CnxForwardIterator(Ref(T))`, for an
/// example).

#ifndef CNX_ITERATOR
	/// @brief Declarations and definitions related to Cnx iterators
	#define CNX_ITERATOR

	/// @brief The maximum compatible size of a concrete type implementing a Cnx iterator trait
	///
	/// Cnx iterators are designed to have a maximum concrete size, as they are SizedTraits, and
	/// to be compatible a concrete type can't exceed this (doing so would be UB and **should**
	/// cause a compiler error or warning).
	/// @ingroup iterators
	#define CNX_ITERATOR_CONCRETE_TYPE_SIZE (sizeof(void*) + sizeof(isize))

	/// @brief Generates the function name for converting from one iterator category to its
	/// subcategory
	#define IntoIteratorSubType(CollectionType, IteratorType, ConversionName, IteratorSubType) \
		CONCAT3(UNIQUE_VAR(CollectionType),                                                    \
				IteratorType,                                                                  \
				CONCAT2(CONCAT3(_, ConversionName, _), IteratorSubType))

	/// @brief Macro alias for `CnxForwardIterator` of type `T`
	///
	/// Used for creating and referencing a typedef for a specific forward iterator instantiation.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define CnxForwardIterator(T) CONCAT2(CnxForwardIterator, T)

	/// @brief Instantiates `CnxForwardIterator(T)` for the type `T`
	///
	/// Instantiates the necessary declarations and definitions for `CnxForwardIterator(T)`,
	/// enabling its use elsewhere.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define DeclCnxForwardIterator(T)                                               \
		typedef struct CnxForwardIterator(T) CnxForwardIterator(T);                 \
		SizedTrait(CnxForwardIterator(T),                                           \
				   CNX_ITERATOR_CONCRETE_TYPE_SIZE,                                 \
				   T (*const next)(CnxForwardIterator(T)* restrict self);           \
				   T(*const current)(const CnxForwardIterator(T)* restrict self);   \
				   bool (*const equals)(const CnxForwardIterator(T)* restrict self, \
										const CnxForwardIterator(T)* restrict rhs););

	/// @brief Instantiates an `into_iter` function declaration.
	///
	/// Instantiates the declaration of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `CnxForwardIterator(T)` associated with the collection.
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @ingroup iterators
	#define DeclIntoCnxForwardIterator(CollectionType, T, IntoIterFuncName) \
		CnxForwardIterator(T) IntoIterFuncName(const CollectionType* restrict self);

	/// @brief Instantiates an `into_iter` function definition.
	///
	/// Instantiates the definition of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `CnxForwardIterator(T)` associated with the collection.
	/// Requires that the following are already declared and/or defined:
	/// 1. `CreateFunction` - A factory function returning the concrete type backing the
	/// 	collection's iterators of the signature:
	///
	/// 		ConcreteType (*const CreateFunction)(const Collection* restrict self);
	///
	/// 2. `NextFunction` - Advances the iterator to the next element in the iteration and returns
	/// 	the element. Takes the signature:
	///
	/// 		T (*const NextFunction)(CnxForwardIterator(T)* restrict self);
	///
	/// 3. `CurrentFunction` - Returns the element associated with the current position of the
	/// 	iterator. Takes the signature:
	///
	/// 		T (*const CurrentFunction)(const CnxForwardIterator(T)* restrict self);
	///
	/// 4. `EqualsFunction` - Determines if two given iterators are equal. Takes the signature:
	///
	/// 		bool (*const EqualsFunction)(const CnxForwardIterator(T)* restrict self,
	/// 							 		 const CnxForwardIterator(T)* restrict rhs);
	///
	/// Since iterators are a Trait object, your internal implementations can cast the `self`
	/// pointer to a pointer to the concrete type and operate on it as required.
	/// Example:
	///
	/// @code {.c}
	/// bool (*const EqualsFunction)(const CnxForwardIterator(T)* restrict self,
	/// 							 const CnxForwardIterator(T)* restrict rhs) {
	///		let _self = static_cast(const ConcreteType*)(self->m_self);
	/// 	let _rhs = static_cast(const ConcreteType*)(rhs->m_self);
	/// 	return _self->m_collection_ptr == _rhs->m_collection_ptr
	/// 		   && _self->m_index == _rhs->m_index;
	/// }
	/// @endcode
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @param CreateFunction - The factory function for the concrete type that will implement
	/// `CnxForwardIterator(T)` for your collection.
	/// @param NextFunction - The function advancing the iterator
	/// @param CurrentFunction - The function returning the element currently associated with the
	/// iterator
	/// @param EqualsFunction - The function determining if two iterators are equal
	/// @ingroup iterators
	#define ImplIntoCnxForwardIterator(CollectionType,                                           \
									   T,                                                        \
									   IntoIterFuncName,                                         \
									   CreateFunction,                                           \
									   NextFunction,                                             \
									   CurrentFunction,                                          \
									   EqualsFunction)                                           \
		CnxForwardIterator(T) IntoIterFuncName(const CollectionType* restrict self) {            \
			T (*const _next)(CnxForwardIterator(T)* restrict self) = (NextFunction);             \
			ignore(_next);                                                                       \
                                                                                                 \
			T (*const _current)(const CnxForwardIterator(T)* restrict self) = (CurrentFunction); \
			ignore(_current);                                                                    \
                                                                                                 \
			bool (*const _equals)(const CnxForwardIterator(T)* restrict self,                    \
								  const CnxForwardIterator(T)* restrict rhs)                     \
				= (EqualsFunction);                                                              \
			ignore(_equals);                                                                     \
                                                                                                 \
			static ImplTraitFor(CnxForwardIterator(T),                                           \
								CollectionType,                                                  \
								(NextFunction),                                                  \
								(CurrentFunction),                                               \
								(EqualsFunction));                                               \
                                                                                                 \
			let begin = CreateFunction(self);                                                    \
			return as_sized_trait(CnxForwardIterator(T), CollectionType, begin);                 \
		}

	/// @brief Macro alias for `CnxReverseIterator` of type `T`
	///
	/// Used for creating and referencing a typedef for a specific reverse iterator instantiation.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define CnxReverseIterator(T) CONCAT2(CnxReverseIterator, T)

	/// @brief Instantiates `CnxReverseIterator(T)` for the type `T`
	///
	/// Instantiates the necessary declarations and definitions for `CnxReverseIterator(T)`,
	/// enabling its use elsewhere.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define DeclCnxReverseIterator(T)                                               \
		SizedTrait(CnxReverseIterator(T),                                           \
				   CNX_ITERATOR_CONCRETE_TYPE_SIZE,                                 \
				   T (*const next)(CnxReverseIterator(T)* restrict self);           \
				   T(*const current)(const CnxReverseIterator(T)* restrict self);   \
				   bool (*const equals)(const CnxReverseIterator(T)* restrict self, \
										const CnxReverseIterator(T)* restrict rhs););

	/// @brief Instantiates an `into_iter` function declaration.
	///
	/// Instantiates the declaration of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `CnxReverseIterator(T)` associated with the collection.
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @ingroup iterators
	#define DeclIntoCnxReverseIterator(CollectionType, T, IntoIterFuncName) \
		CnxReverseIterator(T) IntoIterFuncName(const CollectionType* restrict self);

	/// @brief Instantiates an `into_iter` function definition.
	///
	/// Instantiates the definition of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `CnxReverseIterator(T)` associated with the collection.
	/// Requires that the following are already declared and/or defined:
	/// 1. `CreateFunction` - A factory function returning the concrete type backing the
	/// 	collection's iterators of the signature:
	///
	/// 		ConcreteType (*const CreateFunction)(const Collection* restrict self);
	///
	/// 2. `NextFunction` - Advances the iterator to the next element in the iteration and returns
	/// 	the element. Takes the signature:
	///
	/// 		T (*const NextFunction)(CnxReverseIterator(T)* restrict self);
	///
	/// 3. `CurrentFunction` - Returns the element associated with the current position of the
	/// 	iterator. Takes the signature:
	///
	/// 		T (*const CurrentFunction)(const CnxReverseIterator(T)* restrict self);
	///
	/// 4. `EqualsFunction` - Determines if two given iterators are equal. Takes the signature:
	///
	/// 		bool (*const EqualsFunction)(const CnxReverseIterator(T)* restrict self,
	/// 							 		 const CnxReverseIterator(T)* restrict rhs);
	///
	/// Since iterators are a Trait object, your internal implementations can cast the `self`
	/// pointer to a pointer to the concrete type and operate on it as required.
	/// Example:
	///
	/// @code {.c}
	/// bool (*const EqualsFunction)(const CnxReverseIterator(T)* restrict self,
	/// 							 const CnxReverseIterator(T)* restrict rhs) {
	///		let _self = static_cast(const ConcreteType*)(self->m_self);
	/// 	let _rhs = static_cast(const ConcreteType*)(rhs->m_self);
	/// 	return _self->m_collection_ptr == _rhs->m_collection_ptr
	/// 		   && _self->m_index == _rhs->m_index;
	/// }
	/// @endcode
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @param CreateFunction - The factory function for the concrete type that will implement
	/// `CnxReverseIterator(T)` for your collection.
	/// @param NextFunction - The function advancing the iterator
	/// @param CurrentFunction - The function returning the element currently associated with the
	/// iterator
	/// @param EqualsFunction - The function determining if two iterators are equal
	/// @ingroup iterators
	#define ImplIntoCnxReverseIterator(CollectionType,                                           \
									   T,                                                        \
									   IntoIterFuncName,                                         \
									   CreateFunction,                                           \
									   NextFunction,                                             \
									   CurrentFunction,                                          \
									   EqualsFunction)                                           \
		CnxReverseIterator(T) IntoIterFuncName(const CollectionType* restrict self) {            \
			T (*const _next)(CnxReverseIterator(T)* restrict self) = (NextFunction);             \
			ignore(_next);                                                                       \
                                                                                                 \
			T (*const _current)(const CnxReverseIterator(T)* restrict self) = (CurrentFunction); \
			ignore(_current);                                                                    \
                                                                                                 \
			bool (*const _equals)(const CnxReverseIterator(T)* restrict self,                    \
								  const CnxReverseIterator(T)* restrict rhs)                     \
				= (EqualsFunction);                                                              \
			ignore(_equals);                                                                     \
                                                                                                 \
			static ImplTraitFor(CnxReverseIterator(T),                                           \
								CollectionType,                                                  \
								(NextFunction),                                                  \
								(CurrentFunction),                                               \
								(EqualsFunction));                                               \
                                                                                                 \
			let begin = CreateFunction(self);                                                    \
			return as_sized_trait(CnxReverseIterator(T), CollectionType, begin);                 \
		}

	/// @brief Macro alias for `CnxBidirectionalIterator` of type `T`
	///
	/// Used for creating and referencing a typedef for a specific bidirectional iterator
	/// instantiation.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define CnxBidirectionalIterator(T) CONCAT2(CnxBidirectionalIterator, T)

	/// @brief Instantiates `CnxBidirectionalIterator(T)` for the type `T`
	///
	/// Instantiates the necessary declarations and definitions for `CnxBidirectionalIterator(T)`,
	/// enabling its use elsewhere.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define DeclCnxBidirectionalIterator(T)                                               \
		SizedTrait(CnxBidirectionalIterator(T),                                           \
				   CNX_ITERATOR_CONCRETE_TYPE_SIZE,                                       \
				   T (*const next)(CnxBidirectionalIterator(T)* restrict self);           \
				   T(*const previous)(CnxBidirectionalIterator(T)* restrict self);        \
				   T(*const current)(const CnxBidirectionalIterator(T)* restrict self);   \
				   bool (*const equals)(const CnxBidirectionalIterator(T)* restrict self, \
										const CnxBidirectionalIterator(T)* restrict rhs); \
				   CnxForwardIterator(T)(*const into_forward_iterator)(                   \
					   const CnxBidirectionalIterator(T)* restrict self);                 \
				   CnxReverseIterator(T)(*const into_reverse_iterator)(                   \
					   const CnxBidirectionalIterator(T)* restrict self););

	/// @brief Instantiates an `into_iter` function declaration and iterator conversion function
	/// declarations.
	///
	/// Instantiates the declaration of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `CnxBidirectionalIterator(T)` associated with the collection.
	/// Also Instantiates the declarations for conversion functions from
	/// `CnxBidirectionalIterator(T)` to its subcategories `CnxForwardIterator(T)` and
	/// `CnxReverseIterator(T)`.
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @param IteratorConversionName - The root name for the conversion functions
	/// @ingroup iterators
	#define DeclIntoCnxBidirectionalIterator(CollectionType,                   \
											 T,                                \
											 IntoIterFuncName,                 \
											 IteratorConversionName)           \
                                                                               \
		CnxForwardIterator(T) IntoIteratorSubType(CollectionType,              \
												  CnxBidirectionalIterator(T), \
												  IteratorConversionName,      \
												  CnxForwardIterator(T))(      \
			const CnxBidirectionalIterator(T)* restrict self);                 \
                                                                               \
		CnxReverseIterator(T) IntoIteratorSubType(CollectionType,              \
												  CnxBidirectionalIterator(T), \
												  IteratorConversionName,      \
												  CnxReverseIterator(T))(      \
			const CnxBidirectionalIterator(T)* restrict self);                 \
                                                                               \
		CnxBidirectionalIterator(T) IntoIterFuncName(const CollectionType* restrict self);

	/// @brief Instantiates an `into_iter` function definition and iterator conversion function
	/// definitions.
	///
	/// Instantiates the definition of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `CnxBidirectionalIterator(T)` associated with the collection.
	/// Also Instantiates the definitions for conversion functions from
	/// `CnxBidirectionalIterator(T)` to its subcategories `CnxForwardIterator(T)` and
	/// `CnxReverseIterator(T)`.
	/// Requires that the following are already declared and/or defined:
	/// 1. `CreateFunction` - A factory function returning the concrete type backing the
	/// 	collection's iterators of the signature:
	///
	/// 		ConcreteType (*const CreateFunction)(const Collection* restrict self);
	///
	/// 2. `NextFunction` - Advances the iterator to the next element in the iteration and returns
	/// 	the element. Takes the signature:
	///
	/// 		T (*const NextFunction)(CnxBidirectionalIterator(T)* restrict self);
	///
	/// 3. `PreviousFunction` - Decrements the iterator to the previous element in the iteration and
	/// 	returns the element. Takes the signature:
	///
	/// 		T (*const PreviousFunction)(CnxBidirectionalIterator(T)* restrict self);
	///
	/// 4. `CurrentFunction` - Returns the element associated with the current position of the
	/// 	iterator. Takes the signature:
	///
	/// 		T (*const CurrentFunction)(const CnxBidirectionalIterator(T)* restrict self);
	///
	/// 5. `EqualsFunction` - Determines if two given iterators are equal. Takes the signature:
	///
	/// 		bool (*const EqualsFunction)(const CnxBidirectionalIterator(T)* restrict self,
	/// 							 		 const CnxBidirectionalIterator(T)* restrict rhs);
	///
	/// Since iterators are a Trait object, your internal implementations can cast the `self`
	/// pointer to a pointer to the concrete type and operate on it as required.
	/// Example:
	///
	/// @code {.c}
	/// bool (*const EqualsFunction)(const CnxBidirectionalIterator(T)* restrict self,
	/// 							 const CnxBidirectionalIterator(T)* restrict rhs) {
	///		let _self = static_cast(const ConcreteType*)(self->m_self);
	/// 	let _rhs = static_cast(const ConcreteType*)(rhs->m_self);
	/// 	return _self->m_collection_ptr == _rhs->m_collection_ptr
	/// 		   && _self->m_index == _rhs->m_index;
	/// }
	/// @endcode
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @param IteratorConversionName - The root name for the conversion functions
	/// @param CreateFunction - The factory function for the concrete type that will implement
	/// `CnxBidirectionalIterator(T)` for your collection.
	/// @param NextFunction - The function advancing the iterator
	/// @param PreviousFunction - The function decrementing the iterator
	/// @param CurrentFunction - The function returning the element currently associated with the
	/// iterator
	/// @param EqualsFunction - The function determining if two iterators are equal
	/// @ingroup iterators
	#define ImplIntoCnxBidirectionalIterator(CollectionType,                                       \
											 T,                                                    \
											 IntoIterFuncName,                                     \
											 IteratorConversionName,                               \
											 CreateFunction,                                       \
											 NextFunction,                                         \
											 PreviousFunction,                                     \
											 CurrentFunction,                                      \
											 EqualsFunction)                                       \
		CnxForwardIterator(T) IntoIteratorSubType(CollectionType,                                  \
												  CnxBidirectionalIterator(T),                     \
												  IteratorConversionName,                          \
												  CnxForwardIterator(T))(                          \
			CnxBidirectionalIterator(T) restrict self) {                                           \
			static ImplTraitFor(                                                                   \
				CnxForwardIterator(T),                                                             \
				CollectionType,                                                                    \
				static_cast(T(*const)(CnxForwardIterator(T)* restrict))(NextFunction),             \
				static_cast(T(*const)(const CnxForwardIterator(T)* restrict))(CurrentFunction),    \
				static_cast(bool (*const)(const CnxForwardIterator(T)* restrict,                   \
										  const CnxForwardIterator(T)* restrict))(                 \
					EqualsFunction));                                                              \
                                                                                                   \
			return as_sized_trait(CnxForwardIterator(T), CollectionType, self->m_self);            \
		}                                                                                          \
                                                                                                   \
		CnxReverseIterator(T) IntoIteratorSubType(CollectionType,                                  \
												  CnxBidirectionalIterator(T),                     \
												  IteratorConversionName,                          \
												  CnxReverseIterator(T))(                          \
			CnxBidirectionalIterator(T) restrict self) {                                           \
			static ImplTraitFor(                                                                   \
				CnxReverseIterator(T),                                                             \
				CollectionType,                                                                    \
				static_cast(T(*const)(CnxReverseIterator(T)* restrict))(PreviousFunction),         \
				static_cast(T(*const)(const CnxReverseIterator(T)* restrict))(CurrentFunction),    \
				static_cast(bool (*const)(const CnxReverseIterator(T)* restrict,                   \
										  const CnxReverseIterator(T)* restrict))(                 \
					EqualsFunction));                                                              \
                                                                                                   \
			return as_sized_trait(CnxReverseIterator(T), CollectionType, self->m_self);            \
		}                                                                                          \
                                                                                                   \
		CnxBidirectionalIterator(T) IntoIterFuncName(const CollectionType* restrict self) {        \
			T (*const _next)(CnxBidirectionalIterator(T)* restrict self) = (NextFunction);         \
			ignore(_next);                                                                         \
			T (*const _previous)(CnxBidirectionalIterator(T)* restrict self) = (PreviousFunction); \
			ignore(_previous);                                                                     \
                                                                                                   \
			T(*const _current)                                                                     \
			(const CnxBidirectionalIterator(T)* restrict self) = (CurrentFunction);                \
			ignore(_current);                                                                      \
                                                                                                   \
			bool (*const _equals)(const CnxBidirectionalIterator(T)* restrict self,                \
								  const CnxBidirectionalIterator(T)* restrict rhs)                 \
				= (EqualsFunction);                                                                \
			ignore(_equals);                                                                       \
                                                                                                   \
			static ImplTraitFor(CnxBidirectionalIterator(T),                                       \
								CollectionType,                                                    \
								(NextFunction),                                                    \
								(PreviousFunction),                                                \
								(CurrentFunction),                                                 \
								(EqualsFunction),                                                  \
								IntoIteratorSubType(CollectionType,                                \
													CnxBidirectionalIterator(T),                   \
													IteratorConversionName,                        \
													CnxForwardIterator(T)),                        \
								IntoIteratorSubType(CollectionType,                                \
													CnxBidirectionalIterator(T),                   \
													IteratorConversionName,                        \
													CnxReverseIterator(T)));                       \
                                                                                                   \
			let begin = CreateFunction(self);                                                      \
			return as_sized_trait(CnxBidirectionalIterator(T), CollectionType, begin);             \
		}

	/// @brief Macro alias for `CnxRandomAccessIterator` of type `T`
	///
	/// Used for creating and referencing a typedef for a specific random access iterator
	/// instantiation.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define CnxRandomAccessIterator(T) CONCAT2(CnxRandomAccessIterator, T)

	/// @brief Instantiates `CnxRandomAccessIterator(T)` for the type `T`
	///
	/// Instantiates the necessary declarations and definitions for `CnxRandomAccessIterator(T)`,
	/// enabling its use elsewhere.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define DeclCnxRandomAccessIterator(T)                                                     \
		SizedTrait(CnxRandomAccessIterator(T),                                                 \
				   CNX_ITERATOR_CONCRETE_TYPE_SIZE,                                            \
				   T (*const next)(CnxRandomAccessIterator(T)* restrict self);                 \
				   T(*const previous)(CnxRandomAccessIterator(T)* restrict self);              \
				   T(*const at)(const CnxRandomAccessIterator(T)* restrict self, usize index); \
				   T(*const current)(const CnxRandomAccessIterator(T)* restrict self);         \
				   bool (*const equals)(const CnxRandomAccessIterator(T)* restrict self,       \
										const CnxRandomAccessIterator(T)* restrict rhs);       \
				   CnxForwardIterator(T)(*const into_forward_iterator)(                        \
					   const CnxRandomAccessIterator(T)* restrict self);                       \
				   CnxReverseIterator(T)(*const into_reverse_iterator)(                        \
					   const CnxRandomAccessIterator(T)* restrict self);                       \
				   CnxBidirectionalIterator(T)(*const into_bidirectional_iterator)(            \
					   const CnxRandomAccessIterator(T)* restrict self););

	/// @brief Instantiates an `into_iter` function declaration and iterator conversion function
	/// declarations.
	///
	/// Instantiates the declaration of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `CnxRandomAccessIterator(T)` associated with the collection.
	/// Also Instantiates the declarations for conversion functions from
	/// `CnxRandomAccessIterator(T)` to its subcategories `CnxForwardIterator(T)`,
	/// `CnxReverseIterator(T)`, and `CnxBidirectionalIterator(T)`.
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @param IteratorConversionName - The root name for the conversion functions
	/// @ingroup iterators
	#define DeclIntoCnxRandomAccessIterator(CollectionType,                           \
											T,                                        \
											IntoIterFuncName,                         \
											IteratorConversionName)                   \
                                                                                      \
		CnxForwardIterator(T) IntoIteratorSubType(CollectionType,                     \
												  CnxRandomAccessIterator(T),         \
												  IteratorConversionName,             \
												  CnxForwardIterator(T))(             \
			const CnxRandomAccessIterator(T)* restrict self);                         \
                                                                                      \
		CnxReverseIterator(T) IntoIteratorSubType(CollectionType,                     \
												  CnxRandomAccessIterator(T),         \
												  IteratorConversionName,             \
												  CnxReverseIterator(T))(             \
			const CnxRandomAccessIterator(T)* restrict self);                         \
                                                                                      \
		CnxBidirectionalIterator(T) IntoIteratorSubType(CollectionType,               \
														CnxRandomAccessIterator(T),   \
														IteratorConversionName,       \
														CnxBidirectionalIterator(T))( \
			const CnxRandomAccessIterator(T)* restrict self);                         \
                                                                                      \
		CnxRandomAccessIterator(T) IntoIterFuncName(const CollectionType* restrict self);

	/// @brief Instantiates an `into_iter` function definition and iterator conversion function
	/// definitions.
	///
	/// Instantiates the definition of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `CnxRandomAccessIterator(T)` associated with the collection.
	/// Also Instantiates the definitions for conversion functions from
	/// `CnxRandomAccessIterator(T)` to its subcategories `CnxForwardIterator(T)`,
	/// `CnxReverseIterator(T)`, and `CnxBidirectionalIterator(T)`.
	/// Requires that the following are already declared and/or defined:
	/// 1. `CreateFunction` - A factory function returning the concrete type backing the
	/// 	collection's iterators of the signature:
	///
	/// 		ConcreteType (*const CreateFunction)(const Collection* restrict self);
	///
	/// 2. `NextFunction` - Advances the iterator to the next element in the iteration and returns
	/// 	the element. Takes the signature:
	///
	/// 		T (*const NextFunction)(CnxRandomAccessIterator(T)* restrict self);
	///
	/// 3. `PreviousFunction` - Decrements the iterator to the previous element in the iteration and
	/// 	returns the element. Takes the signature:
	///
	/// 		T (*const PreviousFunction)(CnxRandomAccessIterator(T)* restrict self);
	///
	/// 4. `AtFunction` - Returns the element in the iteration associated with the given index.
	/// 	Takes the signature:
	///
	/// 		T (*const AtFunction)(CnxRandomAccessIterator(T)* restrict self, usize index);
	///
	/// 5. `CurrentFunction` - Returns the element associated with the current position of the
	/// 	iterator. Takes the signature:
	///
	/// 		T (*const CurrentFunction)(const CnxRandomAccessIterator(T)* restrict self);
	///
	/// 6. `EqualsFunction` - Determines if two given iterators are equal. Takes the signature:
	///
	/// 		bool (*const EqualsFunction)(const CnxRandomAccessIterator(T)* restrict self,
	/// 							 		 const CnxRandomAccessIterator(T)* restrict rhs);
	///
	/// Since iterators are a Trait object, your internal implementations can cast the `self`
	/// pointer to a pointer to the concrete type and operate on it as required.
	/// Example:
	///
	/// @code {.c}
	/// bool (*const EqualsFunction)(const CnxRandomAccessIterator(T)* restrict self,
	/// 							 const CnxRandomAccessIterator(T)* restrict rhs) {
	///		let _self = static_cast(const ConcreteType*)(self->m_self);
	/// 	let _rhs = static_cast(const ConcreteType*)(rhs->m_self);
	/// 	return _self->m_collection_ptr == _rhs->m_collection_ptr
	/// 		   && _self->m_index == _rhs->m_index;
	/// }
	/// @endcode
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @param IteratorConversionName - The root name for the conversion functions
	/// @param CreateFunction - The factory function for the concrete type that will implement
	/// `CnxRandomAccessIterator(T)` for your collection.
	/// @param NextFunction - The function advancing the iterator
	/// @param PreviousFunction - The function decrementing the iterator
	/// @param AtFunction - The function returning the element associated with the given index into
	/// the iteration
	/// @param CurrentFunction - The function returning the element currently associated with the
	/// iterator
	/// @param EqualsFunction - The function determining if two iterators are equal
	/// @ingroup iterators
	#define ImplIntoCnxRandomAccessIterator(CollectionType,                                       \
											T,                                                    \
											IntoIterFuncName,                                     \
											IteratorConversionName,                               \
											CreateFunction,                                       \
											NextFunction,                                         \
											PreviousFunction,                                     \
											AtFunction,                                           \
											CurrentFunction,                                      \
											EqualsFunction)                                       \
                                                                                                  \
		CnxForwardIterator(T) IntoIteratorSubType(CollectionType,                                 \
												  CnxRandomAccessIterator(T),                     \
												  IteratorConversionName,                         \
												  CnxForwardIterator(T))(                         \
			const CnxRandomAccessIterator(T)* restrict self) {                                    \
			static ImplTraitFor(                                                                  \
				CnxForwardIterator(T),                                                            \
				CollectionType,                                                                   \
				static_cast(T(*const)(CnxForwardIterator(T)* restrict))(NextFunction),            \
				static_cast(T(*const)(const CnxForwardIterator(T)* restrict))(CurrentFunction),   \
				static_cast(bool (*const)(const CnxForwardIterator(T)* restrict,                  \
										  const CnxForwardIterator(T)* restrict))(                \
					EqualsFunction));                                                             \
                                                                                                  \
			return as_sized_trait(CnxForwardIterator(T), CollectionType, self->m_self);           \
		}                                                                                         \
                                                                                                  \
		CnxReverseIterator(T) IntoIteratorSubType(CollectionType,                                 \
												  CnxRandomAccessIterator(T),                     \
												  IteratorConversionName,                         \
												  CnxReverseIterator(T))(                         \
			const CnxRandomAccessIterator(T)* restrict self) {                                    \
			static ImplTraitFor(                                                                  \
				CnxReverseIterator(T),                                                            \
				CollectionType,                                                                   \
				static_cast(T(*const)(CnxReverseIterator(T)* restrict))(PreviousFunction),        \
				static_cast(T(*const)(const CnxReverseIterator(T)* restrict))(CurrentFunction),   \
				static_cast(bool (*const)(const CnxReverseIterator(T)* restrict,                  \
										  const CnxReverseIterator(T)* restrict))(                \
					EqualsFunction));                                                             \
                                                                                                  \
			return as_sized_trait(CnxReverseIterator(T), CollectionType, self->m_self);           \
		}                                                                                         \
                                                                                                  \
		CnxBidirectionalIterator(T) IntoIteratorSubType(CollectionType,                           \
														CnxRandomAccessIterator(T),               \
														IteratorConversionName,                   \
														CnxBidirectionalIterator(T))(             \
			const CnxRandomAccessIterator(T)* restrict self) {                                    \
			static ImplTraitFor(                                                                  \
				CnxBidirectionalIterator(T),                                                      \
				CollectionType,                                                                   \
				static_cast(T(*const)(CnxBidirectionalIterator(T)* restrict))(NextFunction),      \
				static_cast(T(*const)(CnxBidirectionalIterator(T)* restrict))(PreviousFunction),  \
				static_cast(T(*const)(const CnxBidirectionalIterator(T)* restrict))(              \
					CurrentFunction),                                                             \
				static_cast(bool (*const)(const CnxBidirectionalIterator(T)* restrict,            \
										  const CnxBidirectionalIterator(T)* restrict))(          \
					EqualsFunction),                                                              \
				static_cast(CnxForwardIterator(T)(*const)(const CnxBidirectionalIterator(         \
					T)* restrict))(IntoIteratorSubType(CollectionType,                            \
													   CnxRandomAccessIterator(T),                \
													   IteratorConversionName,                    \
													   CnxForwardIterator(T))),                   \
				static_cast(CnxReverseIterator(T)(*const)(const CnxBidirectionalIterator(         \
					T)* restrict))(IntoIteratorSubType(CollectionType,                            \
													   CnxRandomAccessIterator(T),                \
													   IteratorConversionName,                    \
													   CnxReverseIterator(T))));                  \
                                                                                                  \
			return as_sized_trait(CnxBidirectionalIterator(T), CollectionType, self->m_self);     \
		}                                                                                         \
                                                                                                  \
		CnxRandomAccessIterator(T) IntoIterFuncName(const CollectionType* restrict self) {        \
			T (*const _next)(CnxRandomAccessIterator(T)* restrict self) = (NextFunction);         \
			ignore(_next);                                                                        \
			T (*const _previous)(CnxRandomAccessIterator(T)* restrict self) = (PreviousFunction); \
			ignore(_previous);                                                                    \
			T(*const _at)                                                                         \
			(const CnxRandomAccessIterator(T)* restrict self, usize index) = (AtFunction);        \
			ignore(_at);                                                                          \
                                                                                                  \
			T(*const _current)                                                                    \
			(const CnxRandomAccessIterator(T)* restrict self) = (CurrentFunction);                \
			ignore(_current);                                                                     \
                                                                                                  \
			bool (*const _equals)(const CnxRandomAccessIterator(T)* restrict self,                \
								  const CnxRandomAccessIterator(T)* restrict rhs)                 \
				= (EqualsFunction);                                                               \
			ignore(_equals);                                                                      \
                                                                                                  \
			static ImplTraitFor(CnxRandomAccessIterator(T),                                       \
								CollectionType,                                                   \
								(NextFunction),                                                   \
								(PreviousFunction),                                               \
								(AtFunction),                                                     \
								(CurrentFunction),                                                \
								(EqualsFunction),                                                 \
								IntoIteratorSubType(CollectionType,                               \
													CnxRandomAccessIterator(T),                   \
													IteratorConversionName,                       \
													CnxForwardIterator(T)),                       \
								IntoIteratorSubType(CollectionType,                               \
													CnxRandomAccessIterator(T),                   \
													IteratorConversionName,                       \
													CnxReverseIterator(T)),                       \
								IntoIteratorSubType(CollectionType,                               \
													CnxRandomAccessIterator(T),                   \
													IteratorConversionName,                       \
													CnxBidirectionalIterator(T)));                \
                                                                                                  \
			let begin = CreateFunction(self);                                                     \
			return as_sized_trait(CnxRandomAccessIterator(T), CollectionType, begin);             \
		}

	/// @brief Advances the iterator to the next element.
	///
	/// Advances the iterator to the next element and returns a reference to that element.
	///
	/// @param iterator - The iterator to advance
	///
	/// @return a reference to the next element in the iteration
	/// @note The result of calling `cnx_iterator_next(iterator)` on an iterator the is already at
	/// or past the end of the iterator is implementation defined (by that specific iterator's trait
	/// implementation)
	/// @ingroup iterators
	#define cnx_iterator_next(iterator) *(trait_call(next, iterator))
	/// @brief Decrements the iterator to the previous element.
	///
	/// Decrements the iterator to the previous element and returns a reference to that element.
	///
	/// @param iterator - The iterator to decrement
	///
	/// @return a reference to the previous element in the iteration
	/// @note `cnx_iterator_previous(iterator)` can only be called on `CnxBidirectionalIterator(T)`s
	/// and `CnxRandomAccessIterator(T)`s. Other iterator categories do no implement `previous`
	/// @note The result of calling `cnx_iterator_previous(iterator)` on an iterator the is already
	/// at or before the beginning of the iterator is implementation defined (by that specific
	/// iterator's trait implementation)
	/// @ingroup iterators
	#define cnx_iterator_previous(iterator) *(trait_call(previous, iterator))
	/// @brief Returns a reference to the element at the given index in the iteration
	///
	/// @param iterator - The iterator to get an element from
	/// @param index - The index in the iteration of the desired element
	///
	/// @return a reference to the element at the given index
	/// @note `cnx_iterator_at(iterator, index)` can only be called on
	/// `CnxRandomAccessIterator(T)`s. Other iterator categories do no implement `at`
	/// @note The result of calling `cnx_iterator_at(iterator, index)` with an index that is out of
	/// bounds of the iteration (before the beginning or past the end) is implementation defined (by
	/// that specific iterator's trait implementation)
	/// @ingroup iterators
	#define cnx_iterator_at(iterator, index) *(trait_call(at, iterator, index))
	/// @brief Returns a reference to the element associated with the current position of the
	/// iterator
	///
	/// @param iterator - The iterator to get the currently associated element from
	///
	/// @return a reference to the element associated with the current position of the iterator
	/// @note The result of calling `cnx_iterator_current(iterator)` on an iterator that is out of
	/// bounds of the iteration (before the beginning or past the end) is implementation defined (by
	/// that specific iterator's trait implementation)
	/// @ingroup iterators
	#define cnx_iterator_current(iterator) *(trait_call(current, iterator))
	/// @brief Returns whether the given iterators are equal
	///
	/// @param lhs - The LHS iterator of the equality comparison
	/// @param rhs - The RHS iterator of the equality comparison
	///
	/// @return whether the iterators are equal
	/// @ingroup iterators
	#define cnx_iterator_equals(lhs, rhs) trait_call(equals, lhs, &(rhs))
	/// @brief Converts the given `CnxBidirectionalIterator(T)` or `CnxRandomAccessIterator(T)` into
	/// a `CnxForwardIterator(T)`
	///
	/// @param iterator - The iterator to convert
	///
	/// @return `iterator` as a `CnxForwardIterator(T)`
	/// @note `cnx_iterator_into_forward_iterator` can only be called on
	/// `CnxBidirectionalIterator(T)`s and `CnxRandomAccessIterator(T)`s. Other iterator categories
	/// do not implement `into_forward_iterator`
	/// @ingroup iterators
	#define cnx_iterator_into_forward_iterator(iterator) trait_call(into_forward_iterator, iterator)
	/// @brief Converts the given `CnxBidirectionalIterator(T)` or `CnxRandomAccessIterator(T)` into
	/// a `CnxReverseIterator(T)`
	///
	/// @param iterator - The iterator to convert
	///
	/// @return `iterator` as a `CnxReverseIterator(T)`
	/// @note `cnx_iterator_into_reverse_iterator` can only be called on
	/// `CnxBidirectionalIterator(T)`s and `CnxRandomAccessIterator(T)`s. Other iterator categories
	/// do not implement `into_reverse_iterator`
	/// @ingroup iterators
	#define cnx_iterator_into_reverse_iterator(iterator) trait_call(into_reverse_iterator, iterator)
	/// @brief Converts the given `CnxRandomAccessIterator(T)` into a `CnxBidirectionalIterator(T)`
	///
	/// @param iterator - The iterator to convert
	///
	/// @return `iterator` as a `CnxBidirectionalIterator(T)`
	/// @note `cnx_iterator_into_bidirectional_iterator` can only be called on
	/// `CnxRandomAccessIterator(T)`s. Other iterator categories do not implement
	/// `into_bidirectional_iterator`
	/// @ingroup iterators
	#define cnx_iterator_into_bidirectional_iterator(iterator) \
		trait_call(into_bidirectional_iterator, iterator)

	#if CNX_PLATFORM_COMPILER_CLANG
	// clang-format off

	/// @brief Loops over each element in the iteration of the given collection
	///
	/// This category of foreach loop iterates by value (`element` will be a copy)
	///
	/// @param element - The name to use to reference the current element in the iteration
	/// @param collection - The collection to iterate over
	/// @ingroup iterators
	#define foreach(element, collection)                                                            \
		let_mut UNIQUE_VAR(begin) = (collection).m_vtable->cbegin(&(collection));                     \
		let UNIQUE_VAR(end) = (collection).m_vtable->cend(&(collection));                             \
		for(let_mut element = cnx_iterator_current(UNIQUE_VAR(begin)); 							   \
			!cnx_iterator_equals(UNIQUE_VAR(begin), UNIQUE_VAR(end));  							   \
			element = cnx_iterator_next(UNIQUE_VAR(begin)))
#else
	// clang-format off

	/// @brief Loops over each element in the iteration of the given collection
	///
	/// This category of foreach loop iterates by value (`element` will be a copy)
	///
	/// @param element - The name to use to reference the current element in the iteration
	/// @param collection - The collection to iterate over
	/// @ingroup iterators
	#define foreach(element, collection)                                                            \
		_Pragma("GCC diagnostic push")                                                             \
		_Pragma("GCC diagnostic ignored \"-Wdiscarded-qualifiers\"")                       	   \
		let_mut UNIQUE_VAR(begin) = (collection).m_vtable->begin(&(collection));                     \
		let UNIQUE_VAR(end) = (collection).m_vtable->end(&(collection));                             \
		_Pragma("GCC diagnostic pop") 															   \
		for(let_mut element = cnx_iterator_current(UNIQUE_VAR(begin)); 							   \
			!cnx_iterator_equals(UNIQUE_VAR(begin), UNIQUE_VAR(end));  							   \
			element = cnx_iterator_next(UNIQUE_VAR(begin)))
#endif

	/// @brief Loops over each element in the iteration of the given collection
	///
	/// This category of foreach loop iterates by const pointer (`element` will be a pointer to const)
	///
	/// @param element - The name to use to reference the current element in the iteration
	/// @param collection - The collection to iterate over
	/// @ingroup iterators
	#define foreach_ref(element, collection)                                     \
		let_mut UNIQUE_VAR(begin) = (collection).m_vtable->cbegin(&(collection)); \
		let UNIQUE_VAR(end) = (collection).m_vtable->cend(&(collection));         \
		for(let_mut element = &cnx_iterator_current(UNIQUE_VAR(begin));         \
			!cnx_iterator_equals(UNIQUE_VAR(begin), UNIQUE_VAR(end));           \
			element = &cnx_iterator_next(UNIQUE_VAR(begin)))

	/// @brief Loops over each element in the iteration of the given collection
	///
	/// This category of foreach loop iterates by pointer (`element` will be a pointer)
	///
	/// @param element - The name to use to reference the current element in the iteration
	/// @param collection - The collection to iterate over
	/// @ingroup iterators
	#define foreach_ref_mut(element, collection)                                     \
		let_mut UNIQUE_VAR(begin) = (collection).m_vtable->begin(&(collection)); \
		let UNIQUE_VAR(end) = (collection).m_vtable->end(&(collection));         \
		for(let_mut element = &cnx_iterator_current(UNIQUE_VAR(begin));         \
			!cnx_iterator_equals(UNIQUE_VAR(begin), UNIQUE_VAR(end));           \
			element = &cnx_iterator_next(UNIQUE_VAR(begin)))

	/// @brief Instantiates each iterator category for the type T
	///
	/// Instantiates the necessary declarations and definitions for each Cnx iterator category,
	/// enabling their use elsewhere.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define DeclCnxIterators(T)          \
		DeclCnxForwardIterator(T);       \
		DeclCnxReverseIterator(T);       \
		DeclCnxBidirectionalIterator(T); \
		DeclCnxRandomAccessIterator(T);

/// @brief Declares all Cnx iterators for the builtin `char_ref`
DeclCnxIterators(char_ref);
/// @brief Declares all Cnx iterators for the builtin `u8_ref`
DeclCnxIterators(u8_ref);
/// @brief Declares all Cnx iterators for the builtin `u16_ref`
DeclCnxIterators(u16_ref);
/// @brief Declares all Cnx iterators for the builtin `u32_ref`
DeclCnxIterators(u32_ref);
/// @brief Declares all Cnx iterators for the builtin `u64_ref`
DeclCnxIterators(u64_ref);
/// @brief Declares all Cnx iterators for the builtin `usize_ref`
DeclCnxIterators(usize_ref);
/// @brief Declares all Cnx iterators for the builtin `i8_ref`
DeclCnxIterators(i8_ref);
/// @brief Declares all Cnx iterators for the builtin `i16_ref`
DeclCnxIterators(i16_ref);
/// @brief Declares all Cnx iterators for the builtin `i32_ref`
DeclCnxIterators(i32_ref);
/// @brief Declares all Cnx iterators for the builtin `i64_ref`
DeclCnxIterators(i64_ref);
/// @brief Declares all Cnx iterators for the builtin `isize_ref`
DeclCnxIterators(isize_ref);
/// @brief Declares all Cnx iterators for the builtin `f32_ref`
DeclCnxIterators(f32_ref);
/// @brief Declares all Cnx iterators for the builtin `f64_ref`
DeclCnxIterators(f64_ref);
/// @brief Declares all Cnx iterators for the builtin `cstring_ref`
DeclCnxIterators(cstring_ref);
/// @brief Declares all Cnx iterators for the builtin `const_cstring_ref`
DeclCnxIterators(const_cstring_ref);
/// @brief Declares all Cnx iterators for the builtin `const_char_ref`
DeclCnxIterators(const_char_ref);
/// @brief Declares all Cnx iterators for the builtin `const_u8_ref`
DeclCnxIterators(const_u8_ref);
/// @brief Declares all Cnx iterators for the builtin `const_u16_ref`
DeclCnxIterators(const_u16_ref);
/// @brief Declares all Cnx iterators for the builtin `const_u32_ref`
DeclCnxIterators(const_u32_ref);
/// @brief Declares all Cnx iterators for the builtin `const_u64_ref`
DeclCnxIterators(const_u64_ref);
/// @brief Declares all Cnx iterators for the builtin `const_usize_ref`
DeclCnxIterators(const_usize_ref);
/// @brief Declares all Cnx iterators for the builtin `const_i8_ref`
DeclCnxIterators(const_i8_ref);
/// @brief Declares all Cnx iterators for the builtin `const_i16_ref`
DeclCnxIterators(const_i16_ref);
/// @brief Declares all Cnx iterators for the builtin `const_i32_ref`
DeclCnxIterators(const_i32_ref);
/// @brief Declares all Cnx iterators for the builtin `const_i64_ref`
DeclCnxIterators(const_i64_ref);
/// @brief Declares all Cnx iterators for the builtin `const_isize_ref`
DeclCnxIterators(const_isize_ref);
/// @brief Declares all Cnx iterators for the builtin `const_f32_ref`
DeclCnxIterators(const_f32_ref);
/// @brief Declares all Cnx iterators for the builtin `const_f64_ref`
DeclCnxIterators(const_f64_ref);

#endif // CNX_ITERATOR
