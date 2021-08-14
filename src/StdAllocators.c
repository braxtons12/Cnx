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
