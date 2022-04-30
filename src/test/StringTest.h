#ifndef CNX_STRING_TEST
#define CNX_STRING_TEST

#include <Cnx/String.h>
#include <Cnx/StringExt.h>

#include "Criterion.h"

static void* test_malloc([[maybe_unused]] CnxAllocator* self, usize bytes) {
	return malloc(bytes);
}

static void* test_realloc([[maybe_unused]] CnxAllocator* self, void* memory, usize new_size_bytes) {
	return realloc(memory, new_size_bytes);
}

static void test_free([[maybe_unused]] CnxAllocator* self, void* memory) {
	free(memory);
}

TEST(CnxString, new) {
	let_mut string = cnx_string_new();

	TEST_ASSERT_EQUAL(cnx_string_length(string), 0);
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), CNX_STRING_SHORT_OPTIMIZATION_CAPACITY);
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == cnx_allocate);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == cnx_reallocate);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == cnx_deallocate);
	TEST_ASSERT(cnx_string_is_empty(string));
	TEST_ASSERT(!cnx_string_is_full(string));
	cnx_string_free(string);
}

TEST(CnxString, new_with_allocator) {
	let_mut string = cnx_string_new_with_allocator(
		cnx_allocator_from_custom_stateless_allocator(test_malloc, test_realloc, test_free));

	TEST_ASSERT_EQUAL(cnx_string_length(string), 0);
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), CNX_STRING_SHORT_OPTIMIZATION_CAPACITY);
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == test_malloc);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == test_realloc);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == test_free);
	cnx_string_free(string);
}

TEST(CnxString, new_with_capacity) {
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	let_mut string = cnx_string_new_with_capacity(30U);

	TEST_ASSERT_EQUAL(cnx_string_length(string), 0);
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), 30U);
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == cnx_allocate);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == cnx_reallocate);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == cnx_deallocate);
	cnx_string_free(string);
}

TEST(CnxString, new_with_capacity_with_allocator) {
	let_mut string = cnx_string_new_with_capacity_with_allocator(
		// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
		30U,
		cnx_allocator_from_custom_stateless_allocator(test_malloc, test_realloc, test_free));

	TEST_ASSERT_EQUAL(cnx_string_length(string), 0);
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), 30U);
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == test_malloc);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == test_realloc);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == test_free);
	cnx_string_free(string);
}

TEST(CnxString, from_cstring) {
	let test_string = "This is a test test test";
	let_mut string = cnx_string_from(test_string);
	TEST_ASSERT_EQUAL(cnx_string_length(string), strlen(test_string));
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), strlen(test_string));
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == cnx_allocate);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == cnx_reallocate);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == cnx_deallocate);
	TEST_ASSERT_EQUAL(cnx_string_at(string, strlen(test_string) - 1), 't');
	TEST_ASSERT_EQUAL(cnx_string_at(string, 5), 'i');
	TEST_ASSERT_EQUAL(0, strcmp(cnx_string_into_cstring(string), test_string));
	TEST_ASSERT(cnx_string_is_full(string));
	cnx_string_free(string);
}

TEST(CnxString, from_cstring_with_allocator) {
	let test_string = "This is a test test test";
	let_mut string = cnx_string_from_with_allocator(
		test_string,
		cnx_allocator_from_custom_stateless_allocator(test_malloc, test_realloc, test_free));
	TEST_ASSERT_EQUAL(cnx_string_length(string), strlen(test_string));
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), strlen(test_string));
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == test_malloc);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == test_realloc);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == test_free);
	TEST_ASSERT_EQUAL(cnx_string_at(string, strlen(test_string) - 1), 't');
	TEST_ASSERT_EQUAL(cnx_string_at(string, 5), 'i');
	TEST_ASSERT_EQUAL(0, strcmp(cnx_string_into_cstring(string), test_string));
	cnx_string_free(string);
}

