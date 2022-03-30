/// @file StdUniquePtrDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides the macro definitions for a struct template for representing a
/// uniquely owned pointer
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

#ifndef STD_UNIQUE_PTR_DEF
#define STD_UNIQUE_PTR_DEF

#include <C2nxt/StdAssert.h>
#include <C2nxt/StdDef.h>

/// @brief macro alias for a `StdUniquePtr(T)` maintaining ownership of an allocated `T`
///
/// `StdUniquePtr(T)` is a template for a "smart" pointer type that maintains (when used correctly)
/// unique ownership of and manages an object through a contained pointer. The contained object is
/// disposed of ("freed") when the `StdUniquePtr(T)` is explicitly freed, when it goes out of scope
/// when declared with the `unique_scoped(T)` tagged declaration, or when `reset`
/// (`std_unique_ptr_reset`) is called on the `StdUniquePtr(T)`.
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
/// slightly different, though comparable, when `T` is an array type or a singular type.
/// To instantiate the template for an array type, the type must be `typedef`ed,
/// e.g.: ```typedef int int_arr[];```, and the typedef provided as the `UNIQUE_T` template
/// parameter.
///
/// Example:
/// @code {.c}
/// // include for `TRUE`
/// #include <C2nxt/StdDef.h>
/// // include for `i32`
/// #include <C2nxt/StdBasicTypes.h>
///
/// // just for example's sake
/// typedef struct {
/// 	i32 one;
/// 	i32 two;
/// 	i32 three;
/// } MyStruct;
///
/// // instantiate `StdUniquePtr(MyStruct)`
/// #define UNIQUE_T MyStruct
/// #define UNIQUE_DECL TRUE
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
/// @ingroup std_unique_ptr
#define StdUniquePtr(T) CONCAT2(StdUniquePtr, T)
/// @brief macro alias for an identifier (type, function, etc) associated with a `StdUniquePtr(T)`
/// instantiation
///
/// @param T - The type managed by the `StdUniquePtr(T)`
/// @param Identifier - The identifier
#define StdUniquePtrIdentifier(T, Identifier) CONCAT2(std_unique_ptr, CONCAT3(T, _, Identifier))

/// @brief Creates a new uniquely, owned zero-initialized `T`
///
/// @param T - The type to manage in the created `StdUniquePtr(T)`
///
/// @return a `StdUniquePtr(T)` managing a zero-initialized `T`
/// @note Only enabled when `T` is NOT an array type. To create a zero-initialized uniquely-owned
/// dynamic array, use `std_unique_ptr_new_with_capacity`
/// @ingroup std_unique_ptr
#define std_unique_ptr_new(T)                                                         \
	({                                                                                \
		std_static_assert(                                                            \
			!__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                        \
			"std_unique_ptr_new is only available if UNIQUE_T is NOT an array type"); \
		StdUniquePtrIdentifier(T, new)();                                             \
	})

/// @brief Creates a new uniquely owned, zero-initialized `T`, allocated with the given allocator
///
/// @param T - The type to manage in the created `StdUniquePtr(T)`
/// @param allocator - The allocator to allocator and eventually free the managed `T` with
///
/// @return a `StdUniquePtr(T)` managing a zero-initialized `T`
/// @note Only enabled when `T` is NOT an array type. To create a zero-initialized uniquely-owned
/// dynamic array with an associated allocator, use `std_unique_ptr_new_with_capacity_and_allocator`
/// @ingroup std_unique_ptr
#define std_unique_ptr_new_with_allocator(T, allocator)                                         \
	({                                                                                          \
		std_static_assert(!__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                    \
						  "std_unique_ptr_new_with_allocator is only available if UNIQUE_T is " \
						  "NOT an array type");                                                 \
		StdUniquePtrIdentifier(T, new_with_allocator)(allocator);                               \
	})
