/// @file StdAllocators.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdAllocators provides an abstraction to modularize custom memory allocators to make
/// custom allocator use simple and configurable
/// @version 0.1
/// @date 2021-08-15
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

#include <malloc.h>
#include <string.h>

#include "StdBasicTypes.h"

#ifndef STD_ALLOCATORS
	#define STD_ALLOCATORS

	#ifndef STD_DEFAULT_ALLOCATOR_FUNCTION
		/// @brief The default `StdAllocator` allocation function
		/// Define this to your desired function to override the default allocation function
		/// @ingroup memory
		#define STD_DEFAULT_ALLOCATOR_FUNCTION malloc
	#endif // STD_DEFAULT_ALLOCATOR_FUNCION

	#ifndef STD_DEFAULT_DEALLOCATOR_FUNCTION
		/// @brief The default `StdAllocator` deallocation (free) function
		/// Define this to your desired function to override the default deallocation function
		/// @ingroup memory
		#define STD_DEFAULT_DEALLOCATOR_FUNCTION free
	#endif // STD_DEFAULT_DEALLOCATOR_FUNCTION

	#ifndef STD_DEFAULT_ALLOCATOR
		/// @brief The default `StdAllocator`
		/// Define this to your desired `StdAllocator` compound literal to override the default
		/// `StdAllocator`
		/// @ingroup memory
		#define STD_DEFAULT_ALLOCATOR                                      \
			((StdAllocator){.m_allocator = STD_DEFAULT_ALLOCATOR_FUNCTION, \
							.m_deallocator = STD_DEFAULT_DEALLOCATOR_FUNCTION})
	#endif // STD_DEFAULT_ALLOCATOR

	#ifndef STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
		/// @brief `StdAllocator` allocation failure strategy. By default allocation failure
		/// will abort the process. Define this to false to disable this behavior
		/// @ingroup memory
		#define STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE 1
	#endif

/// @brief A memory allocation function used with `StdAllocator` must follow this signature
/// @ingroup memory
typedef void* (*allocator_function)(usize);
/// @brief A memory deallocation function used with `StdAllocator` must follow this signature
/// @ingroup memory
typedef void (*deallocator_function)(void*);

/// @brief A `StdAllocator` is a convenient abstraction for wrapping memory allocation and
/// deallocation, making it simple and easy to write allocator-configurable and/or allocator aware
/// software
/// @ingroup memory
typedef struct StdAllocator {
	/// @brief The function to allocate memory with
	allocator_function m_allocator;
	/// @brief The function to deallocate memory with
	deallocator_function m_deallocator;
} StdAllocator;

/// @brief `StdMemory` is a basic wrapper for passing chunks of allocated memory
/// @ingroup memory
typedef struct StdMemory {
	/// @brief `StdAllocator` allocated memory
	void* m_memory;
	/// @brief the size of the memory, in bytes
	usize m_size_bytes;
} StdMemory;

/// @brief Creates a new `StdAllocator` with the default allocation and default deallocation
/// functions
///
/// @return  a default `StdAllocator`
/// @ingroup memory
StdAllocator std_allocator_new(void);

/// @brief Creates a new `StdAllocator` with the given custom allocator
///
/// @param allocator - The allocator's allocation function
/// @param deallocator - The allocator's deallocation (aka free) function
///
/// @return  a custom `StdAllocator`
/// @ingroup memory
StdAllocator
std_allocator_from_custom_allocator(allocator_function allocator, deallocator_function deallocator);

/// @brief Allocates new memory with the given `StdAllocator`
///
/// @param allocator - The allocator to allocate with
/// @param size_bytes - The size to allocate in bytes
///
/// @return  newly allocated memory
/// @ingroup memory
StdMemory std_allocator_allocate(StdAllocator allocator, usize size_bytes);
/// @brief Allocates new memory for an array of the given size with the given `StdAllocator`
///
/// @param allocator - The allocator to allocate with
/// @param num_elements - The number of elements in the array
/// @param element_size_bytes - The size of an array element, in bytes
///
/// @return newly allocated memory
/// @ingroup memory
StdMemory
std_allocator_allocate_array(StdAllocator allocator, usize num_elements, usize element_size_bytes);
/// @brief Allocates new memory large enough to store `new_size_bytes` bytes of data, and copies the
/// old contents over.
///
/// @param allocator - The allocator to allocate memory with
/// @param memory - The memory to be reallocated
/// @param new_size_bytes - The desired new size of the memory, in bytes
///
/// @return reallocated memory
/// @note If reallocation fails, the original memory will be returned unchanged
/// @ingroup memory
StdMemory std_allocator_reallocate(StdAllocator allocator, StdMemory memory, usize new_size_bytes);
/// @brief Allocates new memory large enough to store `new_num_elements` elements of size
/// `element_size_bytes`, and copies the old contents over
///
/// @param allocator - The allocator to allocate memory with
/// @param memory - The memory to be reallocated
/// @param new_num_elements - The desired new number of elements in the memory
/// @param element_size_bytes - The size of a single element, in bytes
///
/// @return reallocated memory
/// @note If reallocation fails, the original memory will be returned unchanged
/// @ingroup memory
StdMemory std_allocator_reallocate_array(StdAllocator allocator,
										 StdMemory memory,
										 usize new_num_elements,
										 usize element_size_bytes);