TEST(CnxString, from_stringview) {
	let test_string = "This is a test test test";
	let view = (CnxStringView){.m_view = test_string, .m_length = strlen(test_string)};
	let_mut string = cnx_string_from(&view);
	TEST_ASSERT_EQUAL(cnx_string_length(string), strlen(test_string));
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), strlen(test_string));
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == cnx_allocate);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == cnx_reallocate);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == cnx_deallocate);
	TEST_ASSERT_EQUAL(cnx_string_at(string, strlen(test_string) - 1), 't');
	TEST_ASSERT_EQUAL(cnx_string_at(string, 5), 'i');
	TEST_ASSERT(0 == strcmp(cnx_string_into_cstring(string), test_string));
	cnx_string_free(string);
}

TEST(CnxString, from_stringview_with_allocator) {
	let test_string = "This is a test test test";
	let view = (CnxStringView){.m_view = test_string, .m_length = strlen(test_string)};
	let_mut string = cnx_string_from_with_allocator(
		&view,
		cnx_allocator_from_custom_stateless_allocator(test_malloc, test_realloc, test_free));
	TEST_ASSERT_EQUAL(cnx_string_length(string), strlen(test_string));
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), strlen(test_string));
	TEST_ASSERT(string.m_allocator.m_vtable->allocate == test_malloc);
	TEST_ASSERT(string.m_allocator.m_vtable->reallocate == test_realloc);
	TEST_ASSERT(string.m_allocator.m_vtable->deallocate == test_free);
	TEST_ASSERT_EQUAL(cnx_string_at(string, strlen(test_string) - 1), 't');
	TEST_ASSERT_EQUAL(cnx_string_at(string, 5), 'i');
	TEST_ASSERT(0 == strcmp(cnx_string_into_cstring(string), test_string));
	cnx_string_free(string);
}

TEST(CnxString, clone_and_free) {
	let_mut string = cnx_string_from("This is a test test test");
	let_mut string2 = cnx_string_clone(string);
	TEST_ASSERT(0 == strcmp(cnx_string_into_cstring(string), cnx_string_into_cstring(string2)));
	// proof that they don't point to the same memory
	TEST_ASSERT(cnx_string_into_cstring(string) != cnx_string_into_cstring(string2));
	cnx_string_free(string2);
	TEST_ASSERT_EQUAL(cnx_string_capacity(string2), CNX_STRING_SHORT_OPTIMIZATION_CAPACITY);
	TEST_ASSERT(string2.m_long == nullptr);
	cnx_string_free(string);
	cnx_string_free(string2);
}

TEST(CnxString, first) {
	let_mut string = cnx_string_from("This is a test test test");
	let_mut string2 = cnx_string_first(string, 4);
	TEST_ASSERT_EQUAL(cnx_string_length(string2), 4);
	TEST_ASSERT_EQUAL(cnx_string_at(string2, 0), 'T');
	TEST_ASSERT_EQUAL(cnx_string_at(string2, 4), 0);
	TEST_ASSERT(0 == strcmp(cnx_string_into_cstring(string2), "This"));
	cnx_string_free(string);
	cnx_string_free(string2);
}

TEST(CnxString, last) {
	let_mut string = cnx_string_from("This is a test test test");
	let_mut string2 = cnx_string_last(string, 4);
	TEST_ASSERT_EQUAL(cnx_string_length(string2), 4);
	TEST_ASSERT_EQUAL(cnx_string_at(string2, 0), 't');
	TEST_ASSERT_EQUAL(cnx_string_at(string2, 4), 0);
	TEST_ASSERT(0 == strcmp(cnx_string_into_cstring(string2), "test"));
	cnx_string_free(string);
	cnx_string_free(string2);
}

TEST(CnxString, equal) {
	let_mut string1 = cnx_string_from("This is a test test test");
	let_mut string2 = cnx_string_from("This is a test test test");
	let_mut string3 = cnx_string_from("This is a test");
	TEST_ASSERT(cnx_string_equal(string1, &string2));
	TEST_ASSERT_FALSE(cnx_string_equal(string1, &string3));
	cnx_string_free(string1);
	cnx_string_free(string2);
	cnx_string_free(string3);
}

