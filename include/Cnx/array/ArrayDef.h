/// @file ArrayDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides macro definitions for implementing and working with `CnxArray(T, N)`
/// @version 0.2.1
/// @date 2022-04-30
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

#ifndef CNX_ARRAY_DEF
#define CNX_ARRAY_DEF

#include <Cnx/Def.h>

/// @brief Macro alias for a `CnxArray(T, N)` containing up to `N` `T`s
///
/// `CnxArray(T, N)` is a generic, type-safe, fixed-capacity, dynamic-size, stack allocated
/// array type. `CnxArray(T, N)` is bounds safe and allocator aware. It's implemented as a
/// "struct template", which enables 100% type safety while also providing abstractions that
/// enable type agnostic use. It has greatly increased ergonomics over manually managing an
/// array and provides many useful features such as random access iterators and optional scoped
/// destruction.
///
/// Example:
/// @code {.c}
/// #include "Cnx/Array.h"
/// #include "Cnx/IO.h"
/// #include "Array_i32_10.h"
///
/// // create a `CnxArray(i32, 10)` with default allocator, element default-constructor, and
/// element
/// // destructor
/// let_mut array = cnx_array_new(i32, 10);
/// // fill the array with 10 elements
/// ranged_for(i, 0, 9) {
/// 	cnx_array_push_back(array, i);
/// }
///
/// prints information about `array` to `stdout`
/// because `CnxArray(T, N)` is generic, and thus can't be used in a `_Generic` match arm prior
/// to the type's instantiation, we have to explicitly cast to the `CnxFormat` Trait to get
/// `CnxArray(T, N)`'s implementation of the Trait println("{}", cnx_as_format_t(CnxArray(i32,
/// 10), array));
///
/// // prints `array`'s elements to stdout
/// foreach(elem, array) {
/// 	println("{}", elem);
/// }
///
/// {
/// 	let_mut CnxScopedArray(i32, 10) array2 = cnx_array_new(i32, 10);
/// 	foreach(elem, array) {
/// 		cnx_array_push_back(array2, elem);
/// 	}
/// }
/// // `array2` is destroyed here because it was scoped
/// // `array` is not destroyed
/// @endcode
///
/// The above example assumes that the `CnxArray(T, N)` template has been instantiated for
/// `CnxArray(i32, 10)` in "Array_i32_10.h" and an associated ".c" file.
/// See the module-level page for details
///
/// Like other Cnx collections, `CnxArray(T, N)` provides its type-agnostic usage through a
/// vtable pointer contained in the struct, and provides macros which wrap the usage of the
/// vtable, making access simpler. If you prefer to not use this method of access, you can call
/// the typed functions directly by in-fixing the contained type in the associated function
/// name. IE: for `CnxArray(i32, 10)`, and an array `array`, the equivalent function call for
/// `cnx_array_push_back(array, element)` would be `cnx_array_i32_10_push_back(&array, element)`
/// @note Note, however, that using the typed functions instead of the macros results in losing
/// the reference-semantics of function-macros like `cnx_array_at(self, index)` and will instead
/// be dealing with pointers directly. Example:
/// @code {.c}
/// // instead of:
/// cnx_array_at_mut(self, 5) = 5;
/// // you would have to do:
/// *cnx_array_i32_10_at_mut(&self, 5) = 5;
/// @endcode
/// @ingroup cnx_array
#define CnxArray(T, N) CONCAT3(CnxArray, T, N)
/// @brief macro alias for the concrete type for an iterator into the mutable iteration of a
/// `CnxArray(T, N)` containing up to `N` `T`s
#define CnxArrayIterator(T, N) CONCAT2(CnxArray(T, N), Iterator)
/// @brief macro alias for the concrete type for an iterator into the const iteration of a
/// `CnxArray(T, N)` containing up to `N` `T`s
#define CnxArrayConstIterator(T, N) CONCAT2(CnxArray(T, N), ConstIterator)
/// @brief macro alias for an identifier (type, function, etc) associated with a `CnxArray(T,
/// N)` containing up to `N` `T`s
#define CnxArrayIdentifier(T, N, Identifier) \
	CONCAT3(cnx_array_, CONCAT2(T, _), CONCAT3(N, _, Identifier))

