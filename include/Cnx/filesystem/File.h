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
	CnxFileRead = 1,
	CnxFileWrite = 2,
	CnxFileReadWrite = 3
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
	bool dirty;
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

/// @brief Opens the file at the given `path`, using the default allocator to allocate the memory
/// used for buffering.
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

/// @brief Opens the file at the given `path`, using the given allocator to allocate the memory
/// used for buffering.
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
/// 	- `allocator` - The allocator to allocate the memory used for buffering.
/// 	- `options` - The `CnxFileOptions` specifying opening permissions and behavior. This is
/// 	`CNX_FILE_DEFAULT_OPTIONS` by default.
/// 	- `buffer_size` - The buffer size to allocate and use for buffering with the file. This is
/// 	`CNX_FILE_DEFAULT_BUFFER_SIZE` by default.
///
/// @return `CnxResult(CnxFile)` - a `CnxFile` at `path` if successful, otherwise an error.
/// @ingroup cnx_file
#define cnx_file_open_with_allocator(...) \
	CONCAT2_DEFERRED(__cnx_file_open_with_allocator_, PP_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

/// @brief Prints the string resulting from formatting `format_string` and the formatting arguments
/// to the given file.
///
/// Attempts to write the string resulting from formatting to the file. Writing will fail if `file`
/// was not opened with write access permissions or for any reason `fwrite` or similar libc
/// functions may fail
///
/// @param file - The `CnxFile` to print the string to
/// @param format_string - The `cstring` containing the text along with how to format the formatting
/// arguments into the text. Uses the `CnxFormat` formatting syntax
/// @param ... - The formatting arguments to create formatted strings of to insert in
/// `format_string`
///
/// @return `CnxResult(i32)` - The number of characters written on success, otherwise an error
/// @ingroup cnx_file
#define cnx_file_print(file, format_string, ...) \
	__cnx_file_print(file,                       \
					 format_string,              \
					 DEFAULT_ALLOCATOR,          \
					 PP_NUM_ARGS(__VA_ARGS__) __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
/// @brief Prints the string resulting from formatting `format_string` and the formatting arguments
/// to the given file, followed by a newline.
///
/// Attempts to write the string resulting from formatting, followed by a newline, to the file.
/// Writing will fail if `file` was not opened with write access permissions or for any reason
/// `fwrite` or similar libc functions may fail
///
/// @param file - The `CnxFile` to print the string to
/// @param format_string - The `cstring` containing the text along with how to format the formatting
/// arguments into the text. Uses the `CnxFormat` formatting syntax
/// @param ... - The formatting arguments to create formatted strings of to insert in
/// `format_string`
///
/// @return `CnxResult(i32)` - The number of characters written on success, otherwise an error
/// @ingroup cnx_file
#define cnx_file_println(file, format_string, ...) \
	__cnx_file_println(file,                       \
					   format_string,              \
					   DEFAULT_ALLOCATOR,          \
					   PP_NUM_ARGS(__VA_ARGS__)    \
						   __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
/// @brief Prints the string resulting from formatting `format_string` and the formatting arguments
/// to the given file, using the provided allocator for any memory allocations necessary to perform
/// the string formatting.
///
/// Attempts to write the string resulting from formatting to the file. Writing will fail if `file`
/// was not opened with write access permissions or for any reason `fwrite` or similar libc
/// functions may fail
///
/// @param file - The `CnxFile` to print the string to
/// @param allocator - The `CnxAllocator` to use for memory allocations, if necessary
/// @param format_string - The `cstring` containing the text along with how to format the formatting
/// arguments into the text. Uses the `CnxFormat` formatting syntax
/// @param ... - The formatting arguments to create formatted strings of to insert in
/// `format_string`
///
/// @return `CnxResult(i32)` - The number of characters written on success, otherwise an error
/// @ingroup cnx_file
#define cnx_file_print_with_allocator(file, allocator, format_string, ...) \
	__cnx_file_print(file,                                                 \
					 format_string,                                        \
					 allocator,                                            \
					 PP_NUM_ARGS(__VA_ARGS__) __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))
/// @brief Prints the string resulting from formatting `format_string` and the formatting arguments
/// to the given file, followed by a newline. Uses the provided allocator for any memory allocations
/// necessary to perform the string formatting.
///
/// Attempts to write the string resulting from formatting, followed by a newline, to the file.
/// Writing will fail if `file` was not opened with write access permissions or for any reason
/// `fwrite` or similar libc functions may fail
///
/// @param file - The `CnxFile` to print the string to
/// @param allocator - The `CnxAllocator` to use for memory allocations, if necessary
/// @param format_string - The `cstring` containing the text along with how to format the formatting
/// arguments into the text. Uses the `CnxFormat` formatting syntax
/// @param ... - The formatting arguments to create formatted strings of to insert in
/// `format_string`
///
/// @return `CnxResult(i32)` - The number of characters written on success, otherwise an error
/// @ingroup cnx_file
#define cnx_file_println_with_allocator(file, allocator, format_string, ...) \
	__cnx_file_println(file,                                                 \
					   format_string,                                        \
					   allocator,                                            \
					   PP_NUM_ARGS(__VA_ARGS__)                              \
						   __VA_OPT__(, APPLY_TO_LIST(as_format, __VA_ARGS__)))

#define __DISABLE_IF_NULL(file) \
	cnx_disable_if(!(file), "Can't perform a file operation with a nullptr")

/// @brief Writes the given `bytes` to the `file`.
///
/// Attempts to write bytes in the given byte array, `bytes` to the file. Writing will fail if
/// `file` was not opened with write access permissions or for any reason `fwrite` or similar libc
/// functions may fail
///
/// @param file - The `CnxFile` to write to
/// @param bytes - The array of bytes to write
/// @param num_bytes - The number of bytes in `bytes`
///
/// @return `CnxResult(i32)` - The number of bytes written on success, otherwise an error
/// @ingroup cnx_file
__attr(not_null(1, 2)) CnxResult(i32)
	cnx_file_write_bytes(CnxFile* restrict file, const u8* restrict bytes, usize num_bytes)
		__DISABLE_IF_NULL(file);

/// @brief Reads `num_chars` characters from `file` and returns them in a `CnxString`
///
/// Attempts to read `num_chars` characters from `file`, returning the result in a `CnxString`.
/// May read less than `num_chars` characters if `EOF` is reached. Reading will fail if the `file`
/// was not opened with read access permissions or for any reason `fread` or similar libc functions
/// may fail.
///
/// @param file - The `CnxFile` to read from
/// @param num_chars - The number of characters to read
///
/// @return `CnxResult(CnxString)` - The string containing the characters read from the file on
/// success, otherwise an error
/// @ingroup cnx_file
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxString)
	cnx_file_read(CnxFile* restrict file, usize num_chars) __DISABLE_IF_NULL(file);
/// @brief Reads `num_chars` characters from `file` and returns them in a `CnxString` allocated
/// with the given memory allocator.
///
/// Attempts to read `num_chars` characters from `file`, returning the result in a `CnxString`
/// allocated with the given memory allocator. May read less than `num_chars` characters if `EOF`
/// is reached. Reading will fail if the `file` was not opened with read access permissions or for
/// any reason `fread` or similar libc functions may fail.
///
/// @param file - The `CnxFile` to read from
/// @param num_chars - The number of characters to read
/// @param allocator - The memory allocator to allocate the returned string with
///
/// @return `CnxResult(CnxString)` - The string containing the characters read from the file on
/// success, otherwise an error
/// @ingroup cnx_file
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxString)
	cnx_file_read_with_allocator(CnxFile* restrict file, usize num_chars, CnxAllocator allocator)
		__DISABLE_IF_NULL(file);
