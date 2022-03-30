/// @file StdUniquePtr.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing a uniquely owned pointer
/// @version 0.2.0
/// @date 2022-03-29
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

/// @ingroup memory
/// @{
/// @defgroup std_unique_ptr StdUniquePtr
/// `StdUniquePtr(T)` is a struct template similar to C++'s `std::unique_ptr` or Rust's `Box` for a
/// "smart" pointer type that maintains (when used correctly) unique ownership of, and manages an
/// object through, a contained pointer. The contained object is disposed of ("freed") when the
/// `StdUniquePtr(T)` is explicitly freed, when it goes out of scope when declared with the
/// `unique_scoped(T)` tagged declaration, or when `reset` (`std_unique_ptr_reset`) is called on
/// the `StdUniquePtr(T)`.
///
/// `StdUniquePtr(T)` is allocator aware and stores its associated allocator in itself.
///
/// The contained object is "freed" by a `StdDeleter`, a function taking a `T*` and a
/// `StdAllocator`, whose task is to properly cleanup/free/etc. anything associated with the `T`.
/// This deleter can be supplied as a template parameter, `UNIQUE_DELETER` when instantiating the
/// `StdUniquePtr(T)` template. This template parameter is optional, and if not supplied the deleter
/// used will be the default deleter, which will simply deallocate the `T` with the allocator
/// associated with the given `StdUniquePtr(T)` instance.
///
/// `StdUniquePtr(T)` can own dynamically allocated array types, and provided functionality will be
/// slightly different, though comparable, when `T` is an array type vs a singular type.
/// To instantiate the template for an array type, the type must be `typedef`ed,
/// e.g.: ```typedef int int_arr[];```, and the typedef provided as the `UNIQUE_T` template
/// parameter.
///
/// # Instantiation Requirements:
///
/// 1. a `typedef` of your type to to provide an alphanumeric name for it (so it can be used in
/// template and macro parameters)
///
/// We recommend two routes for providing instantations for user-defined types, either:
/// 1. Provide the instantiation together with your type's public interface and implementation OR
/// 2. Provide it as a separate "template instantiation" .h/.c file pair.
///
/// We recommend option one when your type is typically used in tandem with the template
/// instantation or if it requires it for its public interface.
/// We recommend option two for all other circumstances, so that users don't pay the price
/// (compile time, and potentially code size) of template instantiations if they don't use them.
///
/// ## Instantiation Mode Parameters
///
/// These signal to the implementation to instantiation the declarations, definitions, or both, for
/// the template.
/// 1. `UNIQUE_DECL` (Optional) - Defining this to true signals to the implementation to declare the
/// required type declarations and definitions, and any required or public function declarations
/// when you include `<C2nxt/StdUniquePtr.h>`. No functions will be defined. This is optional
/// (but signals intent explicitly) - If required template parameters are defined and `UNIQUE_DECL`
/// is not, then this will be inferred as true by default.
/// 2. `UNIQUE_IMPL` - Defining this to true signals to the implementation to define the functions
/// necessary for the template instantation when you include `<C2nxt/StdUniquePtr.h>`. This is not
/// required to be paired with `UNIQUE_DECL` (you can declare the template, in a header for example,
/// prior to defining it in a source file), but it is eventually required to be used in exactly one
/// translation unit in the resulting binary in order to provide definitions for the template's
/// functions. Failing to complete this instantiation-mode exactly once in exactly one translation
/// unit in your build will cause linking errors due to missing or multiply defined functions.
///
/// ## Template Parameters
///
/// These provide the type or value parameters that the template is parameterized on to the template
/// implementation. These should be `#define`d to their appropriate types/values.
/// 1. `UNIQUE_T` - The type to be stored and managed in the unique_ptr (e.g. `u32` or `StdString`).
/// This is required.
/// 2. `UNIQUE_DELETER` - The deleter function to free the memory associated with the managed type
/// and perform any necessary associated cleanup.
/// This function must take a particular signature:
/// For a type `Y` that evaluates to either:
/// 1. `T` (`UNIQUE_T`) if `T` is __NOT__ an array type
/// 2. The element type of the array type `T` if `T` __IS__ an array type.
/// e.g. for an array type `T`, and an instance `t`, `Y` would be `typeof(t[0])`
///
/// The signature must be `void name(Y* object, StdAllocator allocator);`
/// where `object` is a pointer to the managed object and `allocator` is the allocator associated
/// with the object (the one that allocated it, and will free it).
///
/// Example using instantiation method one:
/// @code {.c}
/// // in MyStruct.h
///
/// // include for `TRUE` (could just use `1`)
/// #include <C2nxt/StdDef.h>
/// // include for `i32`
/// #include <C2nxt/StdBasicTypes.h>
///
/// typedef struct {
/// 	i32 one;
/// 	i32 two;
/// 	i32 three;
/// } MyStruct;
///
/// // instantiate `StdUniquePtr(MyStruct)` declaration
/// #define UNIQUE_T MyStruct
/// #define UNIQUE_DECL TRUE
/// #define UNIQUE_UNDEF_PARAMS TRUE
/// #include <C2nxt/StdUniquePtr.h>
/// #undef UNIQUE_UNDEF_PARAMS
///
/// // in MyStruct.c
///
/// #include "MyStruct.h"
///
/// // instantiate `StdUniquePtr(MyStruct)` declaration
/// #define UNIQUE_T MyStruct
/// #define UNIQUE_IMPL TRUE
/// #define UNIQUE_UNDEF_PARAMS TRUE
/// #include <C2nxt/StdUniquePtr.h>
/// #undef UNIQUE_UNDEF_PARAMS
///
/// void takes_ownership(StdUniquePtr(MyStruct) to_take);
///
/// void example(void) {
/// 	// create a uniquely owned `MyStruct`
/// 	unique_scoped(MyStruct) my_ptr = std_make_unique(MyStruct, .one = 2, .two = 4, .three = 1);
///
/// 	// transfer ownership of the `MyStruct` object managed by `my_ptr` to `takes_ownership`
/// 	// `my_ptr` now contains `nullptr`
/// 	takes_ownership(ptr_move(my_ptr));
///
/// 	// aside: `ptr_move` from is nearly equivalent to `move`, but it ensures that the contained
/// 	// pointer is set to the correct `nullptr` constant (`NULL` is not necessarily 0 on all
/// 	// platforms, and `move` is implemented with zero-setting on the moved-from value).
/// 	// on platforms where `NULL` is guaranteed to be `0`, you can safely use `move` instead of
/// 	// `ptr_move`
/// }
/// @endcode
///
/// Example using instantiation method two:
/// @code {.c}
/// // in MyStruct.h
///
/// // include for `i32`
/// #include <C2nxt/StdBasicTypes.h>
///
/// typedef struct {
/// 	i32 one;
/// 	i32 two;
/// 	i32 three;
/// } MyStruct;
///
///
/// // in StdUniquePtrMyStruct.h
///
/// // include for `TRUE` (could just use `1`)
/// #include <C2nxt/StdDef.h>
///
/// #include "MyStruct.h"
///
/// // instantiate `StdUniquePtr(MyStruct)` declaration
/// #define UNIQUE_T MyStruct
/// #define UNIQUE_DECL TRUE
/// #define UNIQUE_UNDEF_PARAMS TRUE
/// #include <C2nxt/StdUniquePtr.h>
/// #undef UNIQUE_UNDEF_PARAMS
///
/// // in StdUniquePtrMyStruct.c
///
/// #include "StdUniquePtrMyStruct.h"
///
/// // instantiate `StdUniquePtr(MyStruct)` declaration
/// #define UNIQUE_T MyStruct
/// #define UNIQUE_IMPL TRUE
/// #define UNIQUE_UNDEF_PARAMS TRUE
/// #include <C2nxt/StdUniquePtr.h>
/// #undef UNIQUE_UNDEF_PARAMS
///
/// // in MyStruct.c
///
/// void takes_ownership(StdUniquePtr(MyStruct) to_take);
///
/// void example(void) {
/// 	// create a uniquely owned `MyStruct`
/// 	unique_scoped(MyStruct) my_ptr = std_make_unique(MyStruct, .one = 2, .two = 4, .three = 1);
///
/// 	// transfer ownership of the `MyStruct` object managed by `my_ptr` to `takes_ownership`
/// 	// `my_ptr` now contains `nullptr`
/// 	takes_ownership(ptr_move(my_ptr));
///
/// 	// aside: `ptr_move` from is nearly equivalent to `move`, but it ensures that the contained
/// 	// pointer is set to the correct `nullptr` constant (`NULL` is not necessarily 0 on all
/// 	// platforms, and `move` is implemented with zero-setting on the moved-from value).
/// 	// on platforms where `NULL` is guaranteed to be `0`, you can safely use `move` instead of
/// 	// `ptr_move`
/// }
/// @endcode
///
/// Like other C2nxt templates, `StdUniquePtr(T)` provides its type-agnostic usage through a vtable
/// pointer contained in the struct, and provides macros which wrap usage of the vtable, making
/// access simpler. If you prefer to not use this method of access, you can call the typed functions
/// directly by in-fixing the contained type in the associated function name.
/// IE: for a `StdUniquePtr(i32)`, `i32_ptr`, the equivalent function call for
/// `std_unique_ptr_release` would be `std_unique_ptr_i32_release(&i32_ptr)`.
///
/// @note `StdUniquePtr(T)` uses a combination of Clang's `diagnose_if` attribute, static_asserts,
/// and runtime asserts to disable the use of functions that only apply to non-array or array-type
/// `T`s, respectively, when `T` is not the correct type category. This provides compile-time errors
/// in as many situations as possible. However, when compiling with something other than Clang
/// forgoing the macros bypasses the only compile-time checks available (static_asserts), resulting
/// in runtime asserts being the only defense against invoking undefined behavior from calling an
/// array-associated function for a `StdUniquePtr(T)` instantiation where `T` is not an array-type.
/// Because of this, it's particularly discouraged to forgo the function-like macros for
/// `StdUniquePtr(T)`, or any C2nxt smart pointer, for that matter.
/// @}