/// @brief Creates a new `CnxArray(T, N)` with defaulted associated functions.
///
/// Creates a new `CnxArray(T, N)` with:
/// 1. default associated element default-constructor,
/// 2. default associated element destructor
/// 3. default associated memory allocator (potentially used in element constructor and
/// destructor, memory for the `CnxArray(T, N)` itself is stack allocated)
///
/// @param T - The element type of the `CnxArray(T, N)` instantiation to create
/// @param N - The capacity of the `CnxArray(T, N)` instantiation to create
///
/// @return a new `CnxArray(T, N)`
/// @ingroup cnx_array
#define cnx_array_new(T, N) CnxArrayIdentifier(T, N, new)()
/// @brief Creates a new `CnxArray(T, N)` with defaulted associated functions.
///
/// Creates a new `CnxArray(T, N)` with:
/// 1. default associated element default-constructor,
/// 2. default associated element destructor
/// 3. given associated memory allocator (potentially used in element constructor and
/// destructor, memory for the `CnxArray(T, N)` itself is stack allocated)
///
/// @param T - The element type of the `CnxArray(T, N)` instantiation to create
/// @param N - The capacity of the `CnxArray(T, N)` instantiation to create
/// @param allocator - The memory allocator to pass to the element default-constructor and
/// destructor
///
/// @return a new `CnxArray(T, N)`
/// @ingroup cnx_array
#define cnx_array_new_with_allocator(T, N, allocator) \
	CnxArrayIdentifier(T, N, new_with_allocator)(allocator)
/// @brief Creates a new `CnxArray(T, N)` with provided associated functions.
///
/// Creates a new `CnxArray(T, N)` with:
/// 1. possibly user-provided element default-constructor
/// 2. possibly user-provided element destructor
/// 3. default memory allocator (potentially used in element constructor and destructor,
/// memory for the `CnxArray(T, N)` itself is stack allocated)
///
/// @param T - The element type of the `CnxArray(T, N)` instantiation to create
/// @param N - The capacity of the `CnxArray(T, N)` instantiation to create
/// @param collection_data_ptr - Pointer to the `CnxCollectionData` containing the element
/// default-constructor, element destructor, and element copy-constructor to use
///
/// @return a new `CnxArray(T, N)`
/// @ingroup cnx_array
#define cnx_array_new_with_collection_data(T, N, collection_data_ptr) \
	CnxArrayIdentifier(T, N, new_with_collection_data)(collection_data_ptr)
/// @brief Creates a new `CnxArray(T, N)` with provided associated functions.
///
/// Creates a new `CnxArray(T, N)` with:
/// 1. possibly user-provided element default-constructor
/// 2. possibly user-provided element destructor
/// 3. user-provided memory allocator (potentially used in element constructor and destructor,
/// memory for the `CnxArray(T, N)` itself is stack allocated)
///
/// @param T - The element type of the `CnxArray(T, N)` instantiation to create
/// @param N - The capacity of the `CnxArray(T, N)` instantiation to create
/// @param allocator - The memory allocator to pass to the element default-constructor and
/// destructor
/// @param collection_data_ptr - Pointer to the `CnxCollectionData` containing the element
/// default-constructor, element destructor, and element copy-constructor to use
///
/// @return a new `CnxArray(T, N)`
/// @ingroup cnx_array
#define cnx_array_new_with_allocator_and_collection_data(T, N, allocator, collection_data_ptr) \
	CnxArrayIdentifier(T, N, new_with_allocator_and_collection_data)(allocator, collection_data_ptr)
