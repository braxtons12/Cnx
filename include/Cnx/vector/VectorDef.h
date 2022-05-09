/// @file VectorDef.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module provides macro definitions for implementing and working with `CnxVector(T)`
/// @version 0.1.1
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

#include <Cnx/Def.h>

#ifndef CNX_VECTOR_DEF
	#define CNX_VECTOR_DEF

	/// @brief macro alias for a `CnxVector(T)` containing `T`s
	///
	/// `CnxVector(T)` is a bounds-safe, allocator aware, generic dynamic-capacity array type. It is
	/// implemented as a struct template, which enables 100% type safety, while providing
	/// abstractions that allow type agnostic use. It has greatly increased ergonomics over manual
	/// array management and provides many useful features such as random access iterators and
	/// optional scoped destruction.
	///
	/// Example:
	/// @code {.c}
	/// #include <Cnx/Vector.h>
	/// #include <Cnx/IO.h>
	///
	/// // create a `CnxVector(i32)` with default allocator, element default-constructor, and
	/// element
	/// // destructor
	/// let_mut vec = cnx_vector_new(i32);
	///
	/// // append 10 elements to the vector
	/// ranged_for(i, 0, 9) {
	/// 	cnx_vector_push_back(vec, i);
	/// }
	///
	/// // prints information about `vec` to `stdout`
	/// // because `CnxVector(T)` is generic, and thus can't be used in a `_Generic` match arm prior
	/// to
	/// // the type's instantiation, we have to explicitly cast to the `CnxFormat` Trait to get
	/// // `CnxVector(T)`'s implementation of the Trait
	/// println("{}", Cnx_as_format_t(CnxVector(i32), vec));
	/// // prints `vec`'s elements to `stdout`
	/// foreach(elem, vec) {
	///		println("{}", elem);
	/// }
	///
	/// {
	/// 	let_mut CnxScopedVector(i32) vec2 = cnx_vector_new(i32);
	/// 	foreach(elem, vec) {
	///			cnx_vector_push_back(vec2, elem);
	/// 	}
	/// }
	/// // `vec2` is freed here
	/// @endcode
	///
	/// Like other Cnx collections, `CnxVector(T)` provides its type-agnostic usage through a
	/// vtable pointer contained in the struct, and provides macros which wrap the usage of the
	/// vtable, making access simpler. If you prefer to not use this method of access, you can call
	/// the typed functions directly by in-fixing the contained type in the associated function
	/// name. IE: for `CnxVector(i32)`, `vec`, the equivalent function call for
	/// `cnx_vector_push_back(vec, element)` would be `cnx_vector_i32_push_back(&vec, element)`
	/// @ingroup cnx_vector
	#define CnxVector(T) CONCAT2(CnxVector, T)
	/// @brief macro alias for the concrete type for an iterator into the mutable iteration of a
	/// `CnxVector(T)` containing `T`s
	#define CnxVectorIterator(T) CONCAT2(CnxVector(T), Iterator)
	/// @brief macro alias for the concrete type for an iterator into the const iteration of a
	/// `CnxVector(T)` containing `T`s
	#define CnxVectorConstIterator(T) CONCAT2(CnxVector(T), ConstIterator)
	/// @brief macro alias for an identifier (type, function, etc) associated with a
	/// `CnxVector(T)` containing `T`s
	#define CnxVectorIdentifier(T, Identifier) CONCAT3(cnx_vector_, T, CONCAT2(_, Identifier))

	/// @brief The default small-vector optimization capacity if not given as a template parameter
	/// @ingroup cnx_vector
	#define CNX_VECTOR_DEFAULT_SHORT_OPT_CAPACITY 8U

	/// @brief The default heap-allocated capacity if not given as a template parameter
	/// @ingroup cnx_vector
	#define CNX_VECTOR_DEFAULT_LONG_CAPACITY 16U

	/// @brief Creates a new `CnxVector(T)` with defaulted associated functions and initial
	/// capacity.
	///
	/// Creates a new `CnxVector(T)` with:
	/// 1. defaulted initial capacity
	/// 2. defaulted associated element default-constructor
	/// 3. defaulted associated element copy-constructor
	/// 4. defaulted associated element destructor
	/// 5. defaulted associated memory allocator
	///
	/// @param T - The element type of the `CnxVector(T)` instantiation to create
	///
	/// @return a new `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_new(T) CnxVectorIdentifier(T, new)()
	/// @brief Creates a new `CnxVector(T)` with defaulted capacity and associated functions and
	/// provided memory allocator.
	///
	/// Creates a new `CnxVector(T)` with:
	/// 1. defaulted initial capacity
	/// 2. defaulted associated element default-constructor
	/// 3. defaulted associated element copy-constructor
	/// 4. defaulted element destructor
	/// 5. user-provided memory allocator
	///
	/// @param T - The element type of the `CnxVector(T)` instantiation to create
	/// @param allocator - The `CnxAllocator` to use for memory allocations
	///
	/// @return a new `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_new_with_allocator(T, allocator) \
		CnxVectorIdentifier(T, new_with_allocator)(allocator)
	/// @brief Creates a new `CnxVector(T)` with defaulted capacity and provided associated
	/// functions.
	///
	/// Creates a new `CnxVector(T)` with:
	/// 1. defaulted initial capacity
	/// 2. possibly user-provided element default-constructor
	/// 3. possibly user-provided associated element copy-constructor
	/// 4. possibly user-provided element destructor
	/// 5. user-provided memory allocator
	///
	/// @param T - The element type of the `CnxVector(T)` instantiation to create
	/// @param collection_data_ptr - The `CnxCollectionData(CollectionType)` containing the element
	/// default-constructor, element copy-constructor, element destructor, and memory allocator
	/// to use
	///
	/// @return a new `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_new_with_collection_data(T, collection_data_ptr) \
		CnxVectorIdentifier(T, new_with_collection_data)(collection_data_ptr)
	/// @brief Creates a new `CnxVector(T)` with defaulted capacity and provided associated
	/// functions and memory allocator.
	///
	/// Creates a new `CnxVector(T)` with:
	/// 1. defaulted initial capacity
	/// 2. possibly user-provided element default-constructor
	/// 3. possibly user-provided associated element copy-constructor
	/// 4. possibly user-provided element destructor
	/// 5. user-provided memory allocator
	///
	/// @param T - The element type of the `CnxVector(T)` instantiation to create
	/// @param allocator - The `CnxAllocator` to use for memory allocations
	/// @param collection_data_ptr - The `CnxCollectionData(CollectionType)` containing the element
	/// default-constructor, element copy-constructor, element destructor, and memory allocator
	/// to use
	///
	/// @return a new `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_new_with_allocator_and_collection_data(T, allocator, collection_data_ptr) \
		CnxVectorIdentifier(T, new_with_allocator_and_collection_data)(allocator,                \
																	   collection_data_ptr)
	/// @brief Creates a new `CnxVector(T)` with __at least__ the given capacity and defaulted
	/// associated functions.
	///
	/// Creates a new `CnxVector(T)` with:
	/// 1. given initial capacity
	/// 2. defaulted associated element default-constructor
	/// 3. defaulted associated element copy-constructor
	/// 4. defaulted associated element destructor
	/// 5. defaulted memory allocator
	///
	/// @param T - The element type of the `CnxVector(T)` instantiation to create
	/// @param capacity - The initial capacity of the vector
	///
	/// @return a new `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_new_with_capacity(T, capacity) \
		CnxVectorIdentifier(T, new_with_capacity)(capacity)
	/// @brief Creates a new `CnxVector(T)` with __at least__ the given capacity, defaulted
	/// associated functions, and provided memory allocator.
	///
	/// Creates a new `CnxVector(T)` with:
	/// 1. given initial capacity
	/// 2. defaulted associated element default-constructor
	/// 3. defaulted associated element copy-constructor
	/// 4. defaulted associated element destructor
	/// 5. defaulted memory allocator
	///
	/// @param T - The element type of the `CnxVector(T)` instantiation to create
	/// @param capacity - The initial capacity of the vector
	/// @param allocator - The `CnxAllocator` to use for memory allocations
	///
	/// @return a new `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_new_with_capacity_and_allocator(T, capacity, allocator) \
		CnxVectorIdentifier(T, new_with_capacity_and_allocator)(capacity, allocator)
	/// @brief Creates a new `CnxVector(T)` with __at least__ the given capacity and provided
	/// associated functions.
	///
	/// Creates a new `CnxVector(T)` with:
	/// 1. given initial capacity
	/// 2. possibly user-provided associated element default-constructor
	/// 3. possibly user-provided associated element copy-constructor
	/// 4. possibly user-provided associated element destructor
	/// 5. user-provided memory allocator
	///
	/// @param T - The element type of the `CnxVector(T)` instantiation to create
	/// @param capacity - The initial capacity of the vector
	/// @param collection_data_ptr - The `CnxCollectionData(CollectionType)` containing the element
	/// default-constructor, element copy-constructor, element destructor, and memory allocator
	/// to use
	///
	/// @return a new `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_new_with_capacity_and_collection_data(T, capacity, collection_data_ptr) \
		CnxVectorIdentifier(T, new_with_capacity_and_collection_data)(capacity, collection_data_ptr)
	/// @brief Creates a new `CnxVector(T)` with __at least__ the given capacity and provided
	/// associated functions and memory allocator.
	///
	/// Creates a new `CnxVector(T)` with:
	/// 1. given initial capacity
	/// 2. possibly user-provided associated element default-constructor
	/// 3. possibly user-provided associated element copy-constructor
	/// 4. possibly user-provided associated element destructor
	/// 5. user-provided memory allocator
	///
	/// @param T - The element type of the `CnxVector(T)` instantiation to create
	/// @param capacity - The initial capacity of the vector
	/// @param allocator - The `CnxAllocator` to use for memory allocations
	/// @param collection_data_ptr - The `CnxCollectionData(CollectionType)` containing the element
	/// default-constructor, element copy-constructor, element destructor, and memory allocator
	/// to use
	///
	/// @return a new `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_new_with_capacity_allocator_and_collection_data(T,                   \
																	   capacity,            \
																	   allocator,           \
																	   collection_data_ptr) \
		CnxVectorIdentifier(T,                                                              \
							new_with_capacity_allocator_and_collection_data)(capacity,      \
																			 allocator,     \
																			 collection_data_ptr)
	/// @brief Clones the given `CnxVector(T)`
	///
	/// Creates a deep copy of the given `CnxVector(T)` calling the associated copy constructor
	/// for each element stored in it.
	///
	/// @param self - The `CnxVector(T)` to clone
	///
	/// @return a clone of the given vector
	/// @note Requires that a copy constructor is defined for the elements
	/// contained in the vector, either the default one provided by the implementation, or a
	/// user-defined one. If it isn't provided or is explicitly provided as `nullptr` when
	/// creating the `CnxCollectionData(CollectionType)` associated with this vector, this
	/// __may__ trigger a compiler error on clang, and will always trigger a runtime assert for
	/// all compilers
	/// @ingroup cnx_vector
	#define cnx_vector_clone(self) (self).m_vtable->clone(&(self))
	/// @brief Returns a mutable reference to the element at the given `index` into the given
	/// `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get an element from
	/// @param index - The index of the desired element
	///
	/// @return a mutable reference to the element at the given index
	/// @ingroup cnx_vector
	#define cnx_vector_at_mut(self, index) *((self).m_vtable->at_mut(&(self), (index)))
	/// @brief Returns a const reference to the element at the given `index` into the given
	/// `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get an element from
	/// @param index - The index of the desired element
	///
	/// @return a const reference to the element at the given index
	/// @ingroup cnx_vector
	#define cnx_vector_at(self, index) *((self).m_vtable->at_const(&(self), (index)))
	/// @brief Returns a mutable reference to the first element in the given `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get the first element from
	///
	/// @return a mutable reference to the first element
	/// @ingroup cnx_vector
	#define cnx_vector_front_mut(self) *((self).m_vtable->front_mut(&(self)))
	/// @brief Returns a const reference to the first element in the given `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get the first element from
	///
	/// @return a const reference to the first element
	/// @ingroup cnx_vector
	#define cnx_vector_front(self) *((self).m_vtable->front_const(&(self)))
	/// @brief Returns a mutable reference to the last element in the given `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get the last element from
	///
	/// @return a mutable reference to the last element
	/// @ingroup cnx_vector
	#define cnx_vector_back_mut(self) *((self).m_vtable->back_mut(&(self)))
	/// @brief Returns a const reference to the last element in the given `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get the last element from
	///
	/// @return a const reference to the last element
	/// @ingroup cnx_vector
	#define cnx_vector_back(self) *((self).m_vtable->back_const(&(self)))
	/// @brief Returns a pointer to the mutable raw array containing the given `CnxVector(T)`'s
	/// elements
	///
	/// @param self - The `CnxVector(T)` to get the raw array from
	///
	/// @return a pointer to the raw array
	/// @ingroup cnx_vector
	#define cnx_vector_data_mut(self) (self).m_vtable->data_mut(&(self))
	/// @brief Returns a pointer to the const raw array containing the given `CnxVector(T)`'s
	/// elements
	///
	/// @param self - The `CnxVector(T)` to get the raw array from
	///
	/// @return a pointer to the raw array
	/// @ingroup cnx_vector
	#define cnx_vector_data(self) (self).m_vtable->data_const(&(self))
	/// @brief Returns whether the given `CnxVector(T)`is empty
	///
	/// @param self - The `CnxVector(T)` to check for emptiness
	///
	/// @return `true` if empty, `false` otherwise
	/// @ingroup cnx_vector
	#define cnx_vector_is_empty(self) (self).m_vtable->is_empty(&(self))
	/// @brief Returns whether the given `CnxVector(T)` is full (size equals capacity)
	///
	/// @param self - The `CnxVector(T)` to check for fullness
	///
	/// @return `true` if full, `false` otherwise
	/// @ingroup cnx_vector
	#define cnx_vector_is_full(self) (self).m_vtable->is_full(&(self))
	/// @brief Returns the current size of the given `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get the size of
	///
	/// @return the size of the `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_size(self) (self).m_vtable->size(&(self))
	/// @brief Returns the maximum possible size of a `CnxVector(T)` containing type `T`
	///
	/// @param T - The element type of the `CnxVector(T)` instantiation
	///
	/// @return The maximum possible size
	/// @ingroup cnx_vector
	#define cnx_vector_max_size(T) CONCAT3(cnx_vector_, T, _max_size)()
	/// @brief Returns the current capacity of the given `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get the capacity of
	///
	/// @return the capacity of the `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_capacity(self) (self).m_vtable->capacity(&(self))
	/// @brief Ensures enough memory to store at least `new_capacity` number of elements in the
	/// given `CnxVector(T)`, reallocating if necessary.
	///
	/// @param self - The `CnxVector(T)` to reserve memory for
	/// @param new_capacity - The desired minimum number of storable elements
	/// @ingroup cnx_vector
	#define cnx_vector_reserve(self, new_capacity) (self).m_vtable->reserve(&(self), (new_capacity))
	/// @brief Resizes the given `CnxVector(T)` to `new_size` number of elements.
	/// If `new_size` is greater than the current size, this will allocate memory if necessary
	/// and default-construct new elements. If `new_size` is less than the current size, this
	/// will destruct `size - new_size` number of elements and, if `new_size` is less than the
	/// SSO capacity, deallocate memory.
	///
	/// @param self - The `CnxVector(T)` to resize
	/// @param new_size - The desired size of the `CnxVector(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_resize(self, new_size) (self).m_vtable->resize(&(self), (new_size))
	/// @brief Shrinks the memory allocation for the given `CnxVector(T)` to match its current
	/// size
	///
	/// @param self - The `CnxVector(T)` to shrink
	/// @ingroup cnx_vector
	#define cnx_vector_shrink_to_fit(self) (self).m_vtable->shrink_to_fit(&(self))
	/// @brief Clears the contents of the given `CnxVector(T)`, destructing all of its elements
	///
	/// @param self - The `CnxVector(T)` to clear
	/// @ingroup cnx_vector
	#define cnx_vector_clear(self) (self).m_vtable->clear(&(self))
	/// @brief Appends the given element to the end of the given `CnxVector(T)`, reallocating
	/// memory if necessary
	///
	/// @param self - The `CnxVector(T)` to append to
	/// @param element - The element to append
	/// @ingroup cnx_vector
	#define cnx_vector_push_back(self, element) (self).m_vtable->push_back(&(self), (element))
	/// @brief Returns the last element in the given `CnxVector(T)` and removes it, if the size
	/// is greater than zero.
	///
	/// @param self - The `CnxVector(T)` to get the last element from
	///
	/// @return `Some(T)` if size > 0, otherwise `None(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_pop_back(self) (self).m_vtable->pop_back(&(self))
	/// @brief Returns the first element in the given `CnxVector(T)` and removes it, if the size
	/// is greater than zero.
	///
	/// @param self - The `CnxVector(T)` to get the last element from
	///
	/// @return `Some(T)` if size > 0, otherwise `None(T)`
	/// @ingroup cnx_vector
	#define cnx_vector_pop_front(self) (self).m_vtable->pop_back(&(self))
	/// @brief Inserts the given element at the given index in the given `CnxVector(T)`, moving
	/// elements backward in the vector if necessary
	///
	/// @param self - The `CnxVector(T)` to insert into
	/// @param element - The element to insert
	/// @param index - The index at which to insert `element`
	///
	/// @note If `index` is past the end of the `CnxVector(T)` (`index` > size), subsequent
	/// behavior of the collection is undefined
	/// @ingroup cnx_vector
	#define cnx_vector_insert(self, element, index) \
		(self).m_vtable->insert(&(self), (element), (index))
	/// @brief Removes the element at the given index from the given `CnxVector(T)`, moving
	/// elements forward in the vector if necessary
	///
	/// @param self - The `CnxVector(T)` to remove an element from
	/// @param index - The index of the element to remove
	/// @ingroup cnx_vector
	#define cnx_vector_erase(self, index) (self).m_vtable->erase(&(self), (index))
	/// @brief Removes `num_elements` elements from the given `CnxVector(T)`, starting with the
	/// element at `index`, and moving elements forward in the vector afterward, if necessary.
	///
	/// @param self - The `CnxVector(T)` to remove elements from
	/// @param index - The index to begin removal at
	/// @param num_elements - The number of elements to remove from the vector
	///
	/// @note The given `CnxVector(T)` must contain the specified number of elements within the
	/// range [index, size). IE: `index + num_elements` must be strictly less than the current
	/// size of the vector
	/// @ingroup cnx_vector
	#define cnx_vector_erase_n(self, index, num_elements) \
		(self).m_vtable->erase_n(&(self), (index), (num_elements))
	/// @brief Frees the given `CnxVector(T)`, calling the element destructor on each element
	/// and freeing any allocated memory
	///
	/// @param self - The `CnxVector(T)` to free
	/// @ingroup cnx_vector
	#define cnx_vector_free(self) (self).m_vtable->free(&(self))
	/// @brief Returns a `CnxRandomAccessIterator` into the mutable iteration of the given
	/// `CnxVector(T)`, starting at the beginning of the iteration (pointing at the beginning of
	/// the vector)
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the vector
	/// @ingroup cnx_vector
	#define cnx_vector_begin(self) (self).m_vtable->begin(&(self))
	/// @brief Returns a `CnxRandomAccessIterator` into the mutable iteration of the given
	/// `CnxVector(T)`, starting at the end of the iteration (pointing at the end of the vector)
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the end of the vector
	/// @ingroup cnx_vector
	#define cnx_vector_end(self) (self).m_vtable->end(&(self))
	/// @brief Returns a `CnxRandomAccessIterator` into the mutable iteration of the given
	/// `CnxVector(T)`, starting at the beginning of the reversed iteration (pointing at the end
	/// of the vector)
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the reversed vector
	/// @ingroup cnx_vector
	#define cnx_vector_rbegin(self) (self).m_vtable->rbegin(&(self))
	/// @brief Returns a `CnxRandomAccessIterator` into the mutable iteration of the given
	/// `CnxVector(T)`, starting at the end of the reversed iteration (pointing at the beginning
	/// of the vector)
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the end of the reversed vector
	/// @ingroup cnx_vector
	#define cnx_vector_rend(self) (self).m_vtable->rend(&(self))
	/// @brief Returns whether the given pair of iterators are equal (they belong to the same
	/// collection and point to the same element), IE: if `first == second`
	///
	/// @param first - The LHS iterator of the equality check
	/// @param second - The RHS iterator of the equality check
	///
	/// @return `true` if they are equal, `false` otherwise
	/// @ingroup cnx_vector
	#define cnx_vector_iterator_equals(first, second) cnx_iterator_equals(first, second)
	/// @brief Returns a `CnxRandomAccessIterator` into the const iteration of the given
	/// `CnxVector(T)`, starting at the beginning of the iteration (pointing at the beginning of
	/// the vector)
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the vector
	/// @ingroup cnx_vector
	#define cnx_vector_cbegin(self) (self).m_vtable->cbegin(&(self))
	/// @brief Returns a `CnxRandomAccessIterator` into the const iteration of the given
	/// `CnxVector(T)`, starting at the end of the iteration (pointing at the end of the vector)
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the end of the vector
	/// @ingroup cnx_vector
	#define cnx_vector_cend(self) (self).m_vtable->cend(&(self))
	/// @brief Returns a `CnxRandomAccessIterator` into the const iteration of the given
	/// `CnxVector(T)`, starting at the beginning of the reversed iteration (pointing at the end
	/// of the vector)
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the beginning of the reversed vector
	/// @ingroup cnx_vector
	#define cnx_vector_crbegin(self) (self).m_vtable->crbegin(&(self))
	/// @brief Returns a `CnxRandomAccessIterator` into the const iteration of the given
	/// `CnxVector(T)`, starting at the end of the reversed iteration (pointing at the beginning
	/// of the vector)
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator at the end of the reversed vector
	/// @ingroup cnx_vector
	#define cnx_vector_crend(self) (self).m_vtable->crend(&(self))
	/// @brief Returns whether the given pair of const iterators are equal (they belong to the
	/// same collection and point to the same element), IE: if `first == second`
	///
	/// @param first - The LHS iterator of the equality check
	/// @param second - The RHS iterator of the equality check
	///
	/// @ingroup cnx_vector
	#define cnx_vector_const_iterator_equals(first, second) cnx_iterator_equals(first, second)
	/// @brief Returns a `CnxRandomAccessIterator` into the mutable iteration of the given
	/// `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator into the vector
	#define cnx_vector_into_iter(self) (self).m_vtable->into_iter(&(self))
	/// @brief Returns a `CnxRandomAccessIterator` into the mutable reversed iteration of the
	/// given `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator into the reversed vector
	#define cnx_vector_into_reverse_iter(self) (self).m_vtable->into_reverse_iter(&(self))
	/// @brief Returns a `CnxRandomAccessIterator` into the const iteration of the given
	/// `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator into the vector
	#define cnx_vector_into_const_iter(self) (self).m_vtable->into_const_iter(&(self))
	/// @brief Returns a `CnxRandomAccessIterator` into the const reversed iteration of the
	/// given `CnxVector(T)`
	///
	/// @param self - The `CnxVector(T)` to get an iterator to
	///
	/// @return a random access iterator into the reversed vector
	#define cnx_vector_into_reverse_const_iter(self) \
		(self).m_vtable->into_reverse_const_iter(&(self))

	/// @brief declare a `CnxVector(T)` variable with this attribute to have `Cnx_vector_free`
	/// automatically called on it at scope end
	///
	/// @param T - The element type of the `CnxVector(T)` instantiation
	/// @ingroup cnx_vector
	#define CnxScopedVector(T) scoped(CnxVectorIdentifier(T, free))

#endif // CNX_VECTOR_DEF
