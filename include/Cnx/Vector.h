/// @file Vector.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a dynamic-array type comparable to C++'s `std::vector` and Rust's
/// `std::vec::Vec` for Cnx
/// @version 0.2.2
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

/// @ingroup collections
/// @{
/// @defgroup cnx_vector CnxVector
/// `CnxVector(T)` is a struct template for a type-safe dynamic-capacity array. It's
/// allocator aware, provides Cnx compatible random access iterators, and supports
/// user-defined default constructors, copy-constructors, and destructors for its elements.
/// It's design is similar to both C++'s `std::vector` and Rust's `std::vec::Vec`, but with
/// slightly expanded functionality and semantics.
///
/// `CnxVector(T)` has a small size optimization (SSO) that is user configurable. This allows for a
/// compile-time configurable number of elements to be stored on the stack, before resorting to heap
/// allocation, at the cost of `sizeof(CnxVector(T))` being larger than otherwise necessary. By
/// default, this optimization allows for up to 8 elements to be stored on the stack. For
/// information on how to configure this, see "Template Parameters" below.
///
/// Instantiations of `CnxVector(T)` are already provided for builtin types like `i32` and some
/// Cnx types, like `CnxString`. Instantiating it for your own types is simple and requires
/// minimal boiler-plate.
///
/// # Instantiation requirements:
///
/// 1. a `typedef` of your type to provide an alphanumeric name for it. (for template and macro
/// 	parameters)
/// 2. a `typedef` for pointer to your type as `Ref(YourType)`, for use with the iterators.
/// 3. a `typedef` for pointer to const your type as `ConstRef(YourType)`, for use with the
/// 	iterators
/// 4. Instantiations for Cnx iterators for the typedefs provided in (2) and (3)
/// 5. Instantiation of `CnxOption(T)` for your type
///
/// We recommend two routes for providing instantiations for user-defined types, either:
/// 1. Provide the instantiation together with your type's public interface and implementation OR
/// 2. Provide it as a separate "template instantiation" .h/.c file pair
///
/// # Parameters
///
/// `CnxVector(T)` takes several instantiation-time macro parameters, in addition to the
/// instantiation-mode macro parameters required of all Cnx templates.
///
/// ## Instantiation-Mode Parameters
///
/// These signal to the implementation to instantiate the declarations, definitions, or both, for
/// the template.
/// 1. `VECTOR_DECL` (Optional) - Defining this to true signals to the implementation to
/// declare the template instantiation when you include `<Cnx/Vector.h>`. This will instantiate
/// any required type declarations and definitions and any required function declarations. No
/// functions will be defined. This is optional (but signals intent explicitly) - If required
/// template parameters are defined and `VECTOR_IMPL` is not, then this will be inferred as
/// true (`1`) by default.
/// 2. `VECTOR_IMPL` - Defining this to true signals to the implementation to define the
/// template instantiation when you include `<Cnx/Vector.h>`. This will instantiate any
/// required function definitions. This is not required to be paired with `VECTOR_DECL` (you
/// can declare the template without defining it), but it is eventually required to provide the
/// definitions for the functions. If this instantiation-mode hasn't been included in exactly one
/// translation unit in your build, you will get linking errors due to the missing function
/// definitions.
///
/// ## Template Parameters
///
/// These provide the type or value parameters that the template is parameterized on to the
/// template implementation. These should be `#define`d to their appropriate values.
/// 1. `VECTOR_T` - The type to be stored in the vector (e.g. `u32` or `CnxString`). This is
/// required.
/// 2. `VECTOR_SMALL_OPT_CAPACITY` - This is the small-optimization capacity to be stored in the
/// vector directly when the size is at or below this, instead of resorting to heap allocation. `0`
/// is a valid value for this. This is optional, and if not provided will default to
/// `CNX_VECTOR_DEFAULT_SHORT_OPT_CAPACITY` (which is defined as `8`). Heap allocations occurring
/// after size exceeds `VECTOR_SMALL_OPT_CAPACITY` will follow the growth strategy of the
/// collection.
/// 3. `VECTOR_DEFAULT_LONG_CAPACITY` - This is the initial capacity to be stored in the vector if
/// `VECTOR_SMALL_OPT_CAPACITY` is defined as `0`. This is optional. If not provided and
/// `VECTOR_SMALL_OPT_CAPACITY` is provided as `0`, this will default to
/// `CNX_VECTOR_VECTOR_DEFAULT_LONG_CAPACITY` (which is defined as `16`). If
/// `VECTOR_SMALL_OPT_CAPACITY` is defaulted or provided as greater than 0, this will not be used.
/// Heap allocations occurring after size exceeds whichever of the two possible initial storage
/// strategies are used will follow the growth strategy of the collection.
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
/// DeclCnxIterators(Ref(YourType));
/// DeclCnxIterators(ConstRef(YourType));
///
/// #define OPTION_T YourType
/// #define OPTION_DECL TRUE
/// #include <Cnx/Option.h>
/// #undef OPTION_T
/// #undef OPTION_DECL
///
/// // define the template parameter, `VECTOR_T`
/// #define VECTOR_T YourType
/// // tell the template to instantiate the declarations
/// #define VECTOR_DECL TRUE
/// // `#undef`s all macro parameters after instantiating the template,
/// // so they don't propagate around
/// #define VECTOR_UNDEF_PARAMS TRUE
/// #include <Cnx/Vector.h>
///
/// // the rest of your public interface...
///
/// // in `YourType.c`
/// #define OPTION_T YourType
/// #define OPTION_IMPL TRUE
/// #include <Cnx/Option.h>
/// #undef OPTION_T
/// #undef OPTION_IMPL
///
/// // define the template parameter, `VECTOR_T`
/// #define VECTOR_T YourType
/// // tell the template to instantiate the implementations
/// #define VECTOR_IMPL TRUE
/// // `#undef`s all macro parameters after instantiating the template,
/// // so they don't propagate around
/// #define VECTOR_UNDEF_PARAMS TRUE
/// #include <Cnx/Vector.h>
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
/// DeclCnxIterators(Ref(YourType));
/// DeclCnxIterators(ConstRef(YourType));
/// // the rest of your public interface
///
/// // in `CnxOptionYourType.h`
/// #include "YourType.h"
/// #define OPTION_T YourType
/// #define OPTION_DECL TRUE
/// #include <Cnx/Option.h>
/// #undef OPTION_T
/// #undef OPTION_DECL
///
/// // in `CnxOptionYourType.c`
/// #include "CnxOptionYourType.h"
///
/// #define OPTION_T YourType
/// #define OPTION_IMPL TRUE
/// #include <Cnx/Option.h>
/// #undef OPTION_T
/// #undef OPTION_IMPL
///
/// // in `CnxVectorYourType.h`
/// #include "YourType.h"
/// #include "CnxOptionYourType.h"
///
/// // define the template parameter, `VECTOR_T`
/// #define VECTOR_T YourType
/// // tell the template to instantiate the declarations
/// #define VECTOR_DECL TRUE
/// // `#undef`s all macro parameters after instantiating the template,
/// // so they don't propagate around
/// #define VECTOR_UNDEF_PARAMS TRUE
/// #include <Cnx/Vector.h>
///
/// // in `CnxVectorYourType.c`
/// #include "CnxVectorYourType.h"
///
/// // define the template parameter, `VECTOR_T`
/// #define VECTOR_T YourType
/// // tell the template to instantiate the implementations
/// #define VECTOR_IMPL TRUE
/// // `#undef`s all macro parameters after instantiating the template,
/// // so they don't propagate around
/// #define VECTOR_UNDEF_PARAMS TRUE
/// #include <Cnx/Vector.h>
/// @endcode
///
/// Then, somewhere else in your codebase, you can easily use the instantiation:
///
/// @code {.c}
/// #include "YourType.h"
/// // if instantiation method (2)
/// #include "CnxVectorYourType.h"
///
/// CnxVector(YourType) create_and_fill_your_type_vec(usize num_elements) {
///		let_mut vec = cnx_vector_new_with_capacity(YourType, num_elements);
///
///		cnx_vector_resize(vec, num_elements);
///
///		return vec;
/// }
/// @endcode
///
/// `CnxVector(T)` implements `CnxFormat`, but because of its generic type, can't be automatically
/// converted to its `CnxFormat` implementation, so we have to cast it explicitly:
///
/// @code {.c}
/// #include "Cnx/IO.h"
/// void print_vec(const CnxVector(YourType)* vec) {
///		println("{}", as_format_t(CnxVector(YourType), *vec));
/// }
/// @endcode
///
/// @note `CnxVector(T)`'s `CnxFormat` implementation will always be a debug representation,
/// eg: "[size: X, capacity: Y, data_ptr: Q]", and never a printout of contained elements

