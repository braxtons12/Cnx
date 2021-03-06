#ifndef CNX_UNIQUE_PTR_TEST
#define CNX_UNIQUE_PTR_TEST

#include <Cnx/Allocators.h>
#include <Cnx/BasicTypes.h>
#include <Cnx/Def.h>
#include <Cnx/IO.h>

__attr(always_inline) static inline void i32_deleter(i32* self, CnxAllocator allocator) {
	cnx_allocator_deallocate(allocator, self);
}

#define UNIQUE_UNDEF_PARAMS TRUE
#define UNIQUE_T			i32
#define UNIQUE_DELETER		i32_deleter
#define UNIQUE_DECL			TRUE
#define UNIQUE_IMPL			TRUE
#include <Cnx/UniquePtr.h>
#undef UNIQUE_UNDEF_PARAMS

typedef i32 i32_arr[];
#define UNIQUE_UNDEF_PARAMS TRUE
#define UNIQUE_T			i32_arr
#define UNIQUE_DECL			TRUE
#define UNIQUE_IMPL			TRUE
#include <Cnx/UniquePtr.h>
#undef UNIQUE_UNDEF_PARAMS

#include "Criterion.h"

TEST(CnxUniquePtr, new_get_ptr_as_bool_and_delete) {

	let_mut ptr = cnx_unique_ptr_new(i32);

	TEST_ASSERT_NOT_EQUAL(cnx_unique_ptr_get(ptr), nullptr);
	TEST_ASSERT_NOT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT(cnx_unique_ptr_as_bool(ptr));

	*cnx_unique_ptr_get(ptr) = 4;

	TEST_ASSERT_EQUAL(*ptr(ptr), 4);

	cnx_unique_ptr_free(i32, ptr);

	TEST_ASSERT_EQUAL(cnx_unique_ptr_get(ptr), nullptr);
	TEST_ASSERT_EQUAL(ptr(ptr), nullptr);
}

TEST(CnxUniquePtr, new_with_capacity_at_and_delete) {

	let_mut arr = cnx_unique_ptr_new_with_capacity(i32_arr, 5);

	TEST_ASSERT_NOT_EQUAL(cnx_unique_ptr_get(arr), nullptr);
	TEST_ASSERT_NOT_EQUAL(ptr(arr), nullptr);

	cnx_unique_ptr_at(arr, 0) = 1;
	cnx_unique_ptr_at(arr, 1) = 2;
	cnx_unique_ptr_at(arr, 2) = 3;
	cnx_unique_ptr_at(arr, 3) = 4;
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
	cnx_unique_ptr_at(arr, 4) = 5;

	TEST_ASSERT_EQUAL(cnx_unique_ptr_at(arr, 0), 1);
	TEST_ASSERT_EQUAL(cnx_unique_ptr_get(arr)[0], 1);
	TEST_ASSERT_EQUAL(ptr(arr)[0], 1);

	TEST_ASSERT_EQUAL(cnx_unique_ptr_at(arr, 4), 5);
	TEST_ASSERT_EQUAL(cnx_unique_ptr_get(arr)[4], 5);
	TEST_ASSERT_EQUAL(ptr(arr)[4], 5);

	cnx_unique_ptr_free(i32_arr, arr);

	TEST_ASSERT_EQUAL(cnx_unique_ptr_get(arr), nullptr);
	TEST_ASSERT_EQUAL(ptr(arr), nullptr);
}

TEST(CnxUniquePtr, move) {
	UniquePtr(i32) ptr = cnx_unique_ptr_new(i32);

	*ptr(ptr) = 4;

	TEST_ASSERT_NOT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT_EQUAL(*ptr(ptr), 4);

	UniquePtr(i32) moved = ptr_move(ptr);

	TEST_ASSERT_NOT_EQUAL(ptr(moved), nullptr);
	TEST_ASSERT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT_EQUAL(*ptr(moved), 4);
}