/// @brief Creates a new uniquely owned, zero-initialized, dynamically allocated array
///
/// @param T - The array type to manage in the created `StdUniquePtr(T)`. Must be a typedef to an
/// array type (e.g.: ```typedef i32 i32_array[];```).
/// @param capacity - The capacity of the array, in number of elements
///
/// @return a `StdUniquePtr(T)` managing a zero-initialized, dynamically allocated array.
/// @note Only enabled when `T` is an array type. To create a single zero-initialized,
/// uniquely-owned object, use `std_unique_ptr_new`
/// @ingroup std_unique_ptr
#define std_unique_ptr_new_with_capacity(T, capacity)                                       \
	({                                                                                      \
		std_static_assert(__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                 \
						  "std_unique_ptr_new_with_capacity is only available if UNIQUE_T " \
						  "is an array type");                                              \
		StdUniquePtrIdentifier(T, new_with_capacity)(capacity);                             \
	})
/// @brief Creates a new uniquely owned, zero-initialized, dynamically allocated array,
/// allocated with the given allocator.
///
/// @param T - The array type to manage in the created `StdUniquePtr(T)`. Must be a typedef to an
/// array type (e.g.: ```typedef i32 i32_array[];```).
/// @param capacity - The capacity of the array, in number of elements
/// @param allocator - The allocator to allocator and eventually free the managed array with
///
/// @return a `StdUniquePtr(T)` managing a zero-initialized, dynamically allocated array.
/// @note Only enabled when `T` is an array type. To create a single zero-initialized,
/// uniquely-owned object with an associated allocator, use `std_unique_ptr_new_with_allocator`
/// @ingroup std_unique_ptr
#define std_unique_ptr_new_with_capacity_and_allocator(T, capacity, allocator)              \
	({                                                                                      \
		std_static_assert(                                                                  \
			__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                               \
			"std_unique_ptr_new_with_capacity_and_allocator is only available if UNIQUE_T " \
			"is an array type");                                                            \
		StdUniquePtrIdentifier(T, new_with_capacity_and_allocator)(capacity, allocator);    \
	})

/// @brief Creates a new `StdUniquePtr(T)` managing the given pointer
///
/// `T` may be either an array type or a single object.
///
/// Useful when working with a legacy API that returns raw allocated object(s), but you still want
/// use a `StdUniquePtr(T)` to maintain ownership. If you have control over when allocation occurs,
/// prefer `std_unique_ptr_new`, `std_make_unique`, or derivatives.
///
/// @param T - The type to manage in the created `StdUniquePtr(T)`
/// @param ptr - The ptr to manage with the `StdUniquePtr(T)`.
///
/// @return a `StdUniquePtr(T)` managing the given pointer
/// @ingroup std_unique_ptr
#define std_unique_ptr_from(T, ptr) StdUniquePtrIdentifier(T, from)(ptr)

/// @brief Creates a new `StdUniquePtr(T)` managing the given pointer, associated with the given
/// allocator
///
/// `T` may be either an array type or a single object.
///
/// Useful when working with a legacy API that returns raw allocated object(s), but you still want
/// use a `StdUniquePtr(T)` to maintain ownership. If you have control over when allocation occurs,
/// prefer `std_unique_ptr_new_with_allocator` or `std_make_unique_with_allocator`
///
/// @param T - The type to manage in the created `StdUniquePtr(T)`
/// @param ptr - The ptr to manage with the `StdUniquePtr(T)`.
/// @param allocator - The allocator to allocator and eventually free the managed `T` with
///
/// @return a `StdUniquePtr(T)` managing the given pointer
/// @ingroup std_unique_ptr
#define std_unique_ptr_from_with_allocator(T, ptr, allocator) \
	StdUniquePtrIdentifier(T, from_with_allocator)(ptr, allocator)

/// @brief Frees the given `StdUniquePtr(T)`, calling the associated deleter on the managed object
///
/// This should generally only need to be called explicitly in particularly rare circumstances.
/// In typical usage, `StdUniquePtr(T)`s should be declared with the `unique_scoped(T)` tag,
/// so cleanup is performed automatically when the associated `StdUniquePtr(T)` goes out of scope
///
/// @param T - The type managed in the given `StdUniquePtr(T)`
/// @param self - The `StdUniquePtr(T)` to free
/// @ingroup std_unique_ptr
#define std_unique_ptr_free(T, self) StdUniquePtrIdentifier(T, free)(static_cast(void*)(&(self)))

