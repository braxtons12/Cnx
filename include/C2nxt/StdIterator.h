/// @file StdIterator.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides iterator Trait templates similar to the different categories of
/// iterators in C++ for C2nxt.
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

#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdTrait.h>
#include <memory.h>

/// @defgroup iterators Iterators
/// C2nxt defines its own set of iterator SizedTrait templates that allow for simple,
/// collection-agnostic processing of a collection of elements in a way similar to C++ or Rust.
///
/// Iterators provide a uniform interface for accessing the elements of a collection in contained
/// order. They act as reference-wrappers for the element associated with their current position,
/// providing reference-like semantics for accessing values, and because of this are designed to
/// work with pointer-to-T (`typedef`ed as `Ref(T)` or `ConstRef(T)`) as their generic parameter as
/// opposed to T.
///
/// C2nxt defines four categories of iterators, each with different requirements and functionality.
/// 1. Forward Iterators (`StdForwardIterator(T)`) provide forward-only (increment-only)
/// access to the elements in an iteration. This is provided through the associated `next` function.
/// 2. Reverse Iterators (`StdReverseIterator(T)`) provide reverse-on (decrement-only)
/// access to the elements in an iteration. This is provided through the associated `next` function.
/// 3. Bidirectional Iterators (`StdBidirectionalIterator(T)`) provide forward and reverse
/// access to the elements in an iteration. This is provided through the associated `next` and
/// `previous` functions.
/// 4. Random Access Iterators (`StdRandomAccessIterator(T)`) provide bidirectional and
/// random (indexed) access to the elements in an iteration. This is provided through the
/// associated `next`, `previous`, and `at` functions.
///
/// All iterators also provide access to the current value through `current` and equality comparison
/// through `equals`.
///
/// Using iterators are very simple.
///
/// @code {.c}
/// let_mut str = std_string_from("this is a string");
/// // We can use them by hand like so:
/// let_mut begin = std_string_begin(str);
/// let end = std_string_end(str);
///	// for each `char` in `str`, do something
/// for(let_mut character = std_iterator_current(begin);
/// 	!std_iterator_equals(begin, end);
///		character = std_iterator_next(begin)) {
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
/// Most C2nxt collections will require const and non-const iterators for their associated type to
/// be instantiated prior to their own instantiation. This is very simple, to provide instantiations
/// of iterators for a collection of type `T`, just:
///
/// @code {.c}
/// typedef T* Ref(T);
/// typedef const T* ConstRef(T);
/// DeclStdIterators(Ref(T));
/// DeclStdIterators(ConstRef(T));
/// @endcode
///
/// The above will instantiate iterators of all four categories for the type `T`, but you could also
/// only instantiate forward iterators or reverse iterators, depending on the collection you intend
/// to use. When unsure, instantiating all is the simplest solution.
///
/// To provide C2nxt iterators for your own collections, the process is a little more involved.
/// First, you'll need a concrete type to back your iterator (it will implement the desired iterator
/// Traits). By design, these should contain, **exactly**, an `isize` index, `m_index`, and a
/// pointer to associated collection, in **exactly** that order. If you deviate from this size, you
/// will cause undefined behavior and **should** get a compiler error. If you deviate from this
/// layout, you'll break other abstractions built on top of iterators, like `StdRange(T)`, for your
/// type.
///
/// For example, the pseudo-code for the concrete type for `StdVector(T)`'s non-const iterator is:
///
/// @code {.c}
/// typedef StdVector(T)Iterator {
///		StdVector(T)* m_vector;
/// 	isize m_index;
/// } StdVector(T)Iterator;
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
/// For example, for `StdForwardIterator(T)`, this would be:
///
/// @code {.c}
/// // in "YourCollection.h"
/// DeclIntoStdForwardIterator(YourCollection, T, YourDesiredIntoIterFuncName);
/// // in "YourCollection.c"
/// ImplIntoStdForwardIterator(YourCollection,
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
/// `StdString`, `StdVector(T)`, or `StdArray(T)`
///
/// @note Remember that iterators have reference-like semantics, so if you provide iterator
/// implementations for your own collections, keep in mind that if your collection stores `T`, your
/// iterators should wrap and return `Ref(T)` or `ConstRef(T)` (`StdForwardIterator(Ref(T))`, for an
/// example).

