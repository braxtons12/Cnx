#ifndef STD_STRING_TEST
#define STD_STRING_TEST

#include <C2nxt/StdString.h>

#include "Criterion.h"

static void* test_malloc([[maybe_unused]] StdAllocator* self, usize bytes) {
	return malloc(bytes);
}

static void* test_realloc([[maybe_unused]] StdAllocator* self, void* memory, usize new_size_bytes) {
	return realloc(memory, new_size_bytes);
}

static void test_free([[maybe_unused]] StdAllocator* self, void* memory) {
	free(memory);
}

TEST(StdString, new) {
	let_mut string = std_string_new();

	TEST_ASSERT_EQUAL(std_string_length(string), 0);
	TEST_ASSERT_EQUAL(std_string_capacity(string), STD_STRING_SHORT_OPTIMIZATION_CAPACITY);
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == std_allocate);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == std_reallocate);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == std_deallocate);
	TEST_ASSERT(std_string_is_empty(string));
	TEST_ASSERT(!std_string_is_full(string));
	std_string_free(string);
}

TEST(StdString, new_with_allocator) {
	let_mut string = std_string_new_with_allocator(
		std_allocator_from_custom_stateless_allocator(test_malloc, test_realloc, test_free));

	TEST_ASSERT_EQUAL(std_string_length(string), 0);
	TEST_ASSERT_EQUAL(std_string_capacity(string), STD_STRING_SHORT_OPTIMIZATION_CAPACITY);
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == test_malloc);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == test_realloc);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == test_free);
	std_string_free(string);
}

TEST(StdString, new_with_capacity) {
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut string = std_string_new_with_capacity(30U);

	TEST_ASSERT_EQUAL(std_string_length(string), 0);
	TEST_ASSERT_EQUAL(std_string_capacity(string), 30U);
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == std_allocate);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == std_reallocate);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == std_deallocate);
	std_string_free(string);
}

TEST(StdString, new_with_capacity_with_allocator) {
	let_mut string = std_string_new_with_capacity_with_allocator(
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
		30U,
		std_allocator_from_custom_stateless_allocator(test_malloc, test_realloc, test_free));

	TEST_ASSERT_EQUAL(std_string_length(string), 0);
	TEST_ASSERT_EQUAL(std_string_capacity(string), 30U);
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == test_malloc);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == test_realloc);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == test_free);
	std_string_free(string);
}

TEST(StdString, from_cstring) {
	let test_string = "This is a test test test";
	let_mut string = std_string_from(test_string);
	TEST_ASSERT_EQUAL(std_string_length(string), strlen(test_string));
	TEST_ASSERT_EQUAL(std_string_capacity(string), strlen(test_string));
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == std_allocate);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == std_reallocate);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == std_deallocate);
	TEST_ASSERT_EQUAL(std_string_at(string, strlen(test_string) - 1), 't');
	TEST_ASSERT_EQUAL(std_string_at(string, 5), 'i');
	TEST_ASSERT_EQUAL(0, strcmp(std_string_into_cstring(string), test_string));
	TEST_ASSERT(std_string_is_full(string));
	std_string_free(string);
}

TEST(StdString, from_cstring_with_allocator) {
	let test_string = "This is a test test test";
	let_mut string = std_string_from_with_allocator(
		test_string,
		std_allocator_from_custom_stateless_allocator(test_malloc, test_realloc, test_free));
	TEST_ASSERT_EQUAL(std_string_length(string), strlen(test_string));
	TEST_ASSERT_EQUAL(std_string_capacity(string), strlen(test_string));
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == test_malloc);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == test_realloc);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == test_free);
	TEST_ASSERT_EQUAL(std_string_at(string, strlen(test_string) - 1), 't');
	TEST_ASSERT_EQUAL(std_string_at(string, 5), 'i');
	TEST_ASSERT_EQUAL(0, strcmp(std_string_into_cstring(string), test_string));
	std_string_free(string);
}

TEST(StdString, from_stringview) {
	let test_string = "This is a test test test";
	let view = (StdStringView){.m_view = test_string, .m_length = strlen(test_string)};
	let_mut string = std_string_from(&view);
	TEST_ASSERT_EQUAL(std_string_length(string), strlen(test_string));
	TEST_ASSERT_EQUAL(std_string_capacity(string), strlen(test_string));
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == std_allocate);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == std_reallocate);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == std_deallocate);
	TEST_ASSERT_EQUAL(std_string_at(string, strlen(test_string) - 1), 't');
	TEST_ASSERT_EQUAL(std_string_at(string, 5), 'i');
	TEST_ASSERT(0 == strcmp(std_string_into_cstring(string), test_string));
	std_string_free(string);
}