TEST(CnxString, contains) {
	let_mut string = cnx_string_from("This is a test test test");
	TEST_ASSERT(cnx_string_contains(string, "test"));
	cnx_string_free(string);
}

TEST(CnxString, starts_with) {
	let_mut string = cnx_string_from("This is a test test test");
	TEST_ASSERT(cnx_string_starts_with(string, "This is"));
	cnx_string_free(string);
}

TEST(CnxString, ends_with) {
	let_mut string = cnx_string_from("This is a test test test");
	TEST_ASSERT(cnx_string_ends_with(string, "test test"));
	cnx_string_free(string);
}

TEST(CnxString, find_first) {
	let_mut string = cnx_string_from("This is a test test test");
	let_mut maybe_index = cnx_string_find_first(string, "test");
	TEST_ASSERT(cnx_option_is_some(maybe_index));
	TEST_ASSERT_EQUAL(cnx_option_unwrap(maybe_index), 10);
	cnx_string_free(string);
}

TEST(CnxString, find_last) {
	let_mut string = cnx_string_from("This is a test test test");
	let_mut maybe_index = cnx_string_find_last(string, "test");
	TEST_ASSERT(cnx_option_is_some(maybe_index));
	TEST_ASSERT_EQUAL(cnx_option_unwrap(maybe_index), cnx_string_length(string) - 4);
	cnx_string_free(string);
}

TEST(CnxString, substring) {
	let_mut string = cnx_string_from("This is a test test test");
	let_mut substring = cnx_string_substring(string, 8, 6);
	TEST_ASSERT(cnx_string_equal(substring, "a test"));
	cnx_string_free(string);
	cnx_string_free(substring);
}

TEST(CnxString, stringview_of) {
	let_mut string = cnx_string_from("This is a test test test");
	let view = cnx_string_stringview_of(string, 8, 6);
	TEST_ASSERT(0 == memcmp(cnx_stringview_at(view, 0), "a test", 6));
	TEST_ASSERT_EQUAL(cnx_stringview_length(view), 6);

	let view2 = cnx_string_stringview_of(string, 15, 9);
	TEST_ASSERT(0 == memcmp(cnx_stringview_at(view2, 0), "test test", 9));
	TEST_ASSERT_EQUAL(cnx_stringview_length(view2), 9);

	cnx_string_free(string);
}

TEST(CnxString, concatenate) {
	let_mut string = cnx_string_from("This is a test test test");
	let_mut string_left = cnx_string_from("This is a test");
	let_mut string_right = cnx_string_from(" test test");
	let_mut string_concat = cnx_string_concatenate(&string_left, &string_right);
	TEST_ASSERT(cnx_string_equal(string, &string_concat));
	cnx_string_free(string);
	cnx_string_free(string_left);
	cnx_string_free(string_right);
	cnx_string_free(string_concat);
}

TEST(CnxString, fill) {
	let_mut string = cnx_string_new();
	cnx_string_fill(string, 't');
	TEST_ASSERT_EQUAL(cnx_string_length(string), CNX_STRING_SHORT_OPTIMIZATION_CAPACITY);
	TEST_ASSERT_EQUAL(cnx_string_at(string, 0), 't');
	TEST_ASSERT_EQUAL(cnx_string_at(string, CNX_STRING_SHORT_OPTIMIZATION_CAPACITY - 1), 't');
	cnx_string_free(string);
}

TEST(CnxString, clear) {
	let test_string = "This is a test test test";
	let_mut string = cnx_string_from(test_string);
	cnx_string_clear(string);
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), strlen(test_string));
	TEST_ASSERT_EQUAL(cnx_string_length(string), 0);
	cnx_string_free(string);
}

