#include "C2nxt/StdPlatform.h"

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonportable-include-path\"")
#endif

#include "C2nxt/StdIO.h"

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic pop")
#endif

#include "StdArrayi32_10.h"
#include "unity.h"

#ifndef STD_ARRAY_TEST
	#define STD_ARRAY_TEST
		i32 array_test_constructor(StdAllocator allocator) {
	ignore(allocator);
	return 0;
}

void array_test_destructor(i32* elem, StdAllocator allocator) { // NOLINT
	*elem = 0;
	ignore(allocator);
}

void array_test_scoped_destructor(i32* elem, StdAllocator allocator) { // NOLINT
	*elem = 0;
	ignore(allocator);
	println("{}", "running element destructor");
}

void test_array_new(void) {
	let data = (StdCollectionData(StdArray(i32, 10))){.m_constructor = array_test_constructor,
													  .m_destructor = array_test_destructor,
													  .m_allocator = std_allocator_new()};

	let array = std_array_new_with_collection_data(i32, 10, data);
	TEST_ASSERT_EQUAL_PTR(array.m_data.m_constructor, array_test_constructor);
	TEST_ASSERT_EQUAL_PTR(array.m_data.m_destructor, array_test_destructor);
	TEST_ASSERT_EQUAL_INT(std_array_capacity(array), 10);
	TEST_ASSERT_EQUAL_INT(array.m_size, 0);
}

void test_array_at_and_push_back(void) {
	let_mut array = std_array_new(i32, 10);
	TEST_ASSERT_EQUAL_INT(std_array_at(array, 0), 0);
	std_array_push_back(array, 32);
	TEST_ASSERT_EQUAL_INT(std_array_at(array, 0), 32);
	TEST_ASSERT_EQUAL_INT(std_array_size(array), 1);
}

void test_array_push_back_and_front_and_back(void) {
	let_mut array = std_array_new(i32, 10);
	std_array_push_back(array, 32);
	TEST_ASSERT_EQUAL_INT(std_array_front(array), 32);
	TEST_ASSERT_EQUAL_INT(std_array_back(array), 32);
	std_array_push_back(array, 16);
	TEST_ASSERT_EQUAL_INT(std_array_front(array), 32);
	TEST_ASSERT_EQUAL_INT(std_array_back(array), 16);
}

void test_array_accessors(void) {
	let_mut array = std_array_new(i32, 10);
	TEST_ASSERT_TRUE(std_array_is_empty(array));
	TEST_ASSERT_FALSE(std_array_is_full(array));
	TEST_ASSERT_EQUAL_INT(std_array_size(array), 0);
	TEST_ASSERT_EQUAL_INT(std_array_capacity(array), 10);

	let cap = std_array_capacity(array);
	for(let_mut i = 0U; i < cap; ++i) {
		std_array_push_back(array, static_cast(i32)(i));
	}

	TEST_ASSERT_TRUE(std_array_is_full(array));
	TEST_ASSERT_FALSE(std_array_is_empty(array));
	TEST_ASSERT_EQUAL_INT(std_array_size(array), std_array_capacity(array));
}

void test_array_resize(void) {
	let_mut array = std_array_new(i32, 10);
	TEST_ASSERT_EQUAL_INT(std_array_capacity(array), 10);
	TEST_ASSERT_EQUAL_INT(std_array_size(array), 0);
	std_array_resize(array, 10);
	TEST_ASSERT_EQUAL_INT(std_array_size(array), 10);
	TEST_ASSERT_EQUAL_INT(std_array_at(array, std_array_size(array) - 1), 0);
}

void test_array_clear(void) {
	let_mut array = std_array_new(i32, 10);
	TEST_ASSERT_EQUAL_INT(std_array_capacity(array), 10);
	TEST_ASSERT_EQUAL_INT(std_array_size(array), 0);
	std_array_resize(array, 10);
	TEST_ASSERT_EQUAL_INT(std_array_size(array), 10);
	std_array_clear(array);
	TEST_ASSERT_EQUAL_INT(std_array_size(array), 0);
}