/// @brief Clones the given `CnxArray(T, N)`
///
/// Creates a deep copy of the given `CnxArray(T, N)` calling the associated copy constructor
/// for each element stored in it.
///
/// @param self - The `CnxArray(T, N)` to clone
///
/// @return a clone of the given array
/// @note Requires that a copy constructor is defined for the elements
/// contained in the array, either the default one provided by the implementation or a
/// user-defined one. If it isn't provided or is explicitly provided as `nullptr` when creating
/// the `CnxCollectionData(CollectionType)` associated with this array, this __may__ trigger a
/// compiler error on clang, and will always trigger a runtime assert for all compilers
/// @ingroup cnx_array
#define cnx_array_clone(self) (self).m_vtable->clone(&(self))
/// @brief Returns a mutable reference to the element at the given `index` into the given
/// `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get an element from
/// @param index - The index of the desired element
///
/// @return a mutable reference to the element at the given index
/// @ingroup cnx_array
#define cnx_array_at_mut(self, index) *((self).m_vtable->at_mut(&(self), (index)))
/// @brief Returns a const reference to the element at the given `index` into the given
/// `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get an element from
/// @param index - The index of the desired element
///
/// @return a const reference to the element at the given index
/// @ingroup cnx_array
#define cnx_array_at(self, index) *((self).m_vtable->at_const(&(self), (index)))
/// @brief Returns a mutable reference to the first element in the given `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get the first element from
///
/// @return a mutable reference to the first element
/// @ingroup cnx_array
#define cnx_array_front_mut(self) *((self).m_vtable->front_mut(&(self)))
/// @brief Returns a const reference to the first element in the given `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get the first element from
///
/// @return a const reference to the first element
/// @ingroup cnx_array
#define cnx_array_front(self) *((self).m_vtable->front_const(&(self)))
/// @brief Returns a mutable reference to the last element in the given `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get the last element from
///
/// @return a mutable reference to the last element
/// @ingroup cnx_array
#define cnx_array_back_mut(self) *((self).m_vtable->back_mut(&(self)))
/// @brief Returns a const reference to the last element in the given `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get the last element from
///
/// @return a const reference to the last element
/// @ingroup cnx_array
#define cnx_array_back(self) *((self).m_vtable->back_const(&(self)))
/// @brief Returns a mutable reference to the internal array in the given `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get the internal array from
///
/// @return a mutable reference to the internal array
/// @ingroup cnx_array
#define cnx_array_data_mut(self) *((self).m_vtable->data_mut(&(self)))
/// @brief Returns a const reference to the internal array in the given `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get the internal array from
///
/// @return a const reference to the internal array
/// @ingroup cnx_array
#define cnx_array_data(self) *((self).m_vtable->data_const(&(self)))
/// @brief Returns whether the given `CnxArray(T, N)` is empty
///
/// @param self - The `CnxArray(T, N)` to check for emptiness
///
/// @return `true` if empty, `false` otherwise
/// @ingroup cnx_array
#define cnx_array_is_empty(self) (self).m_vtable->is_empty(&(self))
/// @brief Returns whether the given `CnxArray(T, N)` is full
///
/// @param self - The `CnxArray(T, N)` to check for fullness
///
/// @return `true` if full, `false` otherwise
/// @ingroup cnx_array
#define cnx_array_is_full(self) (self).m_vtable->is_full(&(self))
/// @brief Returns the current size of the given `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get the size of
///
/// @return the size of the `CnxArray(T, N)`
/// @ingroup cnx_array
#define cnx_array_size(self) (self).m_vtable->size(&(self))
/// @brief Returns the capacity of the given `CnxArray(T, N)` (ie the `N`)
///
/// @param self - The `CnxArray(T, N)` to get the capacity of
///
/// @return the capacity of the `CnxArray(T, N)`
/// @ingroup cnx_array
#define cnx_array_capacity(self) (self).m_vtable->capacity(&(self))
/// @brief Resizes the given `CnxArray(T, N)` to `new_size`
///
/// `new_size` must be strictly `<= N`.
/// If `new_size` is greater than the current size, elements will be default constructed
/// until `self` contains `new_size` elements.
/// If `new_size` is less than the current size, elements will be destroyed.
///
/// @param self - The `CnxArray(T, N)` to resize
/// @param new_size - The new size for the given array
/// @ingroup cnx_array
#define cnx_array_resize(self, new_size) (self).m_vtable->resize(&(self), (new_size))
/// @brief Resets the given `CnxArray(T, N)` to a size of zero, destroying any active elements
/// in the array.
///
/// @param self - The `CnxArray(T, N)` to clear
/// @ingroup cnx_array
#define cnx_array_clear(self) (self).m_vtable->clear(&(self))
/// @brief Pushes a new element into the given `CnxArray(T, N)`, at the end.
///
/// The current size must be strictly less than `N`.
///
/// @param self - The `CnxArray(T, N)` to add an element to
/// @param element - The element to add to the array
/// @ingroup cnx_array
#define cnx_array_push_back(self, element) (self).m_vtable->push_back(&(self), (element))
/// @brief Removes the last element from the given `CnxArray(T, N)` and returns it
///
/// @param self - The `CnxArray(T, N)` to pop the last element from
///
/// @return `Some(T)` if size greater than 0, otherwise `None(T)`
/// @ingroup cnx_array
#define cnx_array_pop_back(self) (self).m_vtable->pop_back(&(self))
/// @brief Inserts the given element at the given index in the `CnxArray(T, N)`
///
/// The current size must be strictly less than `N`.
/// `index` must be strictly less than `N`.
/// `index must be strictly less than the current size.
///
/// @param self - The `CnxArray(T, N)` to insert in to
/// @param element - The element to insert into the array
/// @param index - The index to insert `element` at
/// @ingroup cnx_array
#define cnx_array_insert(self, element, index) (self).m_vtable->insert(&(self), (element), (index))
/// @brief Erases the element at `index` from the `CnxArray(T, N)`
///
/// `index` must be strictly less than `N`.
/// `index` must be strictly less than the current size.
///
/// @param self - The `CnxArray(T, N)` to erase from
/// @param index - The index of the element to erase
/// @ingroup cnx_array
#define cnx_array_erase(self, index) (self).m_vtable->erase(&(self), (index))
/// @brief Erases `num_elements` elements from the `CnxArray(T, N)`, starting at `index`
///
/// `index` must be strictly less than `N`
/// `index` must be strictly less than the current size.
/// `index + num_elements` must be strictly less than `N`
/// `index + num_elements` must be strictly less than the current size
///
/// @param self - The `CnxArray(T, N)` to erase from
/// @param index - The index to begin erasing at
/// @param num_elements - The number of elements to erase
/// @ingroup cnx_array
#define cnx_array_erase_n(self, index, num_elements) \
	(self).m_vtable->erase_n(&(self), (index), (num_elements))