TEST(StdString, from_stringview_with_allocator) {
	let test_string = "This is a test test test";
	let view = (StdStringView){.m_view = test_string, .m_length = strlen(test_string)};
	let_mut string = std_string_from_with_allocator(
		&view,
		std_allocator_from_custom_stateless_allocator(test_malloc, test_realloc, test_free));
	TEST_ASSERT_EQUAL(std_string_length(string), strlen(test_string));
	TEST_ASSERT_EQUAL(std_string_capacity(string), strlen(test_string));
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == test_malloc);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == test_realloc);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == test_free);
	TEST_ASSERT_EQUAL(std_string_at(string, strlen(test_string) - 1), 't');
	TEST_ASSERT_EQUAL(std_string_at(string, 5), 'i');
	TEST_ASSERT(0 == strcmp(std_string_into_cstring(string), test_string));
	std_string_free(string);
}

TEST(StdString, clone_and_free) {
	let_mut string = std_string_from("This is a test test test");
	let_mut string2 = std_string_clone(string);
	TEST_ASSERT(0 == strcmp(std_string_into_cstring(string), std_string_into_cstring(string2)));
	// proof that they don't point to the same memory
	TEST_ASSERT(std_string_into_cstring(string) != std_string_into_cstring(string2));
	std_string_free(string2);
	TEST_ASSERT_EQUAL(std_string_capacity(string2), STD_STRING_SHORT_OPTIMIZATION_CAPACITY);
	TEST_ASSERT(string2.m_long == nullptr);
	std_string_free(string);
	std_string_free(string2);
}

TEST(StdString, first) {
	let_mut string = std_string_from("This is a test test test");
	let_mut string2 = std_string_first(string, 4);
	TEST_ASSERT_EQUAL(std_string_length(string2), 4);
	TEST_ASSERT_EQUAL(std_string_at(string2, 0), 'T');
	TEST_ASSERT_EQUAL(std_string_at(string2, 4), 0);
	TEST_ASSERT(0 == strcmp(std_string_into_cstring(string2), "This"));
	std_string_free(string);
	std_string_free(string2);
}

TEST(StdString, last) {
	let_mut string = std_string_from("This is a test test test");
	let_mut string2 = std_string_last(string, 4);
	TEST_ASSERT_EQUAL(std_string_length(string2), 4);
	TEST_ASSERT_EQUAL(std_string_at(string2, 0), 't');
	TEST_ASSERT_EQUAL(std_string_at(string2, 4), 0);
	TEST_ASSERT(0 == strcmp(std_string_into_cstring(string2), "test"));
	std_string_free(string);
	std_string_free(string2);
}

TEST(StdString, equal) {
	let_mut string1 = std_string_from("This is a test test test");
	let_mut string2 = std_string_from("This is a test test test");
	let_mut string3 = std_string_from("This is a test");
	TEST_ASSERT(std_string_equal(string1, &string2));
	TEST_ASSERT_FALSE(std_string_equal(string1, &string3));
	std_string_free(string1);
	std_string_free(string2);
	std_string_free(string3);
}

TEST(StdString, contains) {
	let_mut string = std_string_from("This is a test test test");
	TEST_ASSERT(std_string_contains(string, "test"));
	std_string_free(string);
}

TEST(StdString, starts_with) {
	let_mut string = std_string_from("This is a test test test");
	TEST_ASSERT(std_string_starts_with(string, "This is"));
	std_string_free(string);
}

TEST(StdString, ends_with) {
	let_mut string = std_string_from("This is a test test test");
	TEST_ASSERT(std_string_ends_with(string, "test test"));
	std_string_free(string);
}

TEST(StdString, find_first) {
	let_mut string = std_string_from("This is a test test test");
	let_mut maybe_index = std_string_find_first(string, "test");
	TEST_ASSERT(std_option_is_some(maybe_index));
	TEST_ASSERT_EQUAL(std_option_unwrap(maybe_index), 10);
	std_string_free(string);
}

TEST(StdString, find_last) {
	let_mut string = std_string_from("This is a test test test");
	let_mut maybe_index = std_string_find_last(string, "test");
	TEST_ASSERT(std_option_is_some(maybe_index));
	TEST_ASSERT_EQUAL(std_option_unwrap(maybe_index), std_string_length(string) - 4);
	std_string_free(string);
}

TEST(StdString, substring) {
	let_mut string = std_string_from("This is a test test test");
	let_mut substring = std_string_substring(string, 8, 6);
	TEST_ASSERT(std_string_equal(substring, "a test"));
	std_string_free(string);
	std_string_free(substring);
}

TEST(StdString, stringview_of) {
	let_mut string = std_string_from("This is a test test test");
	let view = std_string_stringview_of(string, 8, 6);
	TEST_ASSERT(0 == memcmp(std_stringview_at(view, 0), "a test", 6));
	TEST_ASSERT_EQUAL(std_stringview_length(view), 6);
	std_string_free(string);
}

