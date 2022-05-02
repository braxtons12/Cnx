/// @file File.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief CnxFile provides various functions for working with type safe, uniquely owned files
/// @version 0.2.0
/// @date 2022-04-30
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

#ifndef CNX_FILE
#define CNX_FILE

#include <Cnx/Format.h>
#include <Cnx/IO.h>
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

__attr(always_inline)
	__attr(not_null(1)) static inline void file_deleter(FILE* restrict file,
														__attr(maybe_unused)
															CnxAllocator allocator) {

	__IGNORE_NONNULL_COMPARE_WARNING_START
	if(file != stdout && file != stderr && file != nullptr) {
		ignore(fclose(file));
	}
	__IGNORE_NONNULL_COMPARE_WARNING_STOP
}

#undef __IGNORE_NONNULL_COMPARE_WARNING_START
#undef __IGNORE_NONNULL_COMPARE_WARNING_STOP

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

typedef enum CnxFileAccessMode {
	CnxFileRead = 0,
	CnxFileWrite,
	CnxFileReadWrite
} CnxFileAccessMode;

typedef enum CnxFileAccessModifier {
	CnxFileNone = 0,
	CnxFileAppend = 1,
	CnxFileTruncate = 2,
	CnxFileBinary = 4,
	CnxFileFailIfExists = 8
} CnxFileAccessModifier;

typedef struct CnxFileOptions {
	CnxFileAccessMode mode;
	i32 modifiers;
} CnxFileOptions;

typedef struct CnxFile {
	CnxPath path;
	CnxUniquePtr(FILE) file;
	CnxUniquePtr(FileBuffer) buffer;
	CnxFileOptions options;
} CnxFile;

#define CNX_FILE_DEFAULT_OPTIONS \
	((CnxFileOptions){.mode = CnxFileReadWrite, .modifiers = CnxFileTruncate})

#define RESULT_T	CnxFile
#define RESULT_DECL TRUE
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_DECL

#if CNX_PLATFORM_WINDOWS
	#define CNX_FILE_DEFAULT_BUFFER_SIZE (static_cast(usize)(8192))
#else
	#define CNX_FILE_DEFAULT_BUFFER_SIZE (static_cast(usize)(BUFSIZ))
#endif // CNX_PLATFORM_WINDOWS

#define __DISABLE_IF_NULL(file) \
	cnx_disable_if(!(file), "Can't perform a file operation with a nullptr")

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
#define cnx_file_open(...) CONCAT2_DEFERRED(__cnx_file_open_, PP_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

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

#define CnxScopedFile scoped(cnx_file_free)

#endif // CNX_FILE
