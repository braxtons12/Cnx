#include <C2nxt/StdPlatform.h>

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonportable-include-path\"")
#endif

#include <C2nxt/StdIO.h>

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic pop")
#endif

#include <C2nxt/StdFormat.h>
#include <C2nxt/StdRange.h>

#define STD_TEMPLATE_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <C2nxt/StdVector.h>
#undef STD_TEMPLATE_INCLUDE_DEFAULT_INSTANTIATIONS

#include <unity.h>

#define SHORT_OPT_CAPACITY static_cast(usize)(STD_VECTOR_DEFAULT_SHORT_OPT_CAPACITY)

#ifndef STD_VECTOR_TEST
	#define STD_VECTOR_TEST

		u32 vector_test_constructor(StdAllocator allocator) {
	ignore(allocator);
	return 0;
}

void vector_test_destructor(u32* elem, StdAllocator allocator) { // NOLINT
	*elem = 0;
	ignore(allocator);
}

u32 vector_test_copy_constructor(const u32* restrict elem, StdAllocator allocator) {
	ignore(allocator);
	return *elem;
}

void vector_test_scoped_destructor(u32* elem, StdAllocator allocator) { // NOLINT
	//*elem = 0;
	ignore(elem);
	ignore(allocator);
	let val = static_cast(f64)(*elem) + static_cast(f64)(*elem) / 100.0;
	println("{d}", val);
	// fprintf(stdout, "running element destructor\n");
}

static let data
	= (StdCollectionData(StdVector(u32))){.m_constructor = vector_test_constructor,
										  .m_copy_constructor = vector_test_copy_constructor,
										  .m_destructor = vector_test_destructor};

void test_vector_new(void) {
	let_mut vec = std_vector_new_with_collection_data(u32, &data);
	TEST_ASSERT_EQUAL_PTR(vec.m_data->m_constructor, vector_test_constructor);
	TEST_ASSERT_EQUAL_PTR(vec.m_data->m_copy_constructor, vector_test_copy_constructor);
	TEST_ASSERT_EQUAL_PTR(vec.m_data->m_destructor, vector_test_destructor);
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), 0);
	std_vector_free(vec);
}

void test_vector_at_and_push_back(void) {
	let_mut vec = std_vector_new(u32);
	TEST_ASSERT_EQUAL_INT(std_vector_at(vec, 0), 0U);
	std_vector_push_back(vec, 32U);
	TEST_ASSERT_EQUAL_INT(std_vector_at(vec, 0), 32U);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), 1U);
	std_vector_free(vec);
}

void test_vector_push_back_and_pop_back(void) {
	let_mut vec = std_vector_new(u32);
	std_vector_push_back(vec, 32U);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), 1U);
	let_mut back = std_vector_pop_back(vec);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), 0U);
	TEST_ASSERT_TRUE(std_option_is_some(back));
	TEST_ASSERT_EQUAL_INT(std_option_unwrap(back), 32U);
	std_vector_free(vec);
}

void test_vector_push_back_and_front_and_back(void) {
	let_mut vec = std_vector_new(u32);
	std_vector_push_back(vec, 32U);
	TEST_ASSERT_EQUAL_INT(std_vector_front(vec), 32U);
	TEST_ASSERT_EQUAL_INT(std_vector_back(vec), 32U);
	std_vector_push_back(vec, 16U);
	TEST_ASSERT_EQUAL_INT(std_vector_front(vec), 32U);
	TEST_ASSERT_EQUAL_INT(std_vector_back(vec), 16U);
	std_vector_free(vec);
}

void test_vector_accessors(void) { // NOLINT
	let_mut vec = std_vector_new(u32);
	TEST_ASSERT_EQUAL_INT(std_vector_max_size(u32), (std_max_value(usize) - 1) << 1U); // NOLINT
	TEST_ASSERT_TRUE(std_vector_is_empty(vec));
	TEST_ASSERT_FALSE(std_vector_is_full(vec));
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), 0);
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY);

	let cap = std_vector_capacity(vec);
	for(let_mut i = 0U; i < cap; ++i) {
		std_vector_push_back(vec, i);
	}

	TEST_ASSERT_TRUE(std_vector_is_full(vec));
	TEST_ASSERT_FALSE(std_vector_is_empty(vec));
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), std_vector_capacity(vec));
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), SHORT_OPT_CAPACITY);

	let cap2 = std_vector_capacity(vec);
	for(let_mut i = 0U; i < cap2; ++i) {
		std_vector_push_back(vec, i);
	}
	std_vector_free(vec);
}

void test_vector_reserve(void) {
	let_mut vec = std_vector_new(u32);
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY);
	std_vector_reserve(vec, std_vector_capacity(vec) * 2);
	TEST_ASSERT_TRUE(std_vector_capacity(vec) >= SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), 0);
	std_vector_free(vec);
}

void test_vector_resize(void) {
	let_mut vec = std_vector_new(u32);
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), 0);
	std_vector_resize(vec, SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL_INT(std_vector_at(vec, std_vector_size(vec) - 1), 0);
	std_vector_free(vec);
}

void test_vector_shrink(void) {
	let_mut vec = std_vector_new(u32);
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY);
	std_vector_reserve(vec, std_vector_capacity(vec) * 2);
	TEST_ASSERT_TRUE(std_vector_capacity(vec) >= SHORT_OPT_CAPACITY * 2);
	std_vector_shrink_to_fit(vec);
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY);
	std_vector_free(vec);
}

