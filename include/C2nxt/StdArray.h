/// @file StdArray.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a generic, type-safe, fixed-capacity, dynamic-size, stack allocated
/// array type (`StdArray(T, N)`) similar to `std::array` in C++
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

/// @ingroup collections
/// @{
/// @defgroup std_array StdArray
/// `StdArray(T, N)` is a struct template for a type-safe fixed-capacity, dynamic-size, stack
/// allocated array type. `StdArray(T, N)` is bounds safe, allocator aware, and provides C2nxt
/// compatible random access iterators. It supports used-defined default-constructors,
/// copy-constructors, and destructors for its elements. It's design is based around C++'s
/// `std::array`, but with slightly different semantics and functionality.
///
/// Because of its size, `N`, template parameter, instantiations of `StdArray(T, N)` for builtins or
/// any C2nxt types are __not__ provided, and will have to be instantiated by the user.
/// The requirements are similar to `StdVector(T)`, and requires minimal boiler-plate:
/// 1. a `typedef` of your type to provide an alphanumeric name for it. (for template and macro
/// 	parameters)
///	2. a `typedef` for pointer to your type as `Ref(YourTuple), for use with the iterators
/// 3. a `typedef` for pointer to const your type as `ConstRef(YourType)`, for use with the
/// 	iterators
/// 4. Instantiations for C2nxt iterators for the typedefs provided in (2) and (3) (already
/// 	instantiated for builtins and `StdString`)
/// 5. Instantiation of `StdOption(T)` for your type (already instantiated for builtins and
/// 	`StdString`)
///
/// For `StdArray(T, N)` we recommend you provide the instantiation for user-define types in a
/// separate "template instantiation" .h/.c file pair, but you can also provide them inline with
/// your type like discussed in `StdVector(T)`'s documentation. \see StdVector
///
/// Example:
///
/// @code {.c}
/// // in `YourType.h`
/// typedef struct YourType {
///		// members
/// } YourType;
///
/// typedef YourType* Ref(YourType);
/// typedef const YourType* ConstRef(YourType);
///
/// DeclStdIterators(Ref(YourType));
/// DeclStdIterators(ConstRef(YourType));
/// // the rest of your public interface
///
/// // in `StdOptionYourType.h`
/// #include "YourType.h"
/// DeclStdOption(YourType);
///
/// // in `StdOptionYourType.c`
/// #include "StdOptionYourType.h"
/// ImplStdOption(YourType);
///
/// // in `StdArrayYourTypeYourN.h"
/// #include "YourType.h"
/// #include "StdOptionYourType.h"
/// DeclStdArray(YourType, YourN);
///
/// // in `StdArrayYourTypeYourN.c`
/// #include "StdArrayYourTypeYourN.h"
/// ImplStdArray(YourType, YourN);
/// @endcode
///
/// The, somewhere else in your codebase, you can easily use the instantiation:
///
/// @code {.c}
/// #include "YourType.h"
/// #include "StdArrayYourT/ypeYourN.h"
///
/// StdArray(YourType, YourN) create_and_fill_your_type_array(void) {
///		let_mut array = std_array_new(YourType, YourN);
///
///		std_array_resize(array, YourN);
///
/// 	return array;
/// }
/// @endcode
///
/// `StdArray(T, N)` implements `StdFormat`, but because of its generic type, can't be automatically
/// converted to its `StdFormat` implementation, so we have to cast to it explicitly:
///
/// @code {.c}
/// #include "C2nxt/StdIO.h"
/// void print_array(const StdArray(YourType, YourN)* array) {
///		println("{}", as_format_t(StdArray(YourType, YourN), *array);
/// }
/// @endcode
///
/// You can provide user-defined default-constructor, copy-constructor, and/or destructor for
/// elements of your type, and custom allocator for memory allocations they might need to perform
/// (`StdArray(T, N)` is always stack allocated, so the memory allocator is __only__ for constructor
/// and destructor functions for your type)
///
/// @code {.c}
/// YourType your_type_constructor(StdAllocator allocator) {
///		let_mut t = (YourType){0};
/// 	// whatever you need to do to init your type
/// 	return t;
/// }
///
/// YourType your_type_copy_constructor(const YourType* restrict elem, StdAllocator allocator) {
///		let_mut t = (YourType){0};
/// 	// whatever you need to do to copy `elem` to `t`
///		return t;
/// }
///
/// void your_type_destructor(YourType* restrict t, StdAllocator allocator) {
///		// whatever you need to do to cleanup `t`
/// }
///
/// StdArray(YourType, YourN) create_and_fill_your_type_array(void) {
///		let_mut array = std_array_new_with_collection_data(YourType,
/// 						YourN,
///							// This `StdCollectionData(CollectionType)` will only be
/// 						// associated with this specific array, not all
/// 						// `StdArray(YourType, YourN)`s
///							((StdCollectionData(StdArray(YourType, YourN))) {
///								// can be left NULL, will be defaulted
///								.m_constructor = your_type_constructor,
///								// can be left NULL, will disable `std_array_clone(self)` for
/// 							// this array
/// 							.m_copy_constructor = your_type_copy_constructor,
///								// can be left NULL, will be defaulted
/// 							.m_destructor = your_type_destructor
///								// must explicitly provide an allocator, even just the default one,
/// 							// if providing user-defined collection data
///								.m_allocator = std_allocator_from_custom_allocator(
/// 												your_malloc_func,
/// 												your_free_func)
/// 						})
///						);
///
/// 	// fill the array with default constructed elements
/// 	std_array_resize(array, YourN);
///
/// 	return array;
/// }
/// @endcode
///
/// `StdArray(T, N)` also provides C2nxt random access iterators and optional scoped destruction:
///
/// @code {.c}
/// void example(void) {
///		let_mut std_array_scoped(u32, 10) array = std_array_new(u32, 10);
/// 	ranged_for(i, 0, 9) {
///			std_array_push_back(array, i);
/// 	}
///
/// 	// loop over the elements in the array by value, as an iteration
///		foreach(elem, vec) {
///			println("{}", elem);
/// 	}
///		// array goes out of scope here and `std_array_free(array)` is called on it
/// 	// because we declared it as scoped
/// }
/// @endcode
///
/// Like other C2nxt collections, `StdArray(T, N)` provides its type-agnostic usage through a
/// vtable pointer contained in the struct, and provides macros which wrap the usage of the vtable,
/// making access simpler. If you prefer to not use this method of access, you can call the typed
/// functions directly by in-fixing the contained type in the associated function name.
/// IE: for `StdArray(i32, 10)`, and an array `array`, the equivalent function call for
/// `std_array_push_back(array, element)` would be `std_array_i32_10_push_back(&array, element)`
/// @note Note, however, that using the typed functions instead of the macros results in losing the
/// reference-semantics of function-macros like `std_array_at(self, index)` and will instead be
/// dealing with pointers directly.
/// Example:
/// @code {.c}
/// // instead of:
/// std_array_at_mut(self, 5) = 5;
/// // you would have to do:
/// *std_array_i32_10_at_mut(&self, 5) = 5;
/// @endcode
/// @}

#include <C2nxt/StdAllocators.h>
#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdCollectionsData.h>
#include <C2nxt/StdFormat.h>
#include <C2nxt/StdIterator.h>
#include <C2nxt/StdMath.h>
#include <C2nxt/StdOption.h>
#include <C2nxt/StdResult.h>

