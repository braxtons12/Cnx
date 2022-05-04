/// @file File.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief CnxFile provides various functions for working with type safe, uniquely owned files
/// @version 0.2.0
/// @date 2022-05-04
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

/// @ingroup filesystem
/// @{
/// @defgroup cnx_file CnxFile
/// `CnxFile` provides an abstraction for reading and writing to files that manages buffering,
/// string formatting, and automatic closing of the file when it leaves scope.
///
/// Example:
/// @code {.c}
/// #include <Cnx/filesystem/File.h>
///
/// void example(void) {
///     let_mut maybe_file = cnx_file_open("my_file.txt");
///     cnx_assert(cnx_result_is_ok(maybe_file), "Failed to open my_file.txt");
/// 	CnxScopedFile file = cnx_result_unwrap(maybe_file);
/// 	let my_important_number = 42;
/// 	let my_important_float 24.0F;
/// 	cnx_file_println(&file, "number: {}, float: {}", my_important_number, my_important_float);
///
///     let_mut maybe_file2 = cnx_file_open("my_other_file.txt");
///     cnx_assert(cnx_result_is_ok(maybe_file2), "Failed to open my_other_file.txt");
/// 	CnxScopedFile file2 = cnx_result_unwrap(maybe_file2);
/// 	let_mut maybe_read = cnx_file_read_line(&file2);
/// 	if(cnx_result_is_ok(maybe_read)) {
/// 		CnxScopedString str = cnx_result_unwrap(maybe_read);
/// 		// do something with the line read from `my_other_file.txt`
/// 	}
///
/// 	// files automatically closed when they leave scope because they were declared as
/// 	// `CnxScopedFile`
/// }
/// @endcode
/// @}

#ifndef CNX_FILE
#define CNX_FILE

#include <Cnx/Format.h>
#include <Cnx/String.h>
#include <Cnx/filesystem/Path.h>

#if CNX_PLATFORM_COMPILER_GCC
	#define __IGNORE_NONNULL_COMPARE_WARNING_START \
		_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonnull-compare\"")
	#define __IGNORE_NONNULL_COMPARE_WARNING_STOP _Pragma("GCC diagnostic pop")
#else
	#define __IGNORE_NONNULL_COMPARE_WARNING_START
	#define __IGNORE_NONNULL_COMPARE_WARNING_STOP
#endif // CNX_PLATFORM_COMPILER_GCC

/// @brief `CnxDeleter` function for smart pointers managing `FILE*`s
/// @ingroup cnx_file
__attr(always_inline)
	__attr(not_null(1)) static inline void file_deleter(FILE* restrict file,
														__attr(maybe_unused)
															CnxAllocator allocator) {

	// This should never explicitly be passed a nullptr, but it may be if the `CnxFile`
	// managing `file` is declared as `CnxScopedFile` and subsequently moved from, so we still
	// need to check for it here regardless of the `not_null` attribute
	__IGNORE_NONNULL_COMPARE_WARNING_START
	if(file != stdout && file != stderr && file != nullptr) {
		ignore(fclose(file));
	}
	__IGNORE_NONNULL_COMPARE_WARNING_STOP
}

#undef __IGNORE_NONNULL_COMPARE_WARNING_START
#undef __IGNORE_NONNULL_COMPARE_WARNING_STOP

/// @brief Basic buffer type used to provide buffered for a `CnxFile`
/// @ingroup cnx_file
typedef char FileBuffer[];

#define UNIQUE_T	   FILE
#define UNIQUE_DELETER file_deleter
#define UNIQUE_DECL	   TRUE
#include <Cnx/UniquePtr.h>
#undef UNIQUE_T
#undef UNIQUE_DELETER
#undef UNIQUE_DECL

#define UNIQUE_T	FileBuffer
#define UNIQUE_DECL TRUE
#include <Cnx/UniquePtr.h>
#undef UNIQUE_T
#undef UNIQUE_DECL

#define RESULT_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Result.h>
#undef RESULT_INCLUDE_DEFAULT_INSTANTIATIONS

#define RESULT_T	CnxString
#define RESULT_DECL TRUE
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