TEST(CnxString, shrink_to_fit) {
	let test_string = "This is a test test test";
	let_mut string = cnx_string_from(test_string);
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), strlen(test_string));
	TEST_ASSERT_EQUAL(cnx_string_length(string), strlen(test_string));
	cnx_string_clear(string);
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), strlen(test_string));
	TEST_ASSERT_EQUAL(cnx_string_length(string), 0);
	cnx_string_shrink_to_fit(string);
	TEST_ASSERT_EQUAL(cnx_string_capacity(string), CNX_STRING_SHORT_OPTIMIZATION_CAPACITY);
	cnx_string_free(string);
}

TEST(CnxString, insert) {
	let test_string_full = "This is a test test test";
	let test_string_first_ = "This is ";
	let test_string_second = "a test ";
	let test_string_end = "test test";
	let_mut string = cnx_string_from(test_string_first_);
	let insert_pos = strlen(test_string_first_);
	cnx_string_insert(string, test_string_end, insert_pos);
	TEST_ASSERT(
		0 == memcmp(&cnx_string_at(string, 0), "This is test test", cnx_string_length(string)));
	cnx_string_insert(string, test_string_second, insert_pos);
	TEST_ASSERT(0
				== memcmp(&cnx_string_at(string, 0), test_string_full, cnx_string_length(string)));
	cnx_string_free(string);
}

TEST(CnxString, erase) {
	let_mut string = cnx_string_from("This is a test test test");
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	cnx_string_erase(string, 8);
	let compare_string = cnx_string_from("This is  test test test");
	TEST_ASSERT(cnx_string_equal(string, &compare_string));
	cnx_string_free(string);
}

TEST(CnxString, erase_n) {
	let_mut string = cnx_string_from("This is a test test test");
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	cnx_string_erase_n(string, 8, 7);
	let_mut compare_string = cnx_string_from("This is test test");
	TEST_ASSERT(cnx_string_equal(string, &compare_string));
	cnx_string_free(string);
	cnx_string_free(compare_string);
}

TEST(CnxString, resize) {
	let_mut string = cnx_string_from("This is a test test test");
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	cnx_string_resize(string, 9);
	let_mut compare_string = cnx_string_from("This is a");
	TEST_ASSERT(cnx_string_equal(string, &compare_string));
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	cnx_string_resize(string, 15);
	// even tho `string` was resized, its semantic contents are still the same (the new size is
	// null-padded), so they should still be equal
	TEST_ASSERT(cnx_string_equal(string, &compare_string));
	cnx_string_free(string);
	cnx_string_free(compare_string);
}

TEST(CnxString, reserve) {
	let_mut string = cnx_string_from("This is a test test test");
	let capacity = cnx_string_capacity(string);
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	cnx_string_reserve(string, 32);
	TEST_ASSERT(cnx_string_capacity(string) >= 32);
	TEST_ASSERT_NOT_EQUAL(cnx_string_capacity(string), capacity);
	cnx_string_free(string);
}

TEST(CnxString, push_back) {
	let_mut string = cnx_string_from("This is");
	cnx_string_push_back(string, ' ');
	cnx_string_push_back(string, 'a');
	cnx_string_push_back(string, ' ');
	cnx_string_push_back(string, 't');
	cnx_string_push_back(string, 'e');
	cnx_string_push_back(string, 's');
	cnx_string_push_back(string, 't');
	cnx_string_push_back(string, ' ');
	cnx_string_push_back(string, 't');
	cnx_string_push_back(string, 'e');
	cnx_string_push_back(string, 's');
	cnx_string_push_back(string, 't');
	cnx_string_push_back(string, ' ');
	cnx_string_push_back(string, 't');
	cnx_string_push_back(string, 'e');
	cnx_string_push_back(string, 's');
	cnx_string_push_back(string, 't');
	let_mut compare_string = cnx_string_from("This is a test test test");
	TEST_ASSERT(cnx_string_equal(string, &compare_string));
	TEST_ASSERT_EQUAL(cnx_string_length(string), cnx_string_length(compare_string));
	TEST_ASSERT(cnx_string_capacity(string) >= cnx_string_capacity(compare_string));
	cnx_string_free(string);
	cnx_string_free(compare_string);
}