/// @brief Releases the pointer managed by `self` from ownership
///
/// Useful when you need to transfer ownership of the managed object from yourself to a another API
/// that uses a different method of ownership management
///
/// @param self - The `StdUniquePtr(T)` to release ownership of its managed object
///
/// @return the pointer to the object previously managed by `self`
/// @ingroup std_unique_ptr
#define std_unique_ptr_release(self) ((self).m_vtable->release(&(self)))
/// @brief Frees the object currently managed by `self`, and replaces it with the one
/// pointed to by `new_ptr`
///
/// @param self - The `StdUniquePtr(T)` to have its managed object replaced
/// @param new_ptr - The pointer to the object to transfer ownership of to `self`
/// @note `nullptr` is a valid pointer to pass as `new_ptr`, if you want to update `self` such
/// that it currently manages __no__ object
/// @ingroup std_unique_ptr
#define std_unique_ptr_reset(self, new_ptr) ((self).m_vtable->reset(&(self), (new_ptr)))
/// @brief Swaps the managed objects of `self` and the `StdUniquePtr(T)` pointed to by `other_ptr`
///
/// `self` and the `StdUniquePtr(T)` pointed to by `other_ptr` must manage the same type
///
/// @param self - The `StdUniquePtr(T)` to have its managed object swapped
/// @param other_ptr - Pointer to the `StdUniquePtr(T)` to swap managed objects with
/// @ingroup std_unique_ptr
#define std_unique_ptr_swap(self, other_ptr) ((self).m_vtable->swap(&(self), (other_ptr)))
/// @brief Returns the `StdDeleter(T)` function pointer associated with the `StdUniquePtr(T)`
/// instantiation `self` is an instance of.
///
/// @param self - The `StdUniquePtr(T)` to get the associated deleter of
///
/// @return the `StdDeleter(T)` function pointer associated with `self`
/// @ingroup std_unique_ptr
#define std_unique_ptr_get_deleter(self) ((self).m_vtable->get_deleter(&(self)))
/// @brief Returns whether the given `StdUniquePtr(T)` currently manages an object
///
/// @param self - The `StdUniquePtr(T)` to check if currently manages an object
///
/// @return `true` if `self` currently manages an object, otherwise `false`
/// @ingroup std_unique_ptr
#define std_unique_ptr_as_bool(self) ((self).m_vtable->as_bool(&(self)))

/// @brief Returns a pointer to the object managed by `self`
///
/// This is const correct:
/// 		- If `self` is const, this will return a pointer-to-const of the managed object.
/// 		- Otherwise, this will return a "normal" pointer to the managed object.
/// If you want to explicitly get a pointer-to-const or pointer-to-not-const, use
/// `std_unique_ptr_get_const` or `std_unique_ptr_get_mut`, respectively.
///
/// @param self - The `StdUniquePtr(T)` to get the managed object of
///
/// @return const correct pointer to the managed object
/// @note Do __NOT__ use this to circumvent ownership. If you want to transfer ownership of the
/// managed object, use `std_unique_ptr_release`. This is solely to provide a view to the managed
/// object while maintaining ownership of it.
/// @ingroup std_unique_ptr
#define std_unique_ptr_get(self) ((__std_unique_ptr_get(self)))

/// @brief implementation of `std_unique_ptr_get`
#define __std_unique_ptr_get(self)                                \
	({                                                            \
		IGNORE_DISCARDED_QUALIFIERS_START                         \
		let UNIQUE_VAR(ptr) = &(self);                            \
		_Generic((UNIQUE_VAR(ptr)),           	                 \
			const typeof(self)* : std_unique_ptr_get_const(self), \
			default 			: std_unique_ptr_get_mut(self)); \
		IGNORE_DISCARDED_QUALIFIERS_STOP                          \
	})

