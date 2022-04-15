#ifndef CNX_ARRAY_TEST
#define CNX_ARRAY_TEST

#include <Cnx/IO.h>
#include <Cnx/Platform.h>

#include "Arrayi32_10.h"
#include "Criterion.h"

i32 array_test_constructor([[maybe_unused]] CnxAllocator allocator) {
	return 0;
}

void array_test_destructor(i32* elem, [[maybe_unused]] CnxAllocator allocator) { // NOLINT
	*elem = 0;
}

void array_test_scoped_destructor(i32* elem, [[maybe_unused]] CnxAllocator allocator) { // NOLINT
	*elem = 0;
	println("{}", "running element destructor");
}

TEST(CnxArray, new) {
	let data = (CnxCollectionData(CnxArray(i32, 10))){.m_constructor = array_test_constructor,
													  .m_destructor = array_test_destructor};

	let array = cnx_array_new_with_collection_data(i32, 10, &data);
	TEST_ASSERT_EQUAL(array.m_data->m_constructor, array_test_constructor);
	TEST_ASSERT_EQUAL(array.m_data->m_destructor, array_test_destructor);
	TEST_ASSERT_EQUAL(cnx_array_capacity(array), 10);
	TEST_ASSERT_EQUAL(array.m_size, 0);
}

TEST(CnxArray, at_and_push_back) {
	let_mut array = cnx_array_new(i32, 10);
	TEST_ASSERT_EQUAL(cnx_array_size(array), 0);
	cnx_array_push_back(array, 32);
	TEST_ASSERT_EQUAL(cnx_array_at(array, 0), 32);
	TEST_ASSERT_EQUAL(cnx_array_size(array), 1);
	cnx_array_push_back(array, 24);
	TEST_ASSERT_EQUAL(cnx_array_at(array, 1), 24);
	TEST_ASSERT_EQUAL(cnx_array_size(array), 2);
}

TEST(CnxArray, push_back_and_front_and_back) {
	let_mut array = cnx_array_new(i32, 10);
	cnx_array_push_back(array, 32);
	TEST_ASSERT_EQUAL(cnx_array_front(array), 32);
	TEST_ASSERT_EQUAL(cnx_array_back(array), 32);
	cnx_array_push_back(array, 16);
	TEST_ASSERT_EQUAL(cnx_array_front(array), 32);
	TEST_ASSERT_EQUAL(cnx_array_back(array), 16);
}

TEST(CnxArray, accessors) {
	let_mut array = cnx_array_new(i32, 10);
	TEST_ASSERT_TRUE(cnx_array_is_empty(array));
	TEST_ASSERT_FALSE(cnx_array_is_full(array));
	TEST_ASSERT_EQUAL(cnx_array_size(array), 0);
	TEST_ASSERT_EQUAL(cnx_array_capacity(array), 10);

	let cap = cnx_array_capacity(array);
	for(let_mut i = 0U; i < cap; ++i) {
		cnx_array_push_back(array, static_cast(i32)(i));
	}

	TEST_ASSERT_TRUE(cnx_array_is_full(array));
	TEST_ASSERT_FALSE(cnx_array_is_empty(array));
	TEST_ASSERT_EQUAL(cnx_array_size(array), cnx_array_capacity(array));
}

TEST(CnxArray, resize) {
	let_mut array = cnx_array_new(i32, 10);
	TEST_ASSERT_EQUAL(cnx_array_capacity(array), 10);
	TEST_ASSERT_EQUAL(cnx_array_size(array), 0);
	cnx_array_resize(array, 10);
	TEST_ASSERT_EQUAL(cnx_array_size(array), 10);
	TEST_ASSERT_EQUAL(cnx_array_at(array, cnx_array_size(array) - 1), 0);
}

TEST(CnxArray, clear) {
	let_mut array = cnx_array_new(i32, 10);
	TEST_ASSERT_EQUAL(cnx_array_capacity(array), 10);
	TEST_ASSERT_EQUAL(cnx_array_size(array), 0);
	cnx_array_resize(array, 10);
	TEST_ASSERT_EQUAL(cnx_array_size(array), 10);
	cnx_array_clear(array);
	TEST_ASSERT_EQUAL(cnx_array_size(array), 0);
}

