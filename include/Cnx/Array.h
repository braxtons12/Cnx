/// @file Array.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a generic, type-safe, fixed-capacity, dynamic-size, stack allocated
/// array type (`CnxArray(T, N)`) similar to `std::array` in C++
/// @version 0.2.0
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
/// @defgroup cnx_array CnxArray
/// `CnxArray(T, N)` is a struct template for a type-safe fixed-capacity, dynamic-size, stack
/// allocated array type. `CnxArray(T, N)` is bounds safe, allocator aware, and provides Cnx
/// compatible random access iterators. It supports user-defined default-constructors,
/// copy-constructors, and destructors for its elements. It's design is similar to C++'s
/// `std::array`, but with expanded semantics and functionality.
///
/// Because of its size, `N`, template parameter, instantiations of `CnxArray(T, N)` for builtins or
/// any Cnx types are __not__ provided, and will have to be instantiated by the user.
/// The requirements are similar to `CnxVector(T)`, and requires minimal boiler-plate:
/// 1. a `typedef` of your type to provide an alphanumeric name for it. (for template and macro
/// 	parameters)
///	2. a `typedef` for pointer to your type as `Ref(YourType), for use with the iterators
/// 3. a `typedef` for pointer to const your type as `ConstRef(YourType)`, for use with the
/// 	iterators
/// 4. Instantiations for Cnx iterators for the typedefs provided in (2) and (3) (already
/// 	instantiated for builtins and `CnxString`)
/// 5. Instantiation of `CnxOption(T)` for your type (already instantiated for builtins and
/// 	`CnxString`)
///
/// For `CnxArray(T, N)` we recommend you provide the instantiation for user-defined types in a
/// separate "template instantiation" .h/.c file pair, but you can also provide them inline with
/// your type like discussed in `CnxVector(T)`'s documentation. \see CnxVector
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
/// DeclCnxIterators(Ref(YourType));
/// DeclCnxIterators(ConstRef(YourType));
/// // the rest of your public interface
///
/// // in `CnxOptionYourType.h`
/// #include <Cnx/Def.h>
/// #include "YourType.h"
/// #define OPTION_T YourType
/// #define OPTION_DECL TRUE
/// #include <Cnx/Option.h>
/// #undef OPTION_T
/// #undef OPTION_DECL
///
/// // in `CnxOptionYourType.c`
/// #include "CnxOptionYourType.h"
/// #define OPTION_T YourType
/// #define OPTION_IMPL TRUE
/// #include <Cnx/Option.h>
/// #undef OPTION_T
/// #undef OPTION_IMPL
///
/// // in `CnxArrayYourTypeYourN.h"
/// #include "YourType.h"
/// #include "CnxOptionYourType.h"
///
/// #define ARRAY_T YourType
/// #define ARRAY_N YourN
/// #define ARRAY_DECL TRUE
/// #include <Cnx/Array.h>
/// #undef ARRAY_T
/// #undef ARRAY_N
/// #undef ARRAY_DECL
///
/// // in `CnxArrayYourTypeYourN.c`
/// #include "CnxArrayYourTypeYourN.h"
///
/// #define ARRAY_T YourType
/// #define ARRAY_N YourN
/// #define ARRAY_IMPL TRUE
/// #include <Cnx/Array.h>
/// #undef ARRAY_T
/// #undef ARRAY_N
/// #undef ARRAY_IMPL
/// @endcode
///
/// The, somewhere else in your codebase, you can easily use the instantiation:
///
/// @code {.c}
/// #include "YourType.h"
/// #include "CnxArrayYourTypeYourN.h"
///
/// CnxArray(YourType, YourN) create_and_fill_your_type_array(void) {
///		let_mut array = cnx_array_new(YourType, YourN);
///
///		cnx_array_resize(array, YourN);
///
/// 	return array;
/// }
/// @endcode
///
/// `CnxArray(T, N)` implements `CnxFormat`, but because of its generic type, can't be automatically
/// converted to its `CnxFormat` implementation, so we have to cast to it explicitly:
///
/// @code {.c}
/// #include "Cnx/IO.h"
/// void print_array(const CnxArray(YourType, YourN)* array) {
///		println("{}", as_format_t(CnxArray(YourType, YourN), *array);
/// }
/// @endcode
///
/// @note `CnxArray(T, N)`'s `CnxFormat` implementation will always be a debug representation,
/// eg: "[size: X, capacity: Y, data_ptr: Q]", and never a printout of contained elements
///
/// You can provide user-defined default-constructor, copy-constructor, and destructor for
/// elements of your type, and custom allocator for memory allocations they might need to perform.
/// (`CnxArray(T, N)` is always stack allocated, so the memory allocator is __only__ for constructor
/// and destructor functions for your type)
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
///		return t;
/// }
///
/// void your_type_destructor(YourType* restrict t, CnxAllocator allocator) {
///		// whatever you need to do to cleanup `t`
/// }
///
/// CnxArray(YourType, YourN) create_and_fill_your_type_array(void) {
///		let_mut array = cnx_array_new_with_allocator_and_collection_data(YourType,
/// 						YourN,
///							// This `CnxAllocator` will only be associated with this specific array,
///							// not all `CnxArray(YourType, YourN)`s
///							cnx_allocator_from_custom_stateless_allocator(your_malloc,
/// 																	  your_realloc,
/// 																	  your_free),
///							// This `CnxCollectionData(CollectionType)` will only be
/// 						// associated with this specific array, not all
/// 						// `CnxArray(YourType, YourN)`s
///							((CnxCollectionData(CnxArray(YourType, YourN))) {
///								// can be left NULL, will be defaulted
///								.m_constructor = your_type_constructor,
///								// can be left NULL, will disable `cnx_array_clone(self)` for
/// 							// this array
/// 							.m_copy_constructor = your_type_copy_constructor,
///								// can be left NULL, will be defaulted
/// 							.m_destructor = your_type_destructor
/// 						})
///						);
///
/// 	// fill the array with default constructed elements
/// 	cnx_array_resize(array, YourN);
///
/// 	return array;
/// }
/// @endcode
///
/// `CnxArray(T, N)` also provides Cnx random access iterators and optional scoped destruction:
///
/// @code {.c}
/// void example(void) {
///		cnx_array_scoped(u32, 10) array = cnx_array_new(u32, 10);
/// 	ranged_for(i, 0, 9) {
///			cnx_array_push_back(array, i);
/// 	}
///
/// 	// loop over the elements in the array by value, as an iteration
///		foreach(elem, vec) {
///			println("{}", elem);
/// 	}
///		// array goes out of scope here and `cnx_array_free(array)` is called on it
/// 	// because we declared it as scoped
/// }
/// @endcode
///
/// Like other Cnx collections, `CnxArray(T, N)` provides its type-agnostic usage through a
/// vtable pointer contained in the struct, and provides macros which wrap the usage of the vtable,
/// making access simpler. If you prefer to not use this method of access, you can call the typed
/// functions directly by in-fixing the contained type in the associated function name.
/// IE: for `CnxArray(i32, 10)`, and an array `array`, the equivalent function call for
/// `cnx_array_push_back(array, element)` would be `cnx_array_i32_10_push_back(&array, element)`
/// @note Note, however, that using the typed functions instead of the macros results in losing the
/// reference-semantics of function-macros like `cnx_array_at(self, index)` and will instead be
/// dealing with pointers directly.
/// Example:
/// @code {.c}
/// // instead of:
/// cnx_array_at_mut(self, 5) = 5;
/// // you would have to do:
/// *cnx_array_i32_10_at_mut(&self, 5) = 5;
/// @endcode
/// @}

