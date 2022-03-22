/// @file StdArray.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a generic, type-safe, fixed-capacity, dynamic-size, stack allocated
/// array type (`StdArray(T, N)`) similar to `std::array` in C++
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

#include <C2nxt/std_array/StdArrayDef.h>

#if !defined(ARRAY_DECL) && (!defined(ARRAY_IMPL) || !ARRAY_IMPL) && defined(ARRAY_T) \
	&& defined(ARRAY_N)
	#define ARRAY_DECL TRUE
#endif // !defined(ARRAY_DECL) && (!defined(ARRAY_IMPL) || !ARRAY_IMPL) && defined(ARRAY_T) &&
	   // defined(ARRAY_N)

#if defined(ARRAY_DECL) && (!defined(ARRAY_T) || !defined(ARRAY_N))
	#error "StdArray.h included with ARRAY_DECL defined true but one or more template parameters\
			(ARRAY_T or ARRAY_N) not defined"
#endif // (defined(ARRAY_DECL) || defined(ARRAY_IMPL)) && (!defined(ARRAY_T) || !defined(ARRAY_N))

#if defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_array/StdArrayDecl.h>
#endif // defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_DECL &&
	   // !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_array/StdArrayImpl.h>
#endif // defined(ARRAY_T) && defined(ARRAY_N) && ARRAY_DECL &&
// !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if ARRAY_UNDEF_PARAMS
	#undef ARRAY_T
	#undef ARRAY_N
	#undef ARRAY_DECL
	#undef ARRAY_IMPL
#endif // ARRAY_UNDEF_PARAMS
