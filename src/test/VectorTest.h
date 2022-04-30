#ifndef CNX_VECTOR_TEST
#define CNX_VECTOR_TEST

#include <Cnx/Format.h>
#include <Cnx/IO.h>
#include <Cnx/Platform.h>
#include <Cnx/Range.h>

#define VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Vector.h>
#undef VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS

#include "Criterion.h"

#define SHORT_OPT_CAPACITY static_cast(usize)(CNX_VECTOR_DEFAULT_SHORT_OPT_CAPACITY)

u32 vector_test_constructor(CnxAllocator allocator) {
	ignore(allocator);
	return 0;
}

void vector_test_destructor(u32* elem, CnxAllocator allocator) { // NOLINT
	*elem = 0;
	ignore(allocator);
}

u32 vector_test_copy_constructor(const u32* restrict elem, CnxAllocator allocator) {
	ignore(allocator);
	return *elem;
}

void vector_test_scoped_destructor(u32* elem, CnxAllocator allocator) { // NOLINT
	//*elem = 0;
	ignore(elem);
	ignore(allocator);
	let val = static_cast(f64)(*elem) + static_cast(f64)(*elem) / 100.0;
	println("{d}", val);
	// fprintf(Cnxout, "running element destructor\n");
}

static let data
	= (CnxCollectionData(CnxVector(u32))){.m_constructor = vector_test_constructor,
										  .m_copy_constructor = vector_test_copy_constructor,
										  .m_destructor = vector_test_destructor};

TEST(CnxVector, new) {
	let_mut vec = cnx_vector_new_with_collection_data(u32, &data);
	TEST_ASSERT_EQUAL(vec.m_data->m_constructor, vector_test_constructor);
	TEST_ASSERT_EQUAL(vec.m_data->m_copy_constructor, vector_test_copy_constructor);
	TEST_ASSERT_EQUAL(vec.m_data->m_destructor, vector_test_destructor);
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), 0);
	cnx_vector_free(vec);
}

TEST(CnxVector, at_and_push_back) {
	let_mut vec = cnx_vector_new(u32);
	TEST_ASSERT_EQUAL(cnx_vector_at(vec, 0), 0U);
	cnx_vector_push_back(vec, 32U);
	TEST_ASSERT_EQUAL(cnx_vector_at(vec, 0), 32U);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), 1U);
	cnx_vector_free(vec);
}

TEST(CnxVector, push_back_and_pop_back) {
	let_mut vec = cnx_vector_new(u32);
	cnx_vector_push_back(vec, 32U);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), 1U);
	let_mut back = cnx_vector_pop_back(vec);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), 0U);
	TEST_ASSERT(cnx_option_is_some(back));
	TEST_ASSERT_EQUAL(cnx_option_unwrap(back), 32U);
	cnx_vector_free(vec);
}

TEST(CnxVector, push_back_and_front_and_back) {
	let_mut vec = cnx_vector_new(u32);
	cnx_vector_push_back(vec, 32U);
	TEST_ASSERT_EQUAL(cnx_vector_front(vec), 32U);
	TEST_ASSERT_EQUAL(cnx_vector_back(vec), 32U);
	cnx_vector_push_back(vec, 16U);
	TEST_ASSERT_EQUAL(cnx_vector_front(vec), 32U);
	TEST_ASSERT_EQUAL(cnx_vector_back(vec), 16U);
	cnx_vector_free(vec);
}

TEST(CnxVector, accessors) { // NOLINT
	let_mut vec = cnx_vector_new(u32);
	TEST_ASSERT_EQUAL(cnx_vector_max_size(u32), (cnx_max_value(usize) - 1) << 1U); // NOLINT
	TEST_ASSERT(cnx_vector_is_empty(vec));
	TEST_ASSERT_FALSE(cnx_vector_is_full(vec));
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), 0);
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY);

	let cap = cnx_vector_capacity(vec);
	for(let_mut i = 0U; i < cap; ++i) {
		cnx_vector_push_back(vec, i);
	}

	TEST_ASSERT(cnx_vector_is_full(vec));
	TEST_ASSERT_FALSE(cnx_vector_is_empty(vec));
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), cnx_vector_capacity(vec));
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), SHORT_OPT_CAPACITY);

	let cap2 = cnx_vector_capacity(vec);
	for(let_mut i = 0U; i < cap2; ++i) {
		cnx_vector_push_back(vec, i);
	}
	cnx_vector_free(vec);
}

TEST(CnxVector, reserve) {
	let_mut vec = cnx_vector_new(u32);
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY);
	cnx_vector_reserve(vec, cnx_vector_capacity(vec) * 2);
	TEST_ASSERT(cnx_vector_capacity(vec) >= SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), 0);
	cnx_vector_free(vec);
}

TEST(CnxVector, resize) {
	let_mut vec = cnx_vector_new(u32);
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), 0);
	cnx_vector_resize(vec, SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL(cnx_vector_at(vec, cnx_vector_size(vec) - 1), 0);
	cnx_vector_free(vec);
}

