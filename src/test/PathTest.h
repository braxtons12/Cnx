#ifndef CNX_PATH_TEST
#define CNX_PATH_TEST

#include <Cnx/IO.h>
#include <Cnx/filesystem/Path.h>

#include "Criterion.h"

// NOLINTNEXTLINE
TEST(CnxPath, create_and_remove_file) {
	CnxScopedString path = cnx_string_from("CnxPathTest.txt");

	TEST_ASSERT_TRUE(cnx_path_is_valid(&path));

	TEST_ASSERT_FALSE(cnx_path_exists(&path));

	let_mut res = cnx_path_create_file(&path);

	TEST_ASSERT_TRUE(cnx_result_is_ok(res));

	TEST_ASSERT_TRUE(cnx_path_exists(&path));

	TEST_ASSERT_TRUE(cnx_path_is_file(&path));
	TEST_ASSERT_FALSE(cnx_path_is_directory(&path));

	TEST_ASSERT_TRUE(cnx_path_has_file_extension(&path, "txt"));
	CnxScopedString name_actual = cnx_path_get_file_name_without_extension(&path);
	CnxScopedString name = cnx_string_from("CnxPathTest");
	TEST_ASSERT_TRUE(cnx_string_equal(name, &name_actual));

	res = cnx_path_remove_file(&path);
	TEST_ASSERT_TRUE(cnx_result_is_ok(res));

	TEST_ASSERT_FALSE(cnx_path_exists(&path));
}

// NOLINTNEXTLINE
TEST(CnxPath, create_and_remove_directory) {
	CnxScopedString path = cnx_string_from("CnxPathTest");

	TEST_ASSERT_TRUE(cnx_path_is_valid(&path));

	TEST_ASSERT_FALSE(cnx_path_exists(&path));

	let_mut res = cnx_path_create_directory(&path);

	TEST_ASSERT_TRUE(cnx_result_is_ok(res));

	TEST_ASSERT_TRUE(cnx_path_exists(&path));

	TEST_ASSERT_FALSE(cnx_path_is_file(&path));
	TEST_ASSERT_TRUE(cnx_path_is_directory(&path));

	TEST_ASSERT_FALSE(cnx_path_has_file_extension(&path, "txt"));
	CnxScopedString name_actual = cnx_path_get_file_name(&path);
	CnxScopedString name = cnx_string_from("CnxPathTest");
	TEST_ASSERT_TRUE(cnx_string_equal(name, &name_actual));

	res = cnx_path_remove_directory(&path);
	TEST_ASSERT_TRUE(cnx_result_is_ok(res));

	TEST_ASSERT_FALSE(cnx_path_exists(&path));
}

// NOLINTNEXTLINE
TEST(CnxPath, create_and_remove_symlink) {
	CnxScopedString path = cnx_string_from("Cnx-Test");

	TEST_ASSERT_TRUE(cnx_path_exists(&path));

	CnxScopedString symlink = cnx_string_from("TestSymlink");
	let_mut res = cnx_path_create_symlink(&symlink, &path);

	TEST_ASSERT_TRUE(cnx_result_is_ok(res));

	TEST_ASSERT_TRUE(cnx_path_exists(&path));

	TEST_ASSERT_TRUE(cnx_path_exists(&symlink));

	TEST_ASSERT_TRUE(cnx_path_is_file(&symlink));
	TEST_ASSERT_FALSE(cnx_path_is_directory(&symlink));

	let_mut maybe_target = cnx_path_get_symlink_target(&symlink);
	TEST_ASSERT_TRUE(cnx_result_is_ok(maybe_target));

	CnxScopedPath target = cnx_result_unwrap(maybe_target);
	CnxScopedString target_name = cnx_path_get_file_name(&target);
	TEST_ASSERT_TRUE(cnx_string_equal(path, &target_name));

	TEST_ASSERT_FALSE(cnx_path_has_file_extension(&symlink, "txt"));
	CnxScopedString name_actual = cnx_path_get_file_name_without_extension(&symlink);
	CnxScopedString name = cnx_string_from("TestSymlink");
	TEST_ASSERT_TRUE(cnx_string_equal(name, &name_actual));

	res = cnx_path_remove_symlink(&symlink);
	TEST_ASSERT_TRUE(cnx_result_is_ok(res));

	TEST_ASSERT_TRUE(cnx_path_exists(&path));
	TEST_ASSERT_FALSE(cnx_path_exists(&symlink));
}

#endif // CNX_PATH_TEST