/// You can provide user-defined default-constructor, copy-constructor, and destructor for
/// elements of your type, and custom allocator for all memory allocation:
///
/// @code {.c}
/// YourType your_type_constructor(CnxAllocator allocator) {
///		let_mut t = (YourType){0};
/// 	// whatever you need to do to init your type
/// 	return t;
/// }
///
/// YourType your_type_copy_constructor(const YourType* restrict elem, CnxAllocator allocator) {
///		let_mut t = (YourType){0};
/// 	// whatever you need to do to copy `elem` to `t`
/// 	return t;
/// }
///
/// void your_type_destructor(YourType* restrict t, CnxAllocator allocator) {
///		// whatever you need to do to cleanup `t`
/// }
///
/// CnxVector(YourType) create_and_fill_your_type_vec(usize num_elements) {
///		// init a vec with num_elements capacity (zero size) and our custom collection data
///		let_mut vec = cnx_vector_new_with_capacity_allocator_and_collection_data(YourType,
/// 						capacity,
///							// This `CnxAllocator` will only be associated with this specific array,
///							// not all `CnxArray(YourType, YourN)`s
///							cnx_allocator_from_custom_stateless_allocator(your_malloc,
/// 																	  your_realloc,
/// 																	  your_free),
///							// This `CnxCollectionData(CollectionType)` will only be
/// 						// associated with this specific vector, not all `CnxVector(YourType)`s
///							((CnxCollectionData(CnxVector(YourType))) {
///								// can be left NULL, will be defaulted
///								.m_constructor = your_type_constructor,
///								// can be left NULL, will disable `cnx_vector_clone(self)` for
/// 							// this vector
/// 							.m_copy_constructor = your_type_copy_constructor,
///								// can be left NULL, will be defaulted
/// 							.m_destructor = your_type_destructor
/// 						})
///						);
///
///		// fill the vec with default constructed elements
///		cnx_vector_resize(vec, num_elements);
///
///		return vec;
/// }
/// @endcode
///
/// `CnxVector(T)` also provides Cnx random access iterators and optional scoped destruction:
///
/// @code {.c}
/// void example(void) {
/// 	let_mut cnx_vector_scoped(u32) vec = cnx_vector_new_with_capacity(u32, 10);
/// 	ranged_for(i, 0, 9) {
///			cnx_vector_push_back(vec, i);
/// 	}
///
///		// loop over the elements in the vector by value, as an iteration
///		foreach(elem, vec) {
///			println("{}", elem);
/// 	}
///		// vec goes out of scope here and `cnx_vector_free(vec)` is called on it
/// 	// because we declared it as scoped
/// }
/// @endcode
///
/// Like other Cnx collections, `CnxVector(T)` provides its type-agnostic usage through a vtable
/// pointer contained in the struct, and provides macros which wrap the usage of the vtable, making
/// access simpler. If you prefer to not use this method of access, you can call the
/// typed functions directly by in-fixing the contained type in the associated function name.
/// IE: for `CnxVector(i32)`, `vec`, the equivalent function call for
/// `cnx_vector_push_back(vec, element)` would be `cnx_vector_i32_push_back(&vec, element)`
/// @}