/// @brief Reads a line of text from `file` and returns it in a `CnxString`
///
/// Attempts to read a line of text from `file`, returning the result in a `CnxString`.
/// May read less than a line if `EOF` is reached before a newline is encountered. Reading will fail
/// if the `file` was not opened with read access permissions or for any reason `fread` or similar
/// libc functions may fail.
///
/// @param file - The `CnxFile` to read from
///
/// @return `CnxResult(CnxString)` - The string containing the line read from the file on
/// success, otherwise an error
/// @ingroup cnx_file
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxString)
	cnx_file_read_line(CnxFile* restrict file) __DISABLE_IF_NULL(file);
/// @brief Reads a line of text from `file` and returns it in a `CnxString`
///
/// Attempts to read a line of text from `file`, returning the result in a `CnxString`.
/// May read less than a line if `EOF` is reached before a newline is encountered. Reading will fail
/// if the `file` was not opened with read access permissions or for any reason `fread` or similar
/// libc functions may fail.
///
/// @param file - The `CnxFile` to read from
///
/// @return `CnxResult(CnxString)` - The string containing the line read from the file on
/// success, otherwise an error
/// @ingroup cnx_file
__attr(nodiscard) __attr(not_null(1)) CnxResult(CnxString)
	cnx_file_read_line_with_allocator(CnxFile* restrict file, CnxAllocator allocator)
		__DISABLE_IF_NULL(file);