#include <C2nxt/StdDef.h>
#include <C2nxt/std_unique_ptr/StdUniquePtrDef.h>

#if !defined(UNIQUE_DECL) && (!defined(UNIQUE_IMPL) || !UNIQUE_IMPL) && defined(UNIQUE_T)
	#define UNIQUE_DECL TRUE
#endif // !defined(UNIQUE_DECL) && (!defined(UNIQUE_IMPL) || !UNIQUE_IMPL) && defined(UNIQUE_T)

#if !defined(UNIQUE_DELETER) && (UNIQUE_DECL || UNIQUE_IMPL)
	#define UNIQUE_DELETER			   StdUniquePtrIdentifier(UNIQUE_T, default_deleter)
	#define __UNIQUE_DEFAULTED_DELETER TRUE
#endif // !defined(UNIQUE_DELETER) && UNIQUE_DECL || UNIQUE_IMPL

#if !defined(UNIQUE_T) && UNIQUE_DECL
	#error \
		"StdUniquePtr.h included with UNIQUE_DECL defined true but template parameter UNIQUE_T \
			not defined "
#endif // !defined(UNIQUE_T) && UNIQUE_DECL

#if !defined(UNIQUE_T) && UNIQUE_IMPL
	#error \
		"StdUniquePtr.h included with UNIQUE_IMPL defined true but template parameter UNIQUE_T \
			not defined "
#endif // !defined(UNIQUE_T) && UNIQUE_IMPL

#if defined(UNIQUE_T) && UNIQUE_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_unique_ptr/StdUniquePtrDecl.h>
#endif // defined(UNIQUE_T) && UNIQUE_DECL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(UNIQUE_T) && UNIQUE_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <C2nxt/std_unique_ptr/StdUniquePtrImpl.h>
#endif // defined(UNIQUE_T) && UNIQUE_IMPL && !STD_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if UNIQUE_UNDEF_PARAMS
	#undef UNIQUE_T
	#undef UNIQUE_DELETER
	#undef UNIQUE_DECL
	#undef UNIQUE_IMPL
#endif // UNIQUE_UNDEF_PARAMS

#if __UNIQUE_DEFAULTED_DELETER
	#undef UNIQUE_DELETER
	#undef __UNIQUE_DEFAULTED_DELETER
#endif // __UNIQUE_DEFAULTED_DELETER