#ifndef STD_ITERATOR
	/// @brief Declarations and definitions related to C2nxt iterators
	#define STD_ITERATOR

	/// @brief The maximum compatible size of a concrete type implementing a C2nxt iterator trait
	///
	/// C2nxt iterators are designed to have a maximum concrete size, as they are SizedTraits, and
	/// to be compatible a concrete type can't exceed this (doing so would be UB and **should**
	/// cause a compiler error or warning).
	/// @ingroup iterators
	#define STD_ITERATOR_CONCRETE_TYPE_SIZE (sizeof(void*) + sizeof(isize))

	/// @brief Generates the function name for converting from one iterator category to its
	/// subcategory
	#define IntoIteratorSubType(CollectionType, IteratorType, ConversionName, IteratorSubType) \
		CONCAT3(UNIQUE_VAR(CollectionType),                                                    \
				IteratorType,                                                                  \
				CONCAT2(CONCAT3(_, ConversionName, _), IteratorSubType))

	/// @brief Macro alias for `StdForwardIterator` of type `T`
	///
	/// Used for creating and referencing a typedef for a specific forward iterator instantiation.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define StdForwardIterator(T) CONCAT2(StdForwardIterator, T)

	/// @brief Instantiates `StdForwardIterator(T)` for the type `T`
	///
	/// Instantiates the necessary declarations and definitions for `StdForwardIterator(T)`,
	/// enabling its use elsewhere.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define DeclStdForwardIterator(T)                                               \
		typedef struct StdForwardIterator(T) StdForwardIterator(T);                 \
		SizedTrait(StdForwardIterator(T),                                           \
				   STD_ITERATOR_CONCRETE_TYPE_SIZE,                                 \
				   T (*const next)(StdForwardIterator(T)* restrict self);           \
				   T(*const current)(const StdForwardIterator(T)* restrict self);   \
				   bool (*const equals)(const StdForwardIterator(T)* restrict self, \
										const StdForwardIterator(T)* restrict rhs););

	/// @brief Instantiates an `into_iter` function declaration.
	///
	/// Instantiates the declaration of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `StdForwardIterator(T)` associated with the collection.
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @ingroup iterators
	#define DeclIntoStdForwardIterator(CollectionType, T, IntoIterFuncName) \
		StdForwardIterator(T) IntoIterFuncName(const CollectionType* restrict self);

	/// @brief Instantiates an `into_iter` function definition.
	///
	/// Instantiates the definition of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `StdForwardIterator(T)` associated with the collection.
	/// Requires that the following are already declared and/or defined:
	/// 1. `CreateFunction` - A factory function returning the concrete type backing the
	/// 	collection's iterators of the signature:
	///
	/// 		ConcreteType (*const CreateFunction)(const Collection* restrict self);
	///
	/// 2. `NextFunction` - Advances the iterator to the next element in the iteration and returns
	/// 	the element. Takes the signature:
	///
	/// 		T (*const NextFunction)(StdForwardIterator(T)* restrict self);
	///
	/// 3. `CurrentFunction` - Returns the element associated with the current position of the
	/// 	iterator. Takes the signature:
	///
	/// 		T (*const CurrentFunction)(const StdForwardIterator(T)* restrict self);
	///
	/// 4. `EqualsFunction` - Determines if two given iterators are equal. Takes the signature:
	///
	/// 		bool (*const EqualsFunction)(const StdForwardIterator(T)* restrict self,
	/// 							 		 const StdForwardIterator(T)* restrict rhs);
	///
	/// Since iterators are a Trait object, your internal implementations can cast the `self`
	/// pointer to a pointer to the concrete type and operate on it as required.
	/// Example:
	///
	/// @code {.c}
	/// bool (*const EqualsFunction)(const StdForwardIterator(T)* restrict self,
	/// 							 const StdForwardIterator(T)* restrict rhs) {
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
	/// `StdForwardIterator(T)` for your collection.
	/// @param NextFunction - The function advancing the iterator
	/// @param CurrentFunction - The function returning the element currently associated with the
	/// iterator
	/// @param EqualsFunction - The function determining if two iterators are equal
	/// @ingroup iterators
	#define ImplIntoStdForwardIterator(CollectionType,                                           \
									   T,                                                        \
									   IntoIterFuncName,                                         \
									   CreateFunction,                                           \
									   NextFunction,                                             \
									   CurrentFunction,                                          \
									   EqualsFunction)                                           \
		StdForwardIterator(T) IntoIterFuncName(const CollectionType* restrict self) {            \
			T (*const _next)(StdForwardIterator(T)* restrict self) = (NextFunction);             \
			ignore(_next);                                                                       \
                                                                                                 \
			T (*const _current)(const StdForwardIterator(T)* restrict self) = (CurrentFunction); \
			ignore(_current);                                                                    \
                                                                                                 \
			bool (*const _equals)(const StdForwardIterator(T)* restrict self,                    \
								  const StdForwardIterator(T)* restrict rhs)                     \
				= (EqualsFunction);                                                              \
			ignore(_equals);                                                                     \
                                                                                                 \
			static ImplTraitFor(StdForwardIterator(T),                                           \
								CollectionType,                                                  \
								(NextFunction),                                                  \
								(CurrentFunction),                                               \
								(EqualsFunction));                                               \
                                                                                                 \
			let begin = CreateFunction(self);                                                    \
			return as_sized_trait(StdForwardIterator(T), CollectionType, begin);                 \
		}

	/// @brief Macro alias for `StdReverseIterator` of type `T`
	///
	/// Used for creating and referencing a typedef for a specific reverse iterator instantiation.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define StdReverseIterator(T) CONCAT2(StdReverseIterator, T)

	/// @brief Instantiates `StdReverseIterator(T)` for the type `T`
	///
	/// Instantiates the necessary declarations and definitions for `StdReverseIterator(T)`,
	/// enabling its use elsewhere.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define DeclStdReverseIterator(T)                                               \
		SizedTrait(StdReverseIterator(T),                                           \
				   STD_ITERATOR_CONCRETE_TYPE_SIZE,                                 \
				   T (*const next)(StdReverseIterator(T)* restrict self);           \
				   T(*const current)(const StdReverseIterator(T)* restrict self);   \
				   bool (*const equals)(const StdReverseIterator(T)* restrict self, \
										const StdReverseIterator(T)* restrict rhs););

	/// @brief Instantiates an `into_iter` function declaration.
	///
	/// Instantiates the declaration of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `StdReverseIterator(T)` associated with the collection.
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @ingroup iterators
	#define DeclIntoStdReverseIterator(CollectionType, T, IntoIterFuncName) \
		StdReverseIterator(T) IntoIterFuncName(const CollectionType* restrict self);

	/// @brief Instantiates an `into_iter` function definition.
	///
	/// Instantiates the definition of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `StdReverseIterator(T)` associated with the collection.
	/// Requires that the following are already declared and/or defined:
	/// 1. `CreateFunction` - A factory function returning the concrete type backing the
	/// 	collection's iterators of the signature:
	///
	/// 		ConcreteType (*const CreateFunction)(const Collection* restrict self);
	///
	/// 2. `NextFunction` - Advances the iterator to the next element in the iteration and returns
	/// 	the element. Takes the signature:
	///
	/// 		T (*const NextFunction)(StdReverseIterator(T)* restrict self);
	///
	/// 3. `CurrentFunction` - Returns the element associated with the current position of the
	/// 	iterator. Takes the signature:
	///
	/// 		T (*const CurrentFunction)(const StdReverseIterator(T)* restrict self);
	///
	/// 4. `EqualsFunction` - Determines if two given iterators are equal. Takes the signature:
	///
	/// 		bool (*const EqualsFunction)(const StdReverseIterator(T)* restrict self,
	/// 							 		 const StdReverseIterator(T)* restrict rhs);
	///
	/// Since iterators are a Trait object, your internal implementations can cast the `self`
	/// pointer to a pointer to the concrete type and operate on it as required.
	/// Example:
	///
	/// @code {.c}
	/// bool (*const EqualsFunction)(const StdReverseIterator(T)* restrict self,
	/// 							 const StdReverseIterator(T)* restrict rhs) {
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
	/// `StdReverseIterator(T)` for your collection.
	/// @param NextFunction - The function advancing the iterator
	/// @param CurrentFunction - The function returning the element currently associated with the
	/// iterator
	/// @param EqualsFunction - The function determining if two iterators are equal
	/// @ingroup iterators
	#define ImplIntoStdReverseIterator(CollectionType,                                           \
									   T,                                                        \
									   IntoIterFuncName,                                         \
									   CreateFunction,                                           \
									   NextFunction,                                             \
									   CurrentFunction,                                          \
									   EqualsFunction)                                           \
		StdReverseIterator(T) IntoIterFuncName(const CollectionType* restrict self) {            \
			T (*const _next)(StdReverseIterator(T)* restrict self) = (NextFunction);             \
			ignore(_next);                                                                       \
                                                                                                 \
			T (*const _current)(const StdReverseIterator(T)* restrict self) = (CurrentFunction); \
			ignore(_current);                                                                    \
                                                                                                 \
			bool (*const _equals)(const StdReverseIterator(T)* restrict self,                    \
								  const StdReverseIterator(T)* restrict rhs)                     \
				= (EqualsFunction);                                                              \
			ignore(_equals);                                                                     \
                                                                                                 \
			static ImplTraitFor(StdReverseIterator(T),                                           \
								CollectionType,                                                  \
								(NextFunction),                                                  \
								(CurrentFunction),                                               \
								(EqualsFunction));                                               \
                                                                                                 \
			let begin = CreateFunction(self);                                                    \
			return as_sized_trait(StdReverseIterator(T), CollectionType, begin);                 \
		}

	/// @brief Macro alias for `StdBidirectionalIterator` of type `T`
	///
	/// Used for creating and referencing a typedef for a specific bidirectional iterator
	/// instantiation.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define StdBidirectionalIterator(T) CONCAT2(StdBidirectionalIterator, T)

	/// @brief Instantiates `StdBidirectionalIterator(T)` for the type `T`
	///
	/// Instantiates the necessary declarations and definitions for `StdBidirectionalIterator(T)`,
	/// enabling its use elsewhere.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define DeclStdBidirectionalIterator(T)                                               \
		SizedTrait(StdBidirectionalIterator(T),                                           \
				   STD_ITERATOR_CONCRETE_TYPE_SIZE,                                       \
				   T (*const next)(StdBidirectionalIterator(T)* restrict self);           \
				   T(*const previous)(StdBidirectionalIterator(T)* restrict self);        \
				   T(*const current)(const StdBidirectionalIterator(T)* restrict self);   \
				   bool (*const equals)(const StdBidirectionalIterator(T)* restrict self, \
										const StdBidirectionalIterator(T)* restrict rhs); \
				   StdForwardIterator(T)(*const into_forward_iterator)(                   \
					   const StdBidirectionalIterator(T)* restrict self);                 \
				   StdReverseIterator(T)(*const into_reverse_iterator)(                   \
					   const StdBidirectionalIterator(T)* restrict self););

	/// @brief Instantiates an `into_iter` function declaration and iterator conversion function
	/// declarations.
	///
	/// Instantiates the declaration of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `StdBidirectionalIterator(T)` associated with the collection.
	/// Also Instantiates the declarations for conversion functions from
	/// `StdBidirectionalIterator(T)` to its subcategories `StdForwardIterator(T)` and
	/// `StdReverseIterator(T)`.
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @param IteratorConversionName - The root name for the conversion functions
	/// @ingroup iterators
	#define DeclIntoStdBidirectionalIterator(CollectionType,                   \
											 T,                                \
											 IntoIterFuncName,                 \
											 IteratorConversionName)           \
                                                                               \
		StdForwardIterator(T) IntoIteratorSubType(CollectionType,              \
												  StdBidirectionalIterator(T), \
												  IteratorConversionName,      \
												  StdForwardIterator(T))(      \
			const StdBidirectionalIterator(T)* restrict self);                 \
                                                                               \
		StdReverseIterator(T) IntoIteratorSubType(CollectionType,              \
												  StdBidirectionalIterator(T), \
												  IteratorConversionName,      \
												  StdReverseIterator(T))(      \
			const StdBidirectionalIterator(T)* restrict self);                 \
                                                                               \
		StdBidirectionalIterator(T) IntoIterFuncName(const CollectionType* restrict self);

	/// @brief Instantiates an `into_iter` function definition and iterator conversion function
	/// definitions.
	///
	/// Instantiates the definition of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `StdBidirectionalIterator(T)` associated with the collection.
	/// Also Instantiates the definitions for conversion functions from
	/// `StdBidirectionalIterator(T)` to its subcategories `StdForwardIterator(T)` and
	/// `StdReverseIterator(T)`.
	/// Requires that the following are already declared and/or defined:
	/// 1. `CreateFunction` - A factory function returning the concrete type backing the
	/// 	collection's iterators of the signature:
	///
	/// 		ConcreteType (*const CreateFunction)(const Collection* restrict self);
	///
	/// 2. `NextFunction` - Advances the iterator to the next element in the iteration and returns
	/// 	the element. Takes the signature:
	///
	/// 		T (*const NextFunction)(StdBidirectionalIterator(T)* restrict self);
	///
	/// 3. `PreviousFunction` - Decrements the iterator to the previous element in the iteration and
	/// 	returns the element. Takes the signature:
	///
	/// 		T (*const PreviousFunction)(StdBidirectionalIterator(T)* restrict self);
	///
	/// 4. `CurrentFunction` - Returns the element associated with the current position of the
	/// 	iterator. Takes the signature:
	///
	/// 		T (*const CurrentFunction)(const StdBidirectionalIterator(T)* restrict self);
	///
	/// 5. `EqualsFunction` - Determines if two given iterators are equal. Takes the signature:
	///
	/// 		bool (*const EqualsFunction)(const StdBidirectionalIterator(T)* restrict self,
	/// 							 		 const StdBidirectionalIterator(T)* restrict rhs);
	///
	/// Since iterators are a Trait object, your internal implementations can cast the `self`
	/// pointer to a pointer to the concrete type and operate on it as required.
	/// Example:
	///
	/// @code {.c}
	/// bool (*const EqualsFunction)(const StdBidirectionalIterator(T)* restrict self,
	/// 							 const StdBidirectionalIterator(T)* restrict rhs) {
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
	/// `StdBidirectionalIterator(T)` for your collection.
	/// @param NextFunction - The function advancing the iterator
	/// @param PreviousFunction - The function decrementing the iterator
	/// @param CurrentFunction - The function returning the element currently associated with the
	/// iterator
	/// @param EqualsFunction - The function determining if two iterators are equal
	/// @ingroup iterators
	#define ImplIntoStdBidirectionalIterator(CollectionType,                                       \
											 T,                                                    \
											 IntoIterFuncName,                                     \
											 IteratorConversionName,                               \
											 CreateFunction,                                       \
											 NextFunction,                                         \
											 PreviousFunction,                                     \
											 CurrentFunction,                                      \
											 EqualsFunction)                                       \
		StdForwardIterator(T) IntoIteratorSubType(CollectionType,                                  \
												  StdBidirectionalIterator(T),                     \
												  IteratorConversionName,                          \
												  StdForwardIterator(T))(                          \
			StdBidirectionalIterator(T) restrict self) {                                           \
			static ImplTraitFor(                                                                   \
				StdForwardIterator(T),                                                             \
				CollectionType,                                                                    \
				static_cast(T(*const)(StdForwardIterator(T)* restrict))(NextFunction),             \
				static_cast(T(*const)(const StdForwardIterator(T)* restrict))(CurrentFunction),    \
				static_cast(bool (*const)(const StdForwardIterator(T)* restrict,                   \
										  const StdForwardIterator(T)* restrict))(                 \
					EqualsFunction));                                                              \
                                                                                                   \
			return as_sized_trait(StdForwardIterator(T), CollectionType, self->m_self);            \
		}                                                                                          \
                                                                                                   \
		StdReverseIterator(T) IntoIteratorSubType(CollectionType,                                  \
												  StdBidirectionalIterator(T),                     \
												  IteratorConversionName,                          \
												  StdReverseIterator(T))(                          \
			StdBidirectionalIterator(T) restrict self) {                                           \
			static ImplTraitFor(                                                                   \
				StdReverseIterator(T),                                                             \
				CollectionType,                                                                    \
				static_cast(T(*const)(StdReverseIterator(T)* restrict))(PreviousFunction),         \
				static_cast(T(*const)(const StdReverseIterator(T)* restrict))(CurrentFunction),    \
				static_cast(bool (*const)(const StdReverseIterator(T)* restrict,                   \
										  const StdReverseIterator(T)* restrict))(                 \
					EqualsFunction));                                                              \
                                                                                                   \
			return as_sized_trait(StdReverseIterator(T), CollectionType, self->m_self);            \
		}                                                                                          \
                                                                                                   \
		StdBidirectionalIterator(T) IntoIterFuncName(const CollectionType* restrict self) {        \
			T (*const _next)(StdBidirectionalIterator(T)* restrict self) = (NextFunction);         \
			ignore(_next);                                                                         \
			T (*const _previous)(StdBidirectionalIterator(T)* restrict self) = (PreviousFunction); \
			ignore(_previous);                                                                     \
                                                                                                   \
			T(*const _current)                                                                     \
			(const StdBidirectionalIterator(T)* restrict self) = (CurrentFunction);                \
			ignore(_current);                                                                      \
                                                                                                   \
			bool (*const _equals)(const StdBidirectionalIterator(T)* restrict self,                \
								  const StdBidirectionalIterator(T)* restrict rhs)                 \
				= (EqualsFunction);                                                                \
			ignore(_equals);                                                                       \
                                                                                                   \
			static ImplTraitFor(StdBidirectionalIterator(T),                                       \
								CollectionType,                                                    \
								(NextFunction),                                                    \
								(PreviousFunction),                                                \
								(CurrentFunction),                                                 \
								(EqualsFunction),                                                  \
								IntoIteratorSubType(CollectionType,                                \
													StdBidirectionalIterator(T),                   \
													IteratorConversionName,                        \
													StdForwardIterator(T)),                        \
								IntoIteratorSubType(CollectionType,                                \
													StdBidirectionalIterator(T),                   \
													IteratorConversionName,                        \
													StdReverseIterator(T)));                       \
                                                                                                   \
			let begin = CreateFunction(self);                                                      \
			return as_sized_trait(StdBidirectionalIterator(T), CollectionType, begin);             \
		}

	/// @brief Macro alias for `StdRandomAccessIterator` of type `T`
	///
	/// Used for creating and referencing a typedef for a specific random access iterator
	/// instantiation.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define StdRandomAccessIterator(T) CONCAT2(StdRandomAccessIterator, T)

	/// @brief Instantiates `StdRandomAccessIterator(T)` for the type `T`
	///
	/// Instantiates the necessary declarations and definitions for `StdRandomAccessIterator(T)`,
	/// enabling its use elsewhere.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define DeclStdRandomAccessIterator(T)                                                     \
		SizedTrait(StdRandomAccessIterator(T),                                                 \
				   STD_ITERATOR_CONCRETE_TYPE_SIZE,                                            \
				   T (*const next)(StdRandomAccessIterator(T)* restrict self);                 \
				   T(*const previous)(StdRandomAccessIterator(T)* restrict self);              \
				   T(*const at)(const StdRandomAccessIterator(T)* restrict self, usize index); \
				   T(*const current)(const StdRandomAccessIterator(T)* restrict self);         \
				   bool (*const equals)(const StdRandomAccessIterator(T)* restrict self,       \
										const StdRandomAccessIterator(T)* restrict rhs);       \
				   StdForwardIterator(T)(*const into_forward_iterator)(                        \
					   const StdRandomAccessIterator(T)* restrict self);                       \
				   StdReverseIterator(T)(*const into_reverse_iterator)(                        \
					   const StdRandomAccessIterator(T)* restrict self);                       \
				   StdBidirectionalIterator(T)(*const into_bidirectional_iterator)(            \
					   const StdRandomAccessIterator(T)* restrict self););

	/// @brief Instantiates an `into_iter` function declaration and iterator conversion function
	/// declarations.
	///
	/// Instantiates the declaration of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `StdRandomAccessIterator(T)` associated with the collection.
	/// Also Instantiates the declarations for conversion functions from
	/// `StdRandomAccessIterator(T)` to its subcategories `StdForwardIterator(T)`,
	/// `StdReverseIterator(T)`, and `StdBidirectionalIterator(T)`.
	///
	/// @param CollectionType - The type of the collection to have an `into_iter` function
	/// instantiated for
	/// @param T - The reference type the iterator represents
	/// @param IntoIterFuncName - The name for the `into_iter` function
	/// @param IteratorConversionName - The root name for the conversion functions
	/// @ingroup iterators
	#define DeclIntoStdRandomAccessIterator(CollectionType,                           \
											T,                                        \
											IntoIterFuncName,                         \
											IteratorConversionName)                   \
                                                                                      \
		StdForwardIterator(T) IntoIteratorSubType(CollectionType,                     \
												  StdRandomAccessIterator(T),         \
												  IteratorConversionName,             \
												  StdForwardIterator(T))(             \
			const StdRandomAccessIterator(T)* restrict self);                         \
                                                                                      \
		StdReverseIterator(T) IntoIteratorSubType(CollectionType,                     \
												  StdRandomAccessIterator(T),         \
												  IteratorConversionName,             \
												  StdReverseIterator(T))(             \
			const StdRandomAccessIterator(T)* restrict self);                         \
                                                                                      \
		StdBidirectionalIterator(T) IntoIteratorSubType(CollectionType,               \
														StdRandomAccessIterator(T),   \
														IteratorConversionName,       \
														StdBidirectionalIterator(T))( \
			const StdRandomAccessIterator(T)* restrict self);                         \
                                                                                      \
		StdRandomAccessIterator(T) IntoIterFuncName(const CollectionType* restrict self);

	/// @brief Instantiates an `into_iter` function definition and iterator conversion function
	/// definitions.
	///
	/// Instantiates the definition of an `into_iter` function, associated with the collection type
	/// `CollectionType`, returning a `StdRandomAccessIterator(T)` associated with the collection.
	/// Also Instantiates the definitions for conversion functions from
	/// `StdRandomAccessIterator(T)` to its subcategories `StdForwardIterator(T)`,
	/// `StdReverseIterator(T)`, and `StdBidirectionalIterator(T)`.
	/// Requires that the following are already declared and/or defined:
	/// 1. `CreateFunction` - A factory function returning the concrete type backing the
	/// 	collection's iterators of the signature:
	///
	/// 		ConcreteType (*const CreateFunction)(const Collection* restrict self);
	///
	/// 2. `NextFunction` - Advances the iterator to the next element in the iteration and returns
	/// 	the element. Takes the signature:
	///
	/// 		T (*const NextFunction)(StdRandomAccessIterator(T)* restrict self);
	///
	/// 3. `PreviousFunction` - Decrements the iterator to the previous element in the iteration and
	/// 	returns the element. Takes the signature:
	///
	/// 		T (*const PreviousFunction)(StdRandomAccessIterator(T)* restrict self);
	///
	/// 4. `AtFunction` - Returns the element in the iteration associated with the given index.
	/// 	Takes the signature:
	///
	/// 		T (*const AtFunction)(StdRandomAccessIterator(T)* restrict self, usize index);
	///
	/// 5. `CurrentFunction` - Returns the element associated with the current position of the
	/// 	iterator. Takes the signature:
	///
	/// 		T (*const CurrentFunction)(const StdRandomAccessIterator(T)* restrict self);
	///
	/// 6. `EqualsFunction` - Determines if two given iterators are equal. Takes the signature:
	///
	/// 		bool (*const EqualsFunction)(const StdRandomAccessIterator(T)* restrict self,
	/// 							 		 const StdRandomAccessIterator(T)* restrict rhs);
	///
	/// Since iterators are a Trait object, your internal implementations can cast the `self`
	/// pointer to a pointer to the concrete type and operate on it as required.
	/// Example:
	///
	/// @code {.c}
	/// bool (*const EqualsFunction)(const StdRandomAccessIterator(T)* restrict self,
	/// 							 const StdRandomAccessIterator(T)* restrict rhs) {
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
	/// `StdRandomAccessIterator(T)` for your collection.
	/// @param NextFunction - The function advancing the iterator
	/// @param PreviousFunction - The function decrementing the iterator
	/// @param AtFunction - The function returning the element associated with the given index into
	/// the iteration
	/// @param CurrentFunction - The function returning the element currently associated with the
	/// iterator
	/// @param EqualsFunction - The function determining if two iterators are equal
	/// @ingroup iterators
	#define ImplIntoStdRandomAccessIterator(CollectionType,                                       \
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
		StdForwardIterator(T) IntoIteratorSubType(CollectionType,                                 \
												  StdRandomAccessIterator(T),                     \
												  IteratorConversionName,                         \
												  StdForwardIterator(T))(                         \
			const StdRandomAccessIterator(T)* restrict self) {                                    \
			static ImplTraitFor(                                                                  \
				StdForwardIterator(T),                                                            \
				CollectionType,                                                                   \
				static_cast(T(*const)(StdForwardIterator(T)* restrict))(NextFunction),            \
				static_cast(T(*const)(const StdForwardIterator(T)* restrict))(CurrentFunction),   \
				static_cast(bool (*const)(const StdForwardIterator(T)* restrict,                  \
										  const StdForwardIterator(T)* restrict))(                \
					EqualsFunction));                                                             \
                                                                                                  \
			return as_sized_trait(StdForwardIterator(T), CollectionType, self->m_self);           \
		}                                                                                         \
                                                                                                  \
		StdReverseIterator(T) IntoIteratorSubType(CollectionType,                                 \
												  StdRandomAccessIterator(T),                     \
												  IteratorConversionName,                         \
												  StdReverseIterator(T))(                         \
			const StdRandomAccessIterator(T)* restrict self) {                                    \
			static ImplTraitFor(                                                                  \
				StdReverseIterator(T),                                                            \
				CollectionType,                                                                   \
				static_cast(T(*const)(StdReverseIterator(T)* restrict))(PreviousFunction),        \
				static_cast(T(*const)(const StdReverseIterator(T)* restrict))(CurrentFunction),   \
				static_cast(bool (*const)(const StdReverseIterator(T)* restrict,                  \
										  const StdReverseIterator(T)* restrict))(                \
					EqualsFunction));                                                             \
                                                                                                  \
			return as_sized_trait(StdReverseIterator(T), CollectionType, self->m_self);           \
		}                                                                                         \
                                                                                                  \
		StdBidirectionalIterator(T) IntoIteratorSubType(CollectionType,                           \
														StdRandomAccessIterator(T),               \
														IteratorConversionName,                   \
														StdBidirectionalIterator(T))(             \
			const StdRandomAccessIterator(T)* restrict self) {                                    \
			static ImplTraitFor(                                                                  \
				StdBidirectionalIterator(T),                                                      \
				CollectionType,                                                                   \
				static_cast(T(*const)(StdBidirectionalIterator(T)* restrict))(NextFunction),      \
				static_cast(T(*const)(StdBidirectionalIterator(T)* restrict))(PreviousFunction),  \
				static_cast(T(*const)(const StdBidirectionalIterator(T)* restrict))(              \
					CurrentFunction),                                                             \
				static_cast(bool (*const)(const StdBidirectionalIterator(T)* restrict,            \
										  const StdBidirectionalIterator(T)* restrict))(          \
					EqualsFunction),                                                              \
				static_cast(StdForwardIterator(T)(*const)(const StdBidirectionalIterator(         \
					T)* restrict))(IntoIteratorSubType(CollectionType,                            \
													   StdRandomAccessIterator(T),                \
													   IteratorConversionName,                    \
													   StdForwardIterator(T))),                   \
				static_cast(StdReverseIterator(T)(*const)(const StdBidirectionalIterator(         \
					T)* restrict))(IntoIteratorSubType(CollectionType,                            \
													   StdRandomAccessIterator(T),                \
													   IteratorConversionName,                    \
													   StdReverseIterator(T))));                  \
                                                                                                  \
			return as_sized_trait(StdBidirectionalIterator(T), CollectionType, self->m_self);     \
		}                                                                                         \
                                                                                                  \
		StdRandomAccessIterator(T) IntoIterFuncName(const CollectionType* restrict self) {        \
			T (*const _next)(StdRandomAccessIterator(T)* restrict self) = (NextFunction);         \
			ignore(_next);                                                                        \
			T (*const _previous)(StdRandomAccessIterator(T)* restrict self) = (PreviousFunction); \
			ignore(_previous);                                                                    \
			T(*const _at)                                                                         \
			(const StdRandomAccessIterator(T)* restrict self, usize index) = (AtFunction);        \
			ignore(_at);                                                                          \
                                                                                                  \
			T(*const _current)                                                                    \
			(const StdRandomAccessIterator(T)* restrict self) = (CurrentFunction);                \
			ignore(_current);                                                                     \
                                                                                                  \
			bool (*const _equals)(const StdRandomAccessIterator(T)* restrict self,                \
								  const StdRandomAccessIterator(T)* restrict rhs)                 \
				= (EqualsFunction);                                                               \
			ignore(_equals);                                                                      \
                                                                                                  \
			static ImplTraitFor(StdRandomAccessIterator(T),                                       \
								CollectionType,                                                   \
								(NextFunction),                                                   \
								(PreviousFunction),                                               \
								(AtFunction),                                                     \
								(CurrentFunction),                                                \
								(EqualsFunction),                                                 \
								IntoIteratorSubType(CollectionType,                               \
													StdRandomAccessIterator(T),                   \
													IteratorConversionName,                       \
													StdForwardIterator(T)),                       \
								IntoIteratorSubType(CollectionType,                               \
													StdRandomAccessIterator(T),                   \
													IteratorConversionName,                       \
													StdReverseIterator(T)),                       \
								IntoIteratorSubType(CollectionType,                               \
													StdRandomAccessIterator(T),                   \
													IteratorConversionName,                       \
													StdBidirectionalIterator(T)));                \
                                                                                                  \
			let begin = CreateFunction(self);                                                     \
			return as_sized_trait(StdRandomAccessIterator(T), CollectionType, begin);             \
		}

	/// @brief Advances the iterator to the next element.
	///
	/// Advances the iterator to the next element and returns a reference to that element.
	///
	/// @param iterator - The iterator to advance
	///
	/// @return a reference to the next element in the iteration
	/// @note The result of calling `std_iterator_next(iterator)` on an iterator the is already at
	/// or past the end of the iterator is implementation defined (by that specific iterator's trait
	/// implementation)
	/// @ingroup iterators
	#define std_iterator_next(iterator) *(trait_call(next, iterator))
	/// @brief Decrements the iterator to the previous element.
	///
	/// Decrements the iterator to the previous element and returns a reference to that element.
	///
	/// @param iterator - The iterator to decrement
	///
	/// @return a reference to the previous element in the iteration
	/// @note `std_iterator_previous(iterator)` can only be called on `StdBidirectionalIterator(T)`s
	/// and `StdRandomAccessIterator(T)`s. Other iterator categories do no implement `previous`
	/// @note The result of calling `std_iterator_previous(iterator)` on an iterator the is already
	/// at or before the beginning of the iterator is implementation defined (by that specific
	/// iterator's trait implementation)
	/// @ingroup iterators
	#define std_iterator_previous(iterator) *(trait_call(previous, iterator))
	/// @brief Returns a reference to the element at the given index in the iteration
	///
	/// @param iterator - The iterator to get an element from
	/// @param index - The index in the iteration of the desired element
	///
	/// @return a reference to the element at the given index
	/// @note `std_iterator_at(iterator, index)` can only be called on
	/// `StdRandomAccessIterator(T)`s. Other iterator categories do no implement `at`
	/// @note The result of calling `std_iterator_at(iterator, index)` with an index that is out of
	/// bounds of the iteration (before the beginning or past the end) is implementation defined (by
	/// that specific iterator's trait implementation)
	/// @ingroup iterators
	#define std_iterator_at(iterator, index) *(trait_call(at, iterator, index))
	/// @brief Returns a reference to the element associated with the current position of the
	/// iterator
	///
	/// @param iterator - The iterator to get the currently associated element from
	///
	/// @return a reference to the element associated with the current position of the iterator
	/// @note The result of calling `std_iterator_current(iterator)` on an iterator that is out of
	/// bounds of the iteration (before the beginning or past the end) is implementation defined (by
	/// that specific iterator's trait implementation)
	/// @ingroup iterators
	#define std_iterator_current(iterator) *(trait_call(current, iterator))
	/// @brief Returns whether the given iterators are equal
	///
	/// @param lhs - The LHS iterator of the equality comparison
	/// @param rhs - The RHS iterator of the equality comparison
	///
	/// @return whether the iterators are equal
	/// @ingroup iterators
	#define std_iterator_equals(lhs, rhs) trait_call(equals, lhs, &(rhs))
	/// @brief Converts the given `StdBidirectionalIterator(T)` or `StdRandomAccessIterator(T)` into
	/// a `StdForwardIterator(T)`
	///
	/// @param iterator - The iterator to convert
	///
	/// @return `iterator` as a `StdForwardIterator(T)`
	/// @note `std_iterator_into_std_forward_iterator` can only be called on
	/// `StdBidirectionalIterator(T)`s and `StdRandomAccessIterator(T)`s. Other iterator categories
	/// do not implement `into_std_forward_iterator`
	/// @ingroup iterators
	#define std_iterator_into_std_forward_iterator(iterator) \
		trait_call(into_forward_iterator, iterator)
	/// @brief Converts the given `StdBidirectionalIterator(T)` or `StdRandomAccessIterator(T)` into
	/// a `StdReverseIterator(T)`
	///
	/// @param iterator - The iterator to convert
	///
	/// @return `iterator` as a `StdReverseIterator(T)`
	/// @note `std_iterator_into_std_reverse_iterator` can only be called on
	/// `StdBidirectionalIterator(T)`s and `StdRandomAccessIterator(T)`s. Other iterator categories
	/// do not implement `into_std_reverse_iterator`
	/// @ingroup iterators
	#define std_iterator_into_std_reverse_iterator(iterator) \
		trait_call(into_reverse_iterator, iterator)
	/// @brief Converts the given `StdRandomAccessIterator(T)` into a `StdBidirectionalIterator(T)`
	///
	/// @param iterator - The iterator to convert
	///
	/// @return `iterator` as a `StdBidirectionalIterator(T)`
	/// @note `std_iterator_into_std_bidirectional_iterator` can only be called on
	/// `StdRandomAccessIterator(T)`s. Other iterator categories do not implement
	/// `into_std_bidirectional_iterator`
	/// @ingroup iterators
	#define std_iterator_into_std_bidirectional_iterator(iterator) \
		trait_call(into_bidirectional_iterator, iterator)

	#if STD_PLATFORM_COMPILER_CLANG
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
		for(let_mut element = std_iterator_current(UNIQUE_VAR(begin)); 							   \
			!std_iterator_equals(UNIQUE_VAR(begin), UNIQUE_VAR(end));  							   \
			element = std_iterator_next(UNIQUE_VAR(begin)))
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
		for(let_mut element = std_iterator_current(UNIQUE_VAR(begin)); 							   \
			!std_iterator_equals(UNIQUE_VAR(begin), UNIQUE_VAR(end));  							   \
			element = std_iterator_next(UNIQUE_VAR(begin)))
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
		for(let_mut element = &std_iterator_current(UNIQUE_VAR(begin));         \
			!std_iterator_equals(UNIQUE_VAR(begin), UNIQUE_VAR(end));           \
			element = &std_iterator_next(UNIQUE_VAR(begin)))

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
		for(let_mut element = &std_iterator_current(UNIQUE_VAR(begin));         \
			!std_iterator_equals(UNIQUE_VAR(begin), UNIQUE_VAR(end));           \
			element = &std_iterator_next(UNIQUE_VAR(begin)))

	/// @brief Instantiates each iterator category for the type T
	///
	/// Instantiates the necessary declarations and definitions for each C2nxt iterator category,
	/// enabling their use elsewhere.
	///
	/// @param T - The reference type the iterator represents
	/// @ingroup iterators
	#define DeclStdIterators(T)          \
		DeclStdForwardIterator(T);       \
		DeclStdReverseIterator(T);       \
		DeclStdBidirectionalIterator(T); \
		DeclStdRandomAccessIterator(T);