#include <Cnx/vector/VectorDef.h>

#if !defined(VECTOR_DECL) && (!defined(VECTOR_IMPL) || !VECTOR_IMPL) && defined(VECTOR_T)
	#define VECTOR_DECL 1
#endif // !defined(VECTOR_DECL) && (!defined(VECTOR_IMPL) || !VECTOR_IMPL) &&
	   // defined(T)

#if(defined(VECTOR_DECL) || defined(VECTOR_IMPL)) && !defined(VECTOR_SMALL_OPT_CAPACITY)
	#define VECTOR_SMALL_OPT_CAPACITY CNX_VECTOR_DEFAULT_SHORT_OPT_CAPACITY
#endif // (defined(VECTOR_DECL) || defined(VECTOR_IMPL)) && !defined(VECTOR_SMALL_OPT_CAPACITY)

#if(defined(VECTOR_DECL) || defined(VECTOR_IMPL))                               \
	&& (defined(VECTOR_SMALL_OPT_CAPACITY) && (VECTOR_SMALL_OPT_CAPACITY == 0)) \
	&& !defined(VECTOR_DEFAULT_LONG_CAPACITY)
	#define VECTOR_DEFAULT_LONG_CAPACITY CNX_VECTOR_VECTOR_DEFAULT_LONG_CAPACITY
