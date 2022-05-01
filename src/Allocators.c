/// @file Allocators.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief CnxAllocators provides an abstraction to modularize custom memory allocators to make
/// custom allocator use simple and configurable
/// @version 0.2.2
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
#include <Cnx/Allocators.h>
#include <Cnx/Math.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void* cnx_allocate(__attr(maybe_unused) CnxAllocator* restrict self, usize size_bytes) {
	return malloc(size_bytes);
}

void*
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
cnx_reallocate(__attr(maybe_unused) CnxAllocator* restrict self,
			   void* memory,
			   usize new_size_bytes) {
	return realloc(memory, new_size_bytes);
}

void
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
cnx_deallocate(__attr(maybe_unused) CnxAllocator* restrict self, void* memory) {
	free(memory);
}

const CnxAllocator DEFAULT_ALLOCATOR = CNX_DEFAULT_ALLOCATOR;

CnxAllocator cnx_allocator_new(void) {
	return DEFAULT_ALLOCATOR;
}

void* cnx_allocator_allocate(CnxAllocator allocator, usize size_bytes) {
	let_mut mem = trait_call(allocate, allocator, size_bytes);

	if(mem != nullptr) {
		memset(mem, 0, size_bytes);
		return mem;
	}
	else {
#if CNX_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE == 1
		ignore(fprintf(stderr, "Failed to allocate %zu bytes of memory, aborting\n", size_bytes));
		abort();
#else
		return nullptr;
#endif // CNX_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
	}
}

void* cnx_allocator_allocate_array(CnxAllocator allocator,
								   usize num_elements,
								   usize element_size_bytes) {
	return cnx_allocator_allocate(allocator, num_elements * element_size_bytes);
}

void* cnx_allocator_reallocate(CnxAllocator allocator,
							   void* memory,
							   // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
							   usize old_size_bytes,
							   usize new_size_bytes) {
	let_mut mem = trait_call(reallocate, allocator, memory, new_size_bytes);

	if(mem != nullptr) {
		if(mem != memory) {
			memcpy(mem, memory, cnx_min(new_size_bytes, old_size_bytes));
		}
		return mem;
	}
	else {
		let_mut retry = trait_call(allocate, allocator, new_size_bytes);
		if(retry != nullptr) {
			memcpy(retry, memory, cnx_min(new_size_bytes, old_size_bytes));
			return mem;
		}

#if CNX_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
		ignore(fprintf(stderr,
					   "Failed to allocate " CNX_OFORMAT_USIZE " bytes of memory, aborting\n",
					   new_size_bytes));
		abort();
#else
		return nullptr;
#endif // CNX_ALLOCATOR_ABORT_ON_ALLOCATION_FAILURE
	}
}

void* cnx_allocator_reallocate_array(CnxAllocator allocator,
									 void* memory,
									 usize old_num_elements,
									 usize new_num_elements,
									 usize element_size_bytes) {
	return cnx_allocator_reallocate(allocator,
									memory,
									old_num_elements * element_size_bytes,
									new_num_elements * element_size_bytes);
}

void cnx_allocator_deallocate(CnxAllocator allocator, void* memory) {
	trait_call(deallocate, allocator, memory);
}
