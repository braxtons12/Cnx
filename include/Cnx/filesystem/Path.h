/// @file Path.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Path provides various functions for working with filesystem paths
/// @version 0.2.0
/// @date 2022-05-04
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
/// 	CnxScopedPath path = cnx_string_from("/home/my_user/test.txt");
/// 	let create_res = cnx_path_create_file(&path);
/// 	cnx_assert(cnx_result_is_ok(create_res), "Failed to create test path");
///
/// 	cnx_assert(cnx_path_is_valid(&path),
/// 			   "Path is not a valid filesystem path on this platform");
/// 	cnx_assert(cnx_path_exists(&path), "Path does not exist!");
/// 	cnx_assert(cnx_path_is_file(&path), "Path is not a file!");
/// 	cnx_assert(!cnx_path_is_directory(&path), "Path is a directory !?");
/// 	cnx_assert(!cnx_path_is_symlink(&path), "Path is a symlink !?");
/// 	cnx_assert(cnx_path_is_absolute(&path), "Path is not absolute!");
///
/// 	CnxScopedString name = cnx_string_from("test.txt");
/// 	CnxScopedString name_without_extension = cnx_string_from("test");
/// 	CnxScopedString extension = cnx_string_from("txt");
///
/// 	CnxScopedString name_actual = cnx_path_get_file_name(&path);
/// 	cnx_assert(cnx_string_equal(name_actual, &name), "path name is not test.txt!");
///
/// 	CnxScopedString name_without_extension_actual
/// 		= cnx_path_get_file_name_without_extension(&path);
/// 	cnx_assert(cnx_string_equal(name_without_extension_actual, &name_without_extension),
/// 			   "path name without extension is not test!");
///
/// 	cnx_assert(cnx_path_has_file_extension(&path, &extension),
/// 			   "path name does not have .txt file extension");
///
/// 	CnxScopedString parent = cnx_string_from("/home/my_user");
///
/// 	let maybe_parent = cnx_path_get_parent_directory(&path);
/// 	cnx_assert(cnx_option_is_some(maybe_parent), "Path doesn't have a parent!");
///
/// 	CnxScopedPath parent = cnx_option_unwrap(maybe_parent);
/// 	cnx_assert(cnx_string_equal(parent_actual, &parent), "Path's parent is not /home/my_user!");
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

#define RESULT_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Result.h>
#undef RESULT_INCLUDE_DEFAULT_INSTANTIATIONS

/// @brief `CnxPath` is the system-agnostic path type for performing filesystem queries and
/// modifications.
///
/// Example:
/// @code {.c}
/// void my_example(void) {
/// 	let_mut my_docs_dir = cnx_path_user_dcouments_directory();
/// 	cnx_path_append(&my_docs_dir, "my_file.txt");
/// 	// create the file "$HOME/Documents/my_file.text" if it doesn't already exist
/// 	let_mut result = cnx_path_create_file(&path);
/// 	// failed to create the file
/// 	if(cnx_result_is_err(result)) {
/// 		// do something
/// 	}
///
/// 	cnx_path_free(&my_docs_dir);
/// }
/// @endcode
/// @ingroup cnx_path
typedef CnxString CnxPath;
/// @brief Declare a `CnxPath` as a `CnxScopedPath` for the resources associated with the path
/// to be automatically `free`d when the path leaves the scope.
///
/// Example:
/// @code {.c}
/// void my_example(void) {
/// 	CnxScopedPath my_docs_dir = cnx_path_user_dcouments_directory();
/// 	cnx_path_append(&my_docs_dir, "my_file.txt");
/// 	// create the file "$HOME/Documents/my_file.text" if it doesn't already exist
/// 	let_mut result = cnx_path_create_file(&path);
/// 	// failed to create the file
/// 	if(cnx_result_is_err(result)) {
/// 		// do something
/// 	}
///
/// 	// no need to free `my_docs_dir`, it's automatically freed when it exits scope
/// 	// since we declared it as `CnxScopedPath`
/// }
/// @endcode
/// @ingroup cnx_path
#define CnxScopedPath CnxScopedString

/// @brief Frees the resources associated with the given path
///
/// @param path - The `CnxPath` to free
/// @ingroup cnx_path
#define cnx_path_free(path_ptr) cnx_string_free(*(path_ptr))

#define RESULT_T	CnxPath
#define RESULT_DECL TRUE
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

#define OPTION_T	CnxPath
#define OPTION_DECL TRUE
#include <Cnx/Option.h>
#undef OPTION_T
#undef OPTION_DECL

/// @def CNX_PATHS_CASE_SENSITIVE
/// @brief `TRUE` if filesystem paths are case-sensitive on the host platform, `FALSE` otherwise
///
/// For example, on *NIX systems this will usualy be `TRUE`, but on Windows this is `FALSE`
/// @ingroup cnx_path
#if CNX_PLATFORM_WINDOWS
	#define CNX_PATHS_CASE_SENSITIVE FALSE
#else
	#define CNX_PATHS_CASE_SENSITIVE TRUE
#endif // CNX_PLATFORM_WINDOWS

#define __DISABLE_IF_NULL(path) \
	cnx_disable_if(!(path), "Can't perform a path operation on a nullptr")