#endif // (defined(VECTOR_DECL) || defined(VECTOR_IMPL))     \
	   // && (defined(VECTOR_SMALL_OPT_CAPACITY) && (VECTOR_SMALL_OPT_CAPACITY == 0)) \
	   // && !defined(VECTOR_DEFAULT_LONG_CAPACITY)

#if !defined(VECTOR_T) && VECTOR_DECL && !VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS
	#error Vector.h included with VECTOR_DECL defined true but template parameter VECTOR_T not defined
#endif // !defined(T) && VECTOR_DECL && !VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS

#if !defined(VECTOR_T) && VECTOR_IMPL && !VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS
	#error Vector.h included with VECTOR_IMPL defined true but template parameter VECTOR_T not defined
#endif // !defined(T) && VECTOR_IMPL &&!VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS

#if defined(VECTOR_T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_DECL \
	&& !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <Cnx/vector/VectorDecl.h>
#endif // defined(T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_DECL &&
	   // !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(VECTOR_T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_IMPL \
	&& !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <Cnx/vector/VectorImpl.h>
#endif // defined(T) && defined(VECTOR_SMALL_OPT_CAPACITY) && VECTOR_IMPL &&
	   // !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(VECTOR_T) \
	&& !defined(VECTOR_SMALL_OPT_CAPACITY) && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

	#ifndef CNX_VECTOR
		/// @brief `CnxVector(T)` related declarations and definitions
		#define CNX_VECTOR

		#include <Cnx/Option.h>
		#include <Cnx/String.h>

		#define VECTOR_SMALL_OPT_CAPACITY 8
		#define VECTOR_DECL				  TRUE

		#define VECTOR_T char
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T u8
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T u16
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T u32
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T u64
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T usize
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T i8
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T i16
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T i32
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T i64
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T isize
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T f32
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T f64
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T cstring
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T CnxString
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#define VECTOR_T CnxStringView
		#include <Cnx/vector/VectorDecl.h>
		#undef VECTOR_T

		#undef VECTOR_SMALL_OPT_CAPACITY
		#undef VECTOR_DECL

	#endif // CNX_VECTOR

#endif // VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS && !defined(VECTOR_T)                      \
	   // && !defined(VECTOR_SMALL_OPT_CAPACITY) && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if VECTOR_UNDEF_PARAMS
	#undef VECTOR_T
	#undef VECTOR_SMALL_OPT_CAPACITY
	#undef VECTOR_DECL
	#undef VECTOR_IMPL
#endif // VECTOR_UNDEF_PARAMS