/// @brief Use in a `CnxFileOptions` to specify the access mode a file should be opened with
/// @ingroup cnx_file
typedef enum CnxFileAccessMode {
	CnxFileRead = 0,
	CnxFileWrite,
	CnxFileReadWrite
} CnxFileAccessMode;

/// @brief Use in a `CnxFileOptions` to specify the behavior a file should be opened with
/// @ingroup cnx_file
typedef enum CnxFileOpenBehavior {
	CnxFileNone = 0,
	CnxFileAppend = 1,
	CnxFileTruncate = 2,
	CnxFileBinary = 4,
	CnxFileFailIfExists = 8
} CnxFileOpenBehavior;

/// @brief Use to specify the access mode and behavior a file should be opened with
/// @ingroup cnx_file
typedef struct CnxFileOptions {
	CnxFileAccessMode mode;
	i32 modifiers;
} CnxFileOptions;

/// @brief Cnx type used to manage reading and writing to/from a file, the buffering associated with
/// the file, and closing it when it's no longer used
///
/// Example:
/// @code {.c}
/// #include <Cnx/filesystem/File.h>
///
/// void example(void) {
///     let_mut maybe_file = cnx_file_open("my_file.txt");
///     cnx_assert(cnx_result_is_ok(maybe_file), "Failed to open my_file.txt");
/// 	CnxScopedFile file = cnx_result_unwrap(maybe_file);
/// 	let my_important_number = 42;
/// 	let my_important_float 24.0F;
/// 	cnx_file_println(&file, "number: {}, float: {}", my_important_number, my_important_float);
///
///     let_mut maybe_file2 = cnx_file_open("my_other_file.txt");
///     cnx_assert(cnx_result_is_ok(maybe_file2), "Failed to open my_other_file.txt");
/// 	CnxScopedFile file2 = cnx_result_unwrap(maybe_file2);
/// 	let_mut maybe_read = cnx_file_read_line(&file2);
/// 	if(cnx_result_is_ok(maybe_read)) {
/// 		CnxScopedString str = cnx_result_unwrap(maybe_read);
/// 		// do something with the line read from `my_other_file.txt`
/// 	}
///
/// 	// files automatically closed when they leave scope because they were declared as
/// 	// `CnxScopedFile`
/// }
/// @endcode
/// @ingroup cnx_file
typedef struct CnxFile {
	CnxPath path;
	CnxUniquePtr(FILE) file;
	CnxUniquePtr(FileBuffer) buffer;
	CnxFileOptions options;
} CnxFile;

/// @brief Declare a `CnxFile` with this to ensure that it's automatically closed when it leaves
/// its containing scope.
///
/// Example:
/// @code {.c}
/// #include <Cnx/filesystem/File.h>
///
/// void example(void) {
///     let_mut maybe_file = cnx_file_open("my_file.txt");
///     cnx_assert(cnx_result_is_ok(maybe_file), "Failed to open my_file.txt");
/// 	CnxScopedFile file = cnx_result_unwrap(maybe_file);
/// 	let my_important_number = 42;
/// 	let my_important_float 24.0F;
/// 	cnx_file_println(&file, "number: {}, float: {}", my_important_number, my_important_float);
///
///     let_mut maybe_file2 = cnx_file_open("my_other_file.txt");
///     cnx_assert(cnx_result_is_ok(maybe_file2), "Failed to open my_other_file.txt");
/// 	CnxScopedFile file2 = cnx_result_unwrap(maybe_file2);
/// 	let_mut maybe_read = cnx_file_read_line(&file2);
/// 	if(cnx_result_is_ok(maybe_read)) {
/// 		CnxScopedString str = cnx_result_unwrap(maybe_read);
/// 		// do something with the line read from `my_other_file.txt`
/// 	}
///
/// 	// files automatically closed when they leave scope because they were declared as
/// 	// `CnxScopedFile`
/// }
/// @endcode
/// @ingroup cnx_file
#define CnxScopedFile scoped(cnx_file_free)

/// @brief The default options used to open a `CnxFile`
///
/// The default behavior for opening a file with Cnx is to open in read/write mode, truncate
/// contents on opening, and create a new file if one didn't already exist (equivalent to "w+")
/// @ingroup cnx_file
#define CNX_FILE_DEFAULT_OPTIONS \
	((CnxFileOptions){.mode = CnxFileReadWrite, .modifiers = CnxFileTruncate})