TEST(StdString, concatenate) {
	let_mut string = std_string_from("This is a test test test");
	let_mut string_left = std_string_from("This is a test");
	let_mut string_right = std_string_from(" test test");
	let_mut string_concat = std_string_concatenate(&string_left, &string_right);
	TEST_ASSERT(std_string_equal(string, &string_concat));
	std_string_free(string);
	std_string_free(string_left);
	std_string_free(string_right);
	std_string_free(string_concat);
}

TEST(StdString, fill) {
	let_mut string = std_string_new();
	std_string_fill(string, 't');
	TEST_ASSERT_EQUAL(std_string_length(string), STD_STRING_SHORT_OPTIMIZATION_CAPACITY);
	TEST_ASSERT_EQUAL(std_string_at(string, 0), 't');
	TEST_ASSERT_EQUAL(std_string_at(string, STD_STRING_SHORT_OPTIMIZATION_CAPACITY - 1), 't');
	std_string_free(string);
}

TEST(StdString, clear) {
	let test_string = "This is a test test test";
	let_mut string = std_string_from(test_string);
	std_string_clear(string);
	TEST_ASSERT_EQUAL(std_string_capacity(string), strlen(test_string));
	TEST_ASSERT_EQUAL(std_string_length(string), 0);
	std_string_free(string);
}

TEST(StdString, shrink_to_fit) {
	let test_string = "This is a test test test";
	let_mut string = std_string_from(test_string);
	TEST_ASSERT_EQUAL(std_string_capacity(string), strlen(test_string));
	TEST_ASSERT_EQUAL(std_string_length(string), strlen(test_string));
	std_string_clear(string);
	TEST_ASSERT_EQUAL(std_string_capacity(string), strlen(test_string));
	TEST_ASSERT_EQUAL(std_string_length(string), 0);
	std_string_shrink_to_fit(string);
	TEST_ASSERT_EQUAL(std_string_capacity(string), STD_STRING_SHORT_OPTIMIZATION_CAPACITY);
	std_string_free(string);
}

TEST(StdString, insert) {
	let test_string_full = "This is a test test test";
	let test_string_first_ = "This is ";
	let test_string_second = "a test ";
	let test_string_end = "test test";
	let_mut string = std_string_from(test_string_first_);
	let insert_pos = strlen(test_string_first_);
	std_string_insert(string, test_string_end, insert_pos);
	TEST_ASSERT(
		0 == memcmp(&std_string_at(string, 0), "This is test test", std_string_length(string)));
	std_string_insert(string, test_string_second, insert_pos);
	TEST_ASSERT(0
				== memcmp(&std_string_at(string, 0), test_string_full, std_string_length(string)));
	std_string_free(string);
}

TEST(StdString, erase) {
	let_mut string = std_string_from("This is a test test test");
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	std_string_erase(string, 8);
	let compare_string = std_string_from("This is  test test test");
	TEST_ASSERT(std_string_equal(string, &compare_string));
	std_string_free(string);
}

TEST(StdString, erase_n) {
	let_mut string = std_string_from("This is a test test test");
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	std_string_erase_n(string, 8, 7);
	let_mut compare_string = std_string_from("This is test test");
	TEST_ASSERT(std_string_equal(string, &compare_string));
	std_string_free(string);
	std_string_free(compare_string);
}

TEST(StdString, resize) {
	let_mut string = std_string_from("This is a test test test");
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	std_string_resize(string, 9);
	let_mut compare_string = std_string_from("This is a");
	TEST_ASSERT(std_string_equal(string, &compare_string));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	std_string_resize(string, 15);
	// even tho `string` was resized, its semantic contents are still the same (the new size is
	// null-padded), so they should still be equal
	TEST_ASSERT(std_string_equal(string, &compare_string));
	std_string_free(string);
	std_string_free(compare_string);
}

TEST(StdString, reserve) {
	let_mut string = std_string_from("This is a test test test");
	let capacity = std_string_capacity(string);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	std_string_reserve(string, 32);
	TEST_ASSERT(std_string_capacity(string) >= 32);
	TEST_ASSERT_NOT_EQUAL(std_string_capacity(string), capacity);
	std_string_free(string);
}