/// @brief Checks if the given string would be a valid path on the host
/// platform's filesystem
///
/// Checks if the given path is syntactically valid on the host platform's
/// filesystem. Does not check if the path exists or is semantically logical.
///
/// Example:
/// @code {.c}
/// // fails on all platforms because of mixed path separator use
/// cnx_assert(cnx_path_is_valid("/home\\my_user"), "Path /home\\my_user was invalid");
/// // fails on Windows because it uses *NIX style separators and fs root
/// cnx_assert(cnx_path_is_valid("/home/my_user"), "Path /home/my_user was invalid");
/// // fails on *NIX because it uses Windows style separators and fs root
/// cnx_assert(cnx_path_is_valid("C:\\Users\\my_user"), "Path /home/my_user was invalid");
/// @endcode
///
/// @param path - The path to check for validity. This can be a pointer to any string or string-like
/// type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `bool` - whether the path is valid
/// @ingroup cnx_path
#define cnx_path_is_valid(path) \
	_Generic((path), 								   				   							   \
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

/// @brief Creates a new path from the given string, canonicalized so as to be valid on the host
/// platform.
///
/// If the given string would be a valid path on the host platform's filesystem,
/// this simply returns a clone of the string. Otherwise, this will return a
/// modified, canonicalized clone of the given string so that it would be a valid filesystem path.
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
/// Example:
/// @code {.c}
/// CnxScopedPath home = cnx_path_user_home_directory();
/// // returns a validated and canonicalized form for the path
/// CnxScopedPath file_path = cnx_path_new("my_directory/my_file");
/// cnx_path_append(&home, &file_path);
/// @endcode
///
/// @param path - The string to create a path from. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxPath` - `path` converted to a valid filesystem path
/// @ingroup cnx_path
#define cnx_path_new(path) \
	_Generic((path), 								   					   						   \
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

/// @brief Returns the path to the home directory of the user running this
/// Program
///
/// @return Path to the current user's home directory
/// @ingroup cnx_path
__attr(nodiscard) CnxPath cnx_path_user_home_directory(void);
/// @brief Returns the path to the application data directory for the current user
///
/// For example, this would return $HOME/AppData on Windows, or $HOME/Library/Preferences on Mac
///
/// @return Path to the current user's application data directory
/// @ingroup cnx_path
__attr(nodiscard) CnxPath cnx_path_user_application_data_directory(void);
/// @brief Returns the path to the documents directory for the current user
///
/// For example, this would return $HOME/Documents or "$HOME/My Documents" on Windows, or
/// $HOME/Documents on *NIX
///
/// @return Path to the current user's documents directory
/// @ingroup cnx_path
__attr(nodiscard) CnxPath cnx_path_user_documents_directory(void);
/// @brief Returns the path to the application data directory for the system
///
/// For example, this would return C:/Users/Public/AppData on Windows, or /Library on Mac
///
/// @return Path to the system application data directory
/// @ingroup cnx_path
__attr(nodiscard) CnxPath cnx_path_common_application_data_directory(void);
/// @brief Returns the path to the shared documents directory
///
/// For example, this would return C:/Users/Public/Documents on Windows, or /Users/Shared on Mac
///
/// @return Path to the shared documents directory
/// @ingroup cnx_path
__attr(nodiscard) CnxPath cnx_path_common_documents_directory(void);
/// @brief Returns a path to a directory suitable for storing temporary files
///
/// For example, this would return $HOME/AppData/Local/Temp on Windows, or /tmp on *NIX
///
/// @return Path to a temporary files directory
/// @ingroup cnx_path
__attr(nodiscard) CnxPath cnx_path_temp_directory(void);
/// @brief Returns the path to the currently running executable
///
/// On Mac if the executable is an app, this will return the path to the executable binary
/// contained in the package. If you need the path of the package folder, use
/// `cnx_current_application_file` instead. On other platforms `cnx_current_executable_file` and
/// `cnx_current_application_file` are interchangeable.
///
/// @return Path to the current executable
/// @ingroup cnx_path
__attr(nodiscard) CnxPath cnx_path_current_executable_file(void);
/// @brief Returns the path to the currently running application
///
/// On Mac if the executable is an app, this will return the path to the application package
/// folder. If you need the path of the executable binary, use `cnx_current_executable_file`
/// instead. On other platforms `cnx_current_executable_file` and `cnx_current_application_file`
/// are interchangeable.
///
/// @return Path to the current application
/// @ingroup cnx_path
__attr(nodiscard) CnxPath cnx_path_current_application_file(void);
/// @brief Returns the path to the system applications directory
///
/// On Windows this returns the 64-bit applications directory ("Program Files").
/// For the legacy/32-bit applications directory ("Program Files (x86)") use
/// `cnx_system_applications_directory_x86` instead.
///
/// @return Path to the system applications directory
/// @ingroup cnx_path
__attr(nodiscard) CnxPath cnx_path_system_applications_directory(void);

/// @fn
/// @brief Returns the path to the Legacy/32-bit system applications directory
///
/// This function is only available on Windows. It returns the path to the Legacy/32-bit
/// applications directory ("Program Files (x86)"). To get the 64-bit applications directory,
/// use `cnx_system_applications_directory` instead.
///
/// @return Path to the 32-bit system applications directory
/// @ingroup cnx_path
#if CNX_PLATFORM_WINDOWS

__attr(nodiscard) CnxPath cnx_path_system_applications_directory_x86(void);

#endif // CNX_PLATFORM_WINDOWS

/// @brief Returns the path to the current working directory
///
/// @return Path to the current working directory
/// @ingroup cnx_path
__attr(nodiscard) CnxPath cnx_path_current_working_directory(void);
/// @brief Returns the path separator character of the host platform
///
/// @return The path separator character for the platform
/// @ingroup cnx_path
__attr(nodiscard) char cnx_path_separator_char(void);

/// @brief Checks if the given path is an absolute path
///
/// @param path - The path to check. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `bool` - whether `path` is an absolute path
/// @ingroup cnx_path
#define cnx_path_is_absolute(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_is_absolute_string( 						  	   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_is_absolute_string( 						  	   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_is_absolute_stringview( 					  	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_is_absolute_stringview( 					  	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_is_absolute_cstring( 						   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_is_absolute_cstring( 						   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_is_absolute_cstring( 			/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_is_absolute_cstring( 			/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Checks if the given path is a relative path
///
/// @param path - The path to check. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `bool` - whether `path` is a relative path
/// @ingroup cnx_path
#define cnx_path_is_relative(path) (!(cnx_path_is_absolute(path)))

/// @brief Returns the absolute path of `path`
///
/// Gets the absolute path of `path`. For example if `path` is "my_dir/my_file.txt", this returns
/// "$CWD/my_dir/my_file.txt" where "$CWD" is the absolute path of the current working directory.
/// If `path` is already an absolute path, this returns a clone of `path`
///
/// @param path - The path to get the absolute path of. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxPath` - `path` as an absolute path
/// @ingroup cnx_path
#define cnx_path_get_absolute_path(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_get_absolute_path_string( 						  	   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_get_absolute_path_string( 						  	   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_get_absolute_path_stringview( 					  	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_get_absolute_path_stringview( 					  	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_get_absolute_path_cstring( 						   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_get_absolute_path_cstring( 						   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_get_absolute_path_cstring( 			/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_get_absolute_path_cstring( 			/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Checks if the given path exists
///
/// @param path - The path to check. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `bool` - whether `path` exists
/// @ingroup cnx_path
#define cnx_path_exists(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_exists_string( 						  		   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_exists_string( 						  		   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_exists_stringview( 					  		   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_exists_stringview( 					  		   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_exists_cstring( 						  	   	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_exists_cstring( 						  	   	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_exists_cstring( 					/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_exists_cstring( 					/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Checks if the given path is a path to a file
///
/// @param path - The path to check. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `bool` - whether `path` is a file
/// @ingroup cnx_path
#define cnx_path_is_file(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_is_file_string( 						  		   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_is_file_string( 						  		   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_is_file_stringview( 					  		   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_is_file_stringview( 					  		   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_is_file_cstring( 						  	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_is_file_cstring( 						  	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_is_file_cstring( 				/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_is_file_cstring( 				/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Checks if the given path is a path to a directory
///
/// @param path - The path to check. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `bool` - whether `path` is a directory
/// @ingroup cnx_path
#define cnx_path_is_directory(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_is_directory_string( 						   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_is_directory_string( 						   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_is_directory_stringview( 					   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_is_directory_stringview( 					   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_is_directory_cstring( 						   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_is_directory_cstring( 						   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_is_directory_cstring( 			/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_is_directory_cstring( 			/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Checks if the given path is a path to a filesystem root (ie "C:\\" or "/")
///
/// @param path - The path to check. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `bool` - whether `path` is a filesystem root
/// @ingroup cnx_path
#define cnx_path_is_fs_root(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_is_fs_root_string( 						  	   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_is_fs_root_string( 						  	   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_is_fs_root_stringview( 					  	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_is_fs_root_stringview( 					  	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_is_fs_root_cstring( 						  	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_is_fs_root_cstring( 						  	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_is_fs_root_cstring( 				/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_is_fs_root_cstring( 				/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Checks if the given path is a path to a symbolic link
///
/// @param path - The path to check. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `bool` - whether `path` is a symlink
/// @ingroup cnx_path
#define cnx_path_is_symlink(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_is_symlink_string( 						  	   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_is_symlink_string( 						  	   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_is_symlink_stringview( 					  	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_is_symlink_stringview( 					  	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_is_symlink_cstring( 						  	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_is_symlink_cstring( 						  	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_is_symlink_cstring( 				/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_is_symlink_cstring( 				/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Attempts to get the path of the target of the symbolic link `path`
///
/// @param path - The symlink to get the target of. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxResult(CnxPath`) - If `path` is a symlink and actively links to a target,
/// the target of `path`. Otherwise, an error.
/// @ingroup cnx_path
#define cnx_path_get_symlink_target(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_get_symlink_target_string( 					   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_get_symlink_target_string( 					   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_get_symlink_target_stringview( 				   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_get_symlink_target_stringview( 				   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_get_symlink_target_cstring( 					   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_get_symlink_target_cstring( 					   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_get_symlink_target_cstring( 		/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_get_symlink_target_cstring( 		/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Checks if the given path is a file with the given file extension
///
/// @param path - The path to check. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
/// @param extension - The extension to check for. This should be without the "." prefix. I.E. for a
/// ".txt" extension, this should be "txt". This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `bool` - whether `path` is a symlink
/// @ingroup cnx_path
#define cnx_path_has_file_extension(path, extension) \
	_Generic((path), 						   \
	const CnxString* 				: __cnx_path_has_file_extension( 							   \
										static_cast(const CnxString*)(path), 				   	   \
										extension), 		  	   	   							   \
	CnxString* 						: __cnx_path_has_file_extension( 							   \
										static_cast(const CnxString*)(path), 				   	   \
										extension), 		  	   	   							   \
	const CnxStringView* 			: ({ 														   \
											CnxScopedString UNIQUE_VAR(_path) 					   \
												= cnx_string_from( 								   \
													static_cast(const CnxStringView*)(path)); 	   \
											__cnx_path_has_file_extension( 			   			   \
												&UNIQUE_VAR(_path), 			   				   \
												extension); 	   								   \
										}), 													   \
	CnxStringView* 					: ({ 														   \
											CnxScopedString UNIQUE_VAR(_path) 					   \
												= cnx_string_from( 								   \
													static_cast(const CnxStringView*)(path)); 	   \
											__cnx_path_has_file_extension( 			   			   \
												&UNIQUE_VAR(_path), 			   				   \
												extension); 	   								   \
										}), 													   \
	const_cstring 					: ({ 														   \
											CnxScopedString UNIQUE_VAR(_path) 					   \
												= cnx_string_from( 								   \
													static_cast(const_cstring)(path)); 	   		   \
											__cnx_path_has_file_extension( 			   			   \
												&UNIQUE_VAR(_path), 			   				   \
												extension); 	   								   \
										}), 													   \
	cstring 						: ({ 														   \
											CnxScopedString UNIQUE_VAR(_path) 					   \
												= cnx_string_from( 								   \
													static_cast(const_cstring)(path)); 	   		   \
											__cnx_path_has_file_extension( 			   			   \
												&UNIQUE_VAR(_path), 			   				   \
												extension); 	   								   \
										}), 													   \
	const char[sizeof(path)] 		: ({ 											/** NOLINT **/ \
											CnxScopedString UNIQUE_VAR(_path) 					   \
												= cnx_string_from( 								   \
													static_cast(const_cstring)(path)); 	   		   \
											__cnx_path_has_file_extension( 			   			   \
												&UNIQUE_VAR(_path), 			   				   \
												extension); 	   								   \
										}), 													   \
	char[sizeof(path)] 				: ({ 											/** NOLINT **/ \
											CnxScopedString UNIQUE_VAR(_path) 					   \
												= cnx_string_from( 								   \
													static_cast(const_cstring)(path)); 	   		   \
											__cnx_path_has_file_extension( 			   			   \
												&UNIQUE_VAR(_path), 			   				   \
												extension); 	   								   \
										}))

/// @brief Attempts to get the file extension of the file `path`
///
/// @param path - Path to the file to get the extension of. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxOption(CnxString)` - If `path` is a file with a file extension, the file extension
/// of `path`. Otherwise, `None(CnxString)`.
/// @ingroup cnx_path
#define cnx_path_get_file_extension(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_get_file_extension_string( 					   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_get_file_extension_string( 					   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_get_file_extension_stringview( 				   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_get_file_extension_stringview( 				   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_get_file_extension_cstring( 					   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_get_file_extension_cstring( 					   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_get_file_extension_cstring( 		/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_get_file_extension_cstring( 		/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Gets the file name of the given path, including file extension
///
/// @param path - The path to get the file name of. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxString` - The name of the file indicated by path
/// @note If `path` is a directory, then this will return the directory name
/// @ingroup cnx_path
#define cnx_path_get_file_name(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_get_file_name_string( 					   	   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_get_file_name_string( 					   	   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_get_file_name_stringview( 				   	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_get_file_name_stringview( 				   	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_get_file_name_cstring( 					   	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_get_file_name_cstring( 					   	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_get_file_name_cstring( 			/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_get_file_name_cstring( 			/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Gets the file name of the given path, without the file extension
///
/// @param path - The path to get the file name of. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxString` - The name of the file indicated by path, without the file extension if
/// it has one.
/// @note If `path` is a directory, then this will return the directory name
/// @ingroup cnx_path
#define cnx_path_get_file_name_without_extension(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_get_file_name_without_extension_string( 		   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_get_file_name_without_extension_string( 		   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_get_file_name_without_extension_stringview( 	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_get_file_name_without_extension_stringview( 	   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_get_file_name_without_extension_cstring( 	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_get_file_name_without_extension_cstring( 	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: 											/** NOLINT **/ \
										cnx_path_get_file_name_without_extension_cstring( 		   \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: 											/** NOLINT **/ \
										cnx_path_get_file_name_without_extension_cstring( 		   \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Returns the parent directory of the given `path`
///
/// Returns the parent directory of the given path. For example, for a path "/usr/local/bin" on
/// *NIX, the parent directory would be "/usr/local", or for a path "Documents/my_file.txt", the
/// parent directory would be "$CWD/Documents".
///
/// If `path` is a file system root, `path` is returned.
///
/// The returned path is __always__ an absolute path.
///
/// @param path - The path to get the parent directory of. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxPath` - The parent directory of `path`
/// @ingroup cnx_path
#define cnx_path_get_parent_directory(path) \
	_Generic((path), 								   					   						   \
			const CnxString* 			: cnx_path_get_parent_directory_string( 				   \
											static_cast(const CnxString*)(path)), 		  		   \
			CnxString* 					: cnx_path_get_parent_directory_string( 				   \
											static_cast(const CnxString*)(path)), 		  		   \
			const CnxStringView* 		: cnx_path_get_parent_directory_stringview( 			   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			CnxStringView* 				: cnx_path_get_parent_directory_stringview( 			   \
											static_cast(const CnxStringView*)(path)), 	  		   \
			const_cstring 				: cnx_path_get_parent_directory_cstring( 				   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			cstring 					: cnx_path_get_parent_directory_cstring( 				   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path))), 			   \
			const char[sizeof(path)] 	: cnx_path_get_parent_directory_cstring( 	/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)), 							/** NOLINT **/ \
			char[sizeof(path)] 			: cnx_path_get_parent_directory_cstring( 	/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path)) 							/** NOLINT **/ )

/// @brief Appends the relative path `entry_name` to `path`
///
/// Attempts to append the relative path `entry_name` to the path `path`. `entry_name` __must__ be a
/// relative path: if `entry_name` is absolute, this will return an error.
///
/// @param path - The path to append to. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
/// @param entry_name - The path to be appended. This must be a relative path. If this is an
/// absolute path, an error will be returned. This can be a pointer to any string or string-like
/// type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxResult` - `Ok()` if appending was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_append(path, entry_name) \
	_Generic((entry_name), 								   \
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

/// @brief Creates a new file with the path and name indicated by `path`
///
/// Attempts to create a new file with the path and name indicated by `path`. By default, this will
/// fail if `path` already exists. If `overwrite_existing` is given as `true`, however, all
/// filesystem contents originally existing at `path` will attempt to be deleted, and a new file
/// will be created if deletion was successful. `overwrite_existing` may also be given as `false` to
/// explicitly signal that failure should occur if `path` exists.
///
/// @param ...
/// 	- `path` - The path of the file to create. This can be a pointer to any string or
/// 	string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`,
/// 	or a string literal)
/// 	- `overwrite_existing` - Whether existing filesystem contents should be deleted and
/// 	overwritten if `path` already exists. This is `false` by default.
///
/// @return `CnxResult` - `Ok()` if creating the file was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_create_file(...)                                       \
	CONCAT2_DEFERRED(__cnx_path_create_file_, PP_NUM_ARGS(__VA_ARGS__)) \
	(__VA_ARGS__)
/// @brief Creates a new file with the path and name indicated by `path`
///
/// Attempts to create a new file with the path and name indicated by `path`. Unlike
/// `cnx_path_create_file`, this will always overwrite existing filesystem contents: if `path`
/// already exists, all contents originally existing at `path` will attempt to be deleted, and a new
/// file will be created if deletion was successful.
///
/// @param file_path - The path of the file to create. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxResult` - `Ok()` if creating the file was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_create_file_overwriting(file_path) cnx_path_create_file(file_path, true)

/// @brief Creates a new directory with the path and name indicated by `path`
///
/// Attempts to create a new directory with the path and name indicated by `path`. By default, this
/// will fail if `path` already exists. If `overwrite_existing` is given as `true`, however, all
/// filesystem contents originally existing at `path` will attempt to be deleted, and a new
/// directory will be created if deletion was successful. `overwrite_existing` may also be given as
/// `false` to explicitly signal that failure should occur if `path` exists.
///
/// @param ...
/// 	- `path` - The path of the directory to create. This can be a pointer to any string or
/// 	string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`,
/// 	or a string literal)
/// 	- `overwrite_existing` - Whether existing filesystem contents should be deleted and
/// 	overwritten if `path` already exists. This is `false` by default.
///
/// @return `CnxResult` - `Ok()` if creating the directory was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_create_directory(...)                                       \
	CONCAT2_DEFERRED(__cnx_path_create_directory_, PP_NUM_ARGS(__VA_ARGS__)) \
	(__VA_ARGS__)
/// @brief Creates a new directory with the path and name indicated by `path`
///
/// Attempts to create a new directory with the path and name indicated by `path`. Unlike
/// `cnx_path_create_directory`, this will always overwrite existing filesystem contents: if `path`
/// already exists, all contents originally existing at `path` will attempt to be deleted, and a new
/// directory will be created if deletion was successful.
///
/// @param dir_path - The path of the directory to create. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxResult` - `Ok()` if creating the directory was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_create_directory_overwriting(dir_path) cnx_path_create_directory(dir_path, true)

/// @brief Removes the file located at `path`
///
/// Attempts to remove the file located at `path`. If `path` is not a file (aka, it is a directory),
/// or if `path` does not exist, this will return an error. This may also return an error if an
/// error occurs when removing the file.
///
/// @param file_path - The path of the file to remove. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxResult` - `Ok()` if removing the file was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_remove_file(file_path) \
	_Generic((file_path), 			   															   \
	const CnxString* 				: cnx_path_remove_file_string( 								   \
										static_cast(const CnxString*)(file_path)), 				   \
	CnxString* 						: cnx_path_remove_file_string( 								   \
										static_cast(const CnxString*)(file_path)), 				   \
	const CnxStringView* 			: cnx_path_remove_file_stringview( 							   \
										static_cast(const CnxStringView*)(file_path)), 			   \
	CnxStringView* 					: cnx_path_remove_file_stringview( 							   \
										static_cast(const CnxStringView*)(file_path)), 			   \
	const_cstring 					: cnx_path_remove_file_cstring( 							   \
										static_cast(const_cstring)(file_path), 					   \
										strlen(static_cast(const_cstring)(file_path))), 		   \
	cstring 						: cnx_path_remove_file_cstring( 							   \
										static_cast(const_cstring)(file_path), 					   \
										strlen(static_cast(const_cstring)(file_path))), 		   \
	const char[sizeof(file_path)] 	: cnx_path_remove_file_cstring( 				/** NOLINT **/ \
										static_cast(const_cstring)(file_path), 		/** NOLINT **/ \
										sizeof(file_path)), 			   	   		/** NOLINT **/ \
	char[sizeof(file_path)] 		: cnx_path_remove_file_cstring( 				/** NOLINT **/ \
										static_cast(const_cstring)(file_path), 		/** NOLINT **/ \
										sizeof(file_path)) 			   	   			/** NOLINT **/ )

/// @brief Removes the directory located at `path`
///
/// Attempts to remove the directory located at `path`. By default, this will fail if `path` is a
/// non-empty directory (aka if `path` has contents). If `recursive` is passed as `true`, however,
/// this will recursively delete all filesystem contents contained in `path`. `recursive` can also
/// be passed as `false` to explicitly signal that `path` should __NOT__ be removed recursively.
///
/// If `path` is not a directory (aka, it is a file), or if `path` does not exist, this will return
/// an error. This may also return an error if an error occurs when removing the directory.
///
/// @param ...
/// 	- `path` - The path of the directory to remove. This can be a pointer to any string or
/// 	string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`,
/// 	or a string literal)
/// 	- `recursive` - Whether `path` should be deleted recursively, meaning any filesytem contents
/// 	contained in `path` would be deleted if `path` is not empty. This is `false` by default.
///
/// @return `CnxResult` - `Ok()` if removing the directory was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_remove_directory(...)                                       \
	CONCAT2_DEFERRED(__cnx_path_remove_directory_, PP_NUM_ARGS(__VA_ARGS__)) \
	(__VA_ARGS__)
/// @brief Removes the directory located at `path`
///
/// Attempts to remove the directory located at `path`. Unlike `cnx_path_remove_directory`, this
/// will always delete `path` recursively, and any filesystem contents contained in `path` will be
/// deleted if `path` is not empty.
///
/// If `path` is not a directory (aka, it is a file), or if `path` does not exist, this will return
/// an error. This may also return an error if an error occurs when removing the directory.
///
/// @param dir_path - The path of the directory to remove. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxResult` - `Ok()` if removing the directory was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_remove_directory_recursive(dir_path) cnx_path_remove_directory(dir_path, true)

/// @brief Creates a new symbolic link with the name and path indicated by `link_name` that points
/// to the file or directory at `link_target`
///
/// Attempts to create a new symlink with the path and name indicated by `link_name` that points to
/// the file or directory at `link_target`. By default, this will fail if `link_name` already
/// exists. If `overwrite_existing` is given as `true`, however, all filesystem contents originally
/// existing at `link_name` will attempt to be deleted, and a new symlink to `link_target` will be
/// created if deletion was successful. `overwrite_existing` may also be given as `false` to
/// explicitly signal that failure should occur if `path` exists.
///
/// @param ...
/// 	- `link_name` - The path of the symlink to create. This can be a pointer to any string or
/// 	string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`,
/// 	or a string literal)
/// 	- `link_target` - The path `link_name` should link to. This can be a pointer to any string
/// or 	string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, 	or a string
/// literal)
/// 	- `overwrite_existing` - Whether existing filesystem contents should be deleted and
/// 	overwritten if `path` already exists. This is `false` by default.
///
/// @return `CnxResult` - `Ok()` if creating the symlink was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_create_symlink(...) \
	CONCAT2_DEFERRED(__cnx_path_create_symlink_, PP_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
/// @brief Creates a new symbolic link with the name and path indicated by `link_name` that points
/// to the file or directory at `link_target`
///
/// Attempts to create a new symlink with the path and name indicated by `link_name` that points to
/// the file or directory at `link_target`. Unlike `cnx_path_create_symlink`, this will always
/// delete any filesystem contents at `link_name` if `link_name` already exists.
///
/// @param link_name - The path of the symlink to create. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
/// @param link_target - The path `link_name` should link to. This can be a pointer to any string
/// or string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string
/// literal)
///
/// @return `CnxResult` - `Ok()` if creating the symlink was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_create_symlink_overwriting(link_name, link_target) \
	cnx_path_create_symlink(link_name, link_target, true)

/// @brief Removes the symbolic link `link_path`
///
/// Attempts to remove the symlink `link_path`. This will not remove the filesystem contents at
/// `link_path`'s target. Any contents pointed to by `link_path` will be preserved, only the link
/// will be deleted. If `link_path` is not a symlink, this will return an error.
///
/// @param link_path - The path of the symlink to remove. This can be a pointer to any string or
/// string-like type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `CnxResult` - `Ok()` if removing the symlink was successful, an error otherwise
/// @ingroup cnx_path
#define cnx_path_remove_symlink(link_path) \
	_Generic((link_path), 			   															   \
	const CnxString* 				: cnx_path_remove_symlink_string( 							   \
										static_cast(const CnxString*)(link_path)), 				   \
	CnxString* 						: cnx_path_remove_symlink_string( 							   \
										static_cast(const CnxString*)(link_path)), 				   \
	const CnxStringView* 			: cnx_path_remove_symlink_stringview( 						   \
										static_cast(const CnxStringView*)(link_path)), 			   \
	CnxStringView* 					: cnx_path_remove_symlink_stringview( 						   \
										static_cast(const CnxStringView*)(link_path)), 			   \
	const_cstring 					: cnx_path_remove_symlink_cstring( 							   \
										static_cast(const_cstring)(link_path), 					   \
										strlen(static_cast(const_cstring)(link_path))), 		   \
	cstring 						: cnx_path_remove_symlink_cstring( 							   \
										static_cast(const_cstring)(link_path), 					   \
										strlen(static_cast(const_cstring)(link_path))), 		   \
	const char[sizeof(link_path)] 	: cnx_path_remove_symlink_cstring( 				/** NOLINT **/ \
										static_cast(const_cstring)(link_path), 		/** NOLINT **/ \
										sizeof(link_path)), 			   	   		/** NOLINT **/ \
	char[sizeof(link_path)] 		: cnx_path_remove_symlink_cstring( 				/** NOLINT **/ \
										static_cast(const_cstring)(link_path), 		/** NOLINT **/ \
										sizeof(link_path)) 			   	   			/** NOLINT **/ )

__attr(nodiscard) __attr(not_null(1)) bool cnx_path_is_valid_string(const CnxString* restrict path)
	__DISABLE_IF_NULL(path);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_path_is_valid_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) bool cnx_path_is_valid_cstring(restrict const_cstring path,
																	 usize path_length)
	__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxPath cnx_path_new_string(const CnxString* restrict path)
	__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxPath
	cnx_path_new_stringview(const CnxStringView* restrict path) __DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxPath
	cnx_path_new_cstring(restrict const_cstring path, usize path_length) __DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) bool cnx_path_is_absolute_string(const CnxPath* restrict path)
	__DISABLE_IF_NULL(path);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_path_is_absolute_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) bool cnx_path_is_absolute_cstring(restrict const_cstring path,
																		usize path_length)
	__DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) CnxPath
	cnx_path_get_absolute_path_string(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxPath
	cnx_path_get_absolute_path_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxPath
	cnx_path_get_absolute_path_cstring(restrict const_cstring path, usize path_length)
		__DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) bool cnx_path_exists_string(const CnxPath* restrict path)
	__DISABLE_IF_NULL(path);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_path_exists_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_path_exists_cstring(restrict const_cstring path, usize path_length)
		__DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) bool cnx_path_is_file_string(const CnxPath* restrict path)
	__DISABLE_IF_NULL(path);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_path_is_file_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) bool cnx_path_is_file_cstring(restrict const_cstring path,
																	usize path_length)
	__DISABLE_IF_NULL(path);