/// @brief Reads up to `max_num_bytes` bytes from `file` and writes them to `bytes`, returning the
/// number of bytes read.
///
/// Attempts to read up to `max_num_bytes` bytes from `file`, and writes them to `bytes`.
/// May read less than `max_num_bytes` bytes if `EOF` is reached. Returns the number of bytes read
/// when successful. Reading will fail if the `file` was not opened with read access permissions or
/// for any reason `fread` or similar libc functions may fail.
///
/// @param file - The `CnxFile` to read from
/// @param bytes - The byte array to write the bytes to
/// @param max_num_bytes - The maximum number of bytes able to be written to `bytes`
///
/// @return `CnxResult(usize)` - The number of bytes read from the file and written to `bytes` on
/// success, otherwise an error
/// @ingroup cnx_file
__attr(nodiscard) __attr(not_null(1, 2)) CnxResult(usize)
	cnx_file_read_bytes(CnxFile* restrict file, u8* restrict bytes, usize max_num_bytes)
		__DISABLE_IF_NULL(file);

/// @brief Flushes the given `file`.
///
/// Attempts to flush the given `file`. Flushing may fail for any reason `fflush` from libc may
/// fail.
///
/// @param file - The `CnxFile` to flush
///
/// @return `CnxResult` - `Ok()` on success, otherwise an error
/// @ingroup cnx_file
__attr(not_null(1)) CnxResult cnx_file_flush(CnxFile* restrict file) __DISABLE_IF_NULL(file);

/// @brief Use to identify where a seek should begin from when seeking in a `CnxFile`
/// @ingroup cnx_file
typedef enum CnxFileSeekOrigin {
	/// @brief Indicates a file seek should originate from the beginning of the file
	CnxFileSeekBegin = SEEK_SET,
	/// @brief Indicates a file seek should originate from the current position in the file
	CnxFileSeekCurrent = SEEK_CUR,
	/// @brief Indicates a file seek should originate from the end of the file
	CnxFileSeekEnd = SEEK_END
} CnxFileSeekOrigin;

/// @brief Seeks to the given location in the `file`
///
/// Attempts to seek to the location indicated by `origin` and `offset` in the `file`. May fail for
/// any reason `fseek` from libc may fail.
///
/// @param file - The `CnxFile` to seek in
/// @param offset - The offset from origin to seek to. After successful seeking, the file will be at
/// the effective location of `origin` + `offset`
/// @param origin - The location in the file the seek should originate from
///
/// @return `CnxResult` - `Ok()` on success, otherwise an error
/// @ingroup cnx_file
__attr(not_null(1)) CnxResult
	cnx_file_seek(CnxFile* restrict file, i64 offset, CnxFileSeekOrigin origin)
		__DISABLE_IF_NULL(file);