TEST(StdString, push_back) {
	let_mut string = std_string_from("This is");
	std_string_push_back(string, ' ');
	std_string_push_back(string, 'a');
	std_string_push_back(string, ' ');
	std_string_push_back(string, 't');
	std_string_push_back(string, 'e');
	std_string_push_back(string, 's');
	std_string_push_back(string, 't');
	std_string_push_back(string, ' ');
	std_string_push_back(string, 't');
	std_string_push_back(string, 'e');
	std_string_push_back(string, 's');
	std_string_push_back(string, 't');
	std_string_push_back(string, ' ');
	std_string_push_back(string, 't');
	std_string_push_back(string, 'e');
	std_string_push_back(string, 's');
	std_string_push_back(string, 't');
	let_mut compare_string = std_string_from("This is a test test test");
	TEST_ASSERT(std_string_equal(string, &compare_string));
	TEST_ASSERT_EQUAL(std_string_length(string), std_string_length(compare_string));
	TEST_ASSERT(std_string_capacity(string) >= std_string_capacity(compare_string));
	std_string_free(string);
	std_string_free(compare_string);
}

TEST(StdString, push_front) {
	let_mut string = std_string_from("test");
	std_string_push_front(string, ' ');
	std_string_push_front(string, 't');
	std_string_push_front(string, 's');
	std_string_push_front(string, 'e');
	std_string_push_front(string, 't');
	std_string_push_front(string, ' ');
	std_string_push_front(string, 't');
	std_string_push_front(string, 's');
	std_string_push_front(string, 'e');
	std_string_push_front(string, 't');
	std_string_push_front(string, ' ');
	std_string_push_front(string, 'a');
	std_string_push_front(string, ' ');
	std_string_push_front(string, 's');
	std_string_push_front(string, 'i');
	std_string_push_front(string, ' ');
	std_string_push_front(string, 's');
	std_string_push_front(string, 'i');
	std_string_push_front(string, 'h');
	std_string_push_front(string, 'T');
	let_mut compare_string = std_string_from("This is a test test test");
	TEST_ASSERT(std_string_equal(string, &compare_string));
	TEST_ASSERT_EQUAL(std_string_length(string), std_string_length(compare_string));
	TEST_ASSERT(std_string_capacity(string) >= std_string_capacity(compare_string));
	std_string_free(string);
	std_string_free(compare_string);
}

TEST(StdString, pop_back) {
	let_mut string = std_string_from("This is a test test test");
	let length = std_string_length(string);
	let_mut maybe_char = std_string_pop_back(string);
	TEST_ASSERT_EQUAL(std_string_length(string), length - 1);
	TEST_ASSERT(std_option_is_some(maybe_char));
	TEST_ASSERT_EQUAL(std_option_unwrap(maybe_char), 't');
	let_mut compare_string = std_string_from("This is a test test tes");
	TEST_ASSERT(std_string_equal(string, &compare_string));
	std_string_free(string);
	std_string_free(compare_string);
}

TEST(StdString, pop_front) {
	let_mut string = std_string_from("This is a test test test");
	let length = std_string_length(string);
	let_mut maybe_char = std_string_pop_front(string);
	TEST_ASSERT_EQUAL(std_string_length(string), length - 1);
	TEST_ASSERT(std_option_is_some(maybe_char));
	TEST_ASSERT_EQUAL(std_option_unwrap(maybe_char), 'T');
	let_mut compare_string = std_string_from("his is a test test test");
	TEST_ASSERT(std_string_equal(string, &compare_string));
	std_string_free(string);
	std_string_free(compare_string);
}

TEST(StdString, append) {
	let_mut string = std_string_from("This is a ");
	std_string_append(string, "test test test");
	let_mut compare_string = std_string_from("This is a test test test");
	TEST_ASSERT(std_string_equal(string, &compare_string));
	std_string_free(string);
	std_string_free(compare_string);
}

TEST(StdString, prepend) {
	let_mut string = std_string_from("test test test");
	std_string_prepend(string, "This is a ");
	let_mut compare_string = std_string_from("This is a test test test");
	TEST_ASSERT(std_string_equal(string, &compare_string));
	std_string_free(string);
	std_string_free(compare_string);
}

TEST(StdString, replace) {
	let_mut string = std_string_from("This is a test test test");
	std_string_replace(string, "That", 0);
	let_mut compare_string1 = std_string_from("That is a test test test");
	TEST_ASSERT(std_string_equal(string, &compare_string1));
	std_string_replace(string, "lame", 10);
	let_mut compare_string2 = std_string_from("That is a lame test test");
	TEST_ASSERT(std_string_equal(string, &compare_string2));
	std_string_free(string);
	std_string_free(compare_string1);
	std_string_free(compare_string2);
}

TEST(StdString, iterator) {
	let_mut string = std_string_from("This is a test test test");
	let_mut string2 = std_string_new_with_capacity(std_string_length(string));
	foreach(character, string) {
		std_string_push_back(string2, character);
	}
	TEST_ASSERT(std_string_equal(string, &string2));
	std_string_free(string);
	std_string_free(string2);
}

#endif // STD_STRING_TEST