#define RESULT_T	CnxFile
#define RESULT_DECL TRUE
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

/// @def CNX_FILE_DEFAULT_BUFFER_SIZE
/// @brief The default buffer size used for newly opened `CnxFile`s.
///
/// This is the default buffer size used for newly opened `CnxFile`s. It can be overridden by
/// explicitly passing `buffer_size` to `cnx_file_open`.
///
/// On Windows or MacOS, this is 8K. On other platforms this will be equivalent to that platform's
/// BUFSIZ
/// @ingroup cnx_file
#if CNX_PLATFORM_WINDOWS || CNX_PLATFORM_APPLE
	#define CNX_FILE_DEFAULT_BUFFER_SIZE (static_cast(usize)(8192))
#else
	#define CNX_FILE_DEFAULT_BUFFER_SIZE (static_cast(usize)(BUFSIZ))
#endif // CNX_PLATFORM_WINDOWS

/// @brief Opens the file at the given `path`.
///
/// Attempts to open the file at the given `path`. `path` __must__ be a valild path for a file.
/// By default, this uses `CNX_FILE_DEFAULT_OPTIONS` for `options`, the `CnxFileOptions` indicating
/// file opening permissions and behavior, and `CNX_FILE_DEFAULT_BUFFER_SIZE` for `buffer_size`,
/// the buffer size to allocate and use for buffering in the file. Both of these can be passed
/// explicit values to override this behavior, however.
///
/// `options` __must__ be a valid combination of `CnxFileAccessMode` and `CnxFileOpenBehavior`. For
/// example, `CnxFileRead` combined with `CnxFileTruncate` would be an invalid pairing, and would
/// result in this returning an error. Valid combinations are those equivalent to a valid `mode`
/// for `fopen`.
///
/// @param ...
/// 	- `path` - The path to the file to open. This can be a pointer to any string or string-like
/// 	type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
/// 	- `options` - The `CnxFileOptions` specifying opening permissions and behavior. This is
/// 	`CNX_FILE_DEFAULT_OPTIONS` by default.
/// 	- `buffer_size` - The buffer size to allocate and use for buffering with the file. This is
/// 	`CNX_FILE_DEFAULT_BUFFER_SIZE` by default.
///
/// @return `CnxResult(CnxFile)` - a `CnxFile` at `path` if successful, otherwise an error.
/// @ingroup cnx_file
#define cnx_file_open(...) CONCAT2_DEFERRED(__cnx_file_open_, PP_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

// TODO(braxtons12): Move allocator after path so we can make this variadic, making `options` and
// `buffer_size` optional
#define cnx_file_open_with_allocator(path, options, buffer_size, allocator) \
	_Generic((path), 							   												   \
			const CnxString* 			: cnx_file_open_with_allocator_string( 					   \
											static_cast(const CnxString*)(path), 				   \
											options, 											   \
											buffer_size, 										   \
											allocator), 		  		   						   \
			CnxString* 					: cnx_file_open_with_allocator_string( 					   \
											static_cast(const CnxString*)(path), 		  		   \
											options, 											   \
											buffer_size, 										   \
											allocator), 		  		   						   \
			const CnxStringView* 		: cnx_file_open_with_allocator_stringview( 				   \
											static_cast(const CnxStringView*)(path), 	  		   \
											options, 											   \
											buffer_size, 										   \
											allocator), 		  		   						   \
			CnxStringView* 				: cnx_file_open_with_allocator_stringview( 				   \
											static_cast(const CnxStringView*)(path), 	  		   \
											options, 											   \
											buffer_size, 										   \
											allocator), 		  		   						   \
			const_cstring 				: cnx_file_open_with_allocator_cstring( 				   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path)), 			   \
											options, 											   \
											buffer_size, 										   \
											allocator), 		  		   						   \
			cstring 					: cnx_file_open_with_allocator_cstring( 				   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path)), 			   \
											options, 											   \
											buffer_size, 										   \
											allocator), 		  		   						   \
			const char[sizeof(path)] 	: cnx_file_open_with_allocator_cstring( 	/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path), 							/** NOLINT **/ \
											options, 											   \
											buffer_size, 										   \
											allocator), 		  		   						   \
			char[sizeof(path)] 			: cnx_file_open_with_allocator_cstring( 	/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path), 							/** NOLINT **/ \
											options, 											   \
											buffer_size,\
											allocator))

