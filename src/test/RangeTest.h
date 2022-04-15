#ifndef RANGE_TEST
#define RANGE_TEST

#include <Cnx/Def.h>

#include "Criterion.h"

#define VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Vector.h>
#undef VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS

#define RANGE_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Range.h>
#undef RANGE_INCLUDE_DEFAULT_INSTANTIATIONS

static inline CnxVector(i32) create_test_vector(void) {
	let_mut vec = cnx_vector_new_with_capacity(i32, 10);
	cnx_vector_push_back(vec, 0);
	cnx_vector_push_back(vec, 1);
	cnx_vector_push_back(vec, 2);
	cnx_vector_push_back(vec, 3);
	cnx_vector_push_back(vec, 4);
	cnx_vector_push_back(vec, 5);
	cnx_vector_push_back(vec, 6);
	cnx_vector_push_back(vec, 7);
	cnx_vector_push_back(vec, 8);
	cnx_vector_push_back(vec, 9);

	return vec;
}

TEST(CnxRange, from) {
	let_mut vec = create_test_vector();
	let_mut range = cnx_range_from(i32, vec);
	let_mut index = 0;
	foreach(elem, range) {
		TEST_ASSERT_EQUAL(elem, index, "Range element does not equal expected value");
		TEST_ASSERT_EQUAL(elem,
						  cnx_vector_at(vec, static_cast(usize)(index)),
						  "Range element does not equal value in wrapped collection");
		++index;
	}
}

bool test_filter(const i32* restrict elem) {
	return *elem % 2 == 0;
}

TEST(CnxRange, from_filtered) {
	let_mut vec = create_test_vector();
	let_mut range = cnx_range_from_filtered(i32, vec, test_filter);
	let_mut index = 0;
	foreach(elem, range) {
		TEST_ASSERT_EQUAL(elem, index, "Range element does not equal expected value");
		TEST_ASSERT_EQUAL(elem,
						  cnx_vector_at(vec, static_cast(usize)(index)),
						  "Range element does not equal value in wrapped collection");
		index += 2;
	}
}

TEST(CnxRange, collect) {
	let_mut vec = create_test_vector();
	let_mut range = cnx_range_from_filtered(i32, vec, test_filter);

	let_mut vec2 = cnx_range_collect(i32, range);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec2),
					  cnx_vector_size(vec) / 2,
					  "Collected size does not equal expected size");

	let_mut index = 0;
	foreach(elem, vec2) {
		TEST_ASSERT_EQUAL(elem, index, "Collected element does not equal expected value");
		index += 2;
	}
}

void test_transform(i32* restrict elem) {
	*elem *= 2;
}

TEST(CnxRange, transform) {
	let_mut vec = create_test_vector();
	let_mut range = cnx_range_from(i32, vec);
	cnx_range_transform(range, test_transform);

	let_mut index = 0;
	let_mut value = 0;
	foreach(elem, range) {
		TEST_ASSERT_EQUAL(elem, value, "Transformed Range element does not have expected value");
		TEST_ASSERT_EQUAL(elem,
						  cnx_vector_at(vec, static_cast(usize)(index)),
						  "Transformed Range element does not equal wrapped collection element");
		index++;
		value = index * 2;
	}
}

TEST(CnxRange, take) {
	let_mut vec = create_test_vector();
	let_mut range = cnx_range_take_first(i32, 5, vec);

	let_mut index = 0;
	foreach(elem, range) {
		TEST_ASSERT_EQUAL(elem, index);
		++index;
	}

	// index will be incremented one more times than there are elements
	// because it's incremented in the loop
	TEST_ASSERT_EQUAL(index - 1, 4);

	let_mut vec2 = cnx_range_collect(i32, range);
	TEST_ASSERT_EQUAL(cnx_vector_size(vec2), 5);
}

i32 test_accumulate(i32* restrict current_value, const i32* restrict elem) {
	*current_value += *elem;
	return *current_value;
}

TEST(CnxRange, accumulate) {
	let_mut vec = create_test_vector();
	let_mut range = cnx_range_from(i32, vec);

	let_mut desired = 0;
	foreach(elem, vec) {
		desired += elem;
	}

	let val = cnx_range_accumulate(range, test_accumulate);
	TEST_ASSERT_EQUAL(val, desired);

	let val2 = cnx_accumulate(i32, vec, test_accumulate);
	TEST_ASSERT_EQUAL(val2, desired);
}

#endif // RANGE_TEST
