/// @file StdVector.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a dynamic-array type comparable to C++'s `std::vector` and Rust's
/// `std::vec::Vec` for C2nxt
/// @version 0.2
/// @date 2022-01-08
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
/// 1. a `typedef` of your type to provide an alphanumeric name for it. (for template and macro
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
/// // define the template parameter, `T`
/// #define T YourType
/// // tell the template to instantiate the declarations
/// #define STD_TEMPLATE_DECL 1
/// // `#undef`s `T` and `STD_TEMPLATE_DECL` after instantiating the template,
/// // so they don't propagate around
/// #define STD_TEMPLATE_UNDEF_PARAMS 1
/// #include <C2nxt/StdVector.h>
///
/// // the rest of your public interface...
///
/// // in `YourType.c`
/// ImplStdOption(YourType);
/// // define the template parameter, `T`
/// #define T YourType
/// // tell the template to instantiate the implementations
/// #define STD_TEMPLATE_IMPL 1
/// // `#undef`s `T` and `STD_TEMPLATE_DECL` after instantiating the template,
/// // so they don't propagate around
/// #define STD_TEMPLATE_UNDEF_PARAMS 1
/// #include <C2nxt/StdVector.h>
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
/// // define the template parameter, `T`
/// #define T YourType
/// // tell the template to instantiate the declarations
/// #define STD_TEMPLATE_DECL 1
/// // `#undef`s `T` and `STD_TEMPLATE_DECL` after instantiating the template,
/// // so they don't propagate around
/// #define STD_TEMPLATE_UNDEF_PARAMS 1
/// #include <C2nxt/StdVector.h>
///
/// // in `StdVectorYourType.c`
/// #include "StdVectorYourType.c"
/// // define the template parameter, `T`
/// #define T YourType
/// // tell the template to instantiate the implementations
/// #define STD_TEMPLATE_IMPL 1
/// // `#undef`s `T` and `STD_TEMPLATE_DECL` after instantiating the template,
/// // so they don't propagate around
/// #define STD_TEMPLATE_UNDEF_PARAMS 1
/// #include <C2nxt/StdVector.h>
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
/// typed functions directly by in-fixing the contained type in the associated function name.
/// IE: for `StdVector(i32)`, `vec`, the equivalent function call for
/// `std_vector_push_back(vec, element)` would be `std_vector_i32_push_back(&vec, element)`
/// @}

#include <C2nxt/StdAllocators.h>
#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdCollectionsData.h>
#include <C2nxt/StdFormat.h>
#include <C2nxt/StdIterator.h>
#include <C2nxt/StdMath.h>
#include <C2nxt/StdOption.h>
#include <C2nxt/StdPlatform.h>
#include <C2nxt/StdResult.h>
#include <C2nxt/std_vector/StdVectorDef.h>

#if !defined(STD_TEMPLATE_DECL) && (!defined(STD_TEMPLATE_IMPL) || !STD_TEMPLATE_IMPL) && defined(T)
	#define STD_TEMPLATE_DECL 1
#endif // !defined(STD_TEMPLATE_DECL) && (!defined(STD_TEMPLATE_IMPL) || !STD_TEMPLATE_IMPL) &&
	   // defined(T)

#if !defined(T) && STD_TEMPLATE_DECL
	#error StdVector.h included with STD_TEMPLATE_DECL defined true but template parameter T not defined
#endif // !defined(T) && STD_TEMPLATE_DECL

#if !defined(T) && (defined(STD_TEMPLATE_IMPL) && STD_TEMPLATE_IMPL)
	#error StdVector.h included with STD_TEMPLATE_IMPL defined true but template parameter T not defined
#endif // !defined(T) && (defined(STD_TEMPLATE_IMPL) && STD_TEMPLATE_IMPL)

#if defined(T) && STD_TEMPLATE_DECL
	#include <C2nxt/std_vector/StdVectorDecl.h>
#endif // defined(T) && STD_TEMPLATE_DECL

#if defined(T) && STD_TEMPLATE_IMPL
	#include <C2nxt/std_vector/StdVectorImpl.h>
#endif // defined(T) && STD_TEMPLATE_IMPL

#if defined(STD_TEMPLATE_UNDEF_PARAMS) && STD_TEMPLATE_UNDEF_PARAMS
	#undef T
	#undef STD_TEMPLATE_DECL
	#undef STD_TEMPLATE_IMPL
#endif // defined(STD_TEMPLATE_UNDEF_PARAMS) && STD_TEMPLATE_UNDEF_PARAMS

#if !defined(T) && !defined(STD_TEMPLATE_DECL) && !defined(STD_TEMPLATE_IMPL) \
	&& !defined(STD_TEMPLATE_UNDEF_PARAMS)
	#ifndef STD_VECTOR
		/// @brief `StdVector(T)` related declarations and definitions
		#define STD_VECTOR

		#define T				  char
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u8
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u16
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u32
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u64
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  usize
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i8
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i16
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i32
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i64
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  isize
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  f32
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  f64
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u8_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u16_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u32_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  u64_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  usize_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i8_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i16_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i32_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  i64_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  isize_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  f32_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  f64_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  cstring
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  char_ptr
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  StdString
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL

		#define T				  StdStringView
		#define STD_TEMPLATE_DECL 1
		#include <C2nxt/std_vector/StdVectorDecl.h>
		#undef T
		#undef STD_TEMPLATE_DECL
	#endif // STD_VECTOR

#endif // !defined(T) && !defined(STD_TEMPLATE_DECL) && !defined(STD_TEMPLATE_IMPL) \
	   // && !defined(STD_TEMPLATE_UNDEF_PARAMS)