/// @brief Deallocates (aka frees) the given memory with the given `StdAllocator`
///
/// @param allocator - The allocator to deallocate with
/// @param memory - The memory to deallocate
///
/// @note it is undefined behavior to call this with an allocator that
/// did not allocate the given memory
/// @ingroup memory
void std_allocator_deallocate(StdAllocator allocator, StdMemory memory);

	/// @brief Allocates enough new memory to store a type `T` with the given `StdAllocator`
	///
	/// @param T - The type to allocate memory for
	/// @param allocator - The `StdAllocator` to allocate with
	///
	/// @return  newly allocated memory
	/// @ingroup memory
	#define std_allocator_allocate_t(T, allocator) std_allocator_allocate(allocator, sizeof(T))

	/// @brief Allocates enough new memory to store an array of `num_elements` of type `T` with the
	/// given `StdAllocator`
	///
	/// @param T - The type to allocate memory for
	/// @param allocator - The `StdAllocator to allocate with
	/// @param num_elements - The number of elements in the array
	///
	/// @return  newly allocated memory
	/// @ingroup memory
	#define std_allocator_allocate_array_t(T, allocator, num_elements) \
		std_allocator_allocate_array(allocator, num_elements, sizeof(T))

	/// @brief Allocates new memory large enough to store a `T`, and copies the
	/// old contents over.
	///
	/// @param T - The type to allocate memory for
	/// @param allocator - The `StdAllocator` to allocate memory with
	/// @param memory - The memory to be reallocated
	///
	/// @return reallocated memory
	/// @note If reallocation fails, the original memory will be returned unchanged
	/// @ingroup memory
	#define std_allocator_reallocate_t(T, allocator, memory) \
		std_allocator_reallocate(allocator, memory, sizeof(T))

	/// @brief Allocates new memory large enough to store `new_num_elements` elements of type
	/// `T`, and copies the old contents over
	///
	/// @param T - The type to allocate memory for
	/// @param allocator - The `StdAllocator` to allocate memory with
	/// @param memory - The memory to be reallocated
	/// @param new_num_elements - The desired new number of elements in the memory
	///
	/// @return reallocated memory
	/// @note If reallocation fails, the original memory will be returned unchanged
	/// @ingroup memory
	#define std_allocator_reallocate_array_t(T, allocator, memory, new_num_elements) \
		std_allocator_reallocate_array(allocator, memory, new_num_elements, sizeof(T))

	/// @brief Deallocates the memory at `data_ptr`, of type `T`, with the given `StdAllocator`
	///
	/// @param T - The type to deallocate memory for
	/// @param allocator - The `StdAllocator` to deallocate with
	/// @param data_ptr - The pointer to memory to deallocate
	/// @ingroup memory
	#define std_allocator_deallocate_t(T, allocator, data_ptr) \
		std_allocator_deallocate((allocator),                  \
								 ((StdMemory){.m_memory = (data_ptr), .m_size_bytes = sizeof(T)}))

	/// @brief Deallocates the memory for an array of type `T`s at `data_ptr` of `num_elements` size
	///
	/// @param T - The type of elements stored in the array to deallocate
	/// @param allocator - The `StdAllocator` to deallocate with
	/// @param data_ptr - The pointer to memory to deallocate
	/// @param num_elements - The number of elements in the array to deallocate
	/// @ingroup memory
	#define std_allocator_deallocate_array_t(T, allocator, data_ptr, num_elements) \
		std_allocator_deallocate(                                                  \
			(allocator),                                                           \
			((StdMemory){.m_memory = (data_ptr), .m_size_bytes = sizeof(T) * (num_elements)}))

	/// @brief Copies memory containing an array of type `T` from `src_ptr` to `dst_ptr`
	///
#include "../include/C2nxt/StdAllocators.h"

#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/C2nxt/StdMath.h"

StdAllocator std_allocator_new(void) {
	return STD_DEFAULT_ALLOCATOR;
}

StdAllocator std_allocator_from_custom_allocator(allocator_function allocator,
												 deallocator_function deallocator) {
	return (StdAllocator){.m_allocator = allocator, .m_deallocator = deallocator};
}

StdMemory std_allocator_allocate(StdAllocator allocator, usize size_bytes) {
	let_mut mem = allocator.m_allocator(size_bytes);

	if(mem != nullptr) {
		memset(mem, 0, size_bytes);
		return (StdMemory){.m_memory = mem, .m_size_bytes = size_bytes};
	}
	else {
#if STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE == 1
		fprintf(stderr, "Failed to allocate %zu bytes of memory, aborting\n", size_bytes);
		abort();
#endif // STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
		return (StdMemory){.m_memory = nullptr, .m_size_bytes = 0};
	}
}

StdMemory
std_allocator_allocate_array(StdAllocator allocator, usize num_elements, usize element_size_bytes) {
	return std_allocator_allocate(allocator, num_elements * element_size_bytes);
}

StdMemory std_allocator_reallocate(StdAllocator allocator, StdMemory memory, usize new_size_bytes) {
	let_mut mem = allocator.m_allocator(new_size_bytes);

	if(mem != nullptr) {
		memcpy(mem, memory.m_memory, std_min(new_size_bytes, memory.m_size_bytes));
		return (StdMemory){.m_memory = mem, .m_size_bytes = new_size_bytes};
	}
	else {
#if STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
		fprintf(stderr,
				"Failed to allocate " STD_OFORMAT_USIZE " bytes of memory, aborting\n",
				new_size_bytes);
		abort();
#else
		return memory;
#endif // STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
	}
}

StdMemory std_allocator_reallocate_array(StdAllocator allocator,
										 StdMemory memory,
										 usize new_num_elements,
										 usize element_size_bytes) {
	return std_allocator_reallocate(allocator, memory, new_num_elements * element_size_bytes);
}

void std_allocator_deallocate(StdAllocator allocator, StdMemory memory) {
	allocator.m_deallocator(memory.m_memory);
}