/// @brief Returns a pointer-to-const to the object managed by `self`
///
/// @param self - The `StdUniquePtr(T)` to get the managed object of
///
/// @return pointer-to-const to the managed object
/// @note Do __NOT__ use this to circumvent ownership. If you want to transfer ownership of the
/// managed object, use `std_unique_ptr_release`. This is solely to provide a view to the managed
/// object while maintaining ownership of it.
/// @ingroup std_unique_ptr
#define std_unique_ptr_get_const(self) ((self).m_vtable->get_const(&(self)))
/// @brief Returns a pointer-to-non-const to the object managed by `self`
///
/// @param self - The `StdUniquePtr(T)` to get the managed object of
///
/// @return pointer-to-non-const to the managed object
/// @note Do __NOT__ use this to circumvent ownership. If you want to transfer ownership of the
/// managed object, use `std_unique_ptr_release`. This is solely to provide a view to the managed
/// object while maintaining ownership of it.
/// @ingroup std_unique_ptr
#define std_unique_ptr_get_mut(self) ((self).m_vtable->get(&(self)))

/// @brief Returns a reference to the element at `index` in the array managed by `self`
///
/// This is const correct:
/// 		- If `self` is const, this will return a reference-to-const of the array element.
/// 		- Otherwise, this will return a "normal" reference to the array element.
/// If you want to explicitly get a reference-to-const or reference-to-not-const, use
/// `std_unique_ptr_at_const` or `std_unique_ptr_at_mut`, respectively.
///
/// @param self - The `StdUniquePtr(T)` to get the element from
/// @param index - The index of the element in the array managed by `self`
///
/// @return const correct reference to the array element
/// @note Do __NOT__ use this to circumvent ownership. If you want to transfer ownership of the
/// managed object, use `std_unique_ptr_release`. This is solely to provide a view to the managed
/// object while maintaining ownership of it.
/// @note This is only available if the managed type of `self`
/// (the `T` of its `StdUniquePtr(T)` template instantiation) is an array type.
/// @ingroup std_unique_ptr
#define std_unique_ptr_at(self, index) (*(__std_unique_ptr_at(self, index)))

/// @brief implementation of `std_unique_ptr_at`
#define __std_unique_ptr_at(self, index)                                   \
	({                                                                     \
		let UNIQUE_VAR(ptr) = &(self);                                     \
		_Generic(                     	                                  \
			(UNIQUE_VAR(ptr)),                                       	   \
			const typeof(self)* : &(std_unique_ptr_at_const(self, index)), \
			default 			: &std_unique_ptr_at_mut(self, index)); \
	})

/// @brief Returns a reference-to-const to the element at `index` in the array managed by `self`
///
/// @param self - The `StdUniquePtr(T)` to get the element from
/// @param index - The index of the element in the array managed by `self`
///
/// @return reference-to-const to the array element
/// @note Do __NOT__ use this to circumvent ownership. If you want to transfer ownership of the
/// managed object, use `std_unique_ptr_release`. This is solely to provide a view to the managed
/// object while maintaining ownership of it.
/// @note This is only available if the managed type of `self`
/// (the `T` of its `StdUniquePtr(T)` template instantiation) is an array type.
/// @ingroup std_unique_ptr
#define std_unique_ptr_at_const(self, index) \
	(*(static_cast(const typeof(*((self).m_ptr))*)(__std_unique_ptr_at_const(self, index))))

/// @brief implementation of `std_unique_ptr_at_const`
#define __std_unique_ptr_at_const(self, index)                                     \
	({                                                                             \
		std_static_assert(__UNIQUE_PTR_CHECK_INSTANCE_IS_ARRAY(self),              \
						  "std_unique_ptr_at_const is only available if UNIQUE_T " \
						  "is an array type");                                     \
		&(((self).m_ptr)[index]);                                                  \
	})