#ifndef STD_ARRAY
	/// @brief Declarations and Definitions related to `StdArray(T, N)`
	#define STD_ARRAY

	/// @brief Macro alias for a `StdArray(T, N)` containing up to `N` `T`s
	///
	/// `StdArray(T, N)` is a generic, type-safe, fixed-capacity, dynamic-size, stack allocated
	/// array type. `StdArray(T, N)` is bounds safe and allocator aware. It's implemented as a
	/// "struct template", which enables 100% type safety while also providing abstractions that
	/// enable type agnostic use. It has greatly increased ergonomics over manually managing an
	/// array and provides many useful features such as random access iterators and optional scoped
	/// destruction.
	///
	/// Example:
	/// @code {.c}
	/// #include "C2nxt/StdArray.h"
	/// #include "C2nxt/StdIO.h"
	/// #include "StdArray_i32_10.h"
	///
	/// // create a `StdArray(i32, 10)` with default allocator, element default-constructor, and
	/// element
	/// // destructor
	/// let_mut array = std_array_new(i32, 10);
	/// // fill the array with 10 elements
	/// ranged_for(i, 0, 9) {
	/// 	std_array_push_back(array, i);
	/// }
	///
	/// prints information about `array` to `stdout`
	/// because `StdArray(T, N)` is generic, and thus can't be used in a `_Generic` match arm prior
	/// to the type's instantiation, we have to explicitly cast to the `StdFormat` Trait to get
	/// `StdArray(T, N)`'s implementation of the Trait println("{}", std_as_format_t(StdArray(i32,
	/// 10), array));
	///
	/// // prints `array`'s elements to stdout
	/// foreach(elem, array) {
	/// 	println("{}", elem);
	/// }
	///
	/// {
	/// 	let_mut std_array_scoped(i32, 10) array2 = std_array_new(i32, 10);
	/// 	foreach(elem, array) {
	/// 		std_array_push_back(array2, elem);
	/// 	}
	/// }
	/// // `array2` is destroyed here because it was scoped
	/// // `array` is not destroyed
	/// @endcode
	///
	/// The above example assumes that the `StdArray(T, N)` template has been instantiated for
	/// `StdArray(i32, 10)` in "StdArray_i32_10.h" and an associated ".c" file.
	/// See `DeclStdArray(T, N)`, `ImplStdArray(T, N)`, and the module-level page for details
	///
	/// Like other C2nxt collections, `StdArray(T, N)` provides its type-agnostic usage through a
	/// vtable pointer contained in the struct, and provides macros which wrap the usage of the
	/// vtable, making access simpler. If you prefer to not use this method of access, you can call
	/// the typed functions directly by in-fixing the contained type in the associated function
	/// name. IE: for `StdArray(i32, 10)`, and an array `array`, the equivalent function call for
	/// `std_array_push_back(array, element)` would be `std_array_i32_10_push_back(&array, element)`
	/// @note Note, however, that using the typed functions instead of the macros results in losing
	/// the reference-semantics of function-macros like `std_array_at(self, index)` and will instead
	/// be dealing with pointers directly. Example:
	/// @code {.c}
	/// // instead of:
	/// std_array_at_mut(self, 5) = 5;
	/// // you would have to do:
	/// *std_array_i32_10_at_mut(&self, 5) = 5;
	/// @endcode
	/// @ingroup std_array
	#define StdArray(T, N) CONCAT3(StdArray, T, N)
	/// @brief macro alias for the concrete type for an iterator into the mutable iteration of a
	/// `StdArray(T, N)` containing up to `N` `T`s
	#define StdArrayIterator(T, N) CONCAT2(StdArray(T, N), Iterator)
	/// @brief macro alias for the concrete type for an iterator into the const iteration of a
	/// `StdArray(T, N)` containing up to `N` `T`s
	#define StdArrayConstIterator(T, N) CONCAT2(StdArray(T, N), ConstIterator)
	/// @brief macro alias for an identifier (type, function, etc) associated with a `StdArray(T,
	/// N)` containing up to `N` `T`s
	#define StdArrayIdentifier(T, N, Identifier) \
		CONCAT3(std_array_, CONCAT2(T, _), CONCAT3(N, _, Identifier))

	/// @brief Use this macro to instantiate the declarations for `StdArray(T, N)` containing up to
	/// `N` `T`s. This provides the struct declarations and definitions and function declarations
	/// for `StdArray(T, N)`.
	///
	/// Requires that the following have been met prior to use:
	/// 1. a `typedef` of your type to provide an alphanumeric name for it as `T`
	/// 2. a `typedef` for pointer to your type as `Ref(YourType)`
	/// 3. a `typedef` for pointer to const your type as `ConstRef(YourType)`
	/// 4. Instantiations for C2nxt iterators for the typedefs provided in (2) and (3)
	/// 5. Instantiation of `StdOption(YourType)`, via provided macros (See `StdOption(T)`)
	///
	/// @param T - The element type of the `StdArray(T, N)` instantiation
	/// @param N - The capacity of the `StdArray(T, N)` instantiation
	/// @ingroup std_array
	#define DeclStdArray(T, N)                                                                    \
		DeclStdCollectionData(T, StdArray(T, N));                                                 \
		typedef struct StdArrayIdentifier(T, N, vtable) StdArrayIdentifier(T, N, vtable);         \
		typedef struct StdArray(T, N) {                                                           \
			const StdArrayIdentifier(T, N, vtable) * m_vtable;                                    \
			const StdCollectionData(StdArray(T, N)) m_data;                                       \
			usize m_size;                                                                         \
			T m_array[N];                                                                         \
		}                                                                                         \
		StdArray(T, N);                                                                           \
                                                                                                  \
		typedef struct StdArrayIterator(T, N) {                                                   \
			isize m_index;                                                                        \
			StdArray(T, N) * m_array;                                                             \
		}                                                                                         \
		StdArrayIterator(T, N);                                                                   \
                                                                                                  \
		typedef struct StdArrayConstIterator(T, N) {                                              \
			isize m_index;                                                                        \
			const StdArray(T, N) * m_array;                                                       \
		}                                                                                         \
		StdArrayConstIterator(T, N);                                                              \
                                                                                                  \
		DeclStdOption(StdArray(T, N));                                                            \
		DeclStdResult(StdArray(T, N));                                                            \
                                                                                                  \
		StdArray(T, N) StdArrayIdentifier(T, N, new)(void);                                       \
		StdArray(T, N) StdArrayIdentifier(T, N, new_with_collection_data)(                        \
			StdCollectionData(StdArray(T, N)) data);                                              \
		StdArray(T, N) StdArrayIdentifier(T, N, clone)(const StdArray(T, N)* restrict self)       \
			std_disable_if(!(self->m_data.m_copy_constructor),                                    \
						   "Can't clone a StdArray(T, N) with elements that aren't copyable (no " \
						   "element copy constructor defined)");                                  \
		const T* StdArrayIdentifier(T, N, at_const)(const StdArray(T, N)* restrict self,          \
													usize index);                                 \
		T* StdArrayIdentifier(T, N, at_mut)(StdArray(T, N)* restrict self, usize index);          \
		const T* StdArrayIdentifier(T, N, front_const)(const StdArray(T, N)* restrict self);      \
		T* StdArrayIdentifier(T, N, front_mut)(StdArray(T, N)* restrict self);                    \
		const T* StdArrayIdentifier(T, N, back_const)(const StdArray(T, N)* restrict self);       \
		T* StdArrayIdentifier(T, N, back_mut)(StdArray(T, N)* restrict self);                     \
		const T* StdArrayIdentifier(T, N, data_const)(const StdArray(T, N)* restrict self);       \
		T* StdArrayIdentifier(T, N, data_mut)(StdArray(T, N)* restrict self);                     \
		bool StdArrayIdentifier(T, N, is_empty)(const StdArray(T, N)* restrict self);             \
		bool StdArrayIdentifier(T, N, is_full)(const StdArray(T, N)* restrict self);              \
		usize StdArrayIdentifier(T, N, size)(const StdArray(T, N)* restrict self);                \
		usize StdArrayIdentifier(T, N, capacity)(const StdArray(T, N)* restrict self);            \
		void StdArrayIdentifier(T, N, resize)(StdArray(T, N)* restrict self, usize new_size);     \
		void StdArrayIdentifier(T, N, clear)(StdArray(T, N)* restrict self);                      \
		void StdArrayIdentifier(T, N, push_back)(StdArray(T, N)* restrict self, T element);       \
		StdOption(T) StdArrayIdentifier(T, N, pop_back)(StdArray(T, N)* restrict self);           \
		void StdArrayIdentifier(T, N, insert)(StdArray(T, N)* restrict self,                      \
											  T element,                                          \
											  usize index);                                       \
		void StdArrayIdentifier(T, N, erase)(StdArray(T, N)* restrict self, usize index);         \
		void StdArrayIdentifier(T, N, erase_n)(StdArray(T, N)* restrict self,                     \
											   usize index,                                       \
											   usize length);                                     \
		void StdArrayIdentifier(T, N, free)(void* restrict self);                                 \
		StdString StdArrayIdentifier(T, N, format)(const StdFormat* restrict self,                \
												   StdFormatSpecifier specifier);                 \
		StdString StdArrayIdentifier(T, N, format_with_allocator)(const StdFormat* restrict self, \
																  StdFormatSpecifier specifier,   \
																  StdAllocator allocator);        \
                                                                                                  \
		static maybe_unused ImplTraitFor(StdFormat,                                               \
										 StdArray(T, N),                                          \
										 StdArrayIdentifier(T, N, format),                        \
										 StdArrayIdentifier(T, N, format_with_allocator));        \
                                                                                                  \
		DeclIntoStdRandomAccessIterator(StdArray(T, N),                                           \
										Ref(T),                                                   \
										StdArrayIdentifier(T, N, into_iter),                      \
										into);                                                    \
		DeclIntoStdRandomAccessIterator(StdArray(T, N),                                           \
										Ref(T),                                                   \
										StdArrayIdentifier(T, N, into_reverse_iter),              \
										into_reverse);                                            \
		DeclIntoStdRandomAccessIterator(StdArray(T, N),                                           \
										ConstRef(T),                                              \
										StdArrayIdentifier(T, N, into_const_iter),                \
										into);                                                    \
		DeclIntoStdRandomAccessIterator(StdArray(T, N),                                           \
										ConstRef(T),                                              \
										StdArrayIdentifier(T, N, into_reverse_const_iter),        \
										into_reverse);                                            \
                                                                                                  \
		StdRandomAccessIterator(Ref(T))                                                           \
			StdArrayIdentifier(T, N, begin)(StdArray(T, N)* restrict self);                       \
		StdRandomAccessIterator(Ref(T))                                                           \
			StdArrayIdentifier(T, N, end)(StdArray(T, N)* restrict self);                         \
		StdRandomAccessIterator(Ref(T))                                                           \
			StdArrayIdentifier(T, N, rbegin)(StdArray(T, N)* restrict self);                      \
		StdRandomAccessIterator(Ref(T))                                                           \
			StdArrayIdentifier(T, N, rend)(StdArray(T, N)* restrict self);                        \
                                                                                                  \
		StdRandomAccessIterator(ConstRef(T))                                                      \
			StdArrayIdentifier(T, N, cbegin)(const StdArray(T, N)* restrict self);                \
		StdRandomAccessIterator(ConstRef(T))                                                      \
			StdArrayIdentifier(T, N, cend)(const StdArray(T, N)* restrict self);                  \
		StdRandomAccessIterator(ConstRef(T))                                                      \
			StdArrayIdentifier(T, N, crbegin)(const StdArray(T, N)* restrict self);               \
		StdRandomAccessIterator(ConstRef(T))                                                      \
			StdArrayIdentifier(T, N, crend)(const StdArray(T, N)* restrict self);                 \
                                                                                                  \
		typedef struct StdArrayIdentifier(T, N, vtable) {                                         \
			StdArray(T, N) (*const clone)(const StdArray(T, N)* restrict self);                   \
			const T* (*const at_const)(const StdArray(T, N)* restrict self, usize index);         \
			T* (*const at_mut)(StdArray(T, N)* restrict self, usize index);                       \
			const T* (*const front_const)(const StdArray(T, N)* restrict self);                   \
			T* (*const front_mut)(StdArray(T, N)* restrict self);                                 \
			const T* (*const back_const)(const StdArray(T, N)* restrict self);                    \
			T* (*const back_mut)(StdArray(T, N)* restrict self);                                  \
			const T* (*const data_const)(const StdArray(T, N)* restrict self);                    \
			T* (*const data_mut)(StdArray(T, N)* restrict self);                                  \
			bool (*const is_empty)(const StdArray(T, N)* restrict self);                          \
			bool (*const is_full)(const StdArray(T, N)* restrict self);                           \
			usize (*const size)(const StdArray(T, N)* restrict self);                             \
			usize (*const capacity)(const StdArray(T, N)* restrict self);                         \
			void (*const resize)(StdArray(T, N)* restrict self, usize new_size);                  \
			void (*const clear)(StdArray(T, N)* restrict self);                                   \
			void (*const push_back)(StdArray(T, N)* restrict self, T element);                    \
			StdOption(T) (*const pop_back)(StdArray(T, N)* restrict self);                        \
			void (*const insert)(StdArray(T, N)* restrict self, T element, usize index);          \
			void (*const erase)(StdArray(T, N)* restrict self, usize index);                      \
			void (*const erase_n)(StdArray(T, N)* restrict self, usize index, usize length);      \
			void (*const free)(void* restrict self);                                              \
			StdRandomAccessIterator(Ref(T)) (*const into_iter)(                                   \
				const StdArray(T, N)* restrict self);                                             \
			StdRandomAccessIterator(Ref(T)) (*const into_reverse_iter)(                           \
				const StdArray(T, N)* restrict self);                                             \
			StdRandomAccessIterator(ConstRef(T)) (*const into_const_iter)(                        \
				const StdArray(T, N)* restrict self);                                             \
			StdRandomAccessIterator(ConstRef(T)) (*const into_reverse_const_iter)(                \
				const StdArray(T, N)* restrict self);                                             \
			StdRandomAccessIterator(Ref(T)) (*const begin)(StdArray(T, N)* restrict self);        \
			StdRandomAccessIterator(Ref(T)) (*const end)(StdArray(T, N)* restrict self);          \
			StdRandomAccessIterator(Ref(T)) (*const rbegin)(StdArray(T, N)* restrict self);       \
			StdRandomAccessIterator(Ref(T)) (*const rend)(StdArray(T, N)* restrict self);         \
			StdRandomAccessIterator(ConstRef(T)) (*const cbegin)(                                 \
				const StdArray(T, N)* restrict self);                                             \
			StdRandomAccessIterator(ConstRef(T)) (*const cend)(                                   \
				const StdArray(T, N)* restrict self);                                             \
			StdRandomAccessIterator(ConstRef(T)) (*const crbegin)(                                \
				const StdArray(T, N)* restrict self);                                             \
			StdRandomAccessIterator(ConstRef(T)) (*const crend)(                                  \
				const StdArray(T, N)* restrict self);                                             \
		}                                                                                         \
		StdArrayIdentifier(T, N, vtable);