__attr(nodiscard)
	__attr(not_null(1)) bool cnx_path_is_directory_string(const CnxPath* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_path_is_directory_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_path_is_directory_cstring(restrict const_cstring path,
														   usize path_length)
		__DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) bool cnx_path_is_fs_root_string(const CnxPath* restrict path)
	__DISABLE_IF_NULL(path);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_path_is_fs_root_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) bool cnx_path_is_fs_root_cstring(restrict const_cstring path,
																	   usize path_length)
	__DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) bool cnx_path_is_symlink_string(const CnxPath* restrict path)
	__DISABLE_IF_NULL(path);
__attr(nodiscard)
	__attr(not_null(1)) bool cnx_path_is_symlink_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) bool cnx_path_is_symlink_cstring(restrict const_cstring path,
																	   usize path_length)
	__DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxPath)
	cnx_path_get_symlink_target_string(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxPath)
	cnx_path_get_symlink_target_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxPath)
	cnx_path_get_symlink_target_cstring(restrict const_cstring path, usize path_length)
		__DISABLE_IF_NULL(path);

__attr(nodiscard)
	__attr(not_null(1,
					2)) bool cnx_path_has_file_extension_string(const CnxPath* restrict path,
																const CnxString* restrict extension)
		__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(extension);
__attr(nodiscard) __attr(not_null(1, 2)) bool cnx_path_has_file_extension_stringview(
	const CnxPath* restrict path,
	const CnxStringView* restrict extension) __DISABLE_IF_NULL(path) __DISABLE_IF_NULL(extension);
