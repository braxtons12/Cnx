/// @file StdVector.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a dynamic-array type comparable to C++'s `std::vector` and Rust's
/// `std::vec::Vec` for C2nxt
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

/// @ingroup collections
/// @{
/// @defgroup std_vector StdVector
/// `StdVector(T)` is a struct template for a type-safe dynamic-capacity array. It's
/// allocator aware, provides C2nxt compatible random access iterators, and supports
/// user-defined default constructors, copy-constructors, and destructors for its elements.
/// It's design is based around both C++'s `std::vector` and Rust's `std::vec::Vec`, but with
/// slightly expanded functionality and semantics.
///
/// `StdVector(T)` has a small size optimization (SSO) that is user configurable. This allows for a
/// compile-time configurable number of elements to be stored on the stack, before resorting to heap
/// allocation, at the cost of `sizeof(StdVector(T))` being larger than otherwise necessary. By
/// default, this optimization allows for up to 8 elements to be stored on the stack. This number
/// can be changed by defining `STD_VECTOR_SHORT_OPTIMIZATION_NUM_ELEMENTS` to the desired number of
/// elements. If the small size optimization is disabled (by defining
/// `STD_VECTOR_SHORT_OPTIMIZATION_NUM_ELEMENTS` to 0), the default heap allocation size can be
/// configured by defining `STD_VECTOR_DEFAULT_LONG_CAPACITY` to the desired number of elements. The
/// default for this setting is 16.
///
/// Instantiations of `StdVector(T)` are already provided for builtin types like `i32` and some
/// C2nxt types, like `StdString`. Instantiating it for your own types is simple and requires
/// minimal boiler-plate.
///
/// Instantiation requirements:
/// 1. a `typedef` of your type to provide and alphanumeric name for it. (for template and macro
/// 	parameters)
/// 2. a `typedef` for pointer to your type as `Ref(YourType)`, for use with the iterators.
/// 3. a `typedef` for pointer to const your type as `ConstRef(YourType)`, for use with the
/// 	iterators
/// 4. Instantiations for C2nxt iterators for the typedefs provided in (2) and (3)
/// 5. Instantiation of `StdOption(T)` for your type
///
/// We recommend two routes for providing instantiations for user-defined types, either:
/// 1. Provide the instantiation together with your type's public interface and implementation OR
/// 2. Provide it as a separate "template instantiation" .h/.c file pair
///
/// Example of (1).
///
/// @code {.c}
/// // in `YourType.h`
/// #include "C2nxt/StdVector.h"
/// typedef struct YourType {
///	 // members
/// } YourType;
///
/// typedef YourType* Ref(YourType);
/// typedef const YourType* ConstRef(YourType);
///
/// DeclStdIterators(Ref(YourType));
/// DeclStdIterators(ConstRef(YourType));
///
/// DeclStdOption(YourType);
///
/// DeclStdVector(YourType);
///
/// // the rest of your public interface...
///
/// // in `YourType.c`
/// ImplStdOption(YourType);
/// ImplStdVector(YourType);
///
/// // the rest of your implementation
/// @endcode
///
/// Example of (2):
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
/// // in `StdVectorYourType.h`
/// #include "YourType.h"
/// #include "StdOptionYourType.h"
/// DeclStdVector(YourType);
///
/// // in `StdVectorYourType.c`
/// #include "StdVectorYourType.c"
/// ImplStdVector(YourType);
/// @endcode
///
/// Then, somewhere else in your codebase, you can easily use the instantiation:
///
/// @code {.c}
/// #include "YourType.h"
/// // if instantiation method (2)
/// #include "StdVectorYourType.h"
///
/// StdVector(YourType) create_and_fill_your_type_vec(usize num_elements) {
///		let_mut vec = std_vector_new_with_capacity(YourType, num_elements);
///
///		std_vector_resize(vec, num_elements);
///
///		return vec;
/// }
/// @endcode
///
/// `StdVector(T)` implements `StdFormat`, but because of its generic type, can't be automatically
/// converted to its `StdFormat` implementation, so we have to cast it explicitly:
///
/// @code {.c}
/// #include "C2nxt/StdIO.h"
/// void print_vec(const StdVector(YourType)* vec) {
///		println("{}", as_format_t(StdVector(YourType), *vec));
/// }
/// @endcode
///
/// You can provide user-defined default-constructor, copy-constructor, and/or destructor for
/// elements of your type, and custom allocator for all memory allocation:
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
/// 	return t;
/// }
///
/// void your_type_destructor(YourType* restrict t, StdAllocator allocator) {
///		// whatever you need to do to cleanup `t`
/// }
///
/// StdVector(YourType) create_and_fill_your_type_vec(usize num_elements) {
///		// init a vec with num_elements capacity (zero size) and our custom collection data
///		let_mut vec = std_vector_new_with_capacity_with_collection_data(YourType,
/// 						capacity,
///							// This `StdCollectionData(CollectionType)` will only be
/// 						// associated with this specific vector, not all `StdVector(YourType)`s
///							((StdCollectionData(StdVector(YourType))) {
///								// can be left NULL, will be defaulted
///								.m_constructor = your_type_constructor,
///								// can be left NULL, will disable `std_vector_clone(self)` for
/// 							// this vector
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
///		// fill the vec with default constructed elements
///		std_vector_resize(vec, num_elements);
///
///		return vec;
/// }
/// @endcode
///
/// `StdVector(T)` also provides C2nxt random access iterators and optional scoped destruction:
///
/// @code {.c}
/// void example(void) {
/// 	let_mut std_vector_scoped(u32) vec = std_vector_new_with_capacity(u32, 10);
/// 	ranged_for(i, 0, 9) {
///			std_vector_push_back(vec, i);
/// 	}
///
///		// loop over the elements in the vector by value, as an iteration
///		foreach(elem, vec) {
///			println("{}", elem);
/// 	}
///		// vec goes out of scope here and `std_vector_free(vec)` is called on it
/// 	// because we declared it as scoped
/// }
/// @endcode
///
/// Like other C2nxt collections, `StdVector(T)` provides its type-agnostic usage through a vtable
/// pointer contained in the struct, and provides macros which wrap the usage of the vtable, making
/// access simpler. If you prefer to not use this method of access, you can call the
/// typed functions directly by infixing the contained type in the associated function name.
/// IE: for `StdVector(i32)`, `vec`, the equivalent function call for
/// `std_vector_push_back(vec, element)` would be `std_vector_i32_push_back(&vec, element)`
/// @}

#include "StdAllocators.h"
#include "StdBasicTypes.h"
#include "StdCollectionsData.h"
#include "StdFormat.h"
#include "StdIterator.h"
#include "StdMath.h"
#include "StdOption.h"
#include "StdPlatform.h"
#include "StdResult.h"