#define cnx_file_print(file, format_string, ...) \
	__cnx_file_print(file,                       \
					 format_string,              \
					 DEFAULT_ALLOCATOR,          \
					 PP_NUM_ARGS(__VA_ARGS__) __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
#define cnx_file_println(file, format_string, ...) \
	__cnx_file_println(file,                       \
					   format_string,              \
					   DEFAULT_ALLOCATOR,          \
					   PP_NUM_ARGS(__VA_ARGS__)    \
						   __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
#define cnx_file_print_with_allocator(file, allocator, format_string, ...) \
	__cnx_file_print(file,                                                 \
					 format_string,                                        \
					 allocator,                                            \
					 PP_NUM_ARGS(__VA_ARGS__) __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
#define cnx_file_println_with_allocator(file, allocator, format_string, ...) \
	__cnx_file_println(file,                                                 \
					   format_string,                                        \
					   allocator,                                            \
					   PP_NUM_ARGS(__VA_ARGS__)                              \
						   __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))

#define __DISABLE_IF_NULL(file) \
	cnx_disable_if(!(file), "Can't perform a file operation with a nullptr")

__attr(not_null(1, 2)) CnxResult(i32)
	cnx_file_write_bytes(CnxFile* restrict file, const u8* restrict bytes, usize num_bytes)
		__DISABLE_IF_NULL(file);

__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxString)
	cnx_file_read(CnxFile* restrict file, usize num_chars) __DISABLE_IF_NULL(file);
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxString)
	cnx_file_read_with_allocator(CnxFile* restrict file, usize num_chars, CnxAllocator allocator)
		__DISABLE_IF_NULL(file);
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxString)
	cnx_file_read_line(CnxFile* restrict file) __DISABLE_IF_NULL(file);
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxString)
	cnx_file_read_line_with_allocator(CnxFile* restrict file, CnxAllocator allocator)
		__DISABLE_IF_NULL(file);
__attr(nodiscard) __attr(not_null(1, 2)) CnxResult(usize)
	cnx_file_read_bytes(CnxFile* restrict file, u8* restrict bytes, usize max_num_bytes)
		__DISABLE_IF_NULL(file);

__attr(not_null(1)) CnxResult cnx_file_flush(CnxFile* restrict file) __DISABLE_IF_NULL(file);

typedef enum CnxFileSeekOrigin {
	CnxFileSeekBegin = SEEK_SET,
	CnxFileSeekCurrent = SEEK_CUR,
	CnxFileSeekEnd = SEEK_END
} CnxFileSeekOrigin;

__attr(not_null(1)) CnxResult
	cnx_file_seek(CnxFile* restrict file, i64 offset, CnxFileSeekOrigin origin)
		__DISABLE_IF_NULL(file);
__attr(nodiscard) __attr(not_null(1)) CnxResult(i64) cnx_file_tell(CnxFile* restrict file)
	__DISABLE_IF_NULL(file);

#define cnx_file_is_symlink(file) cnx_path_is_symlink(&((file)->path))
#define cnx_file_has_extension(file, extension) \
	cnx_path_has_file_extension(&((file)->path), extension)
#define cnx_file_get_extension(file) cnx_path_get_file_extension(&((file)->path))
#define cnx_file_get_name(file)		 cnx_path_get_file_name(&((file)->path))
#define cnx_file_get_name_without_extension(file) \
	cnx_path_get_file_name_without_extension(&((file)->path))
#define cnx_file_get_parent_directory(file) cnx_path_get_parent_directory(&((file)->path))

__attr(not_null(1)) void cnx_file_close(CnxFile* restrict file) __DISABLE_IF_NULL(file);

__attr(not_null(1)) void cnx_file_free(void* file) __DISABLE_IF_NULL(file);

