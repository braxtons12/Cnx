/// @file File.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief CnxFile provides various functions for working with type safe, uniquely owned files
/// @version 0.2.0
/// @date 2022-05-01
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

#include <Cnx/filesystem/File.h>
#include <stdio.h>

#define RESULT_T	CnxFile
#define RESULT_IMPL TRUE
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_IMPL

#define UNIQUE_T	   FILE
#define UNIQUE_DELETER file_deleter
#define UNIQUE_IMPL	   TRUE
#include <Cnx/UniquePtr.h>
#undef UNIQUE_T
#undef UNIQUE_DELETER
#undef UNIQUE_IMPL

#define UNIQUE_T	FileBuffer
#define UNIQUE_IMPL TRUE
#include <Cnx/UniquePtr.h> // NOLINT(readability-duplicate-include)
#undef UNIQUE_T
#undef UNIQUE_IMPL

#define RESULT_T	CnxString
#define RESULT_IMPL TRUE
#include <Cnx/Result.h> // NOLINT(readability-duplicate-include)
#undef RESULT_T
#undef RESULT_IMPL

__attr(nodiscard) __attr(always_inline) static inline CnxResult(cstring)
	validate_file_options(CnxFileOptions options) {
#define append_binary_mask		  (~(CnxFileAppend | CnxFileBinary))
#define truncate_binary_mask	  (~(CnxFileTruncate | CnxFileBinary))
#define truncate_fail_mask		  (~(CnxFileTruncate | CnxFileFailIfExists))
#define truncate_binary_fail_mask (~(CnxFileTruncate | CnxFileBinary | CnxFileFailIfExists))

	if(options.mode == CnxFileReadWrite) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		if((options.modifiers & append_binary_mask) == 0) {
			return Ok(cstring, "a+b");
		}
		else if(options.modifiers == CnxFileAppend) {
			return Ok(cstring, "a+");
		}
		else if(options.modifiers == CnxFileTruncate) {
			// NOLINTNEXTLINE(hicpp-signed-bitwise)
			if((options.modifiers & truncate_binary_mask) == 0) {
				return Ok(cstring, "w+b");
			}
			// NOLINTNEXTLINE(hicpp-signed-bitwise)
			else if((options.modifiers & truncate_fail_mask) == 0) {
				return Ok(cstring, "w+x");
			}
			// NOLINTNEXTLINE(hicpp-signed-bitwise)
			else if((options.modifiers & truncate_binary_fail_mask) == 0) {
				return Ok(cstring, "w+xb");
			}
			else {
				return Ok(cstring, "w+");
			}
		}
		else if(options.modifiers == CnxFileBinary) {
			return Ok(cstring, "r+b");
		}
		else if(options.modifiers == CnxFileNone) {
			return Ok(cstring, "r+");
		}
	}
	else if(options.mode == CnxFileRead) {
		if(options.modifiers == CnxFileBinary) {
			return Ok(cstring, "rb");
		}
		else if(options.modifiers == CnxFileNone) {
			return Ok(cstring, "r");
		}
	}
	else if(options.mode == CnxFileWrite) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		if((options.modifiers & append_binary_mask) == 0) {
			return Ok(cstring, "ab");
		}
		else if(options.modifiers == CnxFileAppend) {
			return Ok(cstring, "a");
		}
		else if(options.modifiers == CnxFileTruncate) {
			// NOLINTNEXTLINE(hicpp-signed-bitwise)
			if((options.modifiers & truncate_binary_mask) == 0) {
				return Ok(cstring, "wb");
			}
			// NOLINTNEXTLINE(hicpp-signed-bitwise)
			else if((options.modifiers & truncate_fail_mask) == 0) {
				return Ok(cstring, "wx");
			}
			// NOLINTNEXTLINE(hicpp-signed-bitwise)
			else if((options.modifiers & truncate_binary_fail_mask) == 0) {
				return Ok(cstring, "wbx");
			}
			else {
				return Ok(cstring, "w");
			}
		}
	}

	return Err(cstring, cnx_error_new(EINVAL, CNX_POSIX_ERROR_CATEGORY));
}

