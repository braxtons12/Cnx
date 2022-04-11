/// @file StdAllocators.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdAllocators provides an abstraction to modularize custom memory allocators to make
/// custom allocator use simple and configurable
/// @version 0.2.2
/// @date 2022-04-11
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

#include <C2nxt/StdBasicTypes.h>
#include <C2nxt/StdTrait.h>
#include <malloc.h>
#include <string.h>

#ifndef STD_ALLOCATORS
	#define STD_ALLOCATORS

/// @struct StdAllocator
/// @brief A `StdAllocator` is a convenient abstraction for wrapping memory allocation and
/// deallocation, making it simple and easy to write allocator-configurable and/or allocator aware
/// software
/// @ingroup memory
Trait(StdAllocator,
	  /// @brief Allocates `size_bytes` worth of memory
	  /// @param self - The allocator to allocate with
	  /// @param size_bytes - The amount of memory to allocate
	  /// @return allocated memory
	  /// @ingroup memory
	  void* (*const allocate)(StdAllocator* restrict self, usize size_bytes);
	  /// @brief Reallocates `memory` to be `new_size_bytes` large
	  /// @param self - The allocator to allocate with
	  /// @param memory - The memory to grow or shrink
	  /// @param new_size_bytes - The size the new allocation should be
	  /// @return allocated memory
	  /// @ingroup memory
	  void* (*const reallocate)(StdAllocator* restrict self, void* memory, usize new_size_bytes);
	  /// @brief Deallocates `memory`
	  /// @param self - The allocator to deallocate with
	  /// @param memory - The memory to deallocate
	  /// @ingroup memory
	  void (*const deallocate)(StdAllocator* restrict self, void* memory));

/// @brief A memory allocation function used with `StdAllocator` must follow this signature
/// @ingroup memory
typedef void* (*allocate_function)(StdAllocator* restrict self, usize size_bytes);
/// @brief A memory reallocation function used with `StdAllocator` must follow this signature
/// @ingroup memory
typedef void* (*reallocate_function)(StdAllocator* restrict self,
									 void* memory,
									 usize new_size_bytes);
/// @brief A memory deallocation function used with `StdAllocator` must follow this signature
/// @ingroup memory
typedef void (*deallocate_function)(StdAllocator* restrict self, void* memory);

/// @brief Wrapper for `malloc` so it can be used in `StdAllocator`s
/// Behavior matches that of `malloc`
///
/// @param self - The state of the allocator (unused)
/// @param size_bytes - The number of bytes to allocate
///
/// @return the allocated memory
/// @ingroup memory
[[nodiscard]] [[not_null(1)]] void*
std_allocate([[maybe_unused]] StdAllocator* restrict self, usize size_bytes)
	std_disable_if(!self, "Can't allocate memory with a null allocator");

/// @brief Wrapper for `realloc` so it can be used in `StdAllocator`s
/// Behavior matches that of `realloc`
///
/// @param self - The state of the allocator (unused)
/// @param memory - The memory allocation to shrink or grow
/// @param new_size_bytes - The number of bytes to allocate
///
/// @return the allocated memory
/// @ingroup memory
[[nodiscard]] [[not_null(1)]] void*
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std_reallocate([[maybe_unused]] StdAllocator* restrict self, void* memory, usize new_size_bytes)
	std_disable_if(!self, "Can't reallocate with a null allocator")
		std_disable_if(!memory, "Can't reallocate a nullptr");

/// @brief Wrapper for `free` so it can be used in `StdAllocator`s
/// Behavior matches that of `free`
///
/// @param self - The state of the allocator (unused)
/// @param memory - The memory allocation to deallocate
/// @ingroup memory
[[not_null(1, 2)]] void
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std_deallocate([[maybe_unused]] StdAllocator* restrict self, void* memory)
	std_disable_if(!self, "Can't deallocate with a null allocator")
		std_disable_if(!memory, "Can't free a nullptr");

	#ifndef STD_DEFAULT_ALLOCATOR_FUNCTION
		/// @brief The default `StdAllocator` allocation function
		/// Define this to your desired function to override the default allocation function
		/// @ingroup memory
		#define STD_DEFAULT_ALLOCATOR_FUNCTION std_allocate
	#endif // STD_DEFAULT_ALLOCATOR_FUNCTION

	#ifndef STD_DEFAULT_DEALLOCATOR_FUNCTION
		/// @brief The default `StdAllocator` deallocation (free) function
		/// Define this to your desired function to override the default deallocation function
		/// @ingroup memory
		#define STD_DEFAULT_DEALLOCATOR_FUNCTION std_deallocate
	#endif // STD_DEFAULT_DEALLOCATOR_FUNCTION

	#ifndef STD_DEFAULT_REALLOCATOR_FUNCTION
		/// @brief The default `StdAllocator` reallocation (realloc) function
		/// Define this to your desired function to override the default reallocation function
		#define STD_DEFAULT_REALLOCATOR_FUNCTION std_reallocate
	#endif // STD_DEFAULT_REALLOCATOR_FUNCTION

	#ifndef STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
		/// @brief `StdAllocator` allocation failure strategy. By default allocation failure
		/// will abort the process. Define this to false to disable this behavior
		/// @ingroup memory
		#define STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE 1
	#endif