__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxFile)
	cnx_file_open_string(const CnxPath* restrict path, CnxFileOptions options, usize buffer_size)
		__DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxFile)
	cnx_file_open_stringview(const CnxStringView* restrict path,
							 CnxFileOptions options,
							 usize buffer_size) __DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxFile)
	cnx_file_open_cstring(restrict const_cstring path,
						  usize path_length,
						  CnxFileOptions options,
						  usize buffer_size) __DISABLE_IF_NULL(path);

IGNORE_RESERVED_IDENTIFIER_WARNING_START
#define __cnx_file_open(path, options, buffer_size) \
	_Generic((path), 							   												   \
			const CnxString* 			: cnx_file_open_string( 						  	   	   \
											static_cast(const CnxString*)(path), 				   \
											options, 											   \
											buffer_size), 		  		   						   \
			CnxString* 					: cnx_file_open_string( 						  	   	   \
											static_cast(const CnxString*)(path), 		  		   \
											options, 											   \
											buffer_size), 		  		   						   \
			const CnxStringView* 		: cnx_file_open_stringview( 					  	   	   \
											static_cast(const CnxStringView*)(path), 	  		   \
											options, 											   \
											buffer_size), 		  		   						   \
			CnxStringView* 				: cnx_file_open_stringview( 					  	   	   \
											static_cast(const CnxStringView*)(path), 	  		   \
											options, 											   \
											buffer_size), 		  		   						   \
			const_cstring 				: cnx_file_open_cstring( 						  	   	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path)), 			   \
											options, 											   \
											buffer_size), 		  		   						   \
			cstring 					: cnx_file_open_cstring( 						  	   	   \
											static_cast(const_cstring)(path), 			  		   \
											strlen(static_cast(const_cstring)(path)), 			   \
											options, 											   \
											buffer_size), 		  		   						   \
			const char[sizeof(path)] 	: cnx_file_open_cstring( 					/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path), 							/** NOLINT **/ \
											options, 											   \
											buffer_size), 		  		   						   \
			char[sizeof(path)] 			: cnx_file_open_cstring( 					/** NOLINT **/ \
											static_cast(const_cstring)(path), 			   		   \
											sizeof(path), 							/** NOLINT **/ \
											options, 											   \
											buffer_size))

#define __cnx_file_open_3(...) __cnx_file_open(__VA_ARGS__)
#define __cnx_file_open_2(...) __cnx_file_open(__VA_ARGS__, CNX_FILE_DEFAULT_BUFFER_SIZE)
#define __cnx_file_open_1(...) \
	__cnx_file_open(__VA_ARGS__, CNX_FILE_DEFAULT_OPTIONS, CNX_FILE_DEFAULT_BUFFER_SIZE)

__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxFile)
	cnx_file_open_with_allocator_string(const CnxPath* restrict path,
										CnxFileOptions options,
										usize buffer_size,
										CnxAllocator allocator) __DISABLE_IF_NULL(path);
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxFile)
	cnx_file_open_with_allocator_stringview(const CnxStringView* restrict path,
											CnxFileOptions options,
											usize buffer_size,
											CnxAllocator allocator) __DISABLE_IF_NULL(path);

__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxFile)
	cnx_file_open_with_allocator_cstring(restrict const_cstring path,
										 usize path_length,
										 CnxFileOptions options,
										 usize buffer_size,
										 CnxAllocator allocator) __DISABLE_IF_NULL(path);

__attr(not_null(1, 2)) CnxResult(i32) __cnx_file_print(CnxFile* file,
													   restrict const_cstring format_string,
													   CnxAllocator allocator,
													   usize num_args,
													   ...) __DISABLE_IF_NULL(file)
	__DISABLE_IF_NULL(format_string);
__attr(not_null(1, 2)) CnxResult(i32) __cnx_file_println(CnxFile* file,
														 restrict const_cstring format_string,
														 CnxAllocator allocator,
														 usize num_args,
														 ...) __DISABLE_IF_NULL(file)
	__DISABLE_IF_NULL(format_string);

IGNORE_RESERVED_IDENTIFIER_WARNING_STOP

#undef __DISABLE_IF_NULL
#endif // CNX_FILE