TEST(CnxArray, insert) {
	let_mut array = cnx_array_new(i32, 10);
	let end = cnx_array_capacity(array) - 1;
	for(let_mut i = 0U; i < end; ++i) {
		cnx_array_push_back(array, static_cast(i32)(i));
	}
	let insert_index = cnx_array_capacity(array) / 2;
	cnx_array_insert(array, 1337, insert_index);
	for(let_mut i = 0U; i < insert_index; ++i) {
		TEST_ASSERT_EQUAL(cnx_array_at(array, i), static_cast(i32)(i));
	}
	TEST_ASSERT_EQUAL(cnx_array_at(array, insert_index), 1337);
	for(let_mut i = insert_index + 1; i < cnx_array_capacity(array); ++i) {
		TEST_ASSERT_EQUAL(cnx_array_at(array, i), static_cast(i32)(i - 1));
	}
}

TEST(CnxArray, erase) {
	let_mut array = cnx_array_new(i32, 10);
	let capacity = cnx_array_capacity(array);
	for(let_mut i = 0U; i < capacity; ++i) {
		cnx_array_push_back(array, static_cast(i32)(i));
	}
	TEST_ASSERT_EQUAL(cnx_array_size(array), capacity);
	let erase_index = capacity / 2;
	cnx_array_erase(array, erase_index);
	TEST_ASSERT_EQUAL(cnx_array_size(array), capacity - 1);
	for(let_mut i = 0U; i < erase_index; ++i) {
		TEST_ASSERT_EQUAL(cnx_array_at(array, i), static_cast(i32)(i));
	}
	// cast away const for GCC compat
	for(let_mut i = const_cast(usize)(erase_index); i < capacity - 1; ++i) {
		TEST_ASSERT_EQUAL(cnx_array_at(array, i), static_cast(i32)(i + 1));
	}
}

TEST(CnxArray, erase_n) {
	let_mut array = cnx_array_new(i32, 10);
	let capacity = cnx_array_capacity(array);
	for(let_mut i = 0U; i < capacity; ++i) {
		cnx_array_push_back(array, static_cast(i32)(i));
	}
	TEST_ASSERT_EQUAL(cnx_array_size(array), capacity);
	let erase_index = capacity / 2;
	let erase_length = 3;
	cnx_array_erase_n(array, erase_index, erase_length);
	TEST_ASSERT_EQUAL(cnx_array_size(array), capacity - erase_length);
	for(let_mut i = 0U; i < erase_index; ++i) {
		TEST_ASSERT_EQUAL(cnx_array_at(array, i), static_cast(i32)(i));
	}
	// cast away const for GCC compat
	for(let_mut i = const_cast(usize)(erase_index); i < capacity - erase_length; ++i) {
		TEST_ASSERT_EQUAL(cnx_array_at(array, i), static_cast(i32)(i + erase_length));
	}
}

TEST(CnxArray, free) {
	let_mut array = cnx_array_new(i32, 10);
	cnx_array_resize(array, cnx_array_capacity(array));
	TEST_ASSERT_EQUAL(cnx_array_size(array), cnx_array_capacity(array));
	cnx_array_free(array);
	TEST_ASSERT_EQUAL(cnx_array_size(array), 0);
}

/// should print "running element destructor" to stdout 10 times
TEST(CnxArray, scoped) {
	let data = (CnxCollectionData(CnxArray(i32, 10))){.m_constructor = array_test_constructor,
													  .m_destructor = array_test_scoped_destructor};

	cnx_array_scoped(i32, 10) array = cnx_array_new_with_collection_data(i32, 10, &data);
	cnx_array_resize(array, cnx_array_capacity(array));
	TEST_ASSERT_EQUAL(cnx_array_size(array), cnx_array_capacity(array));
}

TEST(CnxArray, iterators) {
	let_mut array = cnx_array_new(i32, 10);
	for(let_mut i = 0U; i < cnx_array_capacity(array); ++i) {
		cnx_array_push_back(array, static_cast(i32)(i));
	}
	let_mut array2 = cnx_array_new(i32, 10);

	foreach(elem, array) {
		cnx_array_push_back(array2, elem);
	}

	for(let_mut i = 0U; i < cnx_array_capacity(array); ++i) {
		TEST_ASSERT_EQUAL(cnx_array_at(array, i), cnx_array_at(array2, i));
	}
}

#endif // CNX_ARRAY_TEST