typedef struct StdStatelessAllocator {
} StdStatelessAllocator;

/// @brief Creates a new `StdAllocator` with the default allocation and default deallocation
/// functions
///
/// @return  a default `StdAllocator`
/// @ingroup memory
[[nodiscard]] StdAllocator std_allocator_new(void);

	/// @brief Implements `StdAllocator` for the given custom allocator type
	///
	/// @param SelfType - The type of `self`
	/// @param allocate - The function to allocate memory with
	/// @param reallocate - The function to reallocate memory with
	/// @param deallocate - The function to deallocate memory with
	/// @ingroup memory
	#define impl_std_allocator_for_custom_typed_allocator(SelfType,   \
														  allocate,   \
														  reallocate, \
														  deallocate) \
		ImplTraitFor(StdAllocator, SelfType, allocate, reallocate, deallocate)

	/// @brief Creates a new `StdAllocator` with the given custom allocator
	///
	/// @param SelfType - The type of `self`
	/// @param self - The allocator's state
	///
	/// @return  a custom `StdAllocator`
	/// @ingroup memory
	#define std_allocator_from_custom_typed_allocator(SelfType, self) \
		as_trait(StdAllocator, SelfType, self)

	/// @brief Creates a new "stateless" `StdAllocator` with the given custom allocator functions
	///
	/// @param allocate_function - The function to allocate memory with
	/// @param reallocate_function - The function to reallocate memory with
	/// @param deallocate_function - The function to deallocate memory with
	///
	/// @return  a custom `StdAllocator`
	/// @ingroup memory
	#define std_allocator_from_custom_stateless_allocator(allocate_function,        \
														  reallocate_function,      \
														  deallocate_function)      \
		(StdAllocator) {                                                            \
			.m_self = nullptr,                                                      \
			.m_vtable = &(StdAllocatorVTable){.allocate = (allocate_function),      \
											  .reallocate = (reallocate_function),  \
											  .deallocate = (deallocate_function)}, \
		}

	#ifndef STD_DEFAULT_ALLOCATOR
		/// @brief The default `StdAllocator`
		/// Define this to your desired `StdAllocator` compound literal to override the default
		/// `StdAllocator`
		/// @ingroup memory
		#define STD_DEFAULT_ALLOCATOR                                                       \
			std_allocator_from_custom_stateless_allocator(STD_DEFAULT_ALLOCATOR_FUNCTION,   \
														  STD_DEFAULT_REALLOCATOR_FUNCTION, \
														  STD_DEFAULT_DEALLOCATOR_FUNCTION)
	#endif // STD_DEFAULT_ALLOCATOR

/// @brief The default `StdAllocator`
/// @ingroup memory
extern const StdAllocator DEFAULT_ALLOCATOR;

/// @brief Allocates new memory with the given `StdAllocator`
///
/// @param allocator - The allocator to allocate with
/// @param size_bytes - The size to allocate in bytes
///
/// @return  newly allocated memory
/// @ingroup memory
[[nodiscard]] void* std_allocator_allocate(StdAllocator allocator, usize size_bytes);
/// @brief Allocates new memory for an array of the given size with the given `StdAllocator`
///
/// @param allocator - The allocator to allocate with
/// @param num_elements - The number of elements in the array
/// @param element_size_bytes - The size of an array element, in bytes
///
/// @return newly allocated memory
/// @ingroup memory
[[nodiscard]] void*
std_allocator_allocate_array(StdAllocator allocator, usize num_elements, usize element_size_bytes);
/// @brief Allocates new memory large enough to store `new_size_bytes` bytes of data, and copies the
/// old contents over.
///
/// @param allocator - The allocator to allocate memory with
/// @param memory - The memory to be reallocated
/// @param old_size_bytes - The old size of the memory, in bytes
/// @param new_size_bytes - The desired new size of the memory, in bytes
///
/// @return reallocated memory
/// @note If reallocation fails, the original memory will be returned unchanged
/// @ingroup memory
[[nodiscard]] void* std_allocator_reallocate(StdAllocator allocator,
											 void* memory,
											 usize old_size_bytes,
											 usize new_size_bytes);
