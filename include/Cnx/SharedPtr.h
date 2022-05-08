/// @file SharedPtr.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides a struct template for representing a sharedly owned pointer
/// @version 0.2.1
/// @date 2022-05-08
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
/// @defgroup cnx_shared_ptr CnxSharedPtr
/// `CnxSharedPtr(T)` is a struct template similar to C++'s `std::shared_ptr` or Rust's `Box` for a
/// "smart" pointer type that maintains (when used correctly) shared ownership of, and manages an
/// object through, a contained pointer. The shared object is disposed of ("freed") when all
/// `CnxSharedPtr(T)`s sharing its ownership have been explicitly freed, when all owners have gone
/// out of scope when declared with the `shared_scoped(T)` tagged declaration, when `reset`
/// (`cnx_shared_ptr_reset`) has been called on all owners, or some combination thereof.
///
/// `CnxSharedPtr(T)` is allocator aware and stores its associated allocator in itself.
///
/// The contained object is "freed" by a `CnxDeleter`, a function taking a `T*` and a
/// `CnxAllocator`, whose task is to properly cleanup/free/etc. anything associated with the `T`.
/// This deleter can be supplied as a template parameter, `SHARED_DELETER` when instantiating the
/// `CnxSharedPtr(T)` template. This template parameter is optional, and if not supplied the deleter
/// used will be the default deleter, which will simply deallocate the `T` with the allocator
/// associated with the given `CnxSharedPtr(T)` instance.
///
/// `CnxSharedPtr(T)` can own dynamically allocated array types, and provided functionality will be
/// slightly different, though comparable, when `T` is an array type vs a singular type.
/// To instantiate the template for an array type, the type must be `typedef`ed,
/// e.g.: ```typedef int int_arr[];```, and the typedef provided as the `SHARED_T` template
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
/// 1. `SHARED_DECL` (Optional) - Defining this to true signals to the implementation to declare the
/// required type declarations and definitions, and any required or public function declarations
/// when you include `<Cnx/SharedPtr.h>`. No functions will be defined. This is optional
/// (but signals intent explicitly) - If required template parameters are defined and `SHARED_DECL`
/// is not, then this will be inferred as true by default.
/// 2. `SHARED_IMPL` - Defining this to true signals to the implementation to define the functions
/// necessary for the template instantation when you include `<Cnx/SharedPtr.h>`. This is not
/// required to be paired with `SHARED_DECL` (you can declare the template, in a header for example,
/// prior to defining it in a source file), but it is eventually required to be used in exactly one
/// translation unit in the resulting binary in order to provide definitions for the template's
/// functions. Failing to complete this instantiation-mode exactly once in exactly one translation
/// unit in your build will cause linking errors due to missing or multiply defined functions.
///
/// ## Template Parameters
///
/// These provide the type or value parameters that the template is parameterized on to the template
/// implementation. These should be `#define`d to their appropriate types/values.
/// 1. `SHARED_T` - The type to be stored and managed in the shared_ptr (e.g. `u32` or `CnxString`).
/// This is required.
/// 2. `SHARED_DELETER` - The deleter function to free the memory associated with the managed type
/// and perform any necessary associated cleanup.
/// This function must take a particular signature:
/// For a type `Y` that evaluates to either:
/// 1. `T` (`SHARED_T`) if `T` is __NOT__ an array type
/// 2. The element type of the array type `T` if `T` __IS__ an array type.
/// e.g. for an array type `T`, and an instance `t`, `Y` would be `typeof(t[0])`
///
/// The signature must be `void name(Y* object, CnxAllocator allocator);`
/// where `object` is a pointer to the managed object and `allocator` is the allocator associated
/// with the object (the one that allocated it, and will free it).
///
/// Example using instantiation method one:
/// @code {.c}
/// // in MyStruct.h
///
/// // include for `TRUE` (could just use `1`)
/// #include <Cnx/Def.h>
/// // include for `i32`
/// #include <Cnx/BasicTypes.h>
///
/// typedef struct {
/// 	i32 one;
/// 	i32 two;
/// 	i32 three;
/// } MyStruct;
///
/// // instantiate `CnxSharedPtr(MyStruct)` declaration
/// #define SHARED_T MyStruct
/// #define SHARED_DECL TRUE
/// #define SHARED_UNDEF_PARAMS TRUE
/// #include <Cnx/SharedPtr.h>
/// #undef SHARED_UNDEF_PARAMS
///
/// // in MyStruct.c
///
/// #include "MyStruct.h"
///
/// // instantiate `CnxSharedPtr(MyStruct)` declaration
/// #define SHARED_T MyStruct
/// #define SHARED_IMPL TRUE
/// #define SHARED_UNDEF_PARAMS TRUE
/// #include <Cnx/SharedPtr.h>
/// #undef SHARED_UNDEF_PARAMS
///
/// void takes_ownership(CnxSharedPtr(MyStruct) to_take);
///
/// void example(void) {
/// 	// create a sharedly owned `MyStruct`
/// 	shared_scoped(MyStruct) my_ptr = cnx_make_shared(MyStruct, .one = 2, .two = 4, .three = 1);
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
/// #include <Cnx/BasicTypes.h>
///
/// typedef struct {
/// 	i32 one;
/// 	i32 two;
/// 	i32 three;
/// } MyStruct;
///
///
/// // in CnxSharedPtrMyStruct.h
///
/// // include for `TRUE` (could just use `1`)
/// #include <Cnx/Def.h>
///
/// #include "MyStruct.h"
///
/// // instantiate `CnxSharedPtr(MyStruct)` declaration
/// #define SHARED_T MyStruct
/// #define SHARED_DECL TRUE
/// #define SHARED_UNDEF_PARAMS TRUE
/// #include <Cnx/SharedPtr.h>
/// #undef SHARED_UNDEF_PARAMS
///
/// // in CnxSharedPtrMyStruct.c
///
/// #include "CnxSharedPtrMyStruct.h"
///
/// // instantiate `CnxSharedPtr(MyStruct)` declaration
/// #define SHARED_T MyStruct
/// #define SHARED_IMPL TRUE
/// #define SHARED_UNDEF_PARAMS TRUE
/// #include <Cnx/SharedPtr.h>
/// #undef SHARED_UNDEF_PARAMS
///
/// // in MyStruct.c
///
/// void takes_ownership(CnxSharedPtr(MyStruct) to_take);
///
/// void example(void) {
/// 	// create a sharedly owned `MyStruct`
/// 	shared_scoped(MyStruct) my_ptr = cnx_make_shared(MyStruct, .one = 2, .two = 4, .three = 1);
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
/// Like other Cnx templates, `CnxSharedPtr(T)` provides its type-agnostic usage through a vtable
/// pointer contained in the struct, and provides macros which wrap usage of the vtable, making
/// access simpler. If you prefer to not use this method of access, you can call the typed functions
/// directly by in-fixing the contained type in the associated function name.
/// IE: for a `CnxSharedPtr(i32)`, `i32_ptr`, the equivalent function call for
/// `cnx_shared_ptr_release` would be `cnx_shared_ptr_i32_release(&i32_ptr)`.
///
/// @note `CnxSharedPtr(T)` uses a combination of Clang's `diagnose_if` attribute, static_asserts,
/// and runtime asserts to disable the use of functions that only apply to non-array or array-type
/// `T`s, respectively, when `T` is not the correct type category. This provides compile-time errors
/// in as many situations as possible. However, when compiling with something other than Clang
/// forgoing the macros bypasses the only compile-time checks available (static_asserts), resulting
/// in runtime asserts being the only defense against invoking undefined behavior from calling an
/// array-associated function for a `CnxSharedPtr(T)` instantiation where `T` is not an array-type.
/// Because of this, it's particularly discouraged to forgo the function-like macros for
/// `CnxSharedPtr(T)`, or any Cnx smart pointer, for that matter.
/// @}