/// @brief Returns a reference-to-non-const to the element at `index` in the array managed by `self`
///
/// @param self - The `StdUniquePtr(T)` to get the element from
/// @param index - The index of the element in the array managed by `self`
///
/// @return reference-to-non-const to the array element
/// @note Do __NOT__ use this to circumvent ownership. If you want to transfer ownership of the
/// managed object, use `std_unique_ptr_release`. This is solely to provide a view to the managed
/// object while maintaining ownership of it.
/// @note This is only available if the managed type of `self`
/// (the `T` of its `StdUniquePtr(T)` template instantiation) is an array type.
/// @ingroup std_unique_ptr
#define std_unique_ptr_at_mut(self, index) (*(__std_unique_ptr_at_mut(self, index)))

/// @brief implementation of `std_unique_ptr_at_mut`
#define __std_unique_ptr_at_mut(self, index)                                 \
	({                                                                       \
		std_static_assert(__UNIQUE_PTR_CHECK_INSTANCE_IS_ARRAY(self),        \
						  "std_unique_ptr_at is only available if UNIQUE_T " \
						  "is an array type");                               \
		&(((self).m_ptr)[index]);                                            \
	})

/// @brief Creates a `StdUniquePtr(T)` managing a `T` initialized with the given list of
/// (potentially designated) initializers
///
/// @param T - The type the `StdUniquePtr(T)` will manage
/// @param ... - The list of initializers to initialize the `T` with
///
/// @return a `StdUniquePtr(T)` managing a newly allocated `T`
/// @note This is only available if `T` is __NOT__ an array type. To create a uniquely owned dynamic
/// array, use `std_make_unique_array` or `std_unique_ptr_new_with_capacity`.
/// @ingroup std_unique_ptr
#define std_make_unique(T, ...)                                                                 \
	({                                                                                          \
		std_static_assert(!__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                    \
						  "std_make_unique is only available for Ts that are not array types"); \
		let_mut UNIQUE_VAR(ptr) = std_allocator_allocate_t(T, DEFAULT_ALLOCATOR);               \
		*UNIQUE_VAR(ptr) = (T){__VA_ARGS__};                                                    \
		std_unique_ptr_from(T, UNIQUE_VAR(ptr));                                                \
	})

/// @brief Creates a `StdUniquePtr(T)` managing a `T` initialized with the given list of
/// (potentially designated) initializers, allocated with the given allocator
///
/// @param T - The type the `StdUniquePtr(T)` will manage
/// @param allocator - The `StdAllocator` to allocate and eventually free the `T` with
/// @param ... - The list of initializers to initialize the `T` with
///
/// @return a `StdUniquePtr(T)` managing a newly allocated `T`
/// @note This is only available if `T` is __NOT__ an array type. To create a uniquely owned dynamic
/// array with an associated allocator, use `std_make_unique_array_with_allocator` or
/// `std_unique_ptr_new_with_capacity_and_allocator`.
/// @ingroup std_unique_ptr
#define std_make_unique_with_allocator(T, allocator, ...)                                        \
	({                                                                                           \
		std_static_assert(                                                                       \
			!__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                                   \
			"std_make_unique_with_allocator is only available for Ts that are not array types"); \
		let_mut UNIQUE_VAR(ptr) = std_allocator_allocate_t(T, allocator);                        \
		*UNIQUE_VAR(ptr) = (T){__VA_ARGS__};                                                     \
		std_unique_ptr_from_with_allocator(T, UNIQUE_VAR(ptr), allocator);                       \
	})

/// @brief Creates a `StdUniquePtr(T)` managing the array type `T` with the given initial capacity
///
/// @param T - The array type the `StdUniquePtr(T)` will manage. Must be a typedef to an array type
/// (e.g.: ```typedef i32 i32_array[];```)
/// @param capacity - The capacity of the array in number of elements
///
/// @return a `StdUniquePtr(T)` managing a newly allocated dynamic array of capacity `capacity`
/// @note This is only available if `T` __IS__ an array type. To create a uniquely owned single
/// object, use `std_make_unique` or `std_unique_ptr_new`.
/// @ingroup std_unique_ptr
#define std_make_unique_array(T, capacity)                                                        \
	({                                                                                            \
		std_static_assert(__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                       \
						  "std_make_unique_array is only available for Ts that are array types"); \
		let_mut UNIQUE_VAR(ptr) = std_allocator_allocate_array_t(T, DEFAULT_ALLOCATOR, capacity); \
		std_unique_ptr_from(T, UNIQUE_VAR(ptr));                                                  \
	})
