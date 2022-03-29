/// @file StdAllocators.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdAllocators provides an abstraction to modularize custom memory allocators to make
/// custom allocator use simple and configurable
/// @version 0.2.1
/// @date 2022-03-28
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
#include <C2nxt/StdAllocators.h>
#include <C2nxt/StdMath.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void* std_allocate([[maybe_unused]] StdAllocator* restrict self, usize size_bytes) {
	return malloc(size_bytes);
}

void*
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std_reallocate([[maybe_unused]] StdAllocator* restrict self, void* memory, usize new_size_bytes) {
	return realloc(memory, new_size_bytes);
}

void
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std_deallocate([[maybe_unused]] StdAllocator* restrict self, void* memory) {
	free(memory);
}

const StdAllocator DEFAULT_ALLOCATOR = STD_DEFAULT_ALLOCATOR;

StdAllocator std_allocator_new(void) {
	return DEFAULT_ALLOCATOR;
}

void* std_allocator_allocate(StdAllocator allocator, usize size_bytes) {
	let_mut mem = trait_call(allocate, allocator, size_bytes);

	if(mem != nullptr) {
		memset(mem, 0, size_bytes);
		return mem;
	}
	else {
#if STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE == 1
		ignore(fprintf(stderr, "Failed to allocate %zu bytes of memory, aborting\n", size_bytes));
		abort();
#else
		return nullptr;
#endif // STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
	}
}

void* std_allocator_allocate_array(StdAllocator allocator,
								   usize num_elements,
								   usize element_size_bytes) {
	return std_allocator_allocate(allocator, num_elements * element_size_bytes);
}

void* std_allocator_reallocate(StdAllocator allocator,
							   void* memory,
							   // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
							   usize old_size_bytes,
							   usize new_size_bytes) {
	let_mut mem = trait_call(reallocate, allocator, memory, new_size_bytes);

	if(mem != nullptr) {
		if(mem != memory) {
			memcpy(mem, memory, std_min(new_size_bytes, old_size_bytes));
		}
		return mem;
	}
	else {
		let_mut retry = trait_call(allocate, allocator, new_size_bytes);
		if(retry != nullptr) {
			memcpy(retry, memory, std_min(new_size_bytes, old_size_bytes));
			return mem;
		}

#if STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
		ignore(fprintf(stderr,
					   "Failed to allocate " STD_OFORMAT_USIZE " bytes of memory, aborting\n",
					   new_size_bytes));
		abort();
#else
		return memory;
#endif // STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
	}
}

void* std_allocator_reallocate_array(StdAllocator allocator,
									 void* memory,
									 usize old_num_elements,
									 usize new_num_elements,
									 usize element_size_bytes) {
	return std_allocator_reallocate(allocator,
									memory,
									old_num_elements * element_size_bytes,
									new_num_elements * element_size_bytes);
}

void std_allocator_deallocate(StdAllocator allocator, void* memory) {
	trait_call(deallocate, allocator, memory);
}