CnxResult(CnxFile) cnx_file_open_with_allocator(const CnxPath* restrict path,
												CnxFileOptions options,
												usize buffer_size,
												CnxAllocator allocator) {
	let_mut res = validate_file_options(options);
	if(cnx_result_is_err(res)) {
		return Err(CnxFile, cnx_result_unwrap_err(res));
	}

	let_mut file = fopen(cnx_string_into_cstring(*path), cnx_result_unwrap(res));
	if(file != nullptr) {
		UniquePtr(FileBuffer) buffer
			= cnx_make_unique_array_with_allocator(FileBuffer, buffer_size, allocator);
		if(setvbuf(file, cnx_unique_ptr_get(buffer), _IOFBF, buffer_size) != 0) {
			return Err(CnxFile, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
		}

		let cnx_file = (CnxFile){.path = cnx_string_clone_with_allocator(*path, allocator),
								 .file = cnx_unique_ptr_from(FILE, file),
								 .buffer = move(buffer),
								 .options = options};
		return Ok(CnxFile, cnx_file);
	}

	return Err(CnxFile, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
}

CnxResult(CnxFile)
	cnx_file_open(const CnxPath* restrict path, CnxFileOptions options, usize buffer_size) {
	return cnx_file_open_with_allocator(path, options, buffer_size, DEFAULT_ALLOCATOR);
}

CnxResult(i32) __cnx_file_print(CnxFile* file,
								restrict const_cstring format_string,
								CnxAllocator allocator,
								usize num_args,
								...) {

	if(file->options.mode == CnxFileRead) {
		return Err(i32, cnx_error_new(EPERM, CNX_POSIX_ERROR_CATEGORY));
	}

	va_list list = {0};
	va_start(list, num_args);
	CnxScopedString string = cnx_vformat_with_allocator(format_string, allocator, num_args, list);
	let cstr = cnx_string_into_cstring(string);
	let _file = cnx_unique_ptr_get(file->file);
	let len = cnx_string_length(string);
	let res = fwrite(cstr, sizeof(char), len, _file);
	va_end(list);

	if(res < len && errno != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, narrow_cast(i32)(res));
}

CnxResult(i32) __cnx_file_println(CnxFile* file,
								  restrict const_cstring format_string,
								  CnxAllocator allocator,
								  usize num_args,
								  ...) {

	if(file->options.mode == CnxFileRead) {
		return Err(i32, cnx_error_new(EPERM, CNX_POSIX_ERROR_CATEGORY));
	}

	va_list list = {0};
	va_start(list, num_args);
	CnxScopedString string = cnx_vformat_with_allocator(format_string, allocator, num_args, list);
	let cstr = cnx_string_into_cstring(string);
	let _file = cnx_unique_ptr_get(file->file);
	let len = cnx_string_length(string);
	let res = fwrite(cstr, sizeof(char), len, _file);
	let res2 = fputc('\n', _file);
	va_end(list);

	if((res < len && errno != 0) || res2 == EOF) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, narrow_cast(i32)(res));
}

CnxResult(i32)
	cnx_file_write_bytes(CnxFile* restrict file, const u8* restrict bytes, usize num_bytes) {

	if(file->options.mode == CnxFileRead) {
		return Err(i32, cnx_error_new(EPERM, CNX_POSIX_ERROR_CATEGORY));
	}

	let res = fwrite(bytes, sizeof(u8), num_bytes, cnx_unique_ptr_get(file->file));
	if(res != num_bytes) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, narrow_cast(i32)(res));
}

CnxResult(CnxString)
	cnx_file_read_with_allocator(CnxFile* restrict file, usize num_chars, CnxAllocator allocator) {

	if(file->options.mode == CnxFileWrite) {
		return Err(CnxString, cnx_error_new(EPERM, CNX_POSIX_ERROR_CATEGORY));
	}

	CnxScopedString str = cnx_string_new_with_capacity_with_allocator(num_chars, allocator);
	let res = fread(cnx_string_data(str), sizeof(char), num_chars, cnx_unique_ptr_get(file->file));
	if(res < num_chars && ferror(cnx_unique_ptr_get(file->file)) != 0) {
		return Err(CnxString, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(CnxString, move(str));
}

CnxResult(CnxString) cnx_file_read(CnxFile* restrict file, usize num_chars) {
	return cnx_file_read_with_allocator(file, num_chars, DEFAULT_ALLOCATOR);
}

CnxResult(CnxString)
	cnx_file_read_line_with_allocator(CnxFile* restrict file, CnxAllocator allocator) {

	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	if(file->options.mode == CnxFileWrite || (file->options.modifiers & CnxFileBinary) != 0) {
		return Err(CnxString, cnx_error_new(EPERM, CNX_POSIX_ERROR_CATEGORY));
	}

	CnxScopedString str = cnx_string_new_with_allocator(allocator);
	let _file = cnx_unique_ptr_get(file->file);
	let_mut c = static_cast(char)(fgetc(_file)); // NOLINT(readability-identifier-length)
	while(c != '\n') {
		if(c == EOF) {
			if(ferror(_file) != 0) {
				return Err(CnxString, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
			}
			else {
				return Ok(CnxString, move(str));
			}
		}
		else {
			cnx_string_push_back(str, c);
		}

		c = static_cast(char)(fgetc(_file));
	}

	return Ok(CnxString, move(str));
}

CnxResult(CnxString) cnx_file_read_line(CnxFile* restrict file) {
	return cnx_file_read_line_with_allocator(file, DEFAULT_ALLOCATOR);
}

CnxResult(usize)
	cnx_file_read_bytes(CnxFile* restrict file, u8* restrict bytes, usize max_num_bytes) {

	if(file->options.mode == CnxFileWrite) {
		return Err(usize, cnx_error_new(EPERM, CNX_POSIX_ERROR_CATEGORY));
	}

	let _file = cnx_unique_ptr_get(file->file);
	let res = fread(bytes, sizeof(u8), max_num_bytes, _file);
	if(res < max_num_bytes && ferror(_file) != 0) {
		return Err(usize, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(usize, res);
}

CnxResult cnx_file_flush(CnxFile* restrict file) {
	if(file->options.mode == CnxFileRead) {
		return Err(i32, cnx_error_new(EPERM, CNX_POSIX_ERROR_CATEGORY));
	}

	if(fflush(cnx_unique_ptr_get(file->file)) != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CnxResult cnx_file_seek(CnxFile* restrict file, i64 offset, CnxFileSeekOrigin origin) {

	if(fseek(cnx_unique_ptr_get(file->file), narrow_cast(long)(offset), static_cast(int)(origin))
	   != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

CnxResult(i64) cnx_file_tell(CnxFile* restrict file) {
	let res = ftell(cnx_unique_ptr_get(file->file));
	if(res == -1) {
		return Err(i64, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i64, res);
}

void cnx_file_close(CnxFile* restrict file) {
	cnx_string_free(file->path);
	cnx_unique_ptr_free(FILE, file->file);
	cnx_unique_ptr_free(FileBuffer, file->buffer);
}

void cnx_file_free(void* file) {
	cnx_file_close(static_cast(CnxFile*)(file));
}