TEST(CnxVector, shrink_to_fit) {
	let_mut vec = cnx_vector_new(u32);
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY);
	cnx_vector_reserve(vec, cnx_vector_capacity(vec) * 2);
	TEST_ASSERT(cnx_vector_capacity(vec) >= SHORT_OPT_CAPACITY * 2);
	cnx_vector_shrink_to_fit(vec);
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY);
	cnx_vector_free(vec);
}

TEST(CnxVector, clear) {
	let_mut vec = cnx_vector_new(u32);
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), 0);
	cnx_vector_resize(vec, SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), SHORT_OPT_CAPACITY * 2);
	cnx_vector_clear(vec);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), 0);
	cnx_vector_free(vec);
}

TEST(CnxVector, insert) {
	let_mut vec = cnx_vector_new(u32);
	let capacity = cnx_vector_capacity(vec);
	for(let_mut i = 0U; i < capacity; ++i) {
		cnx_vector_push_back(vec, i);
	}
	let insert_index = capacity / 2;
	cnx_vector_insert(vec, 1337, insert_index);
	for(let_mut i = 0U; i < insert_index; ++i) {
		TEST_ASSERT_EQUAL(cnx_vector_at(vec, i), i);
	}
	TEST_ASSERT_EQUAL(cnx_vector_at(vec, insert_index), 1337);
	for(let_mut i = insert_index + 1; i < capacity + 1; ++i) {
		TEST_ASSERT_EQUAL(cnx_vector_at(vec, i), i - 1);
	}
	cnx_vector_free(vec);
}

TEST(CnxVector, erase) {
	let_mut vec = cnx_vector_new(u32);
	let capacity = cnx_vector_capacity(vec);
	for(let_mut i = 0U; i < capacity; ++i) {
		cnx_vector_push_back(vec, i);
	}
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), capacity);
	let erase_index = capacity / 2;
	cnx_vector_erase(vec, erase_index);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), capacity - 1);
	for(let_mut i = 0U; i < erase_index; ++i) {
		TEST_ASSERT_EQUAL(cnx_vector_at(vec, i), i);
	}
	for(usize i = erase_index; i < capacity - 1; ++i) {
		TEST_ASSERT_EQUAL(cnx_vector_at(vec, i), i + 1);
	}
	cnx_vector_free(vec);
}

TEST(CnxVector, erase_n) {
	let_mut vec = cnx_vector_new(u32);
	let capacity = cnx_vector_capacity(vec);
	for(let_mut i = 0U; i < capacity; ++i) {
		cnx_vector_push_back(vec, i);
	}
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), capacity);
	let erase_index = capacity / 2;
	let erase_length = capacity / 4;
	cnx_vector_erase_n(vec, erase_index, erase_length);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), capacity - erase_length);
	for(let_mut i = 0U; i < erase_index; ++i) {
		TEST_ASSERT_EQUAL(cnx_vector_at(vec, i), i);
	}
	for(usize i = erase_index; i < capacity - erase_length; ++i) {
		TEST_ASSERT_EQUAL(cnx_vector_at(vec, i), i + erase_length);
	}
	cnx_vector_free(vec);
}

TEST(CnxVector, free) {
	let_mut vec = cnx_vector_new(u32);
	cnx_vector_resize(vec, SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), SHORT_OPT_CAPACITY * 2);
	cnx_vector_free(vec);
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), 0);
}

static let scoped_data
	= (CnxCollectionData(CnxVector(u32))){.m_constructor = vector_test_constructor,
										  .m_copy_constructor = vector_test_copy_constructor,
										  .m_destructor = vector_test_scoped_destructor};
/// should print "running element destructor" to Cnxout
/// SHORT_OPT_CAPACITY * 2 times
TEST(CnxVector, scoped) {
	CnxScopedVector(u32) vec = cnx_vector_new_with_collection_data(u32, &scoped_data);
	cnx_vector_resize(vec, SHORT_OPT_CAPACITY * 2);
	for(let_mut i = 0U; i < cnx_vector_capacity(vec); ++i) {
		cnx_vector_at_mut(vec, i) = i;
	}
	TEST_ASSERT_EQUAL(cnx_vector_capacity(vec), SHORT_OPT_CAPACITY * 2);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec), SHORT_OPT_CAPACITY * 2);
}

TEST(CnxVector, iterator) {
	let_mut vec = cnx_vector_new(u32);
	for(let_mut i = 0U; i < cnx_vector_capacity(vec); ++i) {
		cnx_vector_push_back(vec, i);
	}

	let_mut vec2 = cnx_vector_new(u32);

	foreach(elem, vec) {
		cnx_vector_push_back(vec2, elem);
	}

	for(let_mut i = 0U; i < cnx_vector_capacity(vec); ++i) {
		TEST_ASSERT_EQUAL(cnx_vector_at(vec, i), cnx_vector_at(vec2, i));
	}
	cnx_vector_free(vec);
	cnx_vector_free(vec2);
}

#endif // CNX_VECTOR_TEST