#ifndef STD_VECTOR
	/// @brief `StdVector(T)` related declarations and definitions
	#define STD_VECTOR

	/// @brief macro alias for a `StdVector(T)` containing `T`s
	///
	/// `StdVector(T)` is a bounds-safe, allocator aware, generic dynamic-capacity array type. It is
	/// implemented as a struct template, which enables 100% type safety, while providing
	/// abstractions that allow type agnostic use. It has greatly increased ergnomics over manual
	/// array management and provides many useful features such as random access iterators and
	/// optional scoped destruction.
	///
	/// Example:
	/// @code {.c}
	/// #include "C2nxt/StdVector.h"
	/// #include "C2nxt/StdIO.h"
	///
	/// // create a `StdVector(i32)` with default allocator, element default-constructor, and
	/// element
	/// // destructor
	/// let_mut vec = std_vector_new(i32);
	///
	/// // append 10 elements to the vector
	/// ranged_for(i, 0, 9) {
	/// 	std_vector_push_back(vec, i);
	/// }
	///
	/// // prints information about `vec` to stdout
	/// // because `StdVector(T)` is generic, and thus can't be used in a `_Generic` match arm prior
	/// to
	/// // the type's instantiation, we have to explictly cast to the `StdFormat` Trait to get
	/// // `StdVector(T)`'s implementation of the Trait
	/// println("{}", std_as_format_t(StdVector(i32), vec));
	/// // prints `vec`'s elements to stdout
	/// foreach(elem, vec) {
	///		println("{}", elem);
	/// }
	///
	/// {
	/// 	let_mut std_vector_scoped(i32) vec2 = std_vector_new(i32);
	/// 	foreach(elem, vec) {
	///			std_vector_push_back(vec2, elem);
	/// 	}
	/// }
	/// // `vec2` is freed here
	/// @endcode
	///
	/// Like other C2nxt collections, `StdVector(T)` provides its type-agnostic usage through a
	/// vtable pointer contained in the struct, and provides macros which wrap the usage of the
	/// vtable, making access simpler. If you prefer to not use this method of access, you can call
	/// the typed functions directly by infixing the contained type in the associated function name.
	/// IE: for `StdVector(i32)`, `vec`, the equivalent function call for
	/// `std_vector_push_back(vec, element)` would be `std_vector_i32_push_back(&vec, element)`
	/// @ingroup std_vector
	#define StdVector(T) CONCAT2(StdVector, T)
	/// @brief macro alias for the concrete type for an iterator into the mutable iteration of a
	/// `StdVector(T)` containing `T`s
	#define StdVectorIterator(T) CONCAT2(StdVector(T), Iterator)
	/// @brief macro alias for the concrete type for an iterator into the const iteration of a
	/// `StdVector(T)` containing `T`s
	#define StdVectorConstIterator(T) CONCAT2(StdVector(T), ConstIterator)
	/// @brief macro alias for an identifier (type, function, etc) associated with a
	/// `StdVector(T)` containing `T`s
	#define StdVectorIdentifier(T, Identifier) CONCAT3(std_vector_, T, CONCAT2(_, Identifier))

	#ifndef STD_VECTOR_SHORT_OPTIMIZATION_NUM_ELEMENTS
		/// @brief The maximum number of elements storable using `StdVector(T)`'s small size
		/// optimization. Define this to a different value to change or disable the small size
		/// optimizatio
		/// @ingroup std_vector
		#define STD_VECTOR_SHORT_OPTIMIZATION_NUM_ELEMENTS 8U
	#endif // STD_VECTOR_SHORT_OPTIMIZATION_NUM_ELEMENTS

	/// @brief macro alias for the maximum number of elements capable of being stored using
	/// `StdVector(T)`'s small size optimization
	#define STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T) STD_VECTOR_SHORT_OPTIMIZATION_NUM_ELEMENTS

	#ifndef STD_VECTOR_DEFAULT_LONG_CAPACITY
		/// @brief The default memory allocation size for `StdVector(T)` if short optimization is
		/// disabled
		/// @ingroup std_vector
		#define STD_VECTOR_DEFAULT_LONG_CAPACITY 16U
	#endif // STD_VECTOR_DEFAULT_LONG_CAPACITY

	/// @brief Use this macro to instantatiate the declarations for `StdVector(T)` containing `T`s.
	/// This provides the struct declarations and definitions and function declarations for
	/// `StdVector(T)`.
	///
	/// Requires that the following have been met prior to use:
	/// 1. a `typedef` of your type to provide an alphanumeric name for it
	/// 2. a `typedef` for pointer to your type as `Ref(YourType)`
	/// 3. a `typedef` for pointer to const your type as `ConstRef(YourType)`
	/// 4. Instantiations for C2nxt iterators for the typedefs provided in (2) and (3)
	/// 5. Instantation of `StdOption(YourType)`, via provided macros \see `StdOption`
	///
	/// @param T - The element type of the `StdVector(T)` instantiation
	/// @ingroup std_vector
	#define DeclStdVector(T)                                                                       \
		DeclStdCollectionData(T, StdVector(T));                                                    \
		typedef struct StdVectorIdentifier(T, vtable) StdVectorIdentifier(T, vtable);              \
		typedef struct StdVector(T) {                                                              \
			union {                                                                                \
				T m_short[STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T)];                              \
				T* m_long;                                                                         \
			};                                                                                     \
			usize m_size;                                                                          \
			usize m_capacity;                                                                      \
			StdCollectionData(StdVector(T)) m_data;                                                \
			const StdVectorIdentifier(T, vtable) * m_vtable;                                       \
		}                                                                                          \
		StdVector(T);                                                                              \
                                                                                                   \
		typedef struct StdVectorIterator(T) {                                                      \
			isize m_index;                                                                         \
			StdVector(T) * m_vector;                                                               \
		}                                                                                          \
		StdVectorIterator(T);                                                                      \
                                                                                                   \
		typedef struct StdVectorConstIterator(T) {                                                 \
			isize m_index;                                                                         \
			const StdVector(T) * m_vector;                                                         \
		}                                                                                          \
		StdVectorConstIterator(T);                                                                 \
                                                                                                   \
		DeclStdOption(StdVector(T));                                                               \
		DeclStdResult(StdVector(T));                                                               \
                                                                                                   \
		StdVector(T) StdVectorIdentifier(T, new)(void);                                            \
		StdVector(T) StdVectorIdentifier(T, new_with_collection_data)(                             \
			StdCollectionData(StdVector(T)) data);                                                 \
		StdVector(T) StdVectorIdentifier(T, new_with_capacity)(usize capacity);                    \
		StdVector(T) StdVectorIdentifier(T, new_with_capacity_with_collection_data)(               \
			usize capacity,                                                                        \
			StdCollectionData(StdVector(T)) data);                                                 \
		StdVector(T) StdVectorIdentifier(T, clone)(const StdVector(T)* restrict self)              \
			std_disable_if(!(self->m_data.m_copy_constructor),                                     \
						   "Can't clone a StdVector(T) with elements that aren't copyable (no "    \
						   "element copy constructor defined)");                                   \
		const T* StdVectorIdentifier(T, at_const)(const StdVector(T)* restrict self, usize index); \
		T* StdVectorIdentifier(T, at_mut)(StdVector(T)* restrict self, usize index);               \
		const T* StdVectorIdentifier(T, front_const)(const StdVector(T)* restrict self);           \
		T* StdVectorIdentifier(T, front_mut)(StdVector(T)* restrict self);                         \
		const T* StdVectorIdentifier(T, back_const)(const StdVector(T)* restrict self);            \
		T* StdVectorIdentifier(T, back_mut)(StdVector(T)* restrict self);                          \
		const T* StdVectorIdentifier(T, data_const)(const StdVector(T)* restrict self);            \
		T* StdVectorIdentifier(T, data_mut)(StdVector(T)* restrict self);                          \
		bool StdVectorIdentifier(T, is_empty)(const StdVector(T)* restrict self);                  \
		bool StdVectorIdentifier(T, is_full)(const StdVector(T)* restrict self);                   \
		usize StdVectorIdentifier(T, size)(const StdVector(T)* restrict self);                     \
		usize StdVectorIdentifier(T, max_size)(void);                                              \
		usize StdVectorIdentifier(T, capacity)(const StdVector(T)* restrict self);                 \
		void StdVectorIdentifier(T, reserve)(StdVector(T)* restrict self, usize new_capacity);     \
		void StdVectorIdentifier(T, resize)(StdVector(T)* restrict self, usize new_size);          \
		void StdVectorIdentifier(T, shrink_to_fit)(StdVector(T)* restrict self);                   \
		void StdVectorIdentifier(T, clear)(StdVector(T)* restrict self);                           \
		void StdVectorIdentifier(T, push_back)(StdVector(T)* restrict self, T element);            \
		StdOption(T) StdVectorIdentifier(T, pop_back)(StdVector(T)* restrict self);                \
		void StdVectorIdentifier(T, insert)(StdVector(T)* restrict self, T element, usize index);  \
		void StdVectorIdentifier(T, erase)(StdVector(T)* restrict self, usize index);              \
		void StdVectorIdentifier(T, erase_n)(StdVector(T)* restrict self,                          \
											 usize index,                                          \
											 usize num_elements);                                  \
		void StdVectorIdentifier(T, free)(void* restrict self);                                    \
		StdString StdVectorIdentifier(T, format)(const StdFormat* restrict self,                   \
												 StdFormatSpecifier specifier);                    \
		StdString StdVectorIdentifier(T, format_with_allocator)(const StdFormat* restrict self,    \
																StdFormatSpecifier specifier,      \
																StdAllocator allocator);           \
                                                                                                   \
		static maybe_unused ImplTraitFor(StdFormat,                                                \
										 StdVector(T),                                             \
										 StdVectorIdentifier(T, format),                           \
										 StdVectorIdentifier(T, format_with_allocator));           \
                                                                                                   \
		DeclIntoStdRandomAccessIterator(StdVector(T),                                              \
										Ref(T),                                                    \
										StdVectorIdentifier(T, into_iter),                         \
										into);                                                     \
		DeclIntoStdRandomAccessIterator(StdVector(T),                                              \
										Ref(T),                                                    \
										StdVectorIdentifier(T, into_reverse_iter),                 \
										into_reverse);                                             \
		DeclIntoStdRandomAccessIterator(StdVector(T),                                              \
										ConstRef(T),                                               \
										StdVectorIdentifier(T, into_const_iter),                   \
										into);                                                     \
		DeclIntoStdRandomAccessIterator(StdVector(T),                                              \
										ConstRef(T),                                               \
										StdVectorIdentifier(T, into_reverse_const_iter),           \
										into_reverse);                                             \
                                                                                                   \
		StdRandomAccessIterator(Ref(T))                                                            \
			CONCAT3(std_vector_, T, _begin)(StdVector(T)* restrict self);                          \
		StdRandomAccessIterator(Ref(T))                                                            \
			CONCAT3(std_vector_, T, _end)(StdVector(T)* restrict self);                            \
		StdRandomAccessIterator(Ref(T))                                                            \
			CONCAT3(std_vector_, T, _rbegin)(StdVector(T)* restrict self);                         \
		StdRandomAccessIterator(Ref(T))                                                            \
			CONCAT3(std_vector_, T, _rend)(StdVector(T)* restrict self);                           \
                                                                                                   \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdVectorIdentifier(T, cbegin)(const StdVector(T)* restrict self);                     \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdVectorIdentifier(T, cend)(const StdVector(T)* restrict self);                       \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdVectorIdentifier(T, crbegin)(const StdVector(T)* restrict self);                    \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdVectorIdentifier(T, crend)(const StdVector(T)* restrict self);                      \
                                                                                                   \
		typedef struct StdVectorIdentifier(T, vtable) {                                            \
			StdVector(T) (*const clone)(const StdVector(T)* restrict self);                        \
			const T* (*const at_const)(const StdVector(T)* restrict self, usize index);            \
			T* (*const at_mut)(StdVector(T)* restrict self, usize index);                          \
			const T* (*const front_const)(const StdVector(T)* restrict self);                      \
			T* (*const front_mut)(StdVector(T)* restrict self);                                    \
			const T* (*const back_const)(const StdVector(T)* restrict self);                       \
			T* (*const back_mut)(StdVector(T)* restrict self);                                     \
			const T* (*const data_const)(const StdVector(T)* restrict self);                       \
			T* (*const data_mut)(StdVector(T)* restrict self);                                     \
			bool (*const is_empty)(const StdVector(T)* restrict self);                             \
			bool (*const is_full)(const StdVector(T)* restrict self);                              \
			usize (*const size)(const StdVector(T)* restrict self);                                \
			usize (*const capacity)(const StdVector(T)* restrict self);                            \
			void (*const reserve)(StdVector(T)* restrict self, usize new_capacity);                \
			void (*const resize)(StdVector(T)* restrict self, usize new_size);                     \
			void (*const shrink_to_fit)(StdVector(T)* restrict self);                              \
			void (*const clear)(StdVector(T)* restrict self);                                      \
			void (*const push_back)(StdVector(T)* restrict self, T element);                       \
			StdOption(T) (*const pop_back)(StdVector(T)* restrict self);                           \
			void (*const insert)(StdVector(T)* restrict self, T element, usize index);             \
			void (*const erase)(StdVector(T)* restrict self, usize index);                         \
			void (*const erase_n)(StdVector(T)* restrict self, usize index, usize num_elements);   \
			void (*const free)(void* restrict self);                                               \
			StdRandomAccessIterator(Ref(T)) (*const into_iter)(const StdVector(T)* restrict self); \
			StdRandomAccessIterator(Ref(T)) (*const into_reverse_iter)(                            \
				const StdVector(T)* restrict self);                                                \
			StdRandomAccessIterator(ConstRef(T)) (*const into_const_iter)(                         \
				const StdVector(T)* restrict self);                                                \
			StdRandomAccessIterator(ConstRef(T)) (*const into_reverse_const_iter)(                 \
				const StdVector(T)* restrict self);                                                \
			StdRandomAccessIterator(Ref(T)) (*const begin)(StdVector(T)* restrict self);           \
			StdRandomAccessIterator(Ref(T)) (*const end)(StdVector(T)* restrict self);             \
			StdRandomAccessIterator(Ref(T)) (*const rbegin)(StdVector(T)* restrict self);          \
			StdRandomAccessIterator(Ref(T)) (*const rend)(StdVector(T)* restrict self);            \
			StdRandomAccessIterator(ConstRef(T)) (*const cbegin)(                                  \
				const StdVector(T)* restrict self);                                                \
			StdRandomAccessIterator(ConstRef(T)) (*const cend)(const StdVector(T)* restrict self); \
			StdRandomAccessIterator(ConstRef(T)) (*const crbegin)(                                 \
				const StdVector(T)* restrict self);                                                \
			StdRandomAccessIterator(ConstRef(T)) (*const crend)(                                   \
				const StdVector(T)* restrict self);                                                \
		}                                                                                          \
		StdVectorIdentifier(T, vtable);

	/// @brief Creates a new `StdVector(T)` with defaulted associated functions and initial
	/// capacity.
	///
	/// Creates a new `StdVector(T)` with:
	/// 1. defaulted initial capacity
	/// 2. defaulted associated element default-constructor
	/// 3. defaulted associated element copy-constructor
	/// 4. defaulted associated element destructor
	/// 5. defaulted associated memory allocator
	///
	/// @param T - The element type of the `StdVector(T)` instantiation to create
	///
	/// @return a new `StdVector(T)`
	/// @ingroup std_vector
	#define std_vector_new(T) StdVectorIdentifier(T, new)()
	/// @brief Creates a new `StdVector(T)` with defaulted capacity and provided associated
	/// functions.
	///
	/// Creates a new `StdVector(T)` with:
	/// 1. defaulted initial capacity
	/// 2. possibly user-provided element default-constructor
	/// 3. possibly user-provided associated element copy-constructor
	/// 4. possibly user-provided element destructor
	/// 5. user-provided memory allocator
	///
	/// @param T - The element type of the `StdVector(T)` instantiation to create
	/// @param collection_data - The `StdCollectionData(CollectionType)` containing the element
	/// default-constructor, element copy-constructor, element destructor, and memory allocator to
	/// use
	///
	/// @return a new `StdVector(T)`
	/// @ingroup std_vector
	#define std_vector_new_with_collection_data(T, collection_data) \
		StdVectorIdentifier(T, new_with_collection_data)(collection_data)
	/// @brief Creates a new `StdVector(T)` with __at least__ the given capacity and defaulted
	/// associated functions.
	///
	/// Creates a new `StdVector(T)` with:
	/// 1. given initial capacity
	/// 2. defaulted associated element default-constructor
	/// 3. defaulted associated element copy-constructor
	/// 4. defaulted associated element destructor
	/// 5. defaulted memory allocator
	///
	/// @param T - The element type of the `StdVector(T)` instantiation to create
	/// @param capacity - The initial capacity of the vector
	///
	/// @return a new `StdVector(T)`
	/// @ingroup std_vector
	#define std_vector_new_with_capacity(T, capacity) \
		StdVectorIdentifier(T, new_with_capacity)(capacity)
	/// @brief Creates a new `StdVector(T)` with __at least__ the given capacity and provided
	/// associated functions.
	///
	/// Creates a new `StdVector(T)` with:
	/// 1. given initial capacity
	/// 2. possibly user-provided associated element default-constructor
	/// 3. possibly user-provided associated element copy-constructor
	/// 4. possibly user-provided associated element destructor
	/// 5. user-provided memory allocator
	///
	/// @param T - The element type of the `StdVector(T)` instantiation to create
	/// @param capacity - The initial capacity of the vector
	/// @param collection_data - The `StdCollectionData(CollectionType)` containing the element
	/// default-constructor, element copy-constructor, element destructor, and memory allocator to
	/// use
	///
	/// @return a new `StdVector(T)`
	/// @ingroup std_vector
	#define std_vector_new_with_capacity_with_collection_data(T, capacity, collection_data) \
		StdVectorIdentifier(T, new_with_capacity_with_collection_data)(capacity, collection_data)
	/// @brief Clones the given `StdVector(T)`
	///
	/// Creates a deep copy of the given `StdVector(T)` calling the associated copy constructor
	/// for each element stored in it.
	///
	/// @param self - The `StdVector(T)` to clone
	///
	/// @return a clone of the given vector
	/// @note Requires that a copy constructor is defined for the elements
	/// contained in the vector, either the default one provided by the implementation, or a
	/// user-defined one. If it isn't provided or is explicitly provided as `nullptr` when creating
	/// the `StdCollectionData(CollectionType)` associated with this vector, this __may__ trigger a
	/// compiler error on clang, and will always trigger a runtime assert for all compilers
	/// @ingroup std_vector
	#define std_vector_clone(self) (self).m_vtable->clone(&(self))
	/// @brief Returns a mutable reference to the element at the given `index` into the given
	/// `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get an element from
	/// @param index - The index of the desired element
	///
	/// @return a mutable reference to the element at the given index
	/// @ingroup std_vector
	#define std_vector_at_mut(self, index) *((self).m_vtable->at_mut(&(self), (index)))
	/// @brief Returns a const reference to the element at the given `index` into the given
	/// `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get an element from
	/// @param index - The index of the desired element
	///
	/// @return a const reference to the element at the given index
	/// @ingroup std_vector
	#define std_vector_at(self, index) *((self).m_vtable->at_const(&(self), (index)))
	/// @brief Returns a mutable reference to the first element in the given `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get the first element from
	///
	/// @return a mutable reference to the first element
	/// @ingroup std_vector
	#define std_vector_front_mut(self) *((self).m_vtable->front_mut(&(self)))
	/// @brief Returns a const reference to the first element in the given `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get the first element from
	///
	/// @return a const reference to the first element
	/// @ingroup std_vector
	#define std_vector_front(self) *((self).m_vtable->front_const(&(self)))
	/// @brief Returns a mutable reference to the last element in the given `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get the last element from
	///
	/// @return a mutable reference to the last element
	/// @ingroup std_vector
	#define std_vector_back_mut(self) *((self).m_vtable->back_mut(&(self)))
	/// @brief Returns a const reference to the last element in the given `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get the last element from
	///
	/// @return a const reference to the last element
	/// @ingroup std_vector
	#define std_vector_back(self) *((self).m_vtable->back_const(&(self)))
	/// @brief Returns a pointer to the mutable raw array containing the given `StdVector(T)`'s
	/// elements
	///
	/// @param self - The `StdVector(T)` to get the raw array from
	///
	/// @return a pointer to the raw array
	/// @ingroup std_vector
	#define std_vector_data_mut(self) (self).m_vtable->data_mut(&(self))
	/// @brief Returns a pointer to the const raw array containing the given `StdVector(T)`'s
	/// elements
	///
	/// @param self - The `StdVector(T)` to get the raw array from
	///
	/// @return a pointer to the raw array
	/// @ingroup std_vector
	#define std_vector_data(self) (self).m_vtable->data_const(&(self))
	/// @brief Returns whether the given `StdVector(T)`is empty
	///
	/// @param self - The `StdVector(T)` to check for emptiness
	///
	/// @return `true` if empty, `false` otherwise
	/// @ingroup std_vector
	#define std_vector_is_empty(self) (self).m_vtable->is_empty(&(self))
	/// @brief Returns whether the given `StdVector(T)` is full (size equals capacity)
	///
	/// @param self - The `StdVector(T)` to check for fullness
	///
	/// @return `true` if full, `false` otherwise
	/// @ingroup std_vector
	#define std_vector_is_full(self) (self).m_vtable->is_full(&(self))
	/// @brief Returns the current size of the given `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get the size of
	///
	/// @return the size of the `StdVector(T)`
	/// @ingroup std_vector
	#define std_vector_size(self) (self).m_vtable->size(&(self))
	/// @brief Returns the maximum possible size of a `StdVector(T)` containing type `T`
	///
	/// @param T - The element type of the `StdVector(T)` instantation
	///
	/// @return The maximum possible size
	/// @ingroup std_vector
	#define std_vector_max_size(T) CONCAT3(std_vector_, T, _max_size)()
	/// @brief Returns the current capacity of the given `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get the capacity of
	///
	/// @return the capacity of the `StdVector(T)`
	/// @ingroup std_vector
	#define std_vector_capacity(self) (self).m_vtable->capacity(&(self))
	/// @brief Ensures enough memory to store at least `new_capacity` number of elements in the
	/// given `StdVector(T)`, reallocating if necessary.
	///
	/// @param self - The `StdVector(T)` to reserve memory for
	/// @param new_capacity - The desired minimum number of storable elements
	/// @ingroup std_vector
	#define std_vector_reserve(self, new_capacity) (self).m_vtable->reserve(&(self), (new_capacity))
	/// @brief Resizes the given `StdVector(T)` to `new_size` number of elements.
	/// If `new_size` is greater than the current size, this will allocate memory if necessary and
	/// default-construct new elements.
	/// If `new_size` is less than the current size, this will destruct `size - new_size` number of
	/// elements and, if `new_size` is less than the SSO capacity, deallocate memory.
	///
	/// @param self - The `StdVector(T)` to resize
	/// @param new_size - The desired size of the `StdVector(T)`
	/// @ingroup std_vector
	#define std_vector_resize(self, new_size) (self).m_vtable->resize(&(self), (new_size))
	/// @brief Shrinks the memory allocation for the given `StdVector(T)` to match its current size
	///
	/// @param self - The `StdVector(T)` to shrink
	/// @ingroup std_vector
	#define std_vector_shrink_to_fit(self) (self).m_vtable->shrink_to_fit(&(self))
	/// @brief Clears the contents of the given `StdVector(T)`, destructing all of its elements
	///
	/// @param self - The `StdVector(T)` to clear
	/// @ingroup std_vector
	#define std_vector_clear(self) (self).m_vtable->clear(&(self))
	/// @brief Appends the given element to the end of the given `StdVector(T)`, reallocating memory
	/// if necessary
	///
	/// @param self - The `StdVector(T)` to append to
	/// @param element - The element to append
	/// @ingroup std_vector
	#define std_vector_push_back(self, element) (self).m_vtable->push_back(&(self), (element))
	/// @brief Removes the last element in the given `StdVector(T)` and removes it, if the size is
	/// greater than zero.
	///
	/// @param self - The `StdVector(T)` to get the last element from
	///
	/// @return `Some(T)` if size > 0, otherwise `None(T)`
	/// @ingroup std_vector
	#define std_vector_pop_back(self) (self).m_vtable->pop_back(&(self))
	/// @brief Inserts the given element at the given index in the given `StdVector(T)`, moving
	/// elements backward in the vector if necessary
	///
	/// @param self - The `StdVector(T)` to insert into
	/// @param element - The element to insert
	/// @param index - The index at which to insert `element`
	///
	/// @note If `index` is past the end of the `StdVector(T)` (`index` > size), subsequent behavior
	/// of the collection is undefined
	/// @ingroup std_vector
	#define std_vector_insert(self, element, index) \
		(self).m_vtable->insert(&(self), (element), (index))
	/// @brief Removes the element at the given index from the given `StdVector(T)`, moving elements
	/// forward in the vector if necessary
	///
	/// @param self - The `StdVector(T)` to remove an element from
	/// @param index - The index of the element to remove
	/// @ingroup std_vector
	#define std_vector_erase(self, index) (self).m_vtable->erase(&(self), (index))
	/// @brief Removes `num_elements` elements from the given `StdVector(T)`, starting with the
	/// element at `index`, and moving elements forward in the vector afterward, if necessary.
	///
	/// @param self - The `StdVector(T)` to remove elements from
	/// @param index - The index to begin removal at
	/// @param num_elements - The number of elements to remove from the vector
	///
	/// @note The given `StdVector(T)` must contain the specified number of elements within the
	/// range [index, size). IE: `index + num_elements` must be strictly less than the current size
	/// of the vector
	/// @ingroup std_vector
	#define std_vector_erase_n(self, index, num_elements) \
		(self).m_vtable->erase_n(&(self), (index), (num_elements))
	/// @brief Frees the given `StdVector(T)`, calling the element destructor on each element and
	/// freeing any allocated memory
	///
	/// @param self - The `StdVector(T)` to free
	/// @ingroup std_vector
	#define std_vector_free(self) (self).m_vtable->free(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the mutable iteration of the given
	/// `StdVector(T)`, starting at the beginning of the iteration (pointing at the beginning of the
	/// vector)
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the vector
	/// @ingroup std_vector
	#define std_vector_begin(self) (self).m_vtable->begin(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the mutable iteration of the given
	/// `StdVector(T)`, starting at the end of the iteration (pointing at the end of the vector)
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the end of the vector
	/// @ingroup std_vector
	#define std_vector_end(self) (self).m_vtable->end(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the mutable iteration of the given
	/// `StdVector(T)`, starting at the beginning of the reversed iteration (pointing at the end of
	/// the vector)
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the reversed vector
	/// @ingroup std_vector
	#define std_vector_rbegin(self) (self).m_vtable->rbegin(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the mutable iteration of the given
	/// `StdVector(T)`, starting at the end of the reversed iteration (pointing at the beginning of
	/// the vector)
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the end of the reversed vector
	/// @ingroup std_vector
	#define std_vector_rend(self) (self).m_vtable->rend(&(self))
	/// @brief Returns whether the given pair of iterators are equal (they belong to the same
	/// collection and point to the same element), IE: if `first == second`
	///
	/// @param first - The LHS iterator of the equality check
	/// @param second - The RHS iterator of the equality check
	///
	/// @return `true` if they are equal, `false` otherwise
	/// @ingroup std_vector
	#define std_vector_iterator_equals(first, second) std_iterator_equals(first, second)
	/// @brief Returns a `StdRandomAccessIterator` into the const iteration of the given
	/// `StdVector(T)`, starting at the beginning of the iteration (pointing at the beginning of the
	/// vector)
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the vector
	/// @ingroup std_vector
	#define std_vector_cbegin(self) (self).m_vtable->cbegin(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the const iteration of the given
	/// `StdVector(T)`, starting at the end of the iteration (pointing at the end of the vector)
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the end of the vector
	/// @ingroup std_vector
	#define std_vector_cend(self) (self).m_vtable->cend(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the const iteration of the given
	/// `StdVector(T)`, starting at the beginning of the reversed iteration (pointing at the end of
	/// the vector)
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the reversed vector
	/// @ingroup std_vector
	#define std_vector_crbegin(self) (self).m_vtable->crbegin(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the const iteration of the given
	/// `StdVector(T)`, starting at the end of the reversed iteration (pointing at the beginning of
	/// the vector)
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the end of the reversed vector
	/// @ingroup std_vector
	#define std_vector_crend(self) (self).m_vtable->crend(&(self))
	/// @brief Returns whether the given pair of const iterators are equal (they belong to the same
	/// collection and point to the same element), IE: if `first == second`
	///
	/// @param first - The LHS iterator of the equality check
	/// @param second - The RHS iterator of the equality check
	///
	/// @ingroup std_vector
	#define std_vector_const_iterator_equals(first, second) std_iterator_equals(first, second)
	/// @brief Returns a `StdRandomAccessIterator` into the mutable iteration of the given
	/// `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator into the vector
	#define std_vector_into_iter(self) (self).m_vtable->into_iter(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the mutable reversed iteration of the given
	/// `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator into the reversed vector
	#define std_vector_into_reverse_iter(self) (self).m_vtable->into_reverse_iter(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the const iteration of the given
	/// `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator into the vector
	#define std_vector_into_const_iter(self) (self).m_vtable->into_const_iter(&(self))
	/// @brief Returns a `StdRandomAccessIterator` into the const reversed iteration of the given
	/// `StdVector(T)`
	///
	/// @param self - The `StdVector(T)` to get an iterator to
	///
	/// @return a random access iterator into the reversed vector
	#define std_vector_into_reverse_const_iter(self) \
		(self).m_vtable->into_reverse_const_iter(&(self))

	/// @brief declare a `StdVector(T)` variable with this attribute to have `std_vector_free`
	/// automatically called on it at scope end
	///
	/// @param T - The element type of the `StdVector(T)` instantiation
	/// @ingroup std_vector
	#define std_vector_scoped(T) scoped(StdVectorIdentifier(T, free))

	/// @brief Use this macro to instantiate the definitions for a `StdVector(T)` containing `T`s.
	/// This provides the function definitions for `StdVector(T)`.
	///
	/// Requires that `DeclStdVector(T)` has already been used and is in scope, and that the
	/// requirements for that have already been met.
	///
	/// @param T - The element type of the `StdArray` instantiation
	/// @ingroup std_vector
	#define ImplStdVector(T)                                                                       \
		ImplStdOption(StdVector(T));                                                               \
		ImplStdResult(StdVector(T));                                                               \
                                                                                                   \
		StdVectorIterator(T)                                                                       \
			StdVectorIdentifier(T, iterator_new)(const StdVector(T)* restrict self);               \
		StdVectorConstIterator(T)                                                                  \
			StdVectorIdentifier(T, const_iterator_new)(const StdVector(T)* restrict self);         \
                                                                                                   \
		Ref(T)                                                                                     \
			StdVectorIdentifier(T, iterator_next)(StdRandomAccessIterator(Ref(T))* restrict self); \
		Ref(T) StdVectorIdentifier(T, iterator_previous)(                                          \
			StdRandomAccessIterator(Ref(T))* restrict self);                                       \
		Ref(T) StdVectorIdentifier(T, iterator_at)(                                                \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			usize index);                                                                          \
		Ref(T) StdVectorIdentifier(T, iterator_rat)(                                               \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			usize index);                                                                          \
		Ref(T) StdVectorIdentifier(T, iterator_current)(                                           \
			const StdRandomAccessIterator(Ref(T))* restrict self);                                 \
		bool StdVectorIdentifier(T, iterator_equals)(                                              \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			const StdRandomAccessIterator(Ref(T))* restrict rhs);                                  \
                                                                                                   \
		ConstRef(T) StdVectorIdentifier(T, iterator_cnext)(                                        \
			StdRandomAccessIterator(ConstRef(T))* restrict self);                                  \
		ConstRef(T) StdVectorIdentifier(T, iterator_cprevious)(                                    \
			StdRandomAccessIterator(ConstRef(T))* restrict self);                                  \
		ConstRef(T) StdVectorIdentifier(T, iterator_cat)(                                          \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			usize index);                                                                          \
		ConstRef(T) StdVectorIdentifier(T, iterator_crat)(                                         \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			usize index);                                                                          \
		ConstRef(T) StdVectorIdentifier(T, iterator_ccurrent)(                                     \
			const StdRandomAccessIterator(ConstRef(T))* restrict self);                            \
		bool StdVectorIdentifier(T, iterator_cequals)(                                             \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			const StdRandomAccessIterator(ConstRef(T))* restrict rhs);                             \
                                                                                                   \
		ImplIntoStdRandomAccessIterator(StdVector(T),                                              \
										Ref(T),                                                    \
										StdVectorIdentifier(T, into_iter),                         \
										into,                                                      \
										StdVectorIdentifier(T, iterator_new),                      \
										StdVectorIdentifier(T, iterator_next),                     \
										StdVectorIdentifier(T, iterator_previous),                 \
										StdVectorIdentifier(T, iterator_at),                       \
										StdVectorIdentifier(T, iterator_current),                  \
										StdVectorIdentifier(T, iterator_equals));                  \
		ImplIntoStdRandomAccessIterator(StdVector(T),                                              \
										Ref(T),                                                    \
										StdVectorIdentifier(T, into_reverse_iter),                 \
										into_reverse,                                              \
										StdVectorIdentifier(T, iterator_new),                      \
										StdVectorIdentifier(T, iterator_next),                     \
										StdVectorIdentifier(T, iterator_previous),                 \
										StdVectorIdentifier(T, iterator_rat),                      \
										StdVectorIdentifier(T, iterator_current),                  \
										StdVectorIdentifier(T, iterator_equals));                  \
                                                                                                   \
		ImplIntoStdRandomAccessIterator(StdVector(T),                                              \
										ConstRef(T),                                               \
										StdVectorIdentifier(T, into_const_iter),                   \
										into,                                                      \
										StdVectorIdentifier(T, const_iterator_new),                \
										StdVectorIdentifier(T, iterator_cnext),                    \
										StdVectorIdentifier(T, iterator_cprevious),                \
										StdVectorIdentifier(T, iterator_cat),                      \
										StdVectorIdentifier(T, iterator_ccurrent),                 \
										StdVectorIdentifier(T, iterator_cequals));                 \
		ImplIntoStdRandomAccessIterator(StdVector(T),                                              \
										ConstRef(T),                                               \
										StdVectorIdentifier(T, into_reverse_const_iter),           \
										into_reverse,                                              \
										StdVectorIdentifier(T, const_iterator_new),                \
										StdVectorIdentifier(T, iterator_cnext),                    \
										StdVectorIdentifier(T, iterator_cprevious),                \
										StdVectorIdentifier(T, iterator_crat),                     \
										StdVectorIdentifier(T, iterator_ccurrent),                 \
										StdVectorIdentifier(T, iterator_cequals));                 \
                                                                                                   \
		always_inline inline static T StdVectorIdentifier(T, default_constructor)(                 \
			StdAllocator allocator) {                                                              \
			ignore(allocator);                                                                     \
			return (T){0};                                                                         \
		}                                                                                          \
                                                                                                   \
		always_inline inline static T StdVectorIdentifier(T, default_copy_constructor)(            \
			const T* restrict elem,                                                                \
			StdAllocator allocator) {                                                              \
			ignore(allocator);                                                                     \
			return *elem;                                                                          \
		}                                                                                          \
                                                                                                   \
		always_inline inline static void StdVectorIdentifier(T, default_destructor)(               \
			T* restrict element, /** NOLINT(readability-non-const-parameter)**/                    \
			StdAllocator allocator) {                                                              \
			ignore(allocator, element);                                                            \
		}                                                                                          \
                                                                                                   \
		static const struct StdVectorIdentifier(T, vtable) StdVectorIdentifier(T, vtable_impl) = { \
			.clone = StdVectorIdentifier(T, clone),                                                \
			.at_const = StdVectorIdentifier(T, at_const),                                          \
			.at_mut = StdVectorIdentifier(T, at_mut),                                              \
			.front_const = StdVectorIdentifier(T, front_const),                                    \
			.front_mut = StdVectorIdentifier(T, front_mut),                                        \
			.back_const = StdVectorIdentifier(T, back_const),                                      \
			.back_mut = StdVectorIdentifier(T, back_mut),                                          \
			.data_const = StdVectorIdentifier(T, data_const),                                      \
			.data_mut = StdVectorIdentifier(T, data_mut),                                          \
			.is_empty = StdVectorIdentifier(T, is_empty),                                          \
			.is_full = StdVectorIdentifier(T, is_full),                                            \
			.size = StdVectorIdentifier(T, size),                                                  \
			.capacity = StdVectorIdentifier(T, capacity),                                          \
			.reserve = StdVectorIdentifier(T, reserve),                                            \
			.resize = StdVectorIdentifier(T, resize),                                              \
			.shrink_to_fit = StdVectorIdentifier(T, shrink_to_fit),                                \
			.clear = StdVectorIdentifier(T, clear),                                                \
			.push_back = StdVectorIdentifier(T, push_back),                                        \
			.pop_back = StdVectorIdentifier(T, pop_back),                                          \
			.insert = StdVectorIdentifier(T, insert),                                              \
			.erase = StdVectorIdentifier(T, erase),                                                \
			.erase_n = StdVectorIdentifier(T, erase_n),                                            \
			.free = StdVectorIdentifier(T, free),                                                  \
			.into_iter = StdVectorIdentifier(T, into_iter),                                        \
			.into_reverse_iter = StdVectorIdentifier(T, into_reverse_iter),                        \
			.into_const_iter = StdVectorIdentifier(T, into_const_iter),                            \
			.into_reverse_const_iter = StdVectorIdentifier(T, into_reverse_const_iter),            \
			.begin = StdVectorIdentifier(T, begin),                                                \
			.end = StdVectorIdentifier(T, end),                                                    \
			.rbegin = StdVectorIdentifier(T, rbegin),                                              \
			.rend = StdVectorIdentifier(T, rend),                                                  \
			.cbegin = StdVectorIdentifier(T, cbegin),                                              \
			.cend = StdVectorIdentifier(T, cend),                                                  \
			.crbegin = StdVectorIdentifier(T, crbegin),                                            \
			.crend = StdVectorIdentifier(T, crend),                                                \
		};                                                                                         \
                                                                                                   \
		static const struct StdCollectionData(StdVector(T))                                        \
			StdVectorIdentifier(T, default_collection_data)                                        \
			= {.m_constructor = StdVectorIdentifier(T, default_constructor),                       \
			   .m_copy_constructor = StdVectorIdentifier(T, default_copy_constructor),             \
			   .m_destructor = StdVectorIdentifier(T, default_destructor),                         \
			   .m_allocator = STD_DEFAULT_ALLOCATOR};                                              \
                                                                                                   \
		always_inline inline static bool StdVectorIdentifier(T, is_short)(                         \
			const StdVector(T)* restrict self) {                                                   \
			return self->m_capacity <= STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T);                  \
		}                                                                                          \
                                                                                                   \
		StdVector(T) StdVectorIdentifier(T, new)(void) {                                           \
			return std_vector_new_with_collection_data(                                            \
				T,                                                                                 \
				StdVectorIdentifier(T, default_collection_data));                                  \
		}                                                                                          \
                                                                                                   \
		StdVector(T) StdVectorIdentifier(T, new_with_collection_data)(                             \
			StdCollectionData(StdVector(T)) data) {                                                \
                                                                                                   \
			let_mut vec = (StdVector(T)){.m_size = 0,                                              \
										 .m_capacity = STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T),  \
										 .m_data = data,                                           \
										 .m_vtable = &StdVectorIdentifier(T, vtable_impl)};        \
                                                                                                   \
			if(vec.m_data.m_constructor == nullptr) {                                              \
				vec.m_data.m_constructor = StdVectorIdentifier(T, default_constructor);            \
			}                                                                                      \
                                                                                                   \
			if(vec.m_data.m_destructor == nullptr) {                                               \
				vec.m_data.m_destructor = StdVectorIdentifier(T, default_destructor);              \
			}                                                                                      \
                                                                                                   \
			if(STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T) == 0) { /** NOLINT **/                    \
				vec.m_long = static_cast(T*)(                                                      \
					std_allocator_allocate_array_t(T,                                              \
												   vec.m_data.m_allocator,                         \
												   STD_VECTOR_DEFAULT_LONG_CAPACITY)               \
						.m_memory);                                                                \
				vec.m_capacity = STD_VECTOR_DEFAULT_LONG_CAPACITY;                                 \
			}                                                                                      \
			else {                                                                                 \
				std_memset(T, vec.m_short, 0, STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T));          \
			}                                                                                      \
                                                                                                   \
			return vec;                                                                            \
		}                                                                                          \
                                                                                                   \
		StdVector(T) StdVectorIdentifier(T, new_with_capacity)(usize capacity) {                   \
			let_mut vec = std_vector_new(T);                                                       \
			std_vector_reserve(vec, capacity);                                                     \
			return vec;                                                                            \
		}                                                                                          \
                                                                                                   \
		StdVector(T) StdVectorIdentifier(T, new_with_capacity_with_collection_data)(               \
			usize capacity,                                                                        \
			StdCollectionData(StdVector(T)) data) {                                                \
                                                                                                   \
			let_mut vec = std_vector_new_with_collection_data(T, data);                            \
			std_vector_reserve(vec, capacity);                                                     \
			return vec;                                                                            \
		}                                                                                          \
                                                                                                   \
		StdVector(T) StdVectorIdentifier(T, clone)(const StdVector(T)* restrict self)              \
			std_disable_if(!(self->m_data.m_copy_constructor),                                     \
						   "Can't clone a StdVector(T) with elements that aren't copyable (no "    \
						   "element copy constructor defined)") {                                  \
			std_assert(self->m_data.m_copy_constructor != nullptr,                                 \
					   "Can't clone StdVector(T) with elements that aren't copyable (no element "  \
					   "copy constructor defined)");                                               \
                                                                                                   \
			let_mut vec                                                                            \
				= std_vector_new_with_capacity_with_collection_data(T,                             \
																	std_vector_capacity(*self),    \
																	self->m_data);                 \
			foreach_ref(elem, *self) {                                                             \
				std_vector_push_back(                                                              \
					vec,                                                                           \
					self->m_data.m_copy_constructor(elem, self->m_data.m_allocator));              \
			}                                                                                      \
			return vec;                                                                            \
		}                                                                                          \
                                                                                                   \
		const T* StdVectorIdentifier(T, at_const)(const StdVector(T)* restrict self,               \
												  usize index) {                                   \
			std_assert(index <= self->m_size,                                                      \
					   "std_vector_at called with index > size (index out of bounds");             \
			std_assert(index < self->m_capacity,                                                   \
					   "std_vector_at called with index >= capacity (index out of bounds");        \
                                                                                                   \
			return StdVectorIdentifier(T, is_short)(self) ? &(self->m_short[index]) :              \
															  &(self->m_long[index]);                \
		}                                                                                          \
                                                                                                   \
		T* StdVectorIdentifier(T, at_mut)(StdVector(T)* restrict self, usize index) {              \
			std_assert(index <= self->m_size,                                                      \
					   "std_vector_at called with index > size (index out of bounds");             \
			std_assert(index < self->m_capacity,                                                   \
					   "std_vector_at called with index >= capacity (index out of bounds");        \
                                                                                                   \
			return StdVectorIdentifier(T, is_short)(self) ? &(self->m_short[index]) :              \
															  &(self->m_long[index]);                \
		}                                                                                          \
                                                                                                   \
		const T* StdVectorIdentifier(T, front_const)(const StdVector(T)* restrict self) {          \
			return &std_vector_at(*self, 0);                                                       \
		}                                                                                          \
                                                                                                   \
		T* StdVectorIdentifier(T, front_mut)(StdVector(T)* restrict self) {                        \
			return &std_vector_at_mut(*self, 0);                                                   \
		}                                                                                          \
                                                                                                   \
		const T* StdVectorIdentifier(T, back_const)(const StdVector(T)* restrict self) {           \
			let size = std_vector_size(*self);                                                     \
			let back_index = size > 0 ? size - 1 : 0;                                              \
			return &std_vector_at(*self, back_index);                                              \
		}                                                                                          \
                                                                                                   \
		T* StdVectorIdentifier(T, back_mut)(StdVector(T)* restrict self) {                         \
			let size = std_vector_size(*self);                                                     \
			let back_index = size > 0 ? size - 1 : 0;                                              \
			return &std_vector_at_mut(*self, back_index);                                          \
		}                                                                                          \
                                                                                                   \
		const T* StdVectorIdentifier(T, data_const)(const StdVector(T)* restrict self) {           \
			return &std_vector_front(*self);                                                       \
		}                                                                                          \
                                                                                                   \
		T* StdVectorIdentifier(T, data_mut)(StdVector(T)* restrict self) {                         \
			return &std_vector_front_mut(*self);                                                   \
		}                                                                                          \
                                                                                                   \
		bool StdVectorIdentifier(T, is_empty)(const StdVector(T)* restrict self) {                 \
			return std_vector_size(*self) == 0;                                                    \
		}                                                                                          \
                                                                                                   \
		bool StdVectorIdentifier(T, is_full)(const StdVector(T)* restrict self) {                  \
			return self->m_size == self->m_capacity;                                               \
		}                                                                                          \
                                                                                                   \
		usize StdVectorIdentifier(T, size)(const StdVector(T)* restrict self) {                    \
			return self->m_size;                                                                   \
		}                                                                                          \
                                                                                                   \
		usize StdVectorIdentifier(T, max_size)(void) {                                             \
			return (std_max_value(usize) - 1) << 1U;                                               \
		}                                                                                          \
                                                                                                   \
		usize StdVectorIdentifier(T, capacity)(const StdVector(T)* restrict self) {                \
			return self->m_capacity;                                                               \
		}                                                                                          \
                                                                                                   \
		void StdVectorIdentifier(T, resize_internal)(StdVector(T)* restrict self,                  \
													 usize new_size) {                             \
			let size = std_vector_size(*self);                                                     \
			if(new_size < size) {                                                                  \
				let num_to_destroy = size - new_size;                                              \
				for(let_mut i = new_size; i < new_size + num_to_destroy; ++i) {                    \
					self->m_data.m_destructor(&std_vector_at_mut(*self, i),                        \
											  self->m_data.m_allocator);                           \
				}                                                                                  \
			}                                                                                      \
			if(new_size > STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T)) {                             \
				let_mut array = static_cast(T*)(                                                   \
					std_allocator_allocate_array_t(T, self->m_data.m_allocator, new_size)          \
						.m_memory);                                                                \
				let num_to_copy = std_min(size, new_size);                                         \
				std_memcpy(T, array, &std_vector_at_mut(*self, 0), num_to_copy);                   \
				if(!StdVectorIdentifier(T, is_short)(self)) {                                      \
					let_mut ptr = self->m_long;                                                    \
					self->m_long = nullptr;                                                        \
					std_allocator_deallocate_array_t(T,                                            \
													 self->m_data.m_allocator,                     \
													 ptr,                                          \
													 self->m_capacity);                            \
				}                                                                                  \
				self->m_capacity = new_size;                                                       \
				self->m_size = num_to_copy;                                                        \
				self->m_long = array;                                                              \
			}                                                                                      \
			else if(self->m_capacity != STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T)                  \
					&& STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T) != 0)                             \
			{                                                                                      \
				let capacity = STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T);                          \
				let_mut array = static_cast(T*)(                                                   \
					std_allocator_allocate_array_t(T, self->m_data.m_allocator, capacity)          \
						.m_memory);                                                                \
				std_memcpy(T, array, self->m_long, capacity);                                      \
				std_allocator_deallocate_array_t(T,                                                \
												 self->m_data.m_allocator,                         \
												 self->m_long,                                     \
												 self->m_capacity);                                \
				std_memcpy(T, self->m_short, array, capacity);                                     \
				std_allocator_deallocate_array_t(T, self->m_data.m_allocator, array, capacity);    \
				self->m_size = capacity;                                                           \
				self->m_capacity = capacity;                                                       \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		static always_inline inline usize StdVectorIdentifier(T, get_expanded_capacity)(           \
			usize old_capacity,                                                                    \
			usize num_increments) {                                                                \
			return num_increments * ((old_capacity * 3) / 2);                                      \
		}                                                                                          \
                                                                                                   \
		void StdVectorIdentifier(T, reserve)(StdVector(T)* restrict self, usize new_capacity) {    \
			if(new_capacity > self->m_capacity) {                                                  \
				let num_increments = 1 + (new_capacity / ((self->m_capacity * 3) / 2));            \
				let actual_new_capacity                                                            \
					= StdVectorIdentifier(T, get_expanded_capacity)(self->m_capacity,              \
																	num_increments);               \
				StdVectorIdentifier(T, resize_internal)(self, actual_new_capacity);                \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		void StdVectorIdentifier(T, resize)(StdVector(T)* restrict self, usize new_size) {         \
			StdVectorIdentifier(T, resize_internal)(self, new_size);                               \
			if(new_size > self->m_size) {                                                          \
				for(let_mut i = self->m_size - 1; i < new_size; ++i) {                             \
					std_vector_at_mut(*self, i)                                                    \
						= self->m_data.m_constructor(self->m_data.m_allocator);                    \
				}                                                                                  \
			}                                                                                      \
			self->m_size = new_size;                                                               \
		}                                                                                          \
                                                                                                   \
		void StdVectorIdentifier(T, shrink_to_fit)(StdVector(T)* restrict self) {                  \
			StdVectorIdentifier(T, resize_internal)(self, std_vector_size(*self));                 \
		}                                                                                          \
                                                                                                   \
		void StdVectorIdentifier(T, clear)(StdVector(T)* restrict self) {                          \
			for(let_mut i = 0U; i < std_vector_size(*self); ++i) {                                 \
				self->m_data.m_destructor(&std_vector_at_mut(*self, i), self->m_data.m_allocator); \
			}                                                                                      \
			self->m_size = 0U;                                                                     \
		}                                                                                          \
                                                                                                   \
		void StdVectorIdentifier(T, push_back)(                                                    \
			StdVector(T)* restrict self,                                                           \
			T element /** NOLINT(readability-non-const-parameter) **/) {                           \
			if(self->m_size + 1 > self->m_capacity) {                                              \
				let new_capacity                                                                   \
					= StdVectorIdentifier(T, get_expanded_capacity)(self->m_capacity, 1);          \
				StdVectorIdentifier(T, resize_internal)(self, new_capacity);                       \
			}                                                                                      \
                                                                                                   \
			std_vector_at_mut(*self, self->m_size) = element;                                      \
			self->m_size++;                                                                        \
		}                                                                                          \
                                                                                                   \
		StdOption(T) StdVectorIdentifier(T, pop_back)(StdVector(T)* restrict self) {               \
			if(self->m_size == 0) {                                                                \
				return None(T);                                                                    \
			}                                                                                      \
                                                                                                   \
			let_mut ptr = &std_vector_at_mut(*self, self->m_size - 1);                             \
			let elem = Some(T, *ptr);                                                              \
			*ptr = (T){0};                                                                         \
			self->m_size--;                                                                        \
                                                                                                   \
			return elem;                                                                           \
		}                                                                                          \
                                                                                                   \
		void StdVectorIdentifier(T,                                                                \
								 insert)(StdVector(T)* restrict self,                              \
										 T element /** NOLINT(readability-non-const-parameter **/, \
										 usize index) {                                            \
			std_assert(index <= self->m_size,                                                      \
					   "std_vector_insert called with index > size (index out of bounds)");        \
                                                                                                   \
			if(self->m_size + 1 > self->m_capacity) {                                              \
				let new_capacity                                                                   \
					= StdVectorIdentifier(T, get_expanded_capacity)(self->m_capacity, 1);          \
				StdVectorIdentifier(T, resize_internal)(self, new_capacity);                       \
			}                                                                                      \
                                                                                                   \
			if(index != self->m_size) {                                                            \
				let num_to_move = self->m_size - index;                                            \
				std_memmove(T,                                                                     \
							&std_vector_at_mut(*self, index + 1),                                  \
							&std_vector_at_mut(*self, index),                                      \
							num_to_move);                                                          \
			}                                                                                      \
			std_vector_at_mut(*self, index) = element;                                             \
			self->m_size++;                                                                        \
		}                                                                                          \
                                                                                                   \
		void StdVectorIdentifier(T, erase)(StdVector(T)* restrict self, usize index) {             \
			std_assert(                                                                            \
				index < self->m_size,                                                              \
				"std_vector_erase called with index >= self->m_size (index out of bounds)");       \
                                                                                                   \
			self->m_data.m_destructor(&std_vector_at_mut(*self, index), self->m_data.m_allocator); \
                                                                                                   \
			if(index != self->m_size - 1) {                                                        \
				let num_to_move = self->m_size - (index + 1);                                      \
				std_memmove(T,                                                                     \
							&std_vector_at_mut(*self, index),                                      \
							&std_vector_at_mut(*self, index + 1),                                  \
							num_to_move);                                                          \
			}                                                                                      \
			self->m_size--;                                                                        \
		}                                                                                          \
                                                                                                   \
		void StdVectorIdentifier(T, erase_n)(StdVector(T)* restrict self,                          \
											 usize index,                                          \
											 usize num_elements) {                                 \
			std_assert(index < self->m_size,                                                       \
					   "std_vector_erase_n called with index >= size (index out of bounds)");      \
			std_assert(index + num_elements < self->m_size,                                        \
					   "std_vector_erase_n called with index + size >= size (range "               \
					   "out of bounds)");                                                          \
                                                                                                   \
			let end = index + num_elements;                                                        \
			let num_to_move = self->m_size - end;                                                  \
                                                                                                   \
			for(let_mut i = index; i < end; ++i) {                                                 \
				self->m_data.m_destructor(&std_vector_at_mut(*self, i), self->m_data.m_allocator); \
			}                                                                                      \
                                                                                                   \
			if(end != self->m_size) {                                                              \
				std_memmove(T,                                                                     \
							&std_vector_at_mut(*self, index),                                      \
							&std_vector_at_mut(*self, end),                                        \
							num_to_move);                                                          \
			}                                                                                      \
			self->m_size -= num_elements;                                                          \
		}                                                                                          \
                                                                                                   \
		void StdVectorIdentifier(T, free)(void* restrict self) {                                   \
			let self_ = static_cast(StdVector(T)*)(self);                                          \
			for(let_mut i = 0U; i < self_->m_size; ++i) {                                          \
				self_->m_data.m_destructor(&std_vector_at_mut(*self_, i),                          \
										   self_->m_data.m_allocator);                             \
			}                                                                                      \
                                                                                                   \
			if(!StdVectorIdentifier(T, is_short)(self_)) {                                         \
				std_allocator_deallocate(                                                          \
					self_->m_data.m_allocator,                                                     \
					(StdMemory){.m_memory = self_->m_long,                                         \
								.m_size_bytes = self_->m_capacity * sizeof(T)});                   \
				self_->m_capacity = STD_VECTOR_SHORT_OPTIMIZATION_CAPACITY(T);                     \
			}                                                                                      \
			self_->m_size = 0U;                                                                    \
		}                                                                                          \
                                                                                                   \
		StdVectorIterator(T)                                                                       \
			StdVectorIdentifier(T, iterator_new)(const StdVector(T)* restrict self) {              \
			return (StdVectorIterator(T)){.m_index = 0U,                                           \
										  .m_vector = const_cast(StdVector(T)*)(self)};            \
		}                                                                                          \
                                                                                                   \
		StdVectorConstIterator(T)                                                                  \
			StdVectorIdentifier(T, const_iterator_new)(const StdVector(T)* restrict self) {        \
			return (StdVectorConstIterator(T)){.m_index = 0U,                                      \
											   .m_vector = const_cast(StdVector(T)*)(self)};       \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdVectorIdentifier(T, iterator_next)(                                              \
			StdRandomAccessIterator(Ref(T))* restrict self) {                                      \
			let _self = static_cast(StdVectorIterator(T)*)(self->m_self);                          \
                                                                                                   \
			std_assert(                                                                            \
				_self->m_index > -1,                                                               \
				"Iterator value accessed when iterator is positioned before the beginning of the " \
				"iteration (iterator out of bounds)");                                             \
			std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,               \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,           \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			_self->m_index++;                                                                      \
			if(static_cast(usize)(_self->m_index) >= _self->m_vector->m_size) {                    \
				return &std_vector_at_mut(*(_self->m_vector), _self->m_vector->m_size - 1);        \
			}                                                                                      \
                                                                                                   \
			return &std_vector_at_mut(*(_self->m_vector), static_cast(usize)(_self->m_index));     \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdVectorIdentifier(T, iterator_previous)(                                          \
			StdRandomAccessIterator(Ref(T))* restrict self) {                                      \
			let _self = static_cast(StdVectorIterator(T)*)(self->m_self);                          \
                                                                                                   \
			std_assert(                                                                            \
				_self->m_index > -1,                                                               \
				"Iterator value accessed when iterator is positioned before the beginning of the " \
				"iteration (iterator out of bounds)");                                             \
			std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,               \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			_self->m_index--;                                                                      \
			if(_self->m_index < 0) {                                                               \
				return &std_vector_at_mut(*(_self->m_vector), 0);                                  \
			}                                                                                      \
                                                                                                   \
			return &std_vector_at_mut(*(_self->m_vector), static_cast(usize)(_self->m_index));     \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdVectorIdentifier(T, iterator_at)(                                                \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			usize index) {                                                                         \
			let _self = static_cast(StdVectorIterator(T)*)(self->m_self);                          \
                                                                                                   \
			std_assert(index < _self->m_vector->m_size,                                            \
					   "std_vector_iterator_at called with index pst the end of the iteration "    \
					   "(iterator index out of bounds)");                                          \
			return &std_vector_at_mut(*(_self->m_vector), index);                                  \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdVectorIdentifier(T, iterator_rat)(                                               \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			usize index) {                                                                         \
			let _self = static_cast(StdVectorIterator(T)*)(self->m_self);                          \
                                                                                                   \
			std_assert(index < _self->m_vector->m_size,                                            \
					   "std_vector_iterator_at called with index pst the end of the iteration "    \
					   "(iterator index out of bounds)");                                          \
			return &std_vector_at_mut(*(_self->m_vector), (_self->m_vector->m_size - 1) - index);  \
		}                                                                                          \
                                                                                                   \
		Ref(T) StdVectorIdentifier(T, iterator_current)(                                           \
			const StdRandomAccessIterator(Ref(T))* restrict self) {                                \
			let _self = static_cast(const StdVectorIterator(T)*)(self->m_self);                    \
                                                                                                   \
			std_assert(                                                                            \
				_self->m_index > -1,                                                               \
				"Iterator value accessed when iterator is positioned before the beginning of the " \
				"iteration (iterator out of bounds)");                                             \
			std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,               \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,           \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			return &std_vector_at_mut(*(_self->m_vector), static_cast(usize)(_self->m_index));     \
		}                                                                                          \
                                                                                                   \
		bool StdVectorIdentifier(T, iterator_equals)(                                              \
			const StdRandomAccessIterator(Ref(T))* restrict self,                                  \
			const StdRandomAccessIterator(Ref(T))* restrict rhs) {                                 \
			let _self = static_cast(const StdVectorIterator(T)*)(self->m_self);                    \
			let _rhs = static_cast(const StdVectorIterator(T)*)(rhs->m_self);                      \
                                                                                                   \
			return _self->m_index == _rhs->m_index && _self->m_vector == _rhs->m_vector;           \
		}                                                                                          \
                                                                                                   \
		ConstRef(T) StdVectorIdentifier(T, iterator_cnext)(                                        \
			StdRandomAccessIterator(ConstRef(T))* restrict self) {                                 \
			let _self = static_cast(StdVectorConstIterator(T)*)(self->m_self);                     \
                                                                                                   \
			std_assert(                                                                            \
				_self->m_index > -1,                                                               \
				"Iterator value accessed when iterator is positioned before the beginning of the " \
				"iteration (iterator out of bounds)");                                             \
			std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,               \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,           \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			_self->m_index++;                                                                      \
			if(static_cast(usize)(_self->m_index) >= _self->m_vector->m_size) {                    \
				return &std_vector_at(*(_self->m_vector), _self->m_vector->m_size - 1);            \
			}                                                                                      \
                                                                                                   \
			return &std_vector_at(*(_self->m_vector), static_cast(usize)(_self->m_index));         \
		}                                                                                          \
                                                                                                   \
		ConstRef(T) StdVectorIdentifier(T, iterator_cprevious)(                                    \
			StdRandomAccessIterator(ConstRef(T))* restrict self) {                                 \
			let _self = static_cast(StdVectorConstIterator(T)*)(self->m_self);                     \
                                                                                                   \
			std_assert(                                                                            \
				_self->m_index > -1,                                                               \
				"Iterator value accessed when iterator is positioned before the beginning of the " \
				"iteration (iterator out of bounds)");                                             \
			std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,               \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			_self->m_index--;                                                                      \
			if(_self->m_index < 0) {                                                               \
				return &std_vector_at(*(_self->m_vector), 0);                                      \
			}                                                                                      \
                                                                                                   \
			return &std_vector_at(*(_self->m_vector), static_cast(usize)(_self->m_index));         \
		}                                                                                          \
                                                                                                   \
		ConstRef(T) StdVectorIdentifier(T, iterator_cat)(                                          \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			usize index) {                                                                         \
			let _self = static_cast(StdVectorConstIterator(T)*)(self->m_self);                     \
                                                                                                   \
			std_assert(index < _self->m_vector->m_size,                                            \
					   "std_vector_iterator_at called with index pst the end of the iteration "    \
					   "(iterator index out of bounds)");                                          \
			return &std_vector_at(*(_self->m_vector), index);                                      \
		}                                                                                          \
                                                                                                   \
		ConstRef(T) StdVectorIdentifier(T, iterator_crat)(                                         \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			usize index) {                                                                         \
			let _self = static_cast(StdVectorConstIterator(T)*)(self->m_self);                     \
                                                                                                   \
			std_assert(index < _self->m_vector->m_size,                                            \
					   "std_vector_iterator_at called with index pst the end of the iteration "    \
					   "(iterator index out of bounds)");                                          \
			return &std_vector_at(*(_self->m_vector), (_self->m_vector->m_size - 1) - index);      \
		}                                                                                          \
                                                                                                   \
		ConstRef(T) StdVectorIdentifier(T, iterator_ccurrent)(                                     \
			const StdRandomAccessIterator(ConstRef(T))* restrict self) {                           \
			let _self = static_cast(const StdVectorConstIterator(T)*)(self->m_self);               \
                                                                                                   \
			std_assert(                                                                            \
				_self->m_index > -1,                                                               \
				"Iterator value accessed when iterator is positioned before the beginning of the " \
				"iteration (iterator out of bounds)");                                             \
			std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_size,               \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			std_assert(static_cast(usize)(_self->m_index) < _self->m_vector->m_capacity,           \
					   "Iterator value accessed when iterator is positioned after the end of the " \
					   "iteration (iterator out of bounds)");                                      \
			return &std_vector_at(*(_self->m_vector), static_cast(usize)(_self->m_index));         \
		}                                                                                          \
                                                                                                   \
		bool StdVectorIdentifier(T, iterator_cequals)(                                             \
			const StdRandomAccessIterator(ConstRef(T))* restrict self,                             \
			const StdRandomAccessIterator(ConstRef(T))* restrict rhs) {                            \
			let _self = static_cast(const StdVectorConstIterator(T)*)(self->m_self);               \
			let _rhs = static_cast(const StdVectorConstIterator(T)*)(rhs->m_self);                 \
                                                                                                   \
			return _self->m_index == _rhs->m_index && _self->m_vector == _rhs->m_vector;           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(Ref(T))                                                            \
			StdVectorIdentifier(T, begin)(StdVector(T)* restrict self) {                           \
			let_mut iter = std_vector_into_iter(*self);                                            \
			let_mut inner = static_cast(StdVectorIterator(T)*)(iter.m_self);                       \
			inner->m_index = 0;                                                                    \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(Ref(T)) StdVectorIdentifier(T, end)(StdVector(T)* restrict self) { \
			let_mut iter = std_vector_into_iter(*self);                                            \
			let_mut inner = static_cast(StdVectorIterator(T)*)(iter.m_self);                       \
			inner->m_index = static_cast(isize)(self->m_size);                                     \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(Ref(T))                                                            \
			StdVectorIdentifier(T, rbegin)(StdVector(T)* restrict self) {                          \
			let_mut iter = std_vector_into_reverse_iter(*self);                                    \
			let_mut inner = static_cast(StdVectorIterator(T)*)(iter.m_self);                       \
			inner->m_index = static_cast(isize)(self->m_size - 1);                                 \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(Ref(T))                                                            \
			StdVectorIdentifier(T, rend)(StdVector(T)* restrict self) {                            \
			let_mut iter = std_vector_into_reverse_iter(*self);                                    \
			let_mut inner = static_cast(StdVectorIterator(T)*)(iter.m_self);                       \
			inner->m_index = -1;                                                                   \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdVectorIdentifier(T, cbegin)(const StdVector(T)* restrict self) {                    \
			let_mut iter = std_vector_into_const_iter(*self);                                      \
			let_mut inner = static_cast(StdVectorConstIterator(T)*)(iter.m_self);                  \
			inner->m_index = 0;                                                                    \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdVectorIdentifier(T, cend)(const StdVector(T)* restrict self) {                      \
			let_mut iter = std_vector_into_const_iter(*self);                                      \
			let_mut inner = static_cast(StdVectorConstIterator(T)*)(iter.m_self);                  \
			inner->m_index = static_cast(isize)(self->m_size);                                     \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdVectorIdentifier(T, crbegin)(const StdVector(T)* restrict self) {                   \
			let_mut iter = std_vector_into_reverse_const_iter(*self);                              \
			let_mut inner = static_cast(StdVectorConstIterator(T)*)(iter.m_self);                  \
			inner->m_index = static_cast(isize)(self->m_size - 1);                                 \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdRandomAccessIterator(ConstRef(T))                                                       \
			StdVectorIdentifier(T, crend)(const StdVector(T)* restrict self) {                     \
			let_mut iter = std_vector_into_reverse_const_iter(*self);                              \
			let_mut inner = static_cast(StdVectorConstIterator(T)*)(iter.m_self);                  \
			inner->m_index = -1;                                                                   \
			return iter;                                                                           \
		}                                                                                          \
                                                                                                   \
		StdString StdVectorIdentifier(T, format)(const StdFormat* restrict self,                   \
												 StdFormatSpecifier specifier) {                   \
			return StdVectorIdentifier(T, format_with_allocator)(self,                             \
																 specifier,                        \
																 std_allocator_new());             \
		}                                                                                          \
                                                                                                   \
		StdString StdVectorIdentifier(T, format_with_allocator)(                                   \
			const StdFormat* restrict self,                                                        \
			StdFormatSpecifier maybe_unused specifier,                                             \
			StdAllocator allocator) {                                                              \
                                                                                                   \
			std_assert(specifier.m_type == STD_FORMAT_TYPE_DEFAULT,                                \
					   "Can't format StdVector with custom specifier");                            \
			let _self = static_cast(const StdVector(T)*)(self->m_self);                            \
			let size = std_vector_size(*_self);                                                    \
			let capacity = std_vector_capacity(*_self);                                            \
			let data = std_vector_data(*_self);                                                    \
			let is_short = StdVectorIdentifier(T, is_short)(_self);                                \
			return std_format_with_allocator(                                                      \
				AS_STRING(StdVector(                                                               \
					T)) ": [size: {d}, capacity: {d}, data pointer: {x}, short optimized: {}]",    \
				allocator,                                                                         \
				size,                                                                              \
				capacity,                                                                          \
				as_format_t(nullptr_t, data),                                                      \
				is_short);                                                                         \
		}

/// @brief Declare `StdVector(T)` for `char`
DeclStdVector(char);
/// @brief Declare `StdVector(T)` for `u8`
DeclStdVector(u8);
/// @brief Declare `StdVector(T)` for `u16`
DeclStdVector(u16);
/// @brief Declare `StdVector(T)` for `u32`
DeclStdVector(u32);
/// @brief Declare `StdVector(T)` for `u64`
DeclStdVector(u64);
/// @brief Declare `StdVector(T)` for `usize`
DeclStdVector(usize);
/// @brief Declare `StdVector(T)` for `i8`
DeclStdVector(i8);
/// @brief Declare `StdVector(T)` for `i16`
DeclStdVector(i16);
/// @brief Declare `StdVector(T)` for `i32`
DeclStdVector(i32);
/// @brief Declare `StdVector(T)` for `i64`
DeclStdVector(i64);
/// @brief Declare `StdVector(T)` for `isize`
DeclStdVector(isize);
/// @brief Declare `StdVector(T)` for `f32`
DeclStdVector(f32);
/// @brief Declare `StdVector(T)` for `f64`
DeclStdVector(f64);
/// @brief Declare `StdVector(T)` for `u8_ptr`
DeclStdVector(u8_ptr);
/// @brief Declare `StdVector(T)` for `u16_ptr`
DeclStdVector(u16_ptr);
/// @brief Declare `StdVector(T)` for `u32_ptr`
DeclStdVector(u32_ptr);
/// @brief Declare `StdVector(T)` for `u64_ptr`
DeclStdVector(u64_ptr);
/// @brief Declare `StdVector(T)` for `usize_ptr`
DeclStdVector(usize_ptr);
/// @brief Declare `StdVector(T)` for `i8_ptr`
DeclStdVector(i8_ptr);
/// @brief Declare `StdVector(T)` for `16_ptr`
DeclStdVector(i16_ptr);
/// @brief Declare `StdVector(T)` for `i32_ptr`
DeclStdVector(i32_ptr);
/// @brief Declare `StdVector(T)` for `i64_ptr`
DeclStdVector(i64_ptr);
/// @brief Declare `StdVector(T)` for `isize_ptr`
DeclStdVector(isize_ptr);
/// @brief Declare `StdVector(T)` for `f32_ptr`
DeclStdVector(f32_ptr);
/// @brief Declare `StdVector(T)` for `f64_ptr`
DeclStdVector(f64_ptr);
/// @brief Declare `StdVector(T)` for `cstring`
DeclStdVector(cstring);
/// @brief Declare `StdVector(T)` for `char_ptr`
DeclStdVector(char_ptr);
/// @brief Declare `StdVector(T)` for `StdString`
DeclStdVector(StdString);
/// @brief Declare `StdVector(T)` for `StdStringView`
DeclStdVector(StdStringView);

#endif // STD_VECTOR