void test_vector_clear(void) {
	let_mut vec = std_vector_new(u32);
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), 0);
	std_vector_resize(vec, SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), SHORT_OPT_CAPACITY * 2);
	std_vector_clear(vec);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), 0);
	std_vector_free(vec);
}

void test_vector_insert(void) {
	let_mut vec = std_vector_new(u32);
	let capacity = std_vector_capacity(vec);
	for(let_mut i = 0U; i < capacity; ++i) {
		std_vector_push_back(vec, i);
	}
	let insert_index = capacity / 2;
	std_vector_insert(vec, 1337, insert_index);
	for(let_mut i = 0U; i < insert_index; ++i) {
		TEST_ASSERT_EQUAL_INT(std_vector_at(vec, i), i);
	}
	TEST_ASSERT_EQUAL_INT(std_vector_at(vec, insert_index), 1337);
	for(let_mut i = insert_index + 1; i < capacity + 1; ++i) {
		TEST_ASSERT_EQUAL_INT(std_vector_at(vec, i), i - 1);
	}
	std_vector_free(vec);
}

void test_vector_erase(void) {
	let_mut vec = std_vector_new(u32);
	let capacity = std_vector_capacity(vec);
	for(let_mut i = 0U; i < capacity; ++i) {
		std_vector_push_back(vec, i);
	}
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), capacity);
	let erase_index = capacity / 2;
	std_vector_erase(vec, erase_index);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), capacity - 1);
	for(let_mut i = 0U; i < erase_index; ++i) {
		TEST_ASSERT_EQUAL_INT(std_vector_at(vec, i), i);
	}
	for(usize i = erase_index; i < capacity - 1; ++i) {
		TEST_ASSERT_EQUAL_INT(std_vector_at(vec, i), i + 1);
	}
	std_vector_free(vec);
}

void test_vector_erase_n(void) {
	let_mut vec = std_vector_new(u32);
	let capacity = std_vector_capacity(vec);
	for(let_mut i = 0U; i < capacity; ++i) {
		std_vector_push_back(vec, i);
	}
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), capacity);
	let erase_index = capacity / 2;
	let erase_length = capacity / 4;
	std_vector_erase_n(vec, erase_index, erase_length);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), capacity - erase_length);
	for(let_mut i = 0U; i < erase_index; ++i) {
		TEST_ASSERT_EQUAL_INT(std_vector_at(vec, i), i);
	}
	for(usize i = erase_index; i < capacity - erase_length; ++i) {
		TEST_ASSERT_EQUAL_INT(std_vector_at(vec, i), i + erase_length);
	}
	std_vector_free(vec);
}

void test_vector_free(void) {
	let_mut vec = std_vector_new(u32);
	std_vector_resize(vec, SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), SHORT_OPT_CAPACITY * 2);
	std_vector_free(vec);
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), 0);
}

static let scoped_data
	= (StdCollectionData(StdVector(u32))){.m_constructor = vector_test_constructor,
										  .m_copy_constructor = vector_test_copy_constructor,
										  .m_destructor = vector_test_scoped_destructor};
/// should print "running element destructor" to stdout
/// SHORT_OPT_CAPACITY * 2 times
void test_vector_scoped(void) {
	let_mut std_vector_scoped(u32) vec = std_vector_new_with_collection_data(u32, &scoped_data);
	std_vector_resize(vec, SHORT_OPT_CAPACITY * 2);
	for(let_mut i = 0U; i < std_vector_capacity(vec); ++i) {
		std_vector_at_mut(vec, i) = i;
	}
	TEST_ASSERT_EQUAL_INT(std_vector_capacity(vec), SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL_INT(std_vector_size(vec), SHORT_OPT_CAPACITY * 2);
}

void test_vector_iterator(void) {
	let_mut vec = std_vector_new(u32);
	for(let_mut i = 0U; i < std_vector_capacity(vec); ++i) {
		std_vector_push_back(vec, i);
	}

	let_mut vec2 = std_vector_new(u32);

	foreach(elem, vec) {
		std_vector_push_back(vec2, elem);
	}

	for(let_mut i = 0U; i < std_vector_capacity(vec); ++i) {
		TEST_ASSERT_EQUAL_INT(std_vector_at(vec, i), std_vector_at(vec2, i));
	}
	std_vector_free(vec);
	std_vector_free(vec2);
}

static bool run_vector_tests(void) {

	RUN_TEST(test_vector_new);
	RUN_TEST(test_vector_at_and_push_back);
	RUN_TEST(test_vector_push_back_and_pop_back);
	RUN_TEST(test_vector_push_back_and_front_and_back);
	RUN_TEST(test_vector_accessors);
	RUN_TEST(test_vector_reserve);
	RUN_TEST(test_vector_resize);
	RUN_TEST(test_vector_shrink);
	RUN_TEST(test_vector_clear);
	RUN_TEST(test_vector_insert);
	RUN_TEST(test_vector_erase);
	RUN_TEST(test_vector_erase_n);
	RUN_TEST(test_vector_free);
	RUN_TEST(test_vector_scoped);
	RUN_TEST(test_vector_iterator);

	return true;
}

#endif // STD_VECTOR_TEST