TEST(CnxUniquePtr, from) {
	let_mut ptr = cnx_allocator_allocate_t(i32, DEFAULT_ALLOCATOR);
	*ptr = 4;
	UniquePtr(i32) unique_ptr = cnx_unique_ptr_from(i32, ptr);

	TEST_ASSERT_NOT_EQUAL(ptr(unique_ptr), nullptr);
	// ptr should not be used after its ownership is transferred to unique_ptr,
	// this is just for verification
	TEST_ASSERT_EQUAL(ptr(unique_ptr), ptr);
	TEST_ASSERT_EQUAL(*ptr(unique_ptr), 4);
}

TEST(CnxUniquePtr, release) {
	UniquePtr(i32) ptr = cnx_unique_ptr_new(i32);

	*ptr(ptr) = 4;

	let_mut released = cnx_unique_ptr_release(ptr);

	TEST_ASSERT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT_NOT_EQUAL(released, nullptr);
	TEST_ASSERT_EQUAL(*released, 4);

	cnx_allocator_deallocate(ptr.m_allocator, released);
}

TEST(CnxUniquePtr, reset) {
	UniquePtr(i32) ptr = cnx_unique_ptr_new(i32);
	*ptr(ptr) = 4;

	let_mut ptr2 = cnx_allocator_allocate_t(i32, DEFAULT_ALLOCATOR);
	*ptr2 = 2;

	cnx_unique_ptr_reset(ptr, ptr2);

	TEST_ASSERT_EQUAL(ptr(ptr), ptr2);
	TEST_ASSERT_EQUAL(*ptr(ptr), 2);
	TEST_ASSERT_EQUAL(*ptr(ptr), *ptr2);
}

TEST(CnxUniquePtr, swap) {
	UniquePtr(i32) ptr1 = cnx_unique_ptr_new(i32);
	*ptr(ptr1) = 4;
	UniquePtr(i32) ptr2 = cnx_unique_ptr_new(i32);
	*ptr(ptr2) = 2;

	cnx_unique_ptr_swap(ptr1, &ptr2);

	TEST_ASSERT_EQUAL(*ptr(ptr1), 2);
	TEST_ASSERT_EQUAL(*ptr(ptr2), 4);
}

TEST(CnxUniquePtr, get_deleter) {
	UniquePtr(i32) ptr = cnx_unique_ptr_new(i32);
	TEST_ASSERT_EQUAL(cnx_unique_ptr_get_deleter(ptr), i32_deleter);
}

TEST(CnxUniquePtr, make_unique) {
	UniquePtr(i32) ptr = cnx_make_unique(i32, 4);

	TEST_ASSERT_NOT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT_EQUAL(*ptr(ptr), 4);
}

typedef struct {
	i32 first;
	i32 second;
	i32 third;
} UniquePtrTestStruct;

#define UNIQUE_T			UniquePtrTestStruct
#define UNIQUE_DECL			TRUE
#define UNIQUE_IMPL			TRUE
#define UNIQUE_UNDEF_PARAMS TRUE
#include <Cnx/UniquePtr.h>
#undef UNIQUE_UNDEF_PARAMS

TEST(CnxUniquePtr, make_unique_struct) {
	UniquePtr(UniquePtrTestStruct) ptr
		= cnx_make_unique(UniquePtrTestStruct, .first = 3, .second = 4, .third = 1);

	TEST_ASSERT_NOT_EQUAL(ptr(ptr), nullptr);

	let contained = *ptr(ptr);

	TEST_ASSERT_EQUAL(contained.first, 3);
	TEST_ASSERT_EQUAL(contained.second, 4);
	TEST_ASSERT_EQUAL(contained.third, 1);

	TEST_ASSERT_EQUAL((*ptr(ptr)).first, 3);
	TEST_ASSERT_EQUAL((*ptr(ptr)).second, 4);
	TEST_ASSERT_EQUAL((*ptr(ptr)).third, 1);
}

#endif // CNX_UNIQUE_PTR_TEST