#include <Cnx/array/ArrayDef.h>

#if !defined(ARRAY_DECL) && (!defined(ARRAY_IMPL) || !ARRAY_IMPL) && defined(ARRAY_T) \
	&& defined(ARRAY_N)
	#define ARRAY_DECL TRUE
#endif // !defined(ARRAY_DECL) && (!defined(ARRAY_IMPL) || !ARRAY_IMPL) && defined(ARRAY_T) &&
	   // defined(ARRAY_N)

#if defined(ARRAY_DECL) && (!defined(ARRAY_T) || !defined(ARRAY_N))
	#error "CnxArray.h included with ARRAY_DECL defined true but one or more template parameters\
			(ARRAY_T or ARRAY_N) not defined"
#endif // (defined(ARRAY_DECL) || defined(ARRAY_IMPL)) && (!defined(ARRAY_T) || !defined(ARRAY_N))

#if defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_DECL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <Cnx/array/ArrayDecl.h>
#endif // defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_DECL &&
	   // !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_IMPL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <Cnx/array/ArrayImpl.h>
#endif // defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_DECL &&
// !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if ARRAY_UNDEF_PARAMS
	#undef ARRAY_T
	#undef ARRAY_N
	#undef ARRAY_DECL
	#undef ARRAY_IMPL
#endif // ARRAY_UNDEF_PARAMS