TEST(CnxString, push_front) {
	let_mut string = cnx_string_from("test");
	cnx_string_push_front(string, ' ');
	cnx_string_push_front(string, 't');
	cnx_string_push_front(string, 's');
	cnx_string_push_front(string, 'e');
	cnx_string_push_front(string, 't');
	cnx_string_push_front(string, ' ');
	cnx_string_push_front(string, 't');
	cnx_string_push_front(string, 's');
	cnx_string_push_front(string, 'e');
	cnx_string_push_front(string, 't');
	cnx_string_push_front(string, ' ');
	cnx_string_push_front(string, 'a');
	cnx_string_push_front(string, ' ');
	cnx_string_push_front(string, 's');
	cnx_string_push_front(string, 'i');
	cnx_string_push_front(string, ' ');
	cnx_string_push_front(string, 's');
	cnx_string_push_front(string, 'i');
	cnx_string_push_front(string, 'h');
	cnx_string_push_front(string, 'T');
	let_mut compare_string = cnx_string_from("This is a test test test");
	TEST_ASSERT(cnx_string_equal(string, &compare_string));
	TEST_ASSERT_EQUAL(cnx_string_length(string), cnx_string_length(compare_string));
	TEST_ASSERT(cnx_string_capacity(string) >= cnx_string_capacity(compare_string));
	cnx_string_free(string);
	cnx_string_free(compare_string);
}

TEST(CnxString, pop_back) {
	let_mut string = cnx_string_from("This is a test test test");
	let length = cnx_string_length(string);
	let_mut maybe_char = cnx_string_pop_back(string);
	TEST_ASSERT_EQUAL(cnx_string_length(string), length - 1);
	TEST_ASSERT(cnx_option_is_some(maybe_char));
	TEST_ASSERT_EQUAL(cnx_option_unwrap(maybe_char), 't');
	let_mut compare_string = cnx_string_from("This is a test test tes");
	TEST_ASSERT(cnx_string_equal(string, &compare_string));
	cnx_string_free(string);
	cnx_string_free(compare_string);
}

TEST(CnxString, pop_front) {
	let_mut string = cnx_string_from("This is a test test test");
	let length = cnx_string_length(string);
	let_mut maybe_char = cnx_string_pop_front(string);
	TEST_ASSERT_EQUAL(cnx_string_length(string), length - 1);
	TEST_ASSERT(cnx_option_is_some(maybe_char));
	TEST_ASSERT_EQUAL(cnx_option_unwrap(maybe_char), 'T');
	let_mut compare_string = cnx_string_from("his is a test test test");
	TEST_ASSERT(cnx_string_equal(string, &compare_string));
	cnx_string_free(string);
	cnx_string_free(compare_string);
}

TEST(CnxString, append) {
	let_mut string = cnx_string_from("This is a ");
	cnx_string_append(string, "test test test");
	let_mut compare_string = cnx_string_from("This is a test test test");
	TEST_ASSERT(cnx_string_equal(string, &compare_string));
	cnx_string_free(string);
	cnx_string_free(compare_string);
}

TEST(CnxString, prepend) {
	let_mut string = cnx_string_from("test test test");
	cnx_string_prepend(string, "This is a ");
	let_mut compare_string = cnx_string_from("This is a test test test");
	TEST_ASSERT(cnx_string_equal(string, &compare_string));
	cnx_string_free(string);
	cnx_string_free(compare_string);
}

TEST(CnxString, replace) {
	let_mut string = cnx_string_from("This is a test test test");
	cnx_string_replace(string, "That", 0);
	let_mut compare_string1 = cnx_string_from("That is a test test test");
	TEST_ASSERT(cnx_string_equal(string, &compare_string1));
	cnx_string_replace(string, "lame", 10);
	let_mut compare_string2 = cnx_string_from("That is a lame test test");
	TEST_ASSERT(cnx_string_equal(string, &compare_string2));
	cnx_string_free(string);
	cnx_string_free(compare_string1);
	cnx_string_free(compare_string2);
}

