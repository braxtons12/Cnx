#ifndef CNX_SHARED_PTR_TEST
#define CNX_SHARED_PTR_TEST

#include <Cnx/Allocators.h>
#include <Cnx/BasicTypes.h>
#include <Cnx/Def.h>
#include <Cnx/IO.h>

__attr(always_inline) static inline void i32_shared_deleter(i32* self, CnxAllocator allocator) {
	cnx_allocator_deallocate(allocator, self);
}

#define SHARED_UNDEF_PARAMS TRUE
#define SHARED_T			i32
#define SHARED_DELETER		i32_shared_deleter
#define SHARED_DECL			TRUE
#define SHARED_IMPL			TRUE
#include <Cnx/SharedPtr.h>
#undef SHARED_UNDEF_PARAMS

typedef i32 i32_arr[];
#define SHARED_UNDEF_PARAMS TRUE
#define SHARED_T			i32_arr
#define SHARED_DECL			TRUE
#define SHARED_IMPL			TRUE
#include <Cnx/SharedPtr.h>
#undef SHARED_UNDEF_PARAMS

#include "Criterion.h"

TEST(CnxSharedPtr, new_get_ptr_as_bool_and_delete) {

	let_mut ptr = cnx_shared_ptr_new(i32);

	TEST_ASSERT_NOT_EQUAL(cnx_shared_ptr_get(ptr), nullptr);
	TEST_ASSERT_NOT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT(cnx_shared_ptr_as_bool(ptr));

	*cnx_shared_ptr_get(ptr) = 4;

	TEST_ASSERT_EQUAL(*ptr(ptr), 4);

	cnx_shared_ptr_free(i32, ptr);

	TEST_ASSERT_EQUAL(cnx_shared_ptr_get(ptr), nullptr);
	TEST_ASSERT_EQUAL(ptr(ptr), nullptr);
}

TEST(CnxSharedPtr, new_with_capacity_at_and_delete) {

	let_mut arr = cnx_shared_ptr_new_with_capacity(i32_arr, 5);

	TEST_ASSERT_NOT_EQUAL(cnx_shared_ptr_get(arr), nullptr);
	TEST_ASSERT_NOT_EQUAL(ptr(arr), nullptr);

	cnx_shared_ptr_at(arr, 0) = 1;
	cnx_shared_ptr_at(arr, 1) = 2;
	cnx_shared_ptr_at(arr, 2) = 3;
	cnx_shared_ptr_at(arr, 3) = 4;
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
	cnx_shared_ptr_at(arr, 4) = 5;

	TEST_ASSERT_EQUAL(cnx_shared_ptr_at(arr, 0), 1);
	TEST_ASSERT_EQUAL(cnx_shared_ptr_get(arr)[0], 1);
	TEST_ASSERT_EQUAL(ptr(arr)[0], 1);

	TEST_ASSERT_EQUAL(cnx_shared_ptr_at(arr, 4), 5);
	TEST_ASSERT_EQUAL(cnx_shared_ptr_get(arr)[4], 5);
	TEST_ASSERT_EQUAL(ptr(arr)[4], 5);

	cnx_shared_ptr_free(i32_arr, arr);

	TEST_ASSERT_EQUAL(cnx_shared_ptr_get(arr), nullptr);
	TEST_ASSERT_EQUAL(ptr(arr), nullptr);
}

TEST(CnxSharedPtr, move) {
	SharedPtr(i32) ptr = cnx_shared_ptr_new(i32);

	*ptr(ptr) = 4;

	TEST_ASSERT_NOT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT_EQUAL(*ptr(ptr), 4);

	SharedPtr(i32) moved = ptr_move(ptr);

	TEST_ASSERT_NOT_EQUAL(ptr(moved), nullptr);
	TEST_ASSERT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT_EQUAL(*ptr(moved), 4);
}

TEST(CnxSharedPtr, clone) {
	SharedPtr(i32) ptr = cnx_shared_ptr_new(i32);

	*ptr(ptr) = 4;

	TEST_ASSERT_NOT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT_EQUAL(*ptr(ptr), 4);

	SharedPtr(i32) cloned = cnx_shared_ptr_clone(ptr);

	TEST_ASSERT_EQUAL(ptr(cloned), ptr(ptr));
	TEST_ASSERT_EQUAL(*ptr(cloned), 4);
	TEST_ASSERT_EQUAL(atomic_load(cloned.m_ref_count), 2);
	TEST_ASSERT_EQUAL(atomic_load(cloned.m_ref_count), atomic_load(ptr.m_ref_count));
	TEST_ASSERT_EQUAL(cloned.m_ref_count, ptr.m_ref_count);
}