void test_array_insert(void) {
	let_mut array = std_array_new(i32, 10);
	let end = std_array_capacity(array) - 1;
	for(let_mut i = 0U; i < end; ++i) {
		std_array_push_back(array, static_cast(i32)(i));
	}
	let insert_index = std_array_capacity(array) / 2;
	std_array_insert(array, 1337, insert_index);
	for(let_mut i = 0U; i < insert_index; ++i) {
		TEST_ASSERT_EQUAL_INT(std_array_at(array, i), i);
	}
	TEST_ASSERT_EQUAL_INT(std_array_at(array, insert_index), 1337);
	for(let_mut i = insert_index + 1; i < std_array_capacity(array); ++i) {
		TEST_ASSERT_EQUAL_INT(std_array_at(array, i), i - 1);
	}
}

void test_array_erase(void) {
	let_mut array = std_array_new(i32, 10);
	let capacity = std_array_capacity(array);
	for(let_mut i = 0U; i < capacity; ++i) {
		std_array_push_back(array, static_cast(i32)(i));
	}
	TEST_ASSERT_EQUAL_INT(std_array_size(array), capacity);
	let erase_index = capacity / 2;
	std_array_erase(array, erase_index);
	TEST_ASSERT_EQUAL_INT(std_array_size(array), capacity - 1);
	for(let_mut i = 0U; i < erase_index; ++i) {
		TEST_ASSERT_EQUAL_INT(std_array_at(array, i), i);
	}
	for(let_mut i = erase_index; i < capacity - 1; ++i) {
		TEST_ASSERT_EQUAL_INT(std_array_at(array, i), i + 1);
	}
}

void test_array_erase_n(void) {
	let_mut array = std_array_new(i32, 10);
	let capacity = std_array_capacity(array);
	for(let_mut i = 0U; i < capacity; ++i) {
		std_array_push_back(array, static_cast(i32)(i));
	}
	TEST_ASSERT_EQUAL_INT(std_array_size(array), capacity);
	let erase_index = capacity / 2;
	let erase_length = 3;
	std_array_erase_n(array, erase_index, erase_length);
	TEST_ASSERT_EQUAL_INT(std_array_size(array), capacity - erase_length);
	for(let_mut i = 0U; i < erase_index; ++i) {
		TEST_ASSERT_EQUAL_INT(std_array_at(array, i), i);
	}
	for(let_mut i = erase_index; i < capacity - erase_length; ++i) {
		TEST_ASSERT_EQUAL_INT(std_array_at(array, i), i + erase_length);
	}
}

void test_array_free(void) {
	let_mut array = std_array_new(i32, 10);
	std_array_resize(array, std_array_capacity(array));
	TEST_ASSERT_EQUAL_INT(std_array_size(array), std_array_capacity(array));
	std_array_free(array);
	TEST_ASSERT_EQUAL_INT(std_array_size(array), 0);
}

/// should print "running element destructor" to stdout 10 times
void test_array_scoped(void) {
	let allocator = std_allocator_new();
	let data = (StdCollectionData(StdArray(i32, 10))){.m_allocator = allocator,
													  .m_constructor = array_test_constructor,
													  .m_destructor = array_test_scoped_destructor};

	let_mut std_array_scoped(i32, 10) array = std_array_new_with_collection_data(i32, 10, data);
	std_array_resize(array, std_array_capacity(array));
	TEST_ASSERT_EQUAL_INT(std_array_size(array), std_array_capacity(array));
}

void test_array_iterator(void) {
	let_mut array = std_array_new(i32, 10);
	for(let_mut i = 0U; i < std_array_capacity(array); ++i) {
		std_array_push_back(array, static_cast(i32)(i));
	}
	let_mut array2 = std_array_new(i32, 10);

	foreach(elem, array) {
		std_array_push_back(array2, elem);
	}

	for(let_mut i = 0U; i < std_array_capacity(array); ++i) {
		TEST_ASSERT_EQUAL_INT(std_array_at(array, i), std_array_at(array2, i));
	}
}

static bool run_array_tests(void) {

	RUN_TEST(test_array_new);
	RUN_TEST(test_array_at_and_push_back);
	RUN_TEST(test_array_push_back_and_front_and_back);
	RUN_TEST(test_array_accessors);
	RUN_TEST(test_array_resize);
	RUN_TEST(test_array_clear);
	RUN_TEST(test_array_insert);
	RUN_TEST(test_array_erase);
	RUN_TEST(test_array_erase_n);
	RUN_TEST(test_array_free);
	RUN_TEST(test_array_scoped);
	RUN_TEST(test_array_iterator);

	return true;
}

#endif // STD_ARRAY_TEST