__attr(nodiscard)
	__attr(not_null(1,
					2)) bool cnx_path_has_file_extension_cstring(const CnxPath* restrict path,
																 restrict const_cstring extension,
																 usize extension_length)
		__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(extension);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __cnx_path_has_file_extension(path, extension) \
	_Generic((extension), 						   \
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
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

__attr(nodiscard) __attr(not_null(1)) CnxOption(CnxString)
	cnx_path_get_file_extension_string(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxOption(CnxString)
	cnx_path_get_file_extension_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxOption(CnxString)
	cnx_path_get_file_extension_cstring(restrict const_cstring path, usize path_length)
		__DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_path_get_file_name_string(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_path_get_file_name_stringview(const CnxStringView* restrict path) __DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_path_get_file_name_cstring(restrict const_cstring path, usize path_length)
		__DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_path_get_file_name_without_extension_string(const CnxPath* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_path_get_file_name_without_extension_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxString
	cnx_path_get_file_name_without_extension_cstring(restrict const_cstring path, usize path_length)
		__DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) CnxPath
	cnx_path_get_parent_directory_string(const CnxPath* restrict path) __DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxPath
	cnx_path_get_parent_directory_stringview(const CnxStringView* restrict path)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxPath
	cnx_path_get_parent_directory_cstring(restrict const_cstring path, usize path_length)
		__DISABLE_IF_NULL(path);

__attr(not_null(1, 2)) CnxResult
	cnx_path_append_string(CnxPath* restrict path, const CnxString* restrict entry_name)
		__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(entry_name);
__attr(not_null(1, 2)) CnxResult cnx_path_append_cstring(CnxPath* restrict path,
														 restrict const_cstring entry_name,
														 usize entry_name_length)
	__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(entry_name);
__attr(not_null(1, 2)) CnxResult
	cnx_path_append_stringview(CnxPath* restrict path, const CnxStringView* restrict entry_name)
		__DISABLE_IF_NULL(path) __DISABLE_IF_NULL(entry_name);

__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_create_file_string(const CnxString* restrict file_path, bool overwrite_existing)
		__DISABLE_IF_NULL(file_path);
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_create_file_stringview(const CnxStringView* restrict file_path,
									bool overwrite_existing) __DISABLE_IF_NULL(file_path);
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_create_file_cstring(restrict const_cstring file_path,
								 usize file_path_length,
								 bool overwrite_existing) __DISABLE_IF_NULL(file_path);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __cnx_path_create_file(file_path, overwrite_existing) \
	_Generic((file_path), 			   \
	const CnxString* 				: cnx_path_create_file_string( 								   \
										static_cast(const CnxString*)(file_path), 				   \
										overwrite_existing), 									   \
	CnxString* 						: cnx_path_create_file_string( 								   \
										static_cast(const CnxString*)(file_path), 				   \
										overwrite_existing), 									   \
	const CnxStringView* 			: cnx_path_create_file_stringview( 							   \
										static_cast(const CnxStringView*)(file_path), 			   \
										overwrite_existing), 									   \
	CnxStringView* 					: cnx_path_create_file_stringview( 							   \
										static_cast(const CnxStringView*)(file_path), 			   \
										overwrite_existing), 									   \
	const_cstring 					: cnx_path_create_file_cstring( 							   \
										static_cast(const_cstring)(file_path), 					   \
										strlen(static_cast(const_cstring)(file_path)), 		   	   \
										overwrite_existing), 									   \
	cstring 						: cnx_path_create_file_cstring( 							   \
										static_cast(const_cstring)(file_path), 					   \
										strlen(static_cast(const_cstring)(file_path)), 		   	   \
										overwrite_existing), 									   \
	const char[sizeof(file_path)] 	: cnx_path_create_file_cstring( 				/** NOLINT **/ \
										static_cast(const_cstring)(file_path), 		/** NOLINT **/ \
										sizeof(file_path), 			   	   			/** NOLINT **/ \
										overwrite_existing), 									   \
	char[sizeof(file_path)] 		: cnx_path_create_file_cstring( 				/** NOLINT **/ \
										static_cast(const_cstring)(file_path), 		/** NOLINT **/ \
										sizeof(file_path), 			   	   			/** NOLINT **/ \
										overwrite_existing))

#define __cnx_path_create_file_2(...) __cnx_path_create_file(__VA_ARGS__)
#define __cnx_path_create_file_1(...) __cnx_path_create_file(__VA_ARGS__, false)
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_create_directory_string(const CnxString* restrict dir_path, bool overwrite_existing)
		__DISABLE_IF_NULL(dir_path);
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_create_directory_stringview(const CnxStringView* restrict dir_path,
										 bool overwrite_existing) __DISABLE_IF_NULL(dir_path);
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_create_directory_cstring(restrict const_cstring dir_path,
									  usize dir_path_length,
									  bool overwrite_existing) __DISABLE_IF_NULL(dir_path);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __cnx_path_create_directory(dir_path, overwrite_existing) \
	_Generic((dir_path), 			   \
	const CnxString* 				: cnx_path_create_directory_string( 						   \
										static_cast(const CnxString*)(dir_path), 				   \
										overwrite_existing), 									   \
	CnxString* 						: cnx_path_create_directory_string( 						   \
										static_cast(const CnxString*)(dir_path), 				   \
										overwrite_existing), 									   \
	const CnxStringView* 			: cnx_path_create_directory_stringview( 					   \
										static_cast(const CnxStringView*)(dir_path), 			   \
										overwrite_existing), 									   \
	CnxStringView* 					: cnx_path_create_directory_stringview( 					   \
										static_cast(const CnxStringView*)(dir_path), 			   \
										overwrite_existing), 									   \
	const_cstring 					: cnx_path_create_directory_cstring( 						   \
										static_cast(const_cstring)(dir_path), 					   \
										strlen(static_cast(const_cstring)(dir_path)), 		   	   \
										overwrite_existing), 									   \
	cstring 						: cnx_path_create_directory_cstring( 						   \
										static_cast(const_cstring)(dir_path), 					   \
										strlen(static_cast(const_cstring)(dir_path)), 		   	   \
										overwrite_existing), 									   \
	const char[sizeof(dir_path)] 	: cnx_path_create_directory_cstring( 			/** NOLINT **/ \
										static_cast(const_cstring)(dir_path), 		/** NOLINT **/ \
										sizeof(dir_path), 			   	   			/** NOLINT **/ \
										overwrite_existing), 									   \
	char[sizeof(dir_path)] 			: cnx_path_create_directory_cstring( 			/** NOLINT **/ \
										static_cast(const_cstring)(dir_path), 		/** NOLINT **/ \
										sizeof(dir_path), 			   	   			/** NOLINT **/ \
										overwrite_existing))

#define __cnx_path_create_directory_2(...) __cnx_path_create_directory(__VA_ARGS__)
#define __cnx_path_create_directory_1(...) __cnx_path_create_directory(__VA_ARGS__, false)
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_remove_file_string(const CnxString* restrict file_path) __DISABLE_IF_NULL(file_path);
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_remove_file_stringview(const CnxStringView* restrict file_path)
		__DISABLE_IF_NULL(file_path);
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_remove_file_cstring(restrict const_cstring file_path, usize file_path_length)
		__DISABLE_IF_NULL(file_path);

__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_remove_directory_string(const CnxPath* restrict dir_path, bool recursive)
		__DISABLE_IF_NULL(dir_path);
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_remove_directory_stringview(const CnxStringView* restrict dir_path, bool recursive)
		__DISABLE_IF_NULL(dir_path);
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_remove_directory_cstring(restrict const_cstring dir_path,
									  usize dir_path_length,
									  bool recursive) __DISABLE_IF_NULL(dir_path);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __cnx_path_remove_directory(dir_path, recursive) \
	_Generic((dir_path), 			   															   \
	const CnxString* 				: cnx_path_remove_directory_string( 						   \
										static_cast(const CnxString*)(dir_path), 				   \
										recursive), 									   		   \
	CnxString* 						: cnx_path_remove_directory_string( 						   \
										static_cast(const CnxString*)(dir_path), 				   \
										recursive), 									   		   \
	const CnxStringView* 			: cnx_path_remove_directory_stringview( 					   \
										static_cast(const CnxStringView*)(dir_path), 			   \
										recursive), 									   		   \
	CnxStringView* 					: cnx_path_remove_directory_stringview( 					   \
										static_cast(const CnxStringView*)(dir_path), 			   \
										recursive), 									   		   \
	const_cstring 					: cnx_path_remove_directory_cstring( 						   \
										static_cast(const_cstring)(dir_path), 					   \
										strlen(static_cast(const_cstring)(dir_path)), 			   \
										recursive), 									   		   \
	cstring 						: cnx_path_remove_directory_cstring( 						   \
										static_cast(const_cstring)(dir_path), 					   \
										strlen(static_cast(const_cstring)(dir_path)), 			   \
										recursive), 									    	   \
	const char[sizeof(dir_path)] 	: cnx_path_remove_directory_cstring( 			/** NOLINT **/ \
										static_cast(const_cstring)(dir_path), 		/** NOLINT **/ \
										sizeof(dir_path), 			   	   			/** NOLINT **/ \
										recursive), 											   \
	char[sizeof(dir_path)] 		: cnx_path_remove_directory_cstring( 				/** NOLINT **/ \
										static_cast(const_cstring)(dir_path), 		/** NOLINT **/ \
										sizeof(dir_path), 			   	   			/** NOLINT **/ \
										recursive))

#define __cnx_path_remove_directory_2(...) __cnx_path_remove_directory(__VA_ARGS__)
#define __cnx_path_remove_directory_1(...) __cnx_path_remove_directory(__VA_ARGS__, false)
IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

__attr(nodiscard) __attr(not_null(1, 2)) CnxResult
	cnx_path_create_symlink_string_string(const CnxPath* restrict link_to_create,
										  const CnxPath* restrict link_target,
										  bool overwrite_existing) __DISABLE_IF_NULL(link_to_create)
		__DISABLE_IF_NULL(link_target);
__attr(nodiscard) __attr(not_null(1, 2)) CnxResult
	cnx_path_create_symlink_string_stringview(const CnxPath* restrict link_to_create,
											  const CnxStringView* restrict link_target,
											  bool overwrite_existing)
		__DISABLE_IF_NULL(link_to_create) __DISABLE_IF_NULL(link_target);
__attr(nodiscard) __attr(not_null(1, 2)) CnxResult
	cnx_path_create_symlink_string_cstring(const CnxPath* restrict link_to_create,
										   restrict const_cstring link_target,
										   usize link_target_length,
										   bool overwrite_existing)
		__DISABLE_IF_NULL(link_to_create) __DISABLE_IF_NULL(link_target);

__attr(nodiscard) __attr(not_null(1, 2)) CnxResult
	cnx_path_create_symlink_stringview_string(const CnxStringView* restrict link_to_create,
											  const CnxPath* restrict link_target,
											  bool overwrite_existing)
		__DISABLE_IF_NULL(link_to_create) __DISABLE_IF_NULL(link_target);
__attr(nodiscard) __attr(not_null(1, 2)) CnxResult
	cnx_path_create_symlink_stringview_stringview(const CnxStringView* restrict link_to_create,
												  const CnxStringView* restrict link_target,
												  bool overwrite_existing)
		__DISABLE_IF_NULL(link_to_create) __DISABLE_IF_NULL(link_target);
__attr(nodiscard) __attr(not_null(1, 2)) CnxResult
	cnx_path_create_symlink_stringview_cstring(const CnxStringView* restrict link_to_create,
											   restrict const_cstring link_target,
											   usize link_target_length,
											   bool overwrite_existing)
		__DISABLE_IF_NULL(link_to_create) __DISABLE_IF_NULL(link_target);

__attr(nodiscard) __attr(not_null(1, 3)) CnxResult
	cnx_path_create_symlink_cstring_string(restrict const_cstring link_to_create,
										   usize link_to_create_length,
										   const CnxPath* restrict link_target,
										   bool overwrite_existing)
		__DISABLE_IF_NULL(link_to_create) __DISABLE_IF_NULL(link_target);
__attr(nodiscard) __attr(not_null(1, 3)) CnxResult
	cnx_path_create_symlink_cstring_stringview(restrict const_cstring link_to_create,
											   usize link_to_create_length,
											   const CnxStringView* restrict link_target,
											   bool overwrite_existing)
		__DISABLE_IF_NULL(link_to_create) __DISABLE_IF_NULL(link_target);
__attr(nodiscard) __attr(not_null(1, 3)) CnxResult
	cnx_path_create_symlink_cstring_cstring(restrict const_cstring link_to_create,
											usize link_to_create_length,
											restrict const_cstring link_target,
											usize link_target_length,
											bool overwrite_existing)
		__DISABLE_IF_NULL(link_to_create) __DISABLE_IF_NULL(link_target);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __cnx_path_create_symlink_3(...) __cnx_path_create_symlink(__VA_ARGS__)
#define __cnx_path_create_symlink_2(...) __cnx_path_create_symlink(__VA_ARGS__, false)

#define __cnx_path_create_symlink_string(to_create, target, overwrite_existing) \
	_Generic((target), 																			   \
	const CnxString* 				: cnx_path_create_symlink_string_string( 				   	   \
										to_create, 				    							   \
										static_cast(const CnxString*)(target), 				   	   \
										overwrite_existing), 									   \
	CnxString* 						: cnx_path_create_symlink_string_string( 				   	   \
										to_create, 												   \
										static_cast(const CnxString*)(target), 				   	   \
										overwrite_existing), 									   \
	const CnxStringView* 			: cnx_path_create_symlink_string_stringview( 			   	   \
										to_create, 												   \
										static_cast(const CnxStringView*)(target), 				   \
										overwrite_existing), 									   \
	CnxStringView* 					: cnx_path_create_symlink_string_stringview( 			   	   \
										to_create, 												   \
										static_cast(const CnxStringView*)(target), 				   \
										overwrite_existing), 									   \
	const_cstring 					: cnx_path_create_symlink_string_cstring( 			   	   	   \
										to_create, 												   \
										static_cast(const_cstring)(target), 					   \
										strlen(static_cast(const_cstring)(target)), 			   \
										overwrite_existing), 									   \
	cstring 						: cnx_path_create_symlink_string_cstring( 			   	   	   \
										to_create, 												   \
										static_cast(const_cstring)(target), 					   \
										strlen(static_cast(const_cstring)(target)), 			   \
										overwrite_existing), 									   \
	const char[sizeof(target)] 		: cnx_path_create_symlink_string_cstring( 		/** NOLINT **/ \
										to_create, 												   \
										static_cast(const_cstring)(target), 		/** NOLINT **/ \
										sizeof(target), 							/** NOLINT **/ \
										overwrite_existing), 									   \
	char[sizeof(target)] 			: cnx_path_create_symlink_string_cstring( 		/** NOLINT **/ \
										to_create, 												   \
										static_cast(const_cstring)(target), 		/** NOLINT **/ \
										sizeof(target), 							/** NOLINT **/ \
										overwrite_existing))

#define __cnx_path_create_symlink_stringview(to_create, target, overwrite_existing) \
	_Generic((target), 																			   \
	const CnxString* 				: cnx_path_create_symlink_stringview_string( 				   \
										to_create, 				    							   \
										static_cast(const CnxString*)(target), 				   	   \
										overwrite_existing), 									   \
	CnxString* 						: cnx_path_create_symlink_stringview_string( 				   \
										to_create, 												   \
										static_cast(const CnxString*)(target), 				   	   \
										overwrite_existing), 									   \
	const CnxStringView* 			: cnx_path_create_symlink_stringview_stringview( 			   \
										to_create, 												   \
										static_cast(const CnxStringView*)(target), 				   \
										overwrite_existing), 									   \
	CnxStringView* 					: cnx_path_create_symlink_stringview_stringview( 			   \
										to_create, 												   \
										static_cast(const CnxStringView*)(target), 				   \
										overwrite_existing), 									   \
	const_cstring 					: cnx_path_create_symlink_stringview_cstring( 			   	   \
										to_create, 												   \
										static_cast(const_cstring)(target), 					   \
										strlen(static_cast(const_cstring)(target)), 			   \
										overwrite_existing), 									   \
	cstring 						: cnx_path_create_symlink_stringview_cstring( 			   	   \
										to_create, 												   \
										static_cast(const_cstring)(target), 					   \
										strlen(static_cast(const_cstring)(target)), 			   \
										overwrite_existing), 									   \
	const char[sizeof(target)] 		: cnx_path_create_symlink_stringview_cstring( 	/** NOLINT **/ \
										to_create, 												   \
										static_cast(const_cstring)(target), 		/** NOLINT **/ \
										sizeof(target), 							/** NOLINT **/ \
										overwrite_existing), 									   \
	char[sizeof(target)] 			: cnx_path_create_symlink_stringview_cstring( 	/** NOLINT **/ \
										to_create, 												   \
										static_cast(const_cstring)(target), 		/** NOLINT **/ \
										sizeof(target), 							/** NOLINT **/ \
										overwrite_existing))

#define __cnx_path_create_symlink_cstring(to_create, to_create_length, target, overwrite_existing) \
	_Generic((target), 																			   \
	const CnxString* 				: cnx_path_create_symlink_cstring_string( 				   	   \
										to_create, 				    							   \
										to_create_length, 										   \
										static_cast(const CnxString*)(target), 				   	   \
										overwrite_existing), 									   \
	CnxString* 						: cnx_path_create_symlink_cstring_string( 				   	   \
										to_create, 												   \
										to_create_length, 										   \
										static_cast(const CnxString*)(target), 				   	   \
										overwrite_existing), 									   \
	const CnxStringView* 			: cnx_path_create_symlink_cstring_stringview( 			   	   \
										to_create, 												   \
										to_create_length, 										   \
										static_cast(const CnxStringView*)(target), 				   \
										overwrite_existing), 									   \
	CnxStringView* 					: cnx_path_create_symlink_cstring_stringview( 			   	   \
										to_create, 												   \
										to_create_length, 										   \
										static_cast(const CnxStringView*)(target), 				   \
										overwrite_existing), 									   \
	const_cstring 					: cnx_path_create_symlink_cstring_cstring( 				   	   \
										to_create, 												   \
										to_create_length, 										   \
										static_cast(const_cstring)(target), 					   \
										strlen(static_cast(const_cstring)(target)), 			   \
										overwrite_existing), 									   \
	cstring 						: cnx_path_create_symlink_cstring_cstring( 				   	   \
										to_create, 												   \
										to_create_length, 										   \
										static_cast(const_cstring)(target), 					   \
										strlen(static_cast(const_cstring)(target)), 			   \
										overwrite_existing), 									   \
	const char[sizeof(target)] 		: cnx_path_create_symlink_cstring_cstring( 		/** NOLINT **/ \
										to_create, 												   \
										to_create_length, 										   \
										static_cast(const_cstring)(target), 		/** NOLINT **/ \
										sizeof(target), 							/** NOLINT **/ \
										overwrite_existing), 									   \
	char[sizeof(target)] 			: cnx_path_create_symlink_cstring_cstring( 		/** NOLINT **/ \
										to_create, 												   \
										to_create_length, 										   \
										static_cast(const_cstring)(target), 		/** NOLINT **/ \
										sizeof(target), 							/** NOLINT **/ \
										overwrite_existing))

#define __cnx_path_create_symlink(to_create, target, overwrite_existing) \
	_Generic((to_create), 																		   \
	const CnxString* 				: __cnx_path_create_symlink_string( 						   \
										static_cast(const CnxString*)(to_create), 				   \
										target, 				   								   \
										overwrite_existing), 									   \
	CnxString* 						: __cnx_path_create_symlink_string( 						   \
										static_cast(const CnxString*)(to_create), 				   \
										target, 												   \
										overwrite_existing), 									   \
	const CnxStringView* 			: __cnx_path_create_symlink_stringview( 					   \
										static_cast(const CnxStringView*)(to_create), 			   \
										target, 												   \
										overwrite_existing), 									   \
	CnxStringView* 					: __cnx_path_create_symlink_stringview( 					   \
										static_cast(const CnxStringView*)(to_create), 			   \
										target, 												   \
										overwrite_existing), 									   \
	const_cstring 					: __cnx_path_create_symlink_cstring( 						   \
										static_cast(const_cstring)(to_create), 					   \
										strlen(static_cast(const_cstring)(to_create)), 			   \
										target, 												   \
										overwrite_existing), 									   \
	cstring 						: __cnx_path_create_symlink_cstring( 						   \
										static_cast(const_cstring)(to_create), 					   \
										strlen(static_cast(const_cstring)(to_create)), 			   \
										target, 												   \
										overwrite_existing), 									   \
	const char[sizeof(to_create)] 	: __cnx_path_create_symlink_cstring( 			/** NOLINT **/ \
										static_cast(const_cstring)(to_create), 		/** NOLINT **/ \
										sizeof(to_create), 			   				/** NOLINT **/ \
										target, 												   \
										overwrite_existing), 									   \
	char[sizeof(to_create)] 		: __cnx_path_create_symlink_cstring( 			/** NOLINT **/ \
										static_cast(const_cstring)(to_create), 		/** NOLINT **/ \
										sizeof(to_create), 			   				/** NOLINT **/ \
										target, 												   \
										overwrite_existing))

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_remove_symlink_string(const CnxPath* restrict link_path);
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_remove_symlink_stringview(const CnxStringView* restrict link_path);
__attr(nodiscard) __attr(not_null(1)) CnxResult
	cnx_path_remove_symlink_cstring(restrict const_cstring link_path, usize link_path_length);

#undef __DISABLE_IF_NULL
#endif // CNX_PATH
