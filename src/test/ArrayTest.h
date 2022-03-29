#ifndef STD_ARRAY_TEST
#define STD_ARRAY_TEST

#include <C2nxt/StdPlatform.h>

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonportable-include-path\"")
#endif

#include <C2nxt/StdIO.h>

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic pop")
#endif

#include "Criterion.h"
#include "StdArrayi32_10.h"

		i32 array_test_constructor([[maybe_unused]] StdAllocator allocator) {
	return 0;
}

void array_test_destructor(i32* elem, [[maybe_unused]] StdAllocator allocator) { // NOLINT
	*elem = 0;
}

void array_test_scoped_destructor(i32* elem, [[maybe_unused]] StdAllocator allocator) { // NOLINT
	*elem = 0;
	println("{}", "running element destructor");
}

TEST(StdArray, new) {
	let data = (StdCollectionData(StdArray(i32, 10))){.m_constructor = array_test_constructor,
													  .m_destructor = array_test_destructor};

	let array = std_array_new_with_collection_data(i32, 10, &data);
	TEST_ASSERT_EQUAL(array.m_data->m_constructor, array_test_constructor);
	TEST_ASSERT_EQUAL(array.m_data->m_destructor, array_test_destructor);
	TEST_ASSERT_EQUAL(std_array_capacity(array), 10);
	TEST_ASSERT_EQUAL(array.m_size, 0);
}

TEST(StdArray, at_and_push_back) {
	let_mut array = std_array_new(i32, 10);
	TEST_ASSERT_EQUAL(std_array_size(array), 0);
	std_array_push_back(array, 32);
	TEST_ASSERT_EQUAL(std_array_at(array, 0), 32);
	TEST_ASSERT_EQUAL(std_array_size(array), 1);
	std_array_push_back(array, 24);
	TEST_ASSERT_EQUAL(std_array_at(array, 1), 24);
	TEST_ASSERT_EQUAL(std_array_size(array), 2);
}

TEST(StdArray, push_back_and_front_and_back) {
	let_mut array = std_array_new(i32, 10);
	std_array_push_back(array, 32);
	TEST_ASSERT_EQUAL(std_array_front(array), 32);
	TEST_ASSERT_EQUAL(std_array_back(array), 32);
	std_array_push_back(array, 16);
	TEST_ASSERT_EQUAL(std_array_front(array), 32);
	TEST_ASSERT_EQUAL(std_array_back(array), 16);
}

TEST(StdArray, accessors) {
	let_mut array = std_array_new(i32, 10);
	TEST_ASSERT_TRUE(std_array_is_empty(array));
	TEST_ASSERT_FALSE(std_array_is_full(array));
	TEST_ASSERT_EQUAL(std_array_size(array), 0);
	TEST_ASSERT_EQUAL(std_array_capacity(array), 10);

	let cap = std_array_capacity(array);
	for(let_mut i = 0U; i < cap; ++i) {
		std_array_push_back(array, static_cast(i32)(i));
	}

	TEST_ASSERT_TRUE(std_array_is_full(array));
	TEST_ASSERT_FALSE(std_array_is_empty(array));
	TEST_ASSERT_EQUAL(std_array_size(array), std_array_capacity(array));
}

TEST(StdArray, resize) {
	let_mut array = std_array_new(i32, 10);
	TEST_ASSERT_EQUAL(std_array_capacity(array), 10);
	TEST_ASSERT_EQUAL(std_array_size(array), 0);
	std_array_resize(array, 10);
	TEST_ASSERT_EQUAL(std_array_size(array), 10);
	TEST_ASSERT_EQUAL(std_array_at(array, std_array_size(array) - 1), 0);
}

TEST(StdArray, clear) {
	let_mut array = std_array_new(i32, 10);
	TEST_ASSERT_EQUAL(std_array_capacity(array), 10);
	TEST_ASSERT_EQUAL(std_array_size(array), 0);
	std_array_resize(array, 10);
	TEST_ASSERT_EQUAL(std_array_size(array), 10);
	std_array_clear(array);
	TEST_ASSERT_EQUAL(std_array_size(array), 0);
}

