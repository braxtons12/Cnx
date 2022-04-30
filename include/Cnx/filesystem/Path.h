/// @file Path.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Path provides various functions for working with filesystem paths
/// @version 0.2.0
/// @date 2022-04-30
///
/// MIT License
/// @copyright Copyright (c) 2022 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.

/// @ingroup filesystem
/// @{
/// @defgroup cnx_path CnxPath
/// `CnxPath` provides utilities for working with filesystem paths and perfoming
/// basic file system operations, such as checking the current status (or lack
/// thereof) of a path on the filesystem, querying attributes of an existing
/// path on the file system, creating and removing files, directories, and
/// symbolic links, and retrieving semantically correct paths for certain
/// special filesystem paths, such as the user home directory.
///
/// Example:
/// @code {.c}
/// #include <Cnx/filesystem/Path.h>
///
/// void example(void) {
/// 	let path = cnx_string_from("/home/my_user/test.txt");
/// 	let create_res = cnx_path_create_file(&path);
/// 	cnx_assert(cnx_result_is_ok(create_res), "Failed to create test path");
///
/// 	cnx_assert(cnx_path_is_valid(&path),
/// 			   "Path is not a valid filesystem path on this
/// platform"); 	cnx_assert(cnx_path_exists(&path), "Path does not exist!");
/// 	cnx_assert(cnx_path_is_file(&path), "Path is not a file!");
/// 	cnx_assert(!cnx_path_is_directory(&path), "Path is a directory !?");
/// 	cnx_assert(!cnx_path_is_symlink(&path), "Path is a symlink !?");
/// 	cnx_assert(cnx_path_is_absolute(&path), "Path is not absolute!");
///
/// 	let name = cnx_string_from("test.txt");
/// 	let name_without_extension = cnx_string_from("test");
/// 	let extension = cnx_string_from("test");
///
/// 	let name_actual = cnx_path_get_file_name(&path);
/// 	cnx_assert(cnx_string_equal(name_actual, &name), "path name is not
/// test.txt!");
///
/// 	let name_without_extension_actual =
/// cnx_path_get_file_name_without_extension(&path);
/// 	cnx_assert(cnx_string_equal(name_without_extension_actual,
/// &name_without_extension), 			   "path name without extension is not test!");
///
/// 	cnx_assert(cnx_path_has_file_extension(&path, &extension),
/// 			   "path name does not have .txt file extension");
///
/// 	let parent = cnx_string_from("/home/my_user");
///
/// 	let parent_actual = cnx_path_get_parent_directory(&path);
/// 	cnx_assert(cnx_string_equal(parent_actual, &parent), "Path's parent is
/// not /home/my_user!");
///
/// 	let remove_res = cnx_path_remove_file(&path);
/// 	cnx_assert(cnx_result_is_ok(remove_res), "Failed to remove test file!");
/// }
/// @endcode
/// @}

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
cnx_path_is_valid_string(const CnxString* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] bool
cnx_path_is_valid_stringview(const CnxStringView* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] bool
cnx_path_is_valid_cstring(restrict const_cstring path, usize path_length) __DISABLE_IF_NULL(path);

// clang-format off

