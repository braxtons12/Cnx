/// @file StdAllocators.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdAllocators provides an abstraction to modularize custom memory allocators to make
/// custom allocator use simple and configurable
/// @version 0.2
/// @date 2022-01-11
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

void* std_allocate(maybe_unused StdAllocator* restrict self, usize size_bytes) {
	return malloc(size_bytes);
}

void*
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std_reallocate(maybe_unused StdAllocator* restrict self, void* memory, usize new_size_bytes) {
return realloc(memory, new_size_bytes);
}

void
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
std_deallocate(maybe_unused StdAllocator* restrict self, void* memory) {
	free(memory);
}

const StdAllocator DEFAULT_ALLOCATOR = STD_DEFAULT_ALLOCATOR;

StdAllocator std_allocator_new(void) {
	return DEFAULT_ALLOCATOR;
}

StdMemory std_allocator_allocate(StdAllocator allocator, usize size_bytes) {
	let_mut mem = trait_call(allocate, allocator, size_bytes);

	if(mem != nullptr) {
		memset(mem, 0, size_bytes);
		return (StdMemory){.m_memory = mem, .m_size_bytes = size_bytes};
	}
	else {
#if STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE == 1
		fprintf(stderr, "Failed to allocate %zu bytes of memory, aborting\n", size_bytes);
		abort();
#else
		return (StdMemory){.m_memory = nullptr, .m_size_bytes = 0};
#endif // STD_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
	}
}

StdMemory
std_allocator_allocate_array(StdAllocator allocator, usize num_elements, usize element_size_bytes) {
	return std_allocator_allocate(allocator, num_elements * element_size_bytes);
}

StdMemory std_allocator_reallocate(StdAllocator allocator, StdMemory memory, usize new_size_bytes) {
	let_mut mem = trait_call(reallocate, allocator, memory.m_memory, new_size_bytes);

	if(mem != nullptr) {
		if(mem != memory.m_memory) {
			memcpy(mem, memory.m_memory, std_min(new_size_bytes, memory.m_size_bytes));
		}
		return (StdMemory){.m_memory = mem, .m_size_bytes = new_size_bytes};
	}
	else {
		let_mut retry = trait_call(allocate, allocator, new_size_bytes);
		if(retry != nullptr) {
			memcpy(retry, memory.m_memory, std_min(new_size_bytes, memory.m_size_bytes));
			return (StdMemory){.m_memory = retry, .m_size_bytes = new_size_bytes};
		}

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
	trait_call(deallocate, allocator, memory.m_memory);
}