TEST(StdArray, insert) {
	let_mut array = std_array_new(i32, 10);
	let end = std_array_capacity(array) - 1;
	for(let_mut i = 0U; i < end; ++i) {
		std_array_push_back(array, static_cast(i32)(i));
	}
	let insert_index = std_array_capacity(array) / 2;
	std_array_insert(array, 1337, insert_index);
	for(let_mut i = 0U; i < insert_index; ++i) {
		TEST_ASSERT_EQUAL(std_array_at(array, i), static_cast(i32)(i));
	}
	TEST_ASSERT_EQUAL(std_array_at(array, insert_index), 1337);
	for(let_mut i = insert_index + 1; i < std_array_capacity(array); ++i) {
		TEST_ASSERT_EQUAL(std_array_at(array, i), static_cast(i32)(i - 1));
	}
}

TEST(StdArray, erase) {
	let_mut array = std_array_new(i32, 10);
	let capacity = std_array_capacity(array);
	for(let_mut i = 0U; i < capacity; ++i) {
		std_array_push_back(array, static_cast(i32)(i));
	}
	TEST_ASSERT_EQUAL(std_array_size(array), capacity);
	let erase_index = capacity / 2;
	std_array_erase(array, erase_index);
	TEST_ASSERT_EQUAL(std_array_size(array), capacity - 1);
	for(let_mut i = 0U; i < erase_index; ++i) {
		TEST_ASSERT_EQUAL(std_array_at(array, i), static_cast(i32)(i));
	}
	// cast away const for GCC compat
	for(let_mut i = const_cast(usize)(erase_index); i < capacity - 1; ++i) {
		TEST_ASSERT_EQUAL(std_array_at(array, i), static_cast(i32)(i + 1));
	}
}

TEST(StdArray, erase_n) {
	let_mut array = std_array_new(i32, 10);
	let capacity = std_array_capacity(array);
	for(let_mut i = 0U; i < capacity; ++i) {
		std_array_push_back(array, static_cast(i32)(i));
	}
	TEST_ASSERT_EQUAL(std_array_size(array), capacity);
	let erase_index = capacity / 2;
	let erase_length = 3;
	std_array_erase_n(array, erase_index, erase_length);
	TEST_ASSERT_EQUAL(std_array_size(array), capacity - erase_length);
	for(let_mut i = 0U; i < erase_index; ++i) {
		TEST_ASSERT_EQUAL(std_array_at(array, i), static_cast(i32)(i));
	}
	// cast away const for GCC compat
	for(let_mut i = const_cast(usize)(erase_index); i < capacity - erase_length; ++i) {
		TEST_ASSERT_EQUAL(std_array_at(array, i), static_cast(i32)(i + erase_length));
	}
}

TEST(StdArray, free) {
	let_mut array = std_array_new(i32, 10);
	std_array_resize(array, std_array_capacity(array));
	TEST_ASSERT_EQUAL(std_array_size(array), std_array_capacity(array));
	std_array_free(array);
	TEST_ASSERT_EQUAL(std_array_size(array), 0);
}

/// should print "running element destructor" to stdout 10 times
TEST(StdArray, scoped) {
	let data = (StdCollectionData(StdArray(i32, 10))){.m_constructor = array_test_constructor,
													  .m_destructor = array_test_scoped_destructor};

	std_array_scoped(i32, 10) array = std_array_new_with_collection_data(i32, 10, &data);
	std_array_resize(array, std_array_capacity(array));
	TEST_ASSERT_EQUAL(std_array_size(array), std_array_capacity(array));
}

TEST(StdArray, iterators) {
	let_mut array = std_array_new(i32, 10);
	for(let_mut i = 0U; i < std_array_capacity(array); ++i) {
		std_array_push_back(array, static_cast(i32)(i));
	}
	let_mut array2 = std_array_new(i32, 10);

	foreach(elem, array) {
		std_array_push_back(array2, elem);
	}

	for(let_mut i = 0U; i < std_array_capacity(array); ++i) {
		TEST_ASSERT_EQUAL(std_array_at(array, i), std_array_at(array2, i));
	}
}

#endif // STD_ARRAY_TEST