/// @brief Frees the given `CnxArray(T, N)`, destroying its elements
///
/// `CnxArray(T, N)` does not allocate memory itself, so it does not necessarily need to be
/// freed, but if elements require a provided destructor to run, then `cnx_array_free` should be
/// called on the array to ensure elements are properly cleaned up.
///
/// @param self - The `CnxArray(T, N)` to free
/// @ingroup cnx_array
#define cnx_array_free(self) (self).m_vtable->free(&(self))
/// @brief Returns a `CnxRandomAccessIterator` into the mutable iteration of the given
/// `CnxArray(T, N)`, starting at the beginning of the iteration (pointing at the beginning of
/// the array)
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator at the beginning of the array
/// @ingroup cnx_array
#define cnx_array_begin(self) (self).m_vtable->begin(&(self))
/// @brief Returns a `CnxRandomAccessIterator` into the mutable iteration of the given
/// `CnxArray(T, N)`, starting at the end of the iteration (pointing at the end of the array)
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator at the end of the array
/// @ingroup cnx_array
#define cnx_array_end(self) (self).m_vtable->end(&(self))
/// @brief Returns a `CnxRandomAccessIterator` into the mutable iteration of the given
/// `CnxArray(T, N)`, starting at the beginning of the reversed iteration (pointing at the end
/// of the array)
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator at the beginning of the reversed array
/// @ingroup cnx_array
#define cnx_array_rbegin(self) (self).m_vtable->rbegin(&(self))
/// @brief Returns a `CnxRandomAccessIterator` into the mutable iteration of the given
/// `CnxArray(T, N)`, starting at the end of the reversed iteration (pointing at the beginning
/// of the array)
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator at the end of the reversed array
/// @ingroup cnx_array
#define cnx_array_rend(self) (self).m_vtable->rend(&(self))
/// @brief Returns whether the given pair of iterators are equal (they belong to the same
/// collection and point to the same element), IE: if `first == second`
///
/// @param first - The LHS iterator of the equality check
/// @param second - The RHS iterator of the equality check
///
/// @return `true` if they are equal, `false` otherwise
/// @ingroup cnx_array
#define cnx_array_iterator_equals(first, second) cnx_iterator_equals(first, second)
/// @brief Returns a `CnxRandomAccessIterator` into the const iteration of the given
/// `CnxArray(T, N)`, starting at the beginning of the iteration (pointing at the beginning of
/// the array)
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator at the beginning of the array
/// @ingroup cnx_array
#define cnx_array_cbegin(self) (self).m_vtable->cbegin(&(self))
/// @brief Returns a `CnxRandomAccessIterator` into the const iteration of the given
/// `CnxArray(T, N)`, starting at the end of the iteration (pointing at the end of the array)
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator at the end of the array
/// @ingroup cnx_array
#define cnx_array_cend(self) (self).m_vtable->cend(&(self))
/// @brief Returns a `CnxRandomAccessIterator` into the const iteration of the given
/// `CnxArray(T, N)`, starting at the beginning of the reversed iteration (pointing at the end
/// of the array)
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator at the beginning of the reversed array
/// @ingroup cnx_array
#define cnx_array_crbegin(self) (self).m_vtable->crbegin(&(self))
/// @brief Returns a `CnxRandomAccessIterator` into the const iteration of the given
/// `CnxArray(T, N)`, starting at the end of the reversed iteration (pointing at the beginning
/// of the array)
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator at the end of the reversed array
/// @ingroup cnx_array
#define cnx_array_crend(self) (self).m_vtable->crend(&(self))
/// @brief Returns whether the given pair of const iterators are equal (they belong to the same
/// collection and point to the same element), IE: if `first == second`
///
/// @param first - The LHS iterator of the equality check
/// @param second - The RHS iterator of the equality check
///
/// @return `true` if they are equal, `false` otherwise
/// @ingroup cnx_array
#define cnx_array_const_iterator_equals(first, second) cnx_iterator_equals(first, second)
/// @brief Returns a `CnxRandomAccessIterator` into the mutable iteration of the given
/// `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator into the array
#define cnx_array_into_iter(self) (self).m_vtable->into_iter(&(self))
/// @brief Returns a `CnxRandomAccessIterator` into the mutable reversed iteration of the given
/// `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator into the reversed array
#define cnx_array_into_reverse_iter(self) (self).m_vtable->into_reverse_iter(&(self))
/// @brief Returns a `CnxRandomAccessIterator` into the const iteration of the given
/// `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator into the array
#define cnx_array_into_const_iter(self) (self).m_vtable->into_const_iter(&(self))
/// @brief Returns a `CnxRandomAccessIterator` into the const reversed iteration of the given
/// `CnxArray(T, N)`
///
/// @param self - The `CnxArray(T, N)` to get an iterator to
///
/// @return a random access iterator into the reversed array
#define cnx_array_into_reverse_const_iter(self) (self).m_vtable->into_reverse_const_iter(&(self))
///
/// @brief declare a `CnxArray(T, N)` variable with this attribute to have `cnx_array_free`
/// automatically called on it at scope end
///
/// @param T - The element type of the `CnxArray(T, N)` instantiation
/// @param N - The capacity of the `CnxArray(T, N)` instantiation
/// @ingroup cnx_array
#define CnxScopedArray(T, N) scoped(CnxArrayIdentifier(T, N, free))

#endif // CNX_ARRAY_DEF