#include <Cnx/Def.h>
#include <Cnx/shared_ptr/SharedPtrDef.h>

#if !defined(SHARED_DECL) && (!defined(SHARED_IMPL) || !SHARED_IMPL) && defined(SHARED_T)
	#define SHARED_DECL TRUE
#endif // !defined(SHARED_DECL) && (!defined(SHARED_IMPL) || !SHARED_IMPL) && defined(SHARED_T)

#if !defined(SHARED_DELETER) && (SHARED_DECL || SHARED_IMPL)
	#define SHARED_DELETER			   CnxSharedPtrIdentifier(SHARED_T, default_deleter)
	#define __SHARED_DEFAULTED_DELETER TRUE
#endif // !defined(SHARED_DELETER) && SHARED_DECL || SHARED_IMPL

#if !defined(SHARED_T) && SHARED_DECL
	#error "SharedPtr.h included with SHARED_DECL defined true but template parameter SHARED_T \
			not defined "
#endif // !defined(SHARED_T) && SHARED_DECL

#if !defined(SHARED_T) && SHARED_IMPL
	#error "SharedPtr.h included with SHARED_IMPL defined true but template parameter SHARED_T \
			not defined "
#endif // !defined(SHARED_T) && SHARED_IMPL

#if SHARED_DECL && SHARED_IMPL
	#define SHARED_STATIC static
	#define SHARED_INLINE inline
#else
	#ifndef SHARED_STATIC
		#define SHARED_STATIC
	#endif // SHARED_STATIC
	#ifndef SHARED_INLINE
		#define SHARED_INLINE
	#endif // SHARED_INLINE
#endif	   // SHARED_DECL && SHARED_IMPL

#if defined(SHARED_T) && SHARED_DECL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <Cnx/shared_ptr/SharedPtrDecl.h>
#endif // defined(SHARED_T) && SHARED_DECL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if defined(SHARED_T) && SHARED_IMPL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS
	#include <Cnx/shared_ptr/SharedPtrImpl.h>
#endif // defined(SHARED_T) && SHARED_IMPL && !CNX_TEMPLATE_SUPPRESS_INSTANTIATIONS

#if SHARED_UNDEF_PARAMS
	#undef SHARED_T
	#undef SHARED_DELETER
	#undef SHARED_DECL
	#undef SHARED_IMPL
#endif // SHARED_UNDEF_PARAMS

#if __SHARED_DEFAULTED_DELETER
	#undef SHARED_DELETER
	#undef __SHARED_DEFAULTED_DELETER
#endif // __SHARED_DEFAULTED_DELETER

#ifdef SHARED_STATIC
	#undef SHARED_STATIC
#endif // SHARED_STATIC
#ifdef SHARED_INLINE
	#undef SHARED_INLINE
#endif // SHARED_INLINE