/// @brief Checks if the given string would be a valid path on the host
/// platform's filesystem
///
/// Checks if the given path is syntactically valid on the host platform's
/// filesystem. Does not check if the path exists or is semantically logical.
///
/// @param path - The path to check for validity
///
/// @return whether the path is valid
/// @ingroup cnx_path
#define cnx_path_is_valid(path) _Generic((path), 								   				   \
			const CnxString* 			: cnx_path_is_valid_string( 						  	   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_is_valid_string( 						  	   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_is_valid_stringview( 					  	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_is_valid_stringview( 					  	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_is_valid_cstring( 						  	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_is_valid_cstring( 						  	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_is_valid_cstring( 				/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_is_valid_cstring( 				/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

// clang-format on

[[nodiscard]] [[not_null(1)]] CnxPath
cnx_path_new_string(const CnxString* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] CnxPath
cnx_path_new_stringview(const CnxStringView* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] CnxPath
cnx_path_new_cstring(restrict const_cstring path, usize path_length) __DISABLE_IF_NULL(path);

// clang-format off

/// @brief Creates a new path from the given string, potentially modified so as
/// to be valid on the host platform.
///
/// If the given string would be a valid path on the host platform's filesystem,
/// this simply returns a clone of the string. Otherwise, this will return a
/// modified version of the given string, converted to be a valid filesystem
/// path.
///
/// For example, if `path` is "/home\\user_name//file.txt" on *NIX, path
/// separators will be converted to all be *NIX style separators and redundant
/// separators will be removed. As a result, the returned path would be
/// "/home/user_name/file.txt"
///
/// Coincidentally, this means that all paths created through this function can
/// use a consistent path separator for all platforms, and separators will
/// automatically be converted to the correct one for the platform at runtime.
/// IE: all paths can be declared with *NIX style separators, but will be
/// correctly converted to '\\' on Windows
///
/// @param path - The string to create a path from
///
/// @return `path` converted to a valid filesystem path
/// @ingroup cnx_path
#define cnx_path_new(path) _Generic((path), 								   					   \
			const CnxString* 			: cnx_path_new_string( 						  		   	   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_new_string( 						  		   	   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_new_stringview( 					  		   	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_new_stringview( 					  		   	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_new_cstring( 						  		   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_new_cstring( 						  		   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_new_cstring( 					/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_new_cstring( 					/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

// clang-format on

/// @brief Returns the path to the home directory of the user running this
/// Program
///
/// @return The current user's home directory
/// @ingroup cnx_path
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
cnx_path_has_file_extension_string(const CnxPath* restrict path,
								   const CnxString* restrict extension) __DISABLE_IF_NULL(path)
	__DISABLE_IF_NULL(extension);
[[nodiscard]] [[not_null(1, 2)]] bool
cnx_path_has_file_extension_stringview(const CnxPath* restrict path,
									   const CnxStringView* restrict extension)
	__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(extension);
[[nodiscard]] [[not_null(1, 2)]] bool
cnx_path_has_file_extension_cstring(const CnxPath* restrict path,
									restrict const_cstring extension,
									usize extension_length) __DISABLE_IF_NULL(path)
	__DISABLE_IF_NULL(extension);

// clang-format off

#define cnx_path_has_file_extension(path, extension) _Generic((extension), 						   \
	const CnxString* 				: cnx_path_has_file_extension_string((path), 				   \
										static_cast(const CnxString*)(extension)), 		  	   	   \
	CnxString* 						: cnx_path_has_file_extension_string((path), 				   \
										static_cast(const CnxString*)(extension)), 		  	   	   \
	const CnxStringView* 			: cnx_path_has_file_extension_stringview((path), 			   \
										static_cast(const CnxStringView*)(extension)), 	  	   	   \
	CnxStringView* 					: cnx_path_has_file_extension_stringview((path), 			   \
										static_cast(const CnxStringView*)(extension)), 	  	   	   \
	const_cstring 					: cnx_path_has_file_extension_cstring((path), 				   \
										static_cast(const_cstring)(extension), 			  	   	   \
										strlen(static_cast(const_cstring)(extension))), 		   \
	cstring 						: cnx_path_has_file_extension_cstring((path), 				   \
										static_cast(const_cstring)(extension), 			  	   	   \
										strlen(static_cast(const_cstring)(extension))), 		   \
	const char[sizeof(extension)] 	: cnx_path_has_file_extension_cstring((path), 	/** NOLINT **/ \
										static_cast(const_cstring)(extension), 			   	   	   \
										sizeof(extension)), 						/** NOLINT **/ \
	char[sizeof(extension)] 		: cnx_path_has_file_extension_cstring((path), 	/** NOLINT **/ \
										static_cast(const_cstring)(extension), 			   	   	   \
										sizeof(extension)) 							/** NOLINT **/ )

// clang-format on

[[nodiscard]] [[not_null(1)]] CnxString
cnx_path_get_file_extension(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] CnxString
cnx_path_get_file_name(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] CnxString
cnx_path_get_file_name_without_extension(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
[[nodiscard]] [[not_null(1)]] CnxResult(CnxPath)
	cnx_path_get_parent_directory(const CnxPath* restrict path) __DISABLE_IF_NULL(path);

[[not_null(1, 2)]] CnxResult
cnx_path_append_string(CnxPath* restrict path, const CnxString* restrict entry_name)
	__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(entry_name);
[[not_null(1, 2)]] CnxResult cnx_path_append_cstring(CnxPath* restrict path,
													 restrict const_cstring entry_name,
													 usize entry_name_length)
	__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(entry_name);
[[not_null(1, 2)]] CnxResult
cnx_path_append_stringview(CnxPath* restrict path, const CnxStringView* restrict entry_name)
	__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(entry_name);

// clang-format off

#define cnx_path_append(path, entry_name) _Generic((entry_name), 								   \
	const CnxString* 				: cnx_path_append_string((path), 						  	   \
										static_cast(const CnxString*)(entry_name)), 		  	   \
	CnxString* 						: cnx_path_append_string((path), 						  	   \
										static_cast(const CnxString*)(entry_name)), 		  	   \
	const CnxStringView* 			: cnx_path_append_stringview((path), 					  	   \
										static_cast(const CnxStringView*)(entry_name)), 	  	   \
	CnxStringView* 					: cnx_path_append_stringview((path), 					  	   \
										static_cast(const CnxStringView*)(entry_name)), 	  	   \
	const_cstring 					: cnx_path_append_cstring((path), 						  	   \
										static_cast(const_cstring)(entry_name), 			  	   \
										strlen(static_cast(const_cstring)(entry_name))), 		   \
	cstring 						: cnx_path_append_cstring((path), 						  	   \
										static_cast(const_cstring)(entry_name), 			  	   \
										strlen(static_cast(const_cstring)(entry_name))), 		   \
	const char[sizeof(entry_name)] 	: cnx_path_append_cstring((path), 				/** NOLINT **/ \
										static_cast(const_cstring)(entry_name), 			   	   \
										sizeof(entry_name)), 						/** NOLINT **/ \
	char[sizeof(entry_name)] 		: cnx_path_append_cstring((path), 				/** NOLINT **/ \
										static_cast(const_cstring)(entry_name), 			   	   \
										sizeof(entry_name)) 						/** NOLINT **/ )

// clang-format on

IGNORE_RESERVED_IDENTIFIER_WARNING_START
[[nodiscard]] [[not_null(1)]] CnxResult
cnx_path_create_file(const CnxPath* restrict file_path, bool overwrite_existing)
	__DISABLE_IF_NULL(file_path);
[[nodiscard]] [[not_null(1)]] CnxResult
cnx_path_create_directory(const CnxPath* restrict dir_path, bool overwrite_existing)
	__DISABLE_IF_NULL(dir_path);

#define __cnx_path_create_file_2(...) cnx_path_create_file(__VA_ARGS__)
#define __cnx_path_create_file_1(...) cnx_path_create_file(__VA_ARGS__, false)
#define cnx_path_create_file(...)                                       \
	CONCAT2_DEFERRED(__cnx_path_create_file_, PP_NUM_ARGS(__VA_ARGS__)) \
	(__VA_ARGS__)
#define cnx_path_create_file_overwriting(file_path) cnx_path_create_file(file_path, true)

#define __cnx_path_create_directory_2(...) cnx_path_create_directory(__VA_ARGS__)
#define __cnx_path_create_directory_1(...) cnx_path_create_directory(__VA_ARGS__, false)
#define cnx_path_create_directory(...)                                       \
	CONCAT2_DEFERRED(__cnx_path_create_directory_, PP_NUM_ARGS(__VA_ARGS__)) \
	(__VA_ARGS__)
#define cnx_path_create_directory_overwriting(dir_path) cnx_path_create_directory(dir_path, true)
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

[[nodiscard]] [[not_null(1)]] CnxResult
cnx_path_remove_file(const CnxPath* restrict file_path) __DISABLE_IF_NULL(file_path);
[[nodiscard]] [[not_null(1)]] CnxResult
cnx_path_remove_directory(const CnxPath* restrict dir_path, bool recursive)
	__DISABLE_IF_NULL(dir_path);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __cnx_path_remove_directory_2(...) cnx_path_remove_directory(__VA_ARGS__)
#define __cnx_path_remove_directory_1(...) cnx_path_remove_directory(__VA_ARGS__, false)
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
#define cnx_path_remove_directory(...)                                       \
	CONCAT2_DEFERRED(__cnx_path_remove_directory_, PP_NUM_ARGS(__VA_ARGS__)) \
	(__VA_ARGS__)
#define cnx_path_remove_directory_recursive(dir_path) cnx_path_remove_directory(dir_path, true)

#define CnxScopedPath CnxScopedString

IGNORE_RESERVED_IDENTIFIER_WARNING_START

[[nodiscard]] [[not_null(1, 2)]] CnxResult
cnx_path_create_symlink(const CnxPath* restrict link_to_create,
						const CnxPath* restrict link_target,
						bool overwrite_existing) __DISABLE_IF_NULL(link_to_create)
	__DISABLE_IF_NULL(link_target);

[[nodiscard]] [[not_null(1)]] CnxResult cnx_path_remove_symlink(const CnxPath* restrict link_path);

#define __cnx_path_create_symlink_3(...) cnx_path_create_symlink(__VA_ARGS__)
#define __cnx_path_create_symlink_2(...) cnx_path_create_symlink(__VA_ARGS__, false)
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP
#define cnx_path_create_symlink(...)                                       \
	CONCAT2_DEFERRED(__cnx_path_create_symlink_, PP_NUM_ARGS(__VA_ARGS__)) \
	(__VA_ARGS__)
#define cnx_path_create_symlink_overwriting(link_name, target_name) \
	cnx_path_create_symlink(link_name, target_name, true)

#if CNX_PLATFORM_WINDOWS
	#define CNX_PATHS_CASE_SENSITIVE FALSE
#else
	#define CNX_PATHS_CASE_SENSITIVE TRUE
#endif // CNX_PLATFORM_WINDOWS

#if CNX_PLATFORM_WINDOWS

[[nodiscard]] CnxPath cnx_system_applications_directory_x86(void);

#endif // CNX_PLATFORM_WINDOWS

#undef __DISABLE_IF_NULL
#endif // CNX_PATH