/// @brief Creates a `StdUniquePtr(T)` managing the array type `T` with the given initial capacity,
/// allocated with the given allocator
///
/// @param T - The array type the `StdUniquePtr(T)` will manage. Must be a typedef to an array type
/// (e.g.: ```typedef i32 i32_array[];```)
/// @param allocator - The `StdAllocator` to allocate and eventually free the array with
/// @param capacity - The capacity of the array in number of elements
///
/// @return a `StdUniquePtr(T)` managing a newly allocated dynamic array of capacity `capacity`
/// @note This is only available if `T` __IS__ an array type. To create a uniquely owned single
/// object with an associated allocator, use `std_make_unique_with_allocator` or
/// `std_unique_ptr_new_with_allocator`.
/// @ingroup std_unique_ptr
#define std_make_unique_array_with_allocator(T, capacity, allocator)                            \
	({                                                                                          \
		std_static_assert(__UNIQUE_PTR_IS_ARRAY_GENERIC(T),                                     \
						  "std_make_unique_array_with_allocator is only available for Ts that " \
						  "are array types");                                                   \
		let_mut UNIQUE_VAR(ptr) = std_allocator_allocate_array_t(T, allocator, capacity);       \
		std_unique_ptr_from_with_allocator(T, UNIQUE_VAR(ptr), allocator);                      \
	})

/// @brief Declare a `StdUniquePtr(T)` with this declaration attribute/type to ensure it
/// (and its managed object) is automatically freed when the `StdUniquePtr(T)` goes out of scope
///
/// @param T - The type managed by the `StdUniquePtr(T)`
/// @ingroup std_unique_ptr
#define unique_scoped(T) scoped(StdUniquePtrIdentifier(T, free))

#include <C2nxt/std_smart_ptrs/StdSmartPtrDef.h>

/// @brief wrapper for __SMART_PTR_CHECK_INSTANCE_IS_ARRAY
#define __UNIQUE_PTR_CHECK_INSTANCE_IS_ARRAY(self) __SMART_PTR_CHECK_INSTANCE_IS_ARRAY(self)
/// @brief wrapper for __SMART_PTR_IS_ARRAY
#define __UNIQUE_PTR_IS_ARRAY __SMART_PTR_IS_ARRAY(UNIQUE_T)
/// @brief wrapper for __SMART_PTR_IS_ARRAY
#define __UNIQUE_PTR_IS_ARRAY_GENERIC(T) __SMART_PTR_IS_ARRAY(T)
/// @brief wrapper for __SMART_PTR_ELEMENT_PTR_BASE
#define __UNIQUE_PTR_ELEMENT_PTR_BASE __SMART_PTR_ELEMENT_PTR_BASE(UNIQUE_T)
/// @brief wrapper for __SMART_PTR_CONST_ELEMENT_PTR_BASE
#define __UNIQUE_PTR_CONST_ELEMENT_PTR_BASE __SMART_PTR_CONST_ELEMENT_PTR_BASE(UNIQUE_T)
/// @brief wrapper for __SMART_PTR_ELEMENT_PTR
#define __UNIQUE_PTR_ELEMENT_PTR __SMART_PTR_ELEMENT_PTR(UNIQUE_T)
/// @brief wrapper for __SMART_PTR_CONST_ELEMENT_PTR
#define __UNIQUE_PTR_CONST_ELEMENT_PTR __SMART_PTR_ELEMENT_PTR(UNIQUE_T)
/// @brief wrapper for __SMART_PTR_ELEMENT
#define __UNIQUE_PTR_ELEMENT __SMART_PTR_ELEMENT(UNIQUE_T)

#endif // STD_UNIQUE_PTR_DEF