__attr(nodiscard) __attr(not_null(1)) CnxResult(i64) cnx_file_tell(CnxFile* restrict file)
	__DISABLE_IF_NULL(file);

/// @brief Closes the given `file`
///
/// Closes the given `file`, freeing the allocated buffer and any operating system level resources
/// associated with the file.
///
/// @param file - The `CnxFile` to close
/// @ingroup cnx_file
__attr(not_null(1)) void cnx_file_close(CnxFile* restrict file) __DISABLE_IF_NULL(file);

/// @brief Returns whether the given `file` is actually symbolic link
///
/// @param file - The pointer to the `CnxFile` to test
///
/// @return `bool` - Whether `file` is a symlink
/// @ingroup cnx_file
#define cnx_file_is_symlink(file) cnx_path_is_symlink(&((file)->path))
/// @brief Returns whether the given `file` has the given file extension
///
/// @param file - The pointer to the `CnxFile` to test
/// @param extension - The extension to test for. This can be a pointer to any string or string-like
/// type (i.e. it can be `CnxString*`, `CnxStringView*`, `cstring`, or a string literal)
///
/// @return `bool` - Whether `file` is a symlink
/// @ingroup cnx_file
#define cnx_file_has_extension(file, extension) \
	cnx_path_has_file_extension(&((file)->path), extension)
/// @brief Returns the file extension of the given `file`
///
/// Attempts to get the file extension of the given `file`. If it does not have a file extension,
/// then this will return `None()`.
///
/// @param file - The pointer to the `CnxFile` to get the file extension of
///
/// @return `CnxOption(CnxString)` - If `file` is a file with a file extension, the file extension
/// of `file`. Otherwise, `None(CnxString)`.
/// @ingroup cnx_file
#define cnx_file_get_extension(file) cnx_path_get_file_extension(&((file)->path))
/// @brief Returns the file name of the given `file`, including the file extension, if it has one.
///
/// @param file - The pointer to the `CnxFile` to get the file name of
///
/// @return `CnxString` - the file name of `file`.
/// @ingroup cnx_file
#define cnx_file_get_name(file) cnx_path_get_file_name(&((file)->path))
/// @brief Returns the file name of the given `file`, excluding the file extension, if it has one.
///
/// @param file - The pointer to the `CnxFile` to get the file name of
///
/// @return `CnxString` - the file name of `file`.
/// @ingroup cnx_file
#define cnx_file_get_name_without_extension(file) \
	cnx_path_get_file_name_without_extension(&((file)->path))
/// @brief Returns the parent directory of the given `file` as an absolute path
///
/// @param file - The pointer to the `CnxFile` to get the parent directory of
///
/// @return `CnxPath` - the parent directory of `file` as an absolute path.
/// @ingroup cnx_file
#define cnx_file_get_parent_directory(file) cnx_path_get_parent_directory(&((file)->path))

/// @brief Frees the given `file`
///
/// Frees the given `file`. Closes the file, freeing the allocated buffer and any operating system
/// level resources associated with the file. This should not be called manually, instead prefer to
/// use `cnx_file_close`, or declare your file as a `CnxScopedFile` so that is is closed
/// automatically when it leaves scope
///
/// @param file - The `CnxFile` to free
/// ingroup cnx_file
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

#define __cnx_file_open_with_allocator_2(...)                \
	__cnx_file_open_with_allocator(__VA_ARGS__,              \
								   CNX_FILE_DEFAULT_OPTIONS, \
								   CNX_FILE_DEFAULT_BUFFER_SIZE)
#define __cnx_file_open_with_allocator_3(...) \
	__cnx_file_open_with_allocator(__VA_ARGS__, CNX_FILE_DEFAULT_BUFFER_SIZE)
#define __cnx_file_open_with_allocator_4(...) __cnx_file_open_with_allocator(__VA_ARGS__)

#define __cnx_file_open_with_allocator(path, allocator, options, buffer_size) \
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
