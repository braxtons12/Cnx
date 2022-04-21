
/// @file Path.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Path provides various functions for working with filesystem paths
/// @version 0.2.0
/// @date 2022-04-20
///
/// MIT License
/// @copyright Copyright (c) 2022 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#ifndef CNX_PATH
#define CNX_PATH

#include <Cnx/Def.h>
#include <Cnx/Platform.h>
#include <Cnx/String.h>
#include <stdio.h>

#define RESULT_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Result.h>
#undef RESULT_INCLUDE_DEFAULT_INSTANTIATIONS

typedef CnxString CnxPath;

#define RESULT_T	CnxPath
#define RESULT_DECL TRUE
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

#define __DISABLE_IF_NULL(path) \
	cnx_disable_if(!(path), "Can't perform a path operation on a nullptr")

[[nodiscard]] [[not_null(1)]] bool
cnx_path_is_valid(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] CnxPath
cnx_path_new(const CnxString* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] CnxPath cnx_user_home_directory(void);
[[nodiscard]] CnxPath cnx_user_application_data_directory(void);
[[nodiscard]] CnxPath cnx_user_documents_directory(void);
[[nodiscard]] CnxPath cnx_common_application_data_directory(void);
[[nodiscard]] CnxPath cnx_common_documents_directory(void);
[[nodiscard]] CnxPath cnx_temp_directory(void);
[[nodiscard]] CnxPath cnx_current_executable_file(void);
[[nodiscard]] CnxPath cnx_current_application_file(void);
[[nodiscard]] CnxPath cnx_system_applications_directory(void);
[[nodiscard]] CnxPath cnx_current_working_directory(void);
[[nodiscard]] char cnx_path_separator_char(void);
[[nodiscard]] [[not_null(1)]] bool
cnx_path_is_absolute(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] bool
cnx_path_exists(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] bool
cnx_path_is_file(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] bool
cnx_path_is_directory(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] bool
cnx_path_is_fs_root(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] bool
cnx_path_is_symlink(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] CnxResult(CnxPath)
	cnx_path_get_symlink_target(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1, 2)]] bool
cnx_path_has_file_extension(const CnxPath* restrict path, const CnxString* restrict extension)
	__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(extension);
[[nodiscard]] [[not_null(1)]] CnxString
cnx_path_get_file_extension(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] CnxString
cnx_path_get_file_name(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] CnxString
cnx_path_get_file_name_without_extension(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] CnxResult(CnxPath)
	cnx_path_get_parent_directory(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[not_null(1, 2)]] CnxResult
cnx_path_append(CnxPath* restrict path, const CnxString* restrict entry_name)
	__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(entry_name);

[[nodiscard]] [[not_null(1, 2)]] CnxResult
cnx_path_create_symlink(const CnxPath* restrict link_to_create,
						const CnxPath* restrict link_target,
						bool overwrite_existing) __DISABLE_IF_NULL(link_to_create)
	__DISABLE_IF_NULL(link_target);

#define path_scoped cnx_string_scoped

#define ___cnx_path_create_symlink_3(...) cnx_path_create_symlink(__VA_ARGS__)
#define ___cnx_path_create_symlink_2(...) cnx_path_create_symlink(__VA_ARGS__, false)
#define cnx_path_create_symlink(...) \
	CONCAT2_DEFERRED(___cnx_path_create_symlink_, PP_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define cnx_path_create_symlink_overwriting(link_name, target_name) \
	cnx_path_create_symlink(link_name, target_name, false)

#if CNX_PLATFORM_WINDOWS
	#define CNX_PATHS_CASE_SENSITIVE FALSE
#else
	#define CNX_PATHS_CASE_SENSITIVE TRUE
#endif // CNX_PLATFORM_WINDOWS

#if CNX_PLATFORM_WINDOWS

[[nodiscard]] CnxPath cnx_system_applications_directory_x86(void);

#endif // CNX_PLATFORM_WINDOWS

#endif // CNX_PATH