/// @brief Creates a new `StdArray(T, N)` with defaulted associated functions.
///
/// Creates a new `StdArray(T, N)` with:
/// 1. default associated element default-constructor,
/// 2. default associated element destructor
/// 3. default associated memory allocator (potentially used in element constructor and
/// destructor, memory for the `StdArray(T, N)` itself is stack allocated)
///
/// @param T - The element type of the `StdArray(T, N)` instantiation to create
/// @param N - The capacity of the `StdArray(T, N)` instantiation to create
///
/// @return a new `StdArray(T, N)`
/// @ingroup std_array
	#define std_array_new(T, N) StdArrayIdentifier(T, N, new)()
	/// @brief Creates a new `StdArray(T, N)` with provided associated functions.
	///
	/// Creates a new `StdArray(T, N)` with:
	/// 1. possibly user-provided element default-constructor
	/// 2. possibly user-provided element destructor
	/// 3. user-provided memory allocator (potentially used in element constructor and destructor,
	/// memory for the `StdArray(T, N)` itself is stack allocated)
	///
	/// @param T - The element type of the `StdArray(T, N)` instantiation to create
	/// @param N - The capacity of the `StdArray(T, N)` instantiation to create
	/// @param collection_data - The `StdCollectionData` containing the element default-constructor,
	/// element destructor, and memory allocator to use
	///
	/// @return a new `StdArray(T, N)`
	/// @ingroup std_array
	#define std_array_new_with_collection_data(T, N, collection_data) \
		StdArrayIdentifier(T, N, new_with_collection_data)(collection_data)
	/// @brief Clones the given `StdArray(T, N)`
	///
	/// Creates a deep copy of the given `StdArray(T, N)` calling the associated copy constructor
	/// for each element stored in it.
	///
	/// @param self - The `StdArray(T, N)` to clone
	///
	/// @return a clone of the given array
	/// @note Requires that a copy constructor is defined for the elements
	/// contained in the array, either the default one provided by the implementation or a
	/// user-defined one. If it isn't provided or is explicitly provided as `nullptr` when creating
	/// the `StdCollectionData(CollectionType)` associated with this array, this __may__ trigger a
	/// compiler error on clang, and will always trigger a runtime assert for all compilers
	/// @ingroup std_array
	#define std_array_clone(self) (self).m_vtable->clone(&(self))
	/// @brief Returns a mutable reference to the element at the given `index` into the given
	/// `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get an element from
	/// @param index - The index of the desired element
	///
	/// @return a mutable reference to the element at the given index
	/// @ingroup std_array
	#define std_array_at_mut(self, index) *((self).m_vtable->at_mut(&(self), (index)))
	/// @brief Returns a const reference to the element at the given `index` into the given
	/// `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get an element from
	/// @param index - The index of the desired element
	///
	/// @return a const reference to the element at the given index
	/// @ingroup std_array
	#define std_array_at(self, index) *((self).m_vtable->at_const(&(self), (index)))
	/// @brief Returns a mutable reference to the first element in the given `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get the first element from
	///
	/// @return a mutable reference to the first element
	/// @ingroup std_array
	#define std_array_front_mut(self) *((self).m_vtable->front_mut(&(self)))
	/// @brief Returns a const reference to the first element in the given `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get the first element from
	///
	/// @return a const reference to the first element
	/// @ingroup std_array
	#define std_array_front(self) *((self).m_vtable->front_const(&(self)))
	/// @brief Returns a mutable reference to the last element in the given `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get the last element from
	///
	/// @return a mutable reference to the last element
	/// @ingroup std_array
	#define std_array_back_mut(self) *((self).m_vtable->back_mut(&(self)))
	/// @brief Returns a const reference to the last element in the given `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get the last element from
	///
	/// @return a const reference to the last element
	/// @ingroup std_array
	#define std_array_back(self) *((self).m_vtable->back_const(&(self)))
	/// @brief Returns a mutable reference to the internal array in the given `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get the internal array from
	///
	/// @return a mutable reference to the internal array
	/// @ingroup std_array
	#define std_array_data_mut(self) *((self).m_vtable->data_mut(&(self)))
	/// @brief Returns a const reference to the internal array in the given `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get the internal array from
	///
	/// @return a const reference to the internal array
	/// @ingroup std_array
	#define std_array_data(self) *((self).m_vtable->data_const(&(self)))
	/// @brief Returns whether the given `StdArray(T, N)` is empty
	///
	/// @param self - The `StdArray(T, N)` to check for emptiness
	///
	/// @return `true` if empty, `false` otherwise
	/// @ingroup std_array
	#define std_array_is_empty(self) (self).m_vtable->is_empty(&(self))
	/// @brief Returns whether the given `StdArray(T, N)` is full
	///
	/// @param self - The `StdArray(T, N)` to check for fullness
	///
	/// @return `true` if full, `false` otherwise
	/// @ingroup std_array
	#define std_array_is_full(self) (self).m_vtable->is_full(&(self))
	/// @brief Returns the current size of the given `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get the size of
	///
	/// @return the size of the `StdArray(T, N)`
	/// @ingroup std_array
	#define std_array_size(self) (self).m_vtable->size(&(self))
	/// @brief Returns the capacity of the given `StdArray(T, N)` (ie the `N`)
	///
	/// @param self - The `StdArray(T, N)` to get the capacity of
	///
	/// @return the capacity of the `StdArray(T, N)`
	/// @ingroup std_array
	#define std_array_capacity(self) (self).m_vtable->capacity(&(self))
	/// @brief Resizes the given `StdArray(T, N)` to `new_size`
	///
	/// `new_size` must be strictly `<= N`.
	/// If `new_size` is greater than the current size, elements will be default constructed
	/// until `self` contains `new_size` elements.
	/// If `new_size` is less than the current size, elements will be destroyed.
	///
	/// @param self - The `StdArray(T, N)` to resize
	/// @param new_size - The new size for the given array
	/// @ingroup std_array
	#define std_array_resize(self, new_size) (self).m_vtable->resize(&(self), (new_size))
	/// @brief Resets the given `StdArray(T, N)` to a size of zero, destroying any active elements
	/// in the array.
	///
	/// @param self - The `StdArray(T, N)` to clear
	/// @ingroup std_array
	#define std_array_clear(self) (self).m_vtable->clear(&(self))
	/// @brief Pushes a new element into the given `StdArray(T, N)`, at the end.
	///
	/// The current size must be strictly less than `N`.
	///
	/// @param self - The `StdArray(T, N)` to add an element to
	/// @param element - The element to add to the array
	/// @ingroup std_array
	#define std_array_push_back(self, element) (self).m_vtable->push_back(&(self), (element))
	/// @brief Removes the last element from the given `StdArray(T, N)` and returns it
	///
	/// @param self - The `StdArray(T, N)` to pop the last element from
	///
	/// @return `Some(T)` if size greater than 0, otherwise `None(T)`
	/// @ingroup std_array
	#define std_array_pop_back(self) (self).m_vtable->pop_back(&(self))
	/// @brief Inserts the given element at the given index in the `StdArray(T, N)`
	///
	/// The current size must be strictly less than `N`.
	/// `index` must be strictly less than `N`.
	/// `index must be strictly less than the current size.
	///
	/// @param self - The `StdArray(T, N)` to insert in to
	/// @param element - The element to insert into the array
	/// @param index - The index to insert `element` at
	/// @ingroup std_array
	#define std_array_insert(self, element, index) \
		(self).m_vtable->insert(&(self), (element), (index))
	/// @brief Erases the element at `index` from the `StdArray(T, N)`
	///
	/// `index` must be strictly less than `N`.
	/// `index` must be strictly less than the current size.
	///
	/// @param self - The `StdArray(T, N)` to erase from
	/// @param index - The index of the element to erase
	/// @ingroup std_array
	#define std_array_erase(self, index) (self).m_vtable->erase(&(self), (index))
	/// @brief Erases `num_elements` elements from the `StdArray(T, N)`, starting at `index`
	///
	/// `index` must be strictly less than `N`
	/// `index` must be strictly less than the current size.
	/// `index + num_elements` must be strictly less than `N`
	/// `index + num_elements` must be strictly less than the current size
	///
	/// @param self - The `StdArray(T, N)` to erase from
	/// @param index - The index to begin erasing at
	/// @param num_elements - The number of elements to erase
	/// @ingroup std_array
	#define std_array_erase_n(self, index, num_elements) \
		(self).m_vtable->erase_n(&(self), (index), (num_elements))
	/// @brief Frees the given `StdArray(T, N)`, destroying its elements
	///
	/// `StdArray(T, N)` does not allocate memory itself, so it does not necessarily need to be
	/// freed, but if elements require a provided destructor to run, then `std_array_free` should be
	/// called on the array to ensure elements are properly cleaned up.
	///
	/// @param self - The `StdArray(T, N)` to free
	/// @ingroup std_array
	#define std_array_free(self) (self).m_vtable->free(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the mutable iteration of the given
	/// `StdArray(T, N)`, starting at the beginning of the iteration (pointing at the beginning of
	/// the array)
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the array
	/// @ingroup std_array
	#define std_array_begin(self) (self).m_vtable->begin(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the mutable iteration of the given
	/// `StdArray(T, N)`, starting at the end of the iteration (pointing at the end of the array)
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator at the end of the array
	/// @ingroup std_array
	#define std_array_end(self) (self).m_vtable->end(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the mutable iteration of the given
	/// `StdArray(T, N)`, starting at the beginning of the reversed iteration (pointing at the end
	/// of the array)
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the reversed array
	/// @ingroup std_array
	#define std_array_rbegin(self) (self).m_vtable->rbegin(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the mutable iteration of the given
	/// `StdArray(T, N)`, starting at the end of the reversed iteration (pointing at the beginning
	/// of the array)
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator at the end of the reversed array
	/// @ingroup std_array
	#define std_array_rend(self) (self).m_vtable->rend(&(self))
	/// @brief Returns whether the given pair of iterators are equal (they belong to the same
	/// collection and point to the same element), IE: if `first == second`
	///
	/// @param first - The LHS iterator of the equality check
	/// @param second - The RHS iterator of the equality check
	///
	/// @return `true` if they are equal, `false` otherwise
	/// @ingroup std_array
	#define std_array_iterator_equals(first, second) std_iterator_equals(first, second)
	/// @brief Returns a `StdRandomAccessIterator` into the const iteration of the given
	/// `StdArray(T, N)`, starting at the beginning of the iteration (pointing at the beginning of
	/// the array)
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the array
	/// @ingroup std_array
	#define std_array_cbegin(self) (self).m_vtable->cbegin(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the const iteration of the given
	/// `StdArray(T, N)`, starting at the end of the iteration (pointing at the end of the array)
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator at the end of the array
	/// @ingroup std_array
	#define std_array_cend(self) (self).m_vtable->cend(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the const iteration of the given
	/// `StdArray(T, N)`, starting at the beginning of the reversed iteration (pointing at the end
	/// of the array)
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the reversed array
	/// @ingroup std_array
	#define std_array_crbegin(self) (self).m_vtable->crbegin(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the const iteration of the given
	/// `StdArray(T, N)`, starting at the end of the reversed iteration (pointing at the beginning
	/// of the array)
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator at the end of the reversed array
	/// @ingroup std_array
	#define std_array_crend(self) (self).m_vtable->crend(&(self))
	/// @brief Returns whether the given pair of const iterators are equal (they belong to the same
	/// collection and point to the same element), IE: if `first == second`
	///
	/// @param first - The LHS iterator of the equality check
	/// @param second - The RHS iterator of the equality check
	///
	/// @return `true` if they are equal, `false` otherwise
	/// @ingroup std_array
	#define std_array_const_iterator_equals(first, second) std_iterator_equals(first, second)
	/// @brief Returns a `StdRandomAccessIterator` into the mutable iteration of the given
	/// `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator into the array
	#define std_array_into_iter(self) (self).m_vtable->into_iter(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the mutable reversed iteration of the given
	/// `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator into the reversed array
	#define std_array_into_reverse_iter(self) (self).m_vtable->into_reverse_iter(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the const iteration of the given
	/// `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator into the array
	#define std_array_into_const_iter(self) (self).m_vtable->into_const_iter(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the const reversed iteration of the given
	/// `StdArray(T, N)`
	///
	/// @param self - The `StdArray(T, N)` to get an iterator to
	///
	/// @return a random access iterator into the reversed array
	#define std_array_into_reverse_const_iter(self) \
		(self).m_vtable->into_reverse_const_iter(&(self))
	///
	/// @brief declare a `StdArray(T, N)` variable with this attribute to have `std_array_free`
	/// automatically called on it at scope end
	///
	/// @param T - The element type of the `StdArray(T, N)` instantiation
	/// @param N - The capacity of the `StdArray(T, N)` instantiation
	/// @ingroup std_array
	#define std_array_scoped(T, N)			scoped(StdArrayIdentifier(T, N, free))
	#define std_array_into_const_iter(self) (self).m_vtable->into_const_iter(&(self))
	/// @brief Use this macro to instantiate the definitions for a `StdArray(T, N)` of capacity `N`
	/// containing `T`s. This provides the function definitions for `StdArray(T, N)`.
	///
	/// Requires that `DeclStdArray(T, N)` has already been used and is in scope, and that the
	/// requirements for that have already been met.
	///
	/// @param T - The element type of the `StdArray(T, N)` instantiation
	/// @param N - The capacity of the `StdArray(T, N)` instantiation
	/// @ingroup std_array
	#define ImplStdArray(T, N)                                                                     \
		ImplStdOption(StdArray(T, N));                                                             \
		ImplStdResult(StdArray(T, N));                                                             \
                                                                                                   \
		StdArrayIterator(T, N)                                                                     \
			StdArrayIdentifier(T, N, iterator_new)(const StdArray(T, N)* restrict self);           \
		StdArrayConstIterator(T, N)                                                                \
			StdArrayIdentifier(T, N, const_iterator_new)(const StdArray(T, N)* restrict self);     \
                                                                                                   \
		Ref(T) StdArrayIdentifier(T, N, iterator_next)(                                            \
			StdRandomAccessIterator(Ref(T))* restrict self);                                       \
		Ref(T) StdArrayIdentifier(T, N, iterator_previous)(                                        \
			StdRandomAccessIterator(Ref(T))* restrict self);                                       \
		Ref(T) StdArrayIdentifier(T, N, iterator_at)(                                              \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			usize index);                                                                          \
		Ref(T) StdArrayIdentifier(T, N, iterator_rat)(                                             \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			usize index);                                                                          \
		Ref(T) StdArrayIdentifier(T, N, iterator_current)(                                         \
			const StdRandomAccessIterator(Ref(T))* restrict self);                                 \
		bool StdArrayIdentifier(T, N, iterator_equals)(                                            \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			const StdRandomAccessIterator(Ref(T))* restrict rhs);                                  \
		ConstRef(T) StdArrayIdentifier(T, N, iterator_cnext)(                                      \
			StdRandomAccessIterator(ConstRef(T))* restrict self);                                  \
		ConstRef(T) StdArrayIdentifier(T, N, iterator_cprevious)(                                  \
			StdRandomAccessIterator(ConstRef(T))* restrict self);                                  \
		ConstRef(T) StdArrayIdentifier(T, N, iterator_cat)(                                        \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			usize index);                                                                          \
		ConstRef(T) StdArrayIdentifier(T, N, iterator_crat)(                                       \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			usize index);                                                                          \
		ConstRef(T) StdArrayIdentifier(T, N, iterator_ccurrent)(                                   \
			const StdRandomAccessIterator(ConstRef(T))* restrict self);                            \
		bool StdArrayIdentifier(T, N, iterator_cequals)(                                           \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			const StdRandomAccessIterator(ConstRef(T))* restrict rhs);                             \
                                                                                                   \
		ImplIntoStdRandomAccessIterator(StdArray(T, N),                                            \
										Ref(T),                                                    \
										StdArrayIdentifier(T, N, into_iter),                       \
										into,                                                      \
										StdArrayIdentifier(T, N, iterator_new),                    \
										StdArrayIdentifier(T, N, iterator_next),                   \
										StdArrayIdentifier(T, N, iterator_previous),               \
										StdArrayIdentifier(T, N, iterator_at),                     \
										StdArrayIdentifier(T, N, iterator_current),                \
										StdArrayIdentifier(T, N, iterator_equals));                \
		ImplIntoStdRandomAccessIterator(StdArray(T, N),                                            \
										Ref(T),                                                    \
										StdArrayIdentifier(T, N, into_reverse_iter),               \
										into_reverse,                                              \
										StdArrayIdentifier(T, N, iterator_new),                    \
										StdArrayIdentifier(T, N, iterator_next),                   \
										StdArrayIdentifier(T, N, iterator_previous),               \
										StdArrayIdentifier(T, N, iterator_rat),                    \
										StdArrayIdentifier(T, N, iterator_current),                \
										StdArrayIdentifier(T, N, iterator_equals));                \
                                                                                                   \
		ImplIntoStdRandomAccessIterator(StdArray(T, N),                                            \
										ConstRef(T),                                               \
										StdArrayIdentifier(T, N, into_const_iter),                 \
										into,                                                      \
										StdArrayIdentifier(T, N, const_iterator_new),              \
										StdArrayIdentifier(T, N, iterator_cnext),                  \
										StdArrayIdentifier(T, N, iterator_cprevious),              \
										StdArrayIdentifier(T, N, iterator_cat),                    \
										StdArrayIdentifier(T, N, iterator_ccurrent),               \
										StdArrayIdentifier(T, N, iterator_cequals));               \
		ImplIntoStdRandomAccessIterator(StdArray(T, N),                                            \
										ConstRef(T),                                               \
										StdArrayIdentifier(T, N, into_reverse_const_iter),         \
										into_reverse,                                              \
										StdArrayIdentifier(T, N, const_iterator_new),              \
										StdArrayIdentifier(T, N, iterator_cnext),                  \
										StdArrayIdentifier(T, N, iterator_cprevious),              \
										StdArrayIdentifier(T, N, iterator_crat),                   \
										StdArrayIdentifier(T, N, iterator_ccurrent),               \
										StdArrayIdentifier(T, N, iterator_cequals));               \
                                                                                                   \
		always_inline static inline T StdArrayIdentifier(T, N, default_constructor)(               \
			StdAllocator allocator) {                                                              \
			ignore(allocator);                                                                     \
			return (T){0};                                                                         \
		}                                                                                          \
                                                                                                   \
		always_inline static inline T StdArrayIdentifier(T, N, default_copy_constructor)(          \
			const T* restrict element,                                                             \
			StdAllocator allocator) {                                                              \
			ignore(allocator);                                                                     \
			return *element;                                                                       \
		}                                                                                          \
                                                                                                   \
		always_inline static inline void StdArrayIdentifier(T, N, default_destructor)(             \
			T* restrict element, /** NOLINT(readability-non-const-parameter) **/                   \
			StdAllocator allocator) {                                                              \
			ignore(allocator, element);                                                            \
		}                                                                                          \
                                                                                                   \
		static const struct StdArrayIdentifier(T, N, vtable) StdArrayIdentifier(T, N, vtable_impl) \
			= {                                                                                    \
				.clone = StdArrayIdentifier(T, N, clone),                                          \
				.at_const = StdArrayIdentifier(T, N, at_const),                                    \
				.at_mut = StdArrayIdentifier(T, N, at_mut),                                        \
				.front_const = StdArrayIdentifier(T, N, front_const),                              \
				.front_mut = StdArrayIdentifier(T, N, front_mut),                                  \
				.back_const = StdArrayIdentifier(T, N, back_const),                                \
				.back_mut = StdArrayIdentifier(T, N, back_mut),                                    \
				.data_const = StdArrayIdentifier(T, N, data_const),                                \
				.data_mut = StdArrayIdentifier(T, N, data_mut),                                    \
				.is_empty = StdArrayIdentifier(T, N, is_empty),                                    \
				.is_full = StdArrayIdentifier(T, N, is_full),                                      \
				.size = StdArrayIdentifier(T, N, size),                                            \
				.capacity = StdArrayIdentifier(T, N, capacity),                                    \
				.resize = StdArrayIdentifier(T, N, resize),                                        \
				.clear = StdArrayIdentifier(T, N, clear),                                          \
				.push_back = StdArrayIdentifier(T, N, push_back),                                  \
				.pop_back = StdArrayIdentifier(T, N, pop_back),                                    \
				.insert = StdArrayIdentifier(T, N, insert),                                        \
				.erase = StdArrayIdentifier(T, N, erase),                                          \
				.erase_n = StdArrayIdentifier(T, N, erase_n),                                      \
				.free = StdArrayIdentifier(T, N, free),                                            \
				.into_iter = StdArrayIdentifier(T, N, into_iter),                                  \
				.into_reverse_iter = StdArrayIdentifier(T, N, into_reverse_iter),                  \
				.into_const_iter = StdArrayIdentifier(T, N, into_const_iter),                      \
				.into_reverse_const_iter = StdArrayIdentifier(T, N, into_reverse_const_iter),      \
				.begin = StdArrayIdentifier(T, N, begin),                                          \
				.end = StdArrayIdentifier(T, N, end),                                              \
				.rbegin = StdArrayIdentifier(T, N, rbegin),                                        \
				.rend = StdArrayIdentifier(T, N, rend),                                            \
				.cbegin = StdArrayIdentifier(T, N, cbegin),                                        \
				.cend = StdArrayIdentifier(T, N, cend),                                            \
				.crbegin = StdArrayIdentifier(T, N, crbegin),                                      \
				.crend = StdArrayIdentifier(T, N, crend),                                          \
		};                                                                                         \
                                                                                                   \
		static const struct StdCollectionData(StdArray(T, N))                                      \
			StdArrayIdentifier(T, N, default_collection_data)                                      \
			= {.m_constructor = StdArrayIdentifier(T, N, default_constructor),                     \
			   .m_copy_constructor = StdArrayIdentifier(T, N, default_copy_constructor),           \
			   .m_destructor = StdArrayIdentifier(T, N, default_destructor),                       \
			   .m_allocator = STD_DEFAULT_ALLOCATOR};                                              \
                                                                                                   \
		StdArray(T, N) StdArrayIdentifier(T, N, new)(void) {                                       \
			return std_array_new_with_collection_data(                                             \
				T,                                                                                 \
				N,                                                                                 \
				StdArrayIdentifier(T, N, default_collection_data));                                \
		}                                                                                          \
                                                                                                   \
		StdArray(T, N) StdArrayIdentifier(T, N, new_with_collection_data)(                         \
			StdCollectionData(StdArray(T, N)) data) {                                              \
			let_mut array = (StdArray(T, N)){.m_size = 0,                                          \
											 .m_data = data,                                       \
											 .m_array = {0},                                       \
											 .m_vtable = &StdArrayIdentifier(T, N, vtable_impl)};  \
			return array;                                                                          \
		}                                                                                          \
                                                                                                   \
		StdArray(T, N) StdArrayIdentifier(T, N, clone)(const StdArray(T, N)* restrict self)        \
			std_disable_if(!(self->m_data.m_copy_constructor),                                     \
						   "Can't clone a StdArray(T, N) with elements that aren't copyable (no "  \
						   "element copy constructor defined)") {                                  \
			std_assert(self->m_data.m_copy_constructor != nullptr,                                 \
					   "Can't clone a `StdArray(T, N)` with elements that aren't copyable (no "    \
					   "element copy-constructor defined)");                                       \
                                                                                                   \
			let_mut array = std_array_new_with_collection_data(T, N, self->m_data);                \
                                                                                                   \
			foreach_ref(elem, *self) {                                                             \
				std_array_push_back(                                                               \
					array,                                                                         \
					self->m_data.m_copy_constructor(elem, self->m_data.m_allocator));              \
			}                                                                                      \
                                                                                                   \
			return array;                                                                          \
		}                                                                                          \
                                                                                                   \
		const T* StdArrayIdentifier(T, N, at_const)(const StdArray(T, N)* restrict self,           \
													usize index) {                                 \
			std_assert(index <= self->m_size,                                                      \
					   "std_array_at_mut called with index > size (index out of bounds)");         \
			std_assert(index < N,                                                                  \
					   "std_array_at_mut called with index >= N (index out of bounds)");           \
			return &(self->m_array[index]);                                                        \
		}                                                                                          \
                                                                                                   \
		T* StdArrayIdentifier(T, N, at_mut)(StdArray(T, N)* restrict self, usize index) {          \
			std_assert(index <= self->m_size,                                                      \
					   "std_array_at_mut called with index > size (index out of bounds)");         \
			std_assert(index < N,                                                                  \
					   "std_array_at_mut called with index >= N (index out of bounds)");           \
			return &(self->m_array[index]);                                                        \
		}                                                                                          \
                                                                                                   \
		const T* StdArrayIdentifier(T, N, front_const)(const StdArray(T, N)* restrict self) {      \
			return &std_array_at(*self, 0);                                                        \
		}                                                                                          \
                                                                                                   \
		T* StdArrayIdentifier(T, N, front_mut)(StdArray(T, N)* restrict self) {                    \
			return &std_array_at_mut(*self, 0);                                                    \
		}                                                                                          \
                                                                                                   \
		const T* StdArrayIdentifier(T, N, back_const)(const StdArray(T, N)* restrict self) {       \
			let back_index = self->m_size > 0 ? self->m_size - 1 : 0;                              \
			return &std_array_at(*self, back_index);                                               \
		}                                                                                          \
                                                                                                   \
		T* StdArrayIdentifier(T, N, back_mut)(StdArray(T, N)* restrict self) {                     \
			let back_index = self->m_size > 0 ? self->m_size - 1 : 0;                              \
			return &std_array_at_mut(*self, back_index);                                           \
		}                                                                                          \
                                                                                                   \
		const T* StdArrayIdentifier(T, N, data_const)(const StdArray(T, N)* restrict self) {       \
			return &std_array_front(*self);                                                        \
		}                                                                                          \
                                                                                                   \
		T* StdArrayIdentifier(T, N, data_mut)(StdArray(T, N)* restrict self) {                     \
			return &std_array_front_mut(*self);                                                    \
		}                                                                                          \
                                                                                                   \
		bool StdArrayIdentifier(T, N, is_empty)(const StdArray(T, N)* restrict self) {             \
			return self->m_size == 0;                                                              \
		}                                                                                          \
                                                                                                   \
		bool StdArrayIdentifier(T, N, is_full)(const StdArray(T, N)* restrict self) {              \
			return self->m_size == N;                                                              \
		}                                                                                          \
                                                                                                   \
		usize StdArrayIdentifier(T, N, size)(const StdArray(T, N)* restrict self) {                \
			return self->m_size;                                                                   \
		}                                                                                          \
                                                                                                   \
		usize StdArrayIdentifier(T, N, capacity)(const StdArray(T, N)* restrict self) {            \
			ignore(self);                                                                          \
			return N;                                                                              \
		}                                                                                          \
                                                                                                   \
		void StdArrayIdentifier(T, N, resize_internal)(StdArray(T, N)* restrict self,              \
													   usize new_size) {                           \
			if(new_size < self->m_size) {                                                          \
				for(let_mut i = new_size; i < self->m_size; ++i) {                                 \
					self->m_data.m_destructor(&std_array_at_mut(*self, i),                         \
											  self->m_data.m_allocator);                           \
				}                                                                                  \
				self->m_size = new_size;                                                           \
			}                                                                                      \
			else {                                                                                 \
				let old_size = self->m_size;                                                       \
				self->m_size = new_size;                                                           \
				for(let_mut i = old_size; i < new_size; ++i) {                                     \
					std_array_at_mut(*self, i)                                                     \
						= self->m_data.m_constructor(self->m_data.m_allocator);                    \
				}                                                                                  \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		void StdArrayIdentifier(T, N, resize)(StdArray(T, N)* restrict self, usize new_size) {     \
			std_assert(new_size <= N, "can't resize an array larger than it's capacity");          \
			StdArrayIdentifier(T, N, resize_internal)(self, new_size);                             \
		}                                                                                          \
                                                                                                   \
		void StdArrayIdentifier(T, N, clear)(StdArray(T, N)* restrict self) {                      \
			for(let_mut i = 0U; i < self->m_size; ++i) {                                           \
				self->m_data.m_destructor(&std_array_at_mut(*self, i), self->m_data.m_allocator);  \
			}                                                                                      \
			self->m_size = 0;                                                                      \
		}                                                                                          \
                                                                                                   \
		void StdArrayIdentifier(T, N, push_back)(                                                  \
			StdArray(T, N)* restrict self,                                                         \
			T element /** NOLINT(readability-non-const-parameter) **/) {                           \
			std_assert(self->m_size < N,                                                           \
					   "std_array_push_back called with self->m_size >= N (array bounds full)");   \
			std_array_at_mut(*self, self->m_size) = element;                                       \
			self->m_size++;                                                                        \
		}                                                                                          \
                                                                                                   \
		StdOption(T) StdArrayIdentifier(T, N, pop_back)(StdArray(T, N)* restrict self) {           \
			if(self->m_size == 0) {                                                                \
				return None(T);                                                                    \
			}                                                                                      \
                                                                                                   \
			let_mut ptr = &std_array_at_mut(*self, self->m_size - 1);                              \
			let elem = Some(T, *ptr);                                                              \
			self->m_data.m_destructor(ptr, self->m_data.m_allocator);                              \
			*ptr = (T){0};                                                                         \
			self->m_size--;                                                                        \
			return elem;                                                                           \
		}                                                                                          \
                                                                                                   \
		void StdArrayIdentifier(T, N, insert)(                                                     \
			StdArray(T, N)* restrict self,                                                         \
			T element /** NOLINT(readability-non-const-parameter **/,                              \
			usize index) {                                                                         \
			std_assert(index < N,                                                                  \
					   "std_array_insert called with index >= N (index out of bounds)");           \
			std_assert(self->m_size < N,                                                           \
					   "std_array_insert called with self->m_size >= N (array bounds full)");      \
                                                                                                   \
			if(index != self->m_size) {                                                            \
				let num_to_move = self->m_size - index;                                            \
				std_memmove(T,                                                                     \
							&std_array_at_mut(*self, index + 1),                                   \
							&std_array_at_mut(*self, index),                                       \
							num_to_move);                                                          \
			}                                                                                      \
			std_array_at_mut(*self, index) = element;                                              \
			self->m_size++;                                                                        \
		}                                                                                          \
                                                                                                   \
		void StdArrayIdentifier(T, N, erase)(StdArray(T, N)* restrict self, usize index) {         \
			std_assert(index < self->m_size,                                                       \
					   "std_array_erase called with index >= self->m_size (index out of bounds)"); \
			self->m_data.m_destructor(&std_array_at_mut(*self, index), self->m_data.m_allocator);  \
			if(index != self->m_size - 1) {                                                        \
				let num_to_move = self->m_size - (index + 1);                                      \
				std_memmove(T,                                                                     \
							&std_array_at_mut(*self, index),                                       \
							&std_array_at_mut(*self, index + 1),                                   \
							num_to_move);                                                          \
			}                                                                                      \
			self->m_size--;                                                                        \
		}                                                                                          \
                                                                                                   \
		void StdArrayIdentifier(T, N, erase_n)(StdArray(T, N)* restrict self,                      \
											   usize index,                                        \
											   usize length) {                                     \
			std_assert(                                                                            \
				index < self->m_size,                                                              \
				"std_array_erase_n called with index >= self->m_size (index out of bounds)");      \
			std_assert(index + length < self->m_size,                                              \
					   "std_array_erase_n called with index + length >= self->m_size (range out "  \
					   "of bounds)");                                                              \
                                                                                                   \
			let end = index + length;                                                              \
			let num_to_move = self->m_size - end;                                                  \
			for(let_mut i = index; i < end; ++i) {                                                 \
				self->m_data.m_destructor(&std_array_at_mut(*self, i), self->m_data.m_allocator);  \
			}                                                                                      \
			if(end != self->m_size) {                                                              \
				std_memmove(T,                                                                     \
							&std_array_at_mut(*self, index),                                       \
							&std_array_at_mut(*self, end),                                         \
							num_to_move);                                                          \
			}                                                                                      \
			self->m_size -= length;                                                                \
		}                                                                                          \
                                                                                                   \
		void StdArrayIdentifier(T, N, free)(void* restrict self) {                                 \
			let self_ = static_cast(StdArray(T, N)*)(self);                                        \
			for(let_mut i = 0U; i < self_->m_size; ++i) {                                          \
				self_->m_data.m_destructor(&std_array_at_mut(*self_, i),                           \
										   self_->m_data.m_allocator);                             \
			}                                                                                      \
			self_->m_size = 0;                                                                     \
		}                                                                                          \
                                                                                                   \
		StdArrayIterator(T, N)                                                                     \
			StdArrayIdentifier(T, N, iterator_new)(const StdArray(T, N)* restrict self) {          \
			return (StdArrayIterator(T, N)){.m_index = 0U,                                         \
											.m_array = const_cast(StdArray(T, N)*)(self)};         \
		}                                                                                          \
                                                                                                   \
		StdArrayConstIterator(T, N)                                                                \
			StdArrayIdentifier(T, N, const_iterator_new)(const StdArray(T, N)* restrict self) {    \
			return (StdArrayConstIterator(T, N)){.m_index = 0U,                                    \
												 .m_array = const_cast(StdArray(T, N)*)(self)};    \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdArrayIdentifier(T, N, iterator_next)(                                            \
			StdRandomAccessIterator(Ref(T))* restrict self) {                                      \
			let _self = static_cast(StdArrayIterator(T, N)*)(self->m_self);                        \
			std_assert(                                                                            \
				_self->m_index > -1,                                                               \
				"Iterator value accessed when iterator is positioned before the beginning of "     \
				"the iteration (iterator out of bounds)");                                         \
			std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),     \
					   "Iterator value accessed when iterator is positioned after the end of "     \
					   "the iteration (iterator out of bounds)");                                  \
			_self->m_index++;                                                                      \
			if(static_cast(usize)(_self->m_index) >= std_array_size(*(_self->m_array))) {          \
				return &std_array_at_mut(*(_self->m_array),                                        \
										 std_array_size(*(_self->m_array)) - 1);                   \
			}                                                                                      \
                                                                                                   \
			return &std_array_at_mut(*(_self->m_array), static_cast(usize)(_self->m_index));       \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdArrayIdentifier(T, N, iterator_previous)(                                        \
			StdRandomAccessIterator(Ref(T))* restrict self) {                                      \
			let _self = static_cast(StdArrayIterator(T, N)*)(self->m_self);                        \
			std_assert(                                                                            \
				_self->m_index > -1,                                                               \
				"Iterator value accessed when iterator is positioned before the beginning of "     \
				"the iteration (iterator out of bounds)");                                         \
			std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),     \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			_self->m_index--;                                                                      \
			if(_self->m_index < 0) {                                                               \
				return &std_array_at_mut(*(_self->m_array), 0);                                    \
			}                                                                                      \
                                                                                                   \
			return &std_array_at_mut(*(_self->m_array), static_cast(usize)(_self->m_index));       \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdArrayIdentifier(T, N, iterator_at)(                                              \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			usize index) {                                                                         \
			let _self = static_cast(StdArrayIterator(T, N)*)(self->m_self);                        \
			std_assert(index < std_array_size(*(_self->m_array)),                                  \
					   "Iterator value accessed when iterator is positioned after the end of "     \
					   "the iteration (iterator out of bounds)");                                  \
			return &std_array_at_mut(*(_self->m_array), index);                                    \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdArrayIdentifier(T, N, iterator_rat)(                                             \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			usize index) {                                                                         \
			let _self = static_cast(StdArrayIterator(T, N)*)(self->m_self);                        \
			std_assert(index < std_array_size(*(_self->m_array)),                                  \
					   "Iterator value accessed when iterator is positioned "                      \
					   "after the end of the iteration (iterator out of bounds)");                 \
			return &std_array_at_mut(*(_self->m_array),                                            \
									 (std_array_size(*(_self->m_array)) - 1) - index);             \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdArrayIdentifier(T, N, iterator_current)(                                         \
			const StdRandomAccessIterator(Ref(T))* restrict self) {                                \
			let _self = static_cast(StdArrayIterator(T, N)*)(self->m_self);                        \
			std_assert(_self->m_index > -1,                                                        \
					   "Iterator value accessed when iterator is positioned before the "           \
					   "beginning of the iteration (iterator out of bounds)");                     \
			std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),     \
					   "Iterator value accessed when iterator is positioned after the "            \
					   "end of the iteration (iterator out of bounds)");                           \
			return &std_array_at_mut(*(_self->m_array), static_cast(usize)(_self->m_index));       \
		}                                                                                          \
                                                                                                   \
		bool StdArrayIdentifier(T, N, iterator_equals)(                                            \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			const StdRandomAccessIterator(Ref(T))* restrict rhs) {                                 \
			let _self = static_cast(const StdArrayIterator(T, N)*)(self->m_self);                  \
			let _rhs = static_cast(const StdArrayIterator(T, N)*)(rhs->m_self);                    \
			return _self->m_index == _rhs->m_index && _self->m_array == _rhs->m_array;             \
		}                                                                                          \
                                                                                                   \
		ConstRef(T) StdArrayIdentifier(T, N, iterator_cnext)(                                      \
			StdRandomAccessIterator(ConstRef(T))* restrict self) {                                 \
			let _self = static_cast(StdArrayConstIterator(T, N)*)(self->m_self);                   \
                                                                                                   \
			std_assert(_self->m_index > -1,                                                        \
					   "Iterator value accessed when iterator is positioned before the "           \
					   "beginning of the iteration (iterator out of bounds)");                     \
			std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),     \
					   "Iterator value accessed when iterator is positioned after the "            \
					   "end of the iteration (iterator out of bounds)");                           \
			_self->m_index++;                                                                      \
			if(static_cast(usize)(_self->m_index) >= std_array_size(*(_self->m_array))) {          \
				return &std_array_at(*(_self->m_array), std_array_size(*(_self->m_array)) - 1);    \
			}                                                                                      \
                                                                                                   \
			return &std_array_at(*(_self->m_array), static_cast(usize)(_self->m_index));           \
		}                                                                                          \
                                                                                                   \
		ConstRef(T) StdArrayIdentifier(T, N, iterator_cprevious)(                                  \
			StdRandomAccessIterator(ConstRef(T))* restrict self) {                                 \
			let _self = static_cast(StdArrayConstIterator(T, N)*)(self->m_self);                   \
			std_assert(_self->m_index > -1,                                                        \
					   "Iterator value accessed when iterator is positioned before "               \
					   "the beginning of the iteration (iterator out of bounds)");                 \
			std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),     \
					   "Iterator value accessed when iterator is positioned after "                \
					   "the end of the iteration (iterator out of bounds)");                       \
			_self->m_index--;                                                                      \
			if(_self->m_index < 0) {                                                               \
				return &std_array_at(*(_self->m_array), 0);                                        \
			}                                                                                      \
                                                                                                   \
			return &std_array_at(*(_self->m_array), static_cast(usize)(_self->m_index));           \
		}                                                                                          \
                                                                                                   \
		ConstRef(T) StdArrayIdentifier(T, N, iterator_cat)(                                        \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			usize index) {                                                                         \
			let _self = static_cast(StdArrayConstIterator(T, N)*)(self->m_self);                   \
			std_assert(index < std_array_size(*(_self->m_array)),                                  \
					   "Iterator value accessed when iterator is positioned "                      \
					   "after the end of the iteration (iterator out of bounds)");                 \
                                                                                                   \
			return &std_array_at(*(_self->m_array), index);                                        \
		}                                                                                          \
                                                                                                   \
		ConstRef(T) StdArrayIdentifier(T, N, iterator_crat)(                                       \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			usize index) {                                                                         \
			let _self = static_cast(StdArrayConstIterator(T, N)*)(self->m_self);                   \
			std_assert(index < std_array_size(*(_self->m_array)),                                  \
					   "Iterator value accessed when iterator is positioned "                      \
					   "after the end of the iteration (iterator out of bounds)");                 \
                                                                                                   \
			return &std_array_at(*(_self->m_array),                                                \
								 (std_array_size(*(_self->m_array)) - 1) - index);                 \
		}                                                                                          \
                                                                                                   \
		ConstRef(T) StdArrayIdentifier(T, N, iterator_ccurrent)(                                   \
			const StdRandomAccessIterator(ConstRef(T))* restrict self) {                           \
			let _self = static_cast(StdArrayConstIterator(T, N)*)(self->m_self);                   \
			std_assert(_self->m_index > -1,                                                        \
					   "Iterator value accessed when iterator is positioned before "               \
					   "the beginning of the iteration (iterator out of bounds)");                 \
			std_assert(static_cast(usize)(_self->m_index) < std_array_size(*(_self->m_array)),     \
					   "Iterator value accessed when iterator is positioned after "                \
					   "the end of the iteration (iterator out of bounds)");                       \
                                                                                                   \
			return &std_array_at(*(_self->m_array), static_cast(usize)(_self->m_index));           \
		}                                                                                          \
                                                                                                   \
		bool StdArrayIdentifier(T, N, iterator_cequals)(                                           \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			const StdRandomAccessIterator(ConstRef(T))* restrict rhs) {                            \
			let _self = static_cast(const StdArrayConstIterator(T, N)*)(self->m_self);             \
			let _rhs = static_cast(const StdArrayConstIterator(T, N)*)(rhs->m_self);               \
			return _self->m_index == _rhs->m_index && _self->m_array == _rhs->m_array;             \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(Ref(T))                                                            \
			StdArrayIdentifier(T, N, begin)(StdArray(T, N)* restrict self) {                       \
			let_mut iter = std_array_into_iter(*self);                                             \
			let_mut inner = static_cast(StdArrayIterator(T, N)*)(iter.m_self);                     \
			inner->m_index = 0;                                                                    \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(Ref(T))                                                            \
			StdArrayIdentifier(T, N, end)(StdArray(T, N)* restrict self) {                         \
			let_mut iter = std_array_into_iter(*self);                                             \
			let_mut inner = static_cast(StdArrayIterator(T, N)*)(iter.m_self);                     \
			inner->m_index = static_cast(isize)(std_array_size(*self));                            \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(Ref(T))                                                            \
			StdArrayIdentifier(T, N, rbegin)(StdArray(T, N)* restrict self) {                      \
			let_mut iter = std_array_into_reverse_iter(*self);                                     \
			let_mut inner = static_cast(StdArrayIterator(T, N)*)(iter.m_self);                     \
			inner->m_index = static_cast(isize)(std_array_size(*self) - 1);                        \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(Ref(T))                                                            \
			StdArrayIdentifier(T, N, rend)(StdArray(T, N)* restrict self) {                        \
			let_mut iter = std_array_into_reverse_iter(*self);                                     \
			let_mut inner = static_cast(StdArrayIterator(T, N)*)(iter.m_self);                     \
			inner->m_index = -1;                                                                   \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdArrayIdentifier(T, N, cbegin)(const StdArray(T, N)* restrict self) {                \
			let_mut iter = std_array_into_const_iter(*self);                                       \
			let_mut inner = static_cast(StdArrayConstIterator(T, N)*)(iter.m_self);                \
			inner->m_index = 0;                                                                    \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdArrayIdentifier(T, N, cend)(const StdArray(T, N)* restrict self) {                  \
			let_mut iter = std_array_into_const_iter(*self);                                       \
			let_mut inner = static_cast(StdArrayConstIterator(T, N)*)(iter.m_self);                \
			inner->m_index = static_cast(isize)(std_array_size(*self));                            \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdArrayIdentifier(T, N, crbegin)(const StdArray(T, N)* restrict self) {               \
			let_mut iter = std_array_into_reverse_const_iter(*self);                               \
			let_mut inner = static_cast(StdArrayConstIterator(T, N)*)(iter.m_self);                \
			inner->m_index = static_cast(isize)(std_array_size(*self) - 1);                        \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdArrayIdentifier(T, N, crend)(const StdArray(T, N)* restrict self) {                 \
			let_mut iter = std_array_into_reverse_const_iter(*self);                               \
			let_mut inner = static_cast(StdArrayConstIterator(T, N)*)(iter.m_self);                \
			inner->m_index = -1;                                                                   \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdString StdArrayIdentifier(T, N, format)(const StdFormat* restrict self,                 \
												   StdFormatSpecifier specifier) {                 \
			return StdArrayIdentifier(T, N, format_with_allocator)(self,                           \
																   specifier,                      \
																   std_allocator_new());           \
		}                                                                                          \
                                                                                                   \
		StdString StdArrayIdentifier(T, N, format_with_allocator)(                                 \
			const StdFormat* restrict self,                                                        \
			StdFormatSpecifier maybe_unused specifier,                                             \
			StdAllocator allocator) {                                                              \
			std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT,                                \
					   "Can't format StdArray with custom specifier");                             \
			let _self = *static_cast(const StdArray(T, N)*)(self->m_self);                         \
			let size = std_array_size(_self);                                                      \
			let capacity = std_array_capacity(_self);                                              \
			let data = std_array_data(_self);                                                      \
			return std_format_with_allocator(                                                      \
				AS_STRING(StdArray(T, N)) ": [size: {d}, capacity: {d}, data pointer: {x}]",       \
				allocator,                                                                         \
				size,                                                                              \
				capacity,                                                                          \
				as_format_t(nullptr_t, data));                                                     \
		}

#endif // STD_ARRAY
