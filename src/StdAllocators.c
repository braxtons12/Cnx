/// @file StdAllocators.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief StdAllocators provides an abstraction to modularize custom memory allocators to make
/// custom allocator use simple and configurable
/// @version 0.1
/// @date 2022-01-02
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

const StdAllocator DEFAULT_ALLOCATOR = STD_DEFAULT_ALLOCATOR;

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