/// @brief Declares all C2nxt iterators for the builtin `char_ref`
DeclStdIterators(char_ref);
/// @brief Declares all C2nxt iterators for the builtin `u8_ref`
DeclStdIterators(u8_ref);
/// @brief Declares all C2nxt iterators for the builtin `u16_ref`
DeclStdIterators(u16_ref);
/// @brief Declares all C2nxt iterators for the builtin `u32_ref`
DeclStdIterators(u32_ref);
/// @brief Declares all C2nxt iterators for the builtin `u64_ref`
DeclStdIterators(u64_ref);
/// @brief Declares all C2nxt iterators for the builtin `usize_ref`
DeclStdIterators(usize_ref);
/// @brief Declares all C2nxt iterators for the builtin `i8_ref`
DeclStdIterators(i8_ref);
/// @brief Declares all C2nxt iterators for the builtin `i16_ref`
DeclStdIterators(i16_ref);
/// @brief Declares all C2nxt iterators for the builtin `i32_ref`
DeclStdIterators(i32_ref);
/// @brief Declares all C2nxt iterators for the builtin `i64_ref`
DeclStdIterators(i64_ref);
/// @brief Declares all C2nxt iterators for the builtin `isize_ref`
DeclStdIterators(isize_ref);
/// @brief Declares all C2nxt iterators for the builtin `f32_ref`
DeclStdIterators(f32_ref);
/// @brief Declares all C2nxt iterators for the builtin `f64_ref`
DeclStdIterators(f64_ref);
/// @brief Declares all C2nxt iterators for the builtin `cstring_ref`
DeclStdIterators(cstring_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_cstring_ref`
DeclStdIterators(const_cstring_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_char_ref`
DeclStdIterators(const_char_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_u8_ref`
DeclStdIterators(const_u8_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_u16_ref`
DeclStdIterators(const_u16_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_u32_ref`
DeclStdIterators(const_u32_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_u64_ref`
DeclStdIterators(const_u64_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_usize_ref`
DeclStdIterators(const_usize_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_i8_ref`
DeclStdIterators(const_i8_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_i16_ref`
DeclStdIterators(const_i16_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_i32_ref`
DeclStdIterators(const_i32_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_i64_ref`
DeclStdIterators(const_i64_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_isize_ref`
DeclStdIterators(const_isize_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_f32_ref`
DeclStdIterators(const_f32_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_f64_ref`
DeclStdIterators(const_f64_ref);
/// @brief Declares all C2nxt iterators for the builtin `char_ptr_ref`
DeclStdIterators(char_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `u8_ptr_ref`
DeclStdIterators(u8_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `u16_ptr_ref`
DeclStdIterators(u16_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `u32_ptr_ref`
DeclStdIterators(u32_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `u64_ptr_ref`
DeclStdIterators(u64_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `usize_ptr_ref`
DeclStdIterators(usize_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `i8_ptr_ref`
DeclStdIterators(i8_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `i16_ptr_ref`
DeclStdIterators(i16_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `i32_ptr_ref`
DeclStdIterators(i32_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `i64_ptr_ref`
DeclStdIterators(i64_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `isize_ptr_ref`
DeclStdIterators(isize_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `f32_ptr_ref`
DeclStdIterators(f32_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `f64_ptr_ref`
DeclStdIterators(f64_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_char_ptr_ref`
DeclStdIterators(const_char_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_u8_ptr_ref`
DeclStdIterators(const_u8_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_u16_ptr_ref`
DeclStdIterators(const_u16_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_u32_ptr_ref`
DeclStdIterators(const_u32_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_u64_ptr_ref`
DeclStdIterators(const_u64_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_usize_ptr_ref`
DeclStdIterators(const_usize_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_i8_ptr_ref`
DeclStdIterators(const_i8_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_i16_ptr_ref`
DeclStdIterators(const_i16_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_i32_ptr_ref`
DeclStdIterators(const_i32_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_i64_ptr_ref`
DeclStdIterators(const_i64_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_isize_ptr_ref`
DeclStdIterators(const_isize_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_f32_ptr_ref`
DeclStdIterators(const_f32_ptr_ref);
/// @brief Declares all C2nxt iterators for the builtin `const_f64_ptr_ref`
DeclStdIterators(const_f64_ptr_ref);

#endif // STD_ITERATOR