/// @brief Allocates new memory large enough to store `new_num_elements` elements of size
/// `element_size_bytes`, and copies the old contents over
///
/// @param allocator - The allocator to allocate memory with
/// @param memory - The memory to be reallocated
/// @param old_num_elements - The old number of elements in the memory
/// @param new_num_elements - The desired new number of elements in the memory
/// @param element_size_bytes - The size of a single element, in bytes
///
/// @return reallocated memory
/// @note If reallocation fails, the original memory will be returned unchanged
/// @ingroup memory
[[nodiscard]] void* std_allocator_reallocate_array(StdAllocator allocator,
												   void* memory,
												   usize old_num_elements,
												   usize new_num_elements,
												   usize element_size_bytes);
/// @brief Deallocates (aka frees) the given memory with the given `StdAllocator`
///
/// @param allocator - The allocator to deallocate with
/// @param memory - The memory to deallocate
///
/// @note it is undefined behavior to call this with an allocator that did not allocate the given
/// memory
/// @ingroup memory
void std_allocator_deallocate(StdAllocator allocator, void* memory);

/// @brief Allocates enough new memory to store a type `T` with the given `StdAllocator`
///
/// @param T - The type to allocate memory for
/// @param allocator - The `StdAllocator` to allocate with
///
/// @return  newly allocated memory
/// @ingroup memory
	#define std_allocator_allocate_t(T, allocator) \
		static_cast(T*)(std_allocator_allocate(allocator, sizeof(T)))

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
		static_cast(T*)(std_allocator_allocate_array(allocator, num_elements, sizeof(T)))

/// @brief Allocates new memory large enough to store a `T`, and copies the old contents over.
///
/// @param T - The type to allocate memory for
/// @param allocator - The `StdAllocator` to allocate memory with
/// @param memory_ptr - The memory to be reallocated
///
/// @return reallocated memory
/// @note If reallocation fails, the original memory will be returned unchanged
/// @ingroup memory
	#define std_allocator_reallocate_t(T, allocator, memory_ptr) \
		static_cast(T*)(std_allocator_reallocate(allocator, memory_ptr, sizeof(T), sizeof(T)))

/// @brief Allocates new memory large enough to store `new_num_elements` elements of type `T`, and
/// copies the old contents over
///
/// @param T - The type to allocate memory for
/// @param allocator - The `StdAllocator` to allocate memory with
/// @param memory_ptr - The memory to be reallocated
/// @param old_num_elements - The previous number of elements in the memory
/// @param new_num_elements - The desired new number of elements in the memory
///
/// @return reallocated memory
/// @note If reallocation fails, the original memory will be returned unchanged
/// @ingroup memory
	#define std_allocator_reallocate_array_t(T,                          \
											 allocator,                  \
											 memory_ptr,                 \
											 old_num_elements,           \
											 new_num_elements)           \
		static_cast(T*)(std_allocator_reallocate_array(allocator,        \
													   memory_ptr,       \
													   old_num_elements, \
													   new_num_elements, \
													   sizeof(T)))

/// @brief Copies memory containing an array of type `T` from `src_ptr` to `dst_ptr`
///
/// @param T - The type contained in the array
/// @param dest_ptr - The destination of the copied data
/// @param src_ptr - The source of the data to copy
/// @param num_elements - The number of elements in the source array
/// @ingroup memory
	#define std_memcpy(T, dest_ptr, src_ptr, num_elements) \
		memcpy(dest_ptr, src_ptr, (num_elements) * sizeof(T))

/// @brief Moves memory containing an array of type `T` from `src_ptr` to `dst_ptr`
///
/// @param T - The type contained in the array
/// @param dest_ptr - The destination of the moved data
/// @param src_ptr - The source of the data to move
/// @param num_elements - The number of elements in the source array
/// @ingroup memory
	#define std_memmove(T, dest_ptr, src_ptr, num_elements) \
		memmove(dest_ptr, src_ptr, (num_elements) * sizeof(T))

/// @brief Sets the memory at `dest_ptr`, containing an array of type `T`, to the given `value`
///
/// @param T - The type contained in the array
/// @param dest_ptr - The location of the memory to set the value of
/// @param value - The value to set the memory located at `dest_ptr` to
/// @param num_elements - The number of elements in the destination array
/// @ingroup memory
	#define std_memset(T, dest_ptr, value, num_elements) \
		memset(dest_ptr, value, (num_elements) * sizeof(T))
#endif // STD_ALLOCATORS