TEST(CnxString, iterator) {
	let_mut string = cnx_string_from("This is a test test test");
	let_mut string2 = cnx_string_new_with_capacity(cnx_string_length(string));
	foreach(character, string) {
		cnx_string_push_back(string2, character);
	}
	TEST_ASSERT(cnx_string_equal(string, &string2));
	cnx_string_free(string);
	cnx_string_free(string2);
}

TEST(CnxString, split_on) {
	CnxScopedString string = cnx_string_from("This=is=a=test=string");

	CnxScopedVector(CnxString) split = cnx_string_split_on(string, '=');

	TEST_ASSERT_EQUAL(cnx_vector_size(split), 5); // NOLINT
	TEST_ASSERT(cnx_string_equal(cnx_vector_at(split, 0), "This"));
	TEST_ASSERT(cnx_string_equal(cnx_vector_at(split, 1), "is"));
	TEST_ASSERT(cnx_string_equal(cnx_vector_at(split, 2), "a"));
	TEST_ASSERT(cnx_string_equal(cnx_vector_at(split, 3), "test"));
	TEST_ASSERT(cnx_string_equal(cnx_vector_at(split, 4), "string"));
}

TEST(CnxString, view_split_on) {
	CnxScopedString string = cnx_string_from("This=is=a=test=string");

	CnxScopedVector(CnxStringView) split = cnx_string_view_split_on(string, '=');

	TEST_ASSERT_EQUAL(cnx_vector_size(split), 5); // NOLINT
	TEST_ASSERT(cnx_stringview_equal(cnx_vector_at(split, 0), "This"));
	TEST_ASSERT(cnx_stringview_equal(cnx_vector_at(split, 1), "is"));
	TEST_ASSERT(cnx_stringview_equal(cnx_vector_at(split, 2), "a"));
	TEST_ASSERT(cnx_stringview_equal(cnx_vector_at(split, 3), "test"));
	TEST_ASSERT(cnx_stringview_equal(cnx_vector_at(split, 4), "string"));
}

TEST(CnxString, occurences_of_char) {
	CnxScopedString string = cnx_string_from("A=test=test=test=string");

	let occurrences = cnx_string_occurrences_of_char(string, '=');

	TEST_ASSERT_EQUAL(occurrences, 4);
}

TEST(CnxString, occurrences_of) {
	CnxScopedString string = cnx_string_from("A=test=test=test=string");

	let occurrences = cnx_string_occurrences_of(string, "test");

	TEST_ASSERT_EQUAL(occurrences, 3);
}

TEST(CnxString, find_occurrences_of_char) {
	CnxScopedString string = cnx_string_from("A=test=test=test=string");

	CnxScopedVector(usize) occurrences = cnx_string_find_occurrences_of_char(string, '=');

	TEST_ASSERT_EQUAL(cnx_vector_size(occurrences), 4);
	TEST_ASSERT_EQUAL(cnx_vector_at(occurrences, 0), 1);
	TEST_ASSERT_EQUAL(cnx_vector_at(occurrences, 1), 6);
	TEST_ASSERT_EQUAL(cnx_vector_at(occurrences, 2), 11);
	TEST_ASSERT_EQUAL(cnx_vector_at(occurrences, 3), 16);
}

TEST(CnxString, find_occurrences_of) {
	CnxScopedString string = cnx_string_from("A=test=test=test=string");

	CnxScopedVector(usize) occurrences = cnx_string_find_occurrences_of(string, "test");

	TEST_ASSERT_EQUAL(cnx_vector_size(occurrences), 3);
	TEST_ASSERT_EQUAL(cnx_vector_at(occurrences, 0), 2);
	TEST_ASSERT_EQUAL(cnx_vector_at(occurrences, 1), 7);
	TEST_ASSERT_EQUAL(cnx_vector_at(occurrences, 2), 12);
}

#endif // CNX_STRING_TEST