TEST(CnxSharedPtr, from) {
	let_mut ptr = cnx_allocator_allocate_t(i32, DEFAULT_ALLOCATOR);
	*ptr = 4;
	SharedPtr(i32) shared_ptr = cnx_shared_ptr_from(i32, ptr);

	TEST_ASSERT_NOT_EQUAL(ptr(shared_ptr), nullptr);
	// ptr should not be used after its ownership is transferred to shared_ptr,
	// this is just for verification
	TEST_ASSERT_EQUAL(ptr(shared_ptr), ptr);
	TEST_ASSERT_EQUAL(*ptr(shared_ptr), 4);
}

TEST(CnxSharedPtr, release) {
	SharedPtr(i32) ptr = cnx_shared_ptr_new(i32);

	*ptr(ptr) = 4;

	let_mut released = cnx_shared_ptr_release(ptr);

	TEST_ASSERT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT_NOT_EQUAL(released, nullptr);
	TEST_ASSERT_EQUAL(*released, 4);

	cnx_allocator_deallocate(ptr.m_allocator, released);
}

TEST(CnxSharedPtr, reset) {
	SharedPtr(i32) ptr = cnx_shared_ptr_new(i32);
	*ptr(ptr) = 4;

	let_mut ptr2 = cnx_allocator_allocate_t(i32, DEFAULT_ALLOCATOR);
	*ptr2 = 2;

	cnx_shared_ptr_reset(ptr, ptr2);

	TEST_ASSERT_EQUAL(ptr(ptr), ptr2);
	TEST_ASSERT_EQUAL(*ptr(ptr), 2);
	TEST_ASSERT_EQUAL(*ptr(ptr), *ptr2);
}

TEST(CnxSharedPtr, swap) {
	SharedPtr(i32) ptr1 = cnx_shared_ptr_new(i32);
	*ptr(ptr1) = 4;
	SharedPtr(i32) ptr2 = cnx_shared_ptr_new(i32);
	*ptr(ptr2) = 2;

	cnx_shared_ptr_swap(ptr1, &ptr2);

	TEST_ASSERT_EQUAL(*ptr(ptr1), 2);
	TEST_ASSERT_EQUAL(*ptr(ptr2), 4);
}

TEST(CnxSharedPtr, get_deleter) {
	SharedPtr(i32) ptr = cnx_shared_ptr_new(i32);
	TEST_ASSERT_EQUAL(cnx_shared_ptr_get_deleter(ptr), i32_shared_deleter);
}

TEST(CnxSharedPtr, make_shared) {
	SharedPtr(i32) ptr = cnx_make_shared(i32, 4);

	TEST_ASSERT_NOT_EQUAL(ptr(ptr), nullptr);
	TEST_ASSERT_EQUAL(*ptr(ptr), 4);
}

typedef struct {
	i32 first;
	i32 second;
	i32 third;
} SharedPtrTestStruct;

#define SHARED_T			SharedPtrTestStruct
#define SHARED_DECL			TRUE
#define SHARED_IMPL			TRUE
#define SHARED_UNDEF_PARAMS TRUE
#include <Cnx/SharedPtr.h>
#undef SHARED_UNDEF_PARAMS

TEST(CnxSharedPtr, make_shared_struct) {
	SharedPtr(SharedPtrTestStruct) ptr
		= cnx_make_shared(SharedPtrTestStruct, .first = 3, .second = 4, .third = 1);

	TEST_ASSERT_NOT_EQUAL(ptr(ptr), nullptr);

	let contained = *ptr(ptr);

	TEST_ASSERT_EQUAL(contained.first, 3);
	TEST_ASSERT_EQUAL(contained.second, 4);
	TEST_ASSERT_EQUAL(contained.third, 1);

	TEST_ASSERT_EQUAL((*ptr(ptr)).first, 3);
	TEST_ASSERT_EQUAL((*ptr(ptr)).second, 4);
	TEST_ASSERT_EQUAL((*ptr(ptr)).third, 1);
}

#endif // CNX_SHARED_PTR_TEST
