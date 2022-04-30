/// @file Path.c
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Path provides various functions for working with filesystem paths
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

#include <Cnx/StringExt.h>
#include <Cnx/filesystem/Path.h>
#include <stdio.h>
#include <sys/stat.h>

#define CNX_PATH_SEPARATOR_UNIX '/'
#define CNX_PATH_SEPARATOR_AS_STRING_UNIX "/"

#define CNX_PATH_SEPARATOR_WINDOWS '\\'
#define CNX_PATH_SEPARATOR_AS_STRING_WINDOWS "\\"

#define CNX_SYSTEM_ROOT_UNIX "/"
#define CNX_SYSTEM_ROOT_WINDOWS "C:\\"

#if CNX_PLATFORM_WINDOWS

	#define CNX_PATH_SEPARATOR			 CNX_PATH_SEPARATOR_WINDOWS
	#define CNX_PATH_SEPARATOR_AS_STRING CNX_PATH_SEPARATOR_AS_STRING_WINDOWS
	#define CNX_SYSTEM_ROOT				 CNX_SYSTEM_ROOT_WINDOWS

	#include <Cnx/TypeTraits.h>
	#include <Windows.h>
	#include <tchar.h>
	#include <fileapi.h>
	#include <io.h>

	#define win32_tchar_to_char(string, size) \
		_Generic((string),\
		char* 			: cnx_string_from(static_cast(char*)(string)),\
		const char* 	: cnx_string_from(static_cast(const char*)(string)),\
		WCHAR* 			: win32_wchar_to_char(static_cast(WCHAR*)(string), size),\
		const WCHAR* 	: win32_wchar_to_char(static_cast(WCHAR*)(string), size))

[[always_inline]] [[nodiscard]] [[not_null(1)]] static inline CnxString
win32_wchar_to_char(WCHAR* wstring, usize wstring_size) {
	let alloc_size = wstring_size * sizeof(WCHAR);
	let_mut alloc = cnx_allocator_allocate_array_t(char, DEFAULT_ALLOCATOR, alloc_size);

	ignore(WideCharToMultiByte(CP_UTF8,
							   0,
							   wstring,
							   narrow_cast(int)(wstring_size),
							   alloc,
							   narrow_cast(int)(alloc_size),
							   nullptr,
							   nullptr));

	CnxScopedString str = cnx_string_from(alloc);

	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return move(str);
}

#else

	#define CNX_PATH_SEPARATOR			 CNX_PATH_SEPARATOR_UNIX
	#define CNX_PATH_SEPARATOR_AS_STRING CNX_PATH_SEPARATOR_AS_STRING_UNIX
	#define CNX_SYSTEM_ROOT				 CNX_SYSTEM_ROOT_UNIX

	#include <pwd.h>
	#include <unistd.h>
	#include <ftw.h>

#endif // CNX_PLATFORM_WINDOWS

#define RESULT_T	CnxPath
#define RESULT_IMPL TRUE
#include <Cnx/Result.h>
#undef RESULT_T
#undef RESULT_IMPL

bool cnx_path_is_valid(const CnxPath* restrict path) {
	CnxScopedVector(usize) occurrences = cnx_string_find_occurrences_of_char_with_allocator(*path,
																		 CNX_PATH_SEPARATOR,
																		 path->m_allocator);
#if CNX_PLATFORM_WINDOWS
	let_mut previous = static_cast(usize)(0);

	foreach(elem, occurrences) {
		if(elem - previous < 2) {
			return false;
		}
		previous = elem;
	}

	return cnx_string_occurrences_of_char(*path, CNX_PATH_SEPARATOR_UNIX) == 0;
#else
	let_mut previous = static_cast(usize)(0);
	foreach(elem, occurrences) {
		if(elem - previous < 2 && previous != 0) {
			return false;
		}
		previous = elem;
	}

	return cnx_string_occurrences_of_char(*path, CNX_PATH_SEPARATOR_WINDOWS) == 0;
#endif // CNX_PLATFORM_WINDOWS
}

#if CNX_PLATFORM_WINDOWS

[[always_inline]] [[nodiscard]] static inline const_cstring get_user_name(void) {

	return getenv("USERNAME");
}

#else

[[always_inline]] [[nodiscard]] static inline const_cstring get_user_name(void) {
	let uid = geteuid();
	let password = getpwuid(uid);
	if(password != nullptr) {
		return password->pw_name;
	}

	return nullptr;
}

#endif // CNX_PLATFORM_WINDOWS

CnxPath (cnx_path_new)(const CnxString* restrict path) {
	if(cnx_path_is_valid(path)) {
		return cnx_string_clone(*path);
	}

	CnxScopedString cloned = cnx_string_clone(*path);
#if CNX_PLATFORM_WINDOWS
	if(cnx_string_occurrences_of_char(*path, CNX_PATH_SEPARATOR_UNIX) != 0) {
		CnxScopedVector(usize) occurrences = cnx_string_find_occurrences_of_char(*path, CNX_PATH_SEPARATOR_WINDOWS);
		foreach(index, occurrences) {
			cnx_string_at(cloned, index) = CNX_PATH_SEPARATOR;
		}
	}
#else
	if(cnx_string_occurrences_of_char(*path, CNX_PATH_SEPARATOR_WINDOWS) != 0) {
		CnxScopedVector(usize) occurrences = cnx_string_find_occurrences_of_char(*path, CNX_PATH_SEPARATOR_WINDOWS);
		foreach(index, occurrences) {
			cnx_string_at(cloned, index) = CNX_PATH_SEPARATOR;
		}
	}
#endif

	CnxScopedVector(CnxString) split
		= cnx_string_split_on_with_allocator(cloned, CNX_PATH_SEPARATOR, path->m_allocator);

	CnxScopedString new_path = cnx_string_new_with_capacity_with_allocator(cnx_string_capacity(*path), path->m_allocator);

	// if running on NOT Windows and path is absolute, make sure to append the root first
	// (on Windows, the root could be a drive other than "C:",
	// and we will pick it up through the string splitting and recombining anyway,
	// so we only need to special case for absolute paths on other platforms)
#if !CNX_PLATFORM_WINDOWS
	if(cnx_path_is_absolute(path)) {
			cnx_string_append(new_path, CNX_SYSTEM_ROOT);
	}
#endif // !CNX_PLATFORM_WINDOWS

	foreach_ref(string, split) {
		cnx_string_append(new_path, string);
		cnx_string_push_back(new_path, CNX_PATH_SEPARATOR);
	}

	cnx_assert(cnx_path_is_valid(&new_path), "Created path is not valid!");
	return move(new_path);
}

CnxPath cnx_user_home_directory(void) {
	CnxScopedString path = cnx_string_new_with_allocator(DEFAULT_ALLOCATOR);

	cnx_string_append(path, CNX_SYSTEM_ROOT);

#if CNX_PLATFORM_WINDOWS || CNX_PLATFORM_APPLE

	CnxScopedString users_directory = cnx_string_from("Users");

#else 

	CnxScopedString users_directory = cnx_string_from("home");
	
#endif // CNX_PLATFORM_WINDOWS || CNX_PLATFORM_APPLE
	
	cnx_path_append(&path, &users_directory);

	CnxScopedString user_name = cnx_string_from(get_user_name());
	cnx_path_append(&path, &user_name);

	return move(path);
}

CnxPath cnx_user_application_data_directory(void) {
	CnxScopedPath path = cnx_user_home_directory();

#if CNX_PLATFORM_WINDOWS

	cnx_string_append(path, "\\AppData");

#elif CNX_PLATFORM_APPLE

	cnx_string_append(path, "/Library/Preferences");

#else // CNX_PLATFORM_APPLE

	cnx_string_append(path, "/.local/share");

#endif // CNX_PLATFORM_WINDOWS

	return move(path);
}

CnxPath cnx_user_documents_directory(void) {
	CnxScopedPath path = cnx_user_home_directory();

	cnx_string_push_back(path, CNX_PATH_SEPARATOR);
	cnx_string_append(path, "Documents");

	return move(path);
}

CnxPath cnx_common_application_data_directory(void) {

#if CNX_PLATFORM_WINDOWS

	return cnx_string_from("C:\\Users\\Public\\AppData");

#elif CNX_PLATFORM_APPLE

	return cnx_string_from("/Library");

#else

	return cnx_string_from("/opt");

#endif // CNX_PLATFORM_WINDOWS
}

CnxPath cnx_common_documents_directory(void) {

#if CNX_PLATFORM_WINDOWS

	return cnx_string_from("C:\\Users\\Public\\Documents");

#elif CNX_PLATFORM_APPLE

	return cnx_string_from("/Users/Shared");

#else

	return cnx_string_from("/local/share");

#endif // CNX_PLATFORM_WINDOWS
}

CnxPath cnx_temp_directory(void) {
#if CNX_PLATFORM_WINDOWS

	CnxScopedString path = cnx_user_application_data_directory();
	cnx_string_append(path, "\\Local\\Temp");
	return move(path);

#else

	return cnx_string_from("/tmp");

#endif // CNX_PLATFORM_WINDOWS
}

#if CNX_PLATFORM_WINDOWS

CnxPath cnx_current_executable_file(void) {
	let_mut size = static_cast(usize)(64); // NOLINT
	let_mut alloc = cnx_allocator_allocate_array_t(TCHAR, DEFAULT_ALLOCATOR, size);
	let_mut actual_size = GetModuleFileNameA(NULL, alloc, narrow_cast(DWORD)(size));

	while(GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		let old_size = size;
		size += size / 2;
		alloc = cnx_allocator_reallocate_array_t(TCHAR, DEFAULT_ALLOCATOR, alloc, old_size, size);
		actual_size = GetModuleFileNameA(NULL, alloc, narrow_cast(DWORD)(size));
	}

	// if TCHAR is WCHAR, we need to convert from widechars (likely UTF16/UCS-2) to char/utf8
	let types_equal = cnx_types_equal(TCHAR, WCHAR);
	if(types_equal) {
		CnxScopedString str = win32_tchar_to_char(alloc, actual_size);
		cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);
		return move(str);
	}
	else {
		CnxScopedString str = cnx_string_from(alloc);

		cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

		return move(str);
	}
}

#elif CNX_PLATFORM_LINUX || CNX_PLATFORM_BSD || CNX_PLATFORM_SOLARIS

	#if CNX_PLATFORM_LINUX

		#define CNX_PROC_EXE_PATH "/proc/self/exe"

	#elif CNX_PLATFORM_BSD

		#define CNX_PROC_EXE_PATH "/proc/curproc/file"

	#else // SOLARIS

		#define CNX_PROC_EXE_PATH "/proc/self/path/a.out"

	#endif // CNX_PLATFORM_LINUX

CnxPath cnx_current_executable_file(void) {
	struct stat stat;
	ignore(lstat(CNX_PROC_EXE_PATH, &stat));
	let size = stat.st_size;

	let_mut alloc
		= cnx_allocator_allocate_array_t(char, DEFAULT_ALLOCATOR, static_cast(usize)(size + 1));

	let ign = readlink(CNX_PROC_EXE_PATH, alloc, static_cast(usize)(size + 1));
	ignore(ign);

	CnxScopedString str = cnx_string_from(alloc);

	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return move(str);
}

#elif CNX_PLATFORM_APPLE

	#include <mach-o/dyld.h>

CnxPath cnx_current_executable_file(void) {
	let_mut size = static_cast(u32)(0);

	ignore(_NSGetExecutablePath(nullptr, &size));

	let_mut alloc = cnx_allocator_allocate_array_t(char, DEFAULT_ALLOCATOR, size); // NOLINT

	ignore(_NSGetExecutablePath(alloc, size));

	CnxScopedString str = cnx_string_from(alloc);

	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return move(str);
}

#else

	#error cnx_current_executable_file not supported on this platform

#endif // CNX_PLATFORM_WINDOWS

#if CNX_PLATFORM_APPLE

CnxPath cnx_current_application_file(void) {
	CnxScopedPath executable = cnx_current_executable_file();

	let_mut parent = cnx_path_get_parent_directory(&executable);

	if(cnx_string_ends_with(parent, "Contents/MacOs")
	   || cnx_string_ends_with(parent, "contents/macos")) {
		let size = cnx_string_size(parent);
		cnx_string_erase_n(parent, size - 14, 14);
	}

	return parent;
}

#else

CnxPath cnx_current_application_file(void) {
	return cnx_current_executable_file();
}

#endif // CNX_PLATFORM_APPLE

CnxPath cnx_system_applications_directory(void) {
#if CNX_PLATFORM_WINDOWS

	return cnx_string_from("C:\\Program Files");

#elif CNX_PLATFORM_APPLE

	return cnx_string_from("/Applications");

#else

	return cnx_string_from("/bin");

#endif // CNX_PLATFORM_WINDOWS
}

#if CNX_PLATFORM_WINDOWS

CnxPath cnx_system_applications_directory_x86(void) {

	return cnx_string_from("C:\\Program Files (x86)");
}

#endif // CNX_PLATFORM_WINDOWS

CnxPath cnx_current_working_directory(void) {
#if CNX_PLATFORM_UNIX || CNX_PLATFORM_APPLE

	let_mut size = static_cast(usize)(64); // NOLINT
	let_mut alloc = cnx_allocator_allocate_array_t(char, DEFAULT_ALLOCATOR, size);

	let_mut cwd = getcwd(alloc, size);
	while(cwd == nullptr && errno == ERANGE) {
		let old_size = size;
		size += size / 2;
		alloc = cnx_allocator_reallocate_array_t(char, DEFAULT_ALLOCATOR, alloc, old_size, size);
		cwd = getcwd(alloc, size);
	}

	CnxScopedString str = cnx_string_from(alloc);

	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return move(str);
#elif CNX_PLATFORM_WINDOWS

	let_mut size = static_cast(usize)(64); // NOLINT
	let_mut alloc = cnx_allocator_allocate_array_t(TCHAR, DEFAULT_ALLOCATOR, size);

	let_mut actual_size = GetCurrentDirectory(size, alloc);
	while(GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		let old_size = size;
		size += size / 2;
		alloc = cnx_allocator_reallocate_array_t(TCHAR, DEFAULT_ALLOCATOR, alloc, old_size, size);
		actual_size = GetCurrentDirectory(size, alloc);
	}

	// if TCHAR is WCHAR, we need to convert from widechars (likely UTF16/UCS-2) to char/utf8
	if(cnx_types_equal(TCHAR, WCHAR)) {
		CnxScopedString str = win32_tchar_to_char(alloc, actual_size);
		cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);
		return move(str);
	}
	else {
		CnxScopedString str = cnx_string_from(alloc);

		cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

		return move(str);
	}
#else

	#error cnx_current_working_directory not supported on this platform

#endif // CNX_PLATFORM_UNIX || CNX_PLATFORM_APPLE
}

char cnx_path_separator_char(void) {
	return CNX_PATH_SEPARATOR;
}

bool cnx_path_is_absolute(const CnxPath* restrict path) {
#if CNX_PLATFORM_WINDOWS
	let first = cnx_string_front(*path);
	let second = cnx_string_at(*path, 1);

	if(!((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z')
		 || (first >= '0' && first <= '9')))
	{
		return false;
	}

	if(second != ':') {
		return false;
	}

	return true;
#else

	return cnx_string_front(*path) == CNX_PATH_SEPARATOR;

#endif
}

#if CNX_PLATFORM_WINDOWS
	#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
		#define S_ISREG(m) (((m)&S_IFMT) == S_IFREG)
	#endif

	#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
		#define S_ISDIR(m) (((m)&S_IFMT) == S_IFDIR)
	#endif

	#if !defined(S_ISLNK) && defined(S_IFMT) && defined(S_IFLNK)
		#define S_ISLNK(m) (((m)&S_IFMT) == S_IFLNK)
	#endif
#endif

bool cnx_path_exists(const CnxPath* restrict path) {
	struct stat info;
	let str = cnx_string_into_cstring(*path);
	if(stat(str, &info) == 0) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		if(S_ISDIR(info.st_mode) || S_ISREG(info.st_mode)) {
			return true;
		}
	}

	return false;
}

bool cnx_path_is_file(const CnxPath* restrict path) {
	struct stat info;
	let str = cnx_string_into_cstring(*path);
	if(stat(str, &info) == 0) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		if(S_ISREG(info.st_mode)) {
			return true;
		}
	}

	return false;
}

bool cnx_path_is_directory(const CnxPath* restrict path) {
	struct stat info;
	let str = cnx_string_into_cstring(*path);
	if(stat(str, &info) == 0) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		if(S_ISDIR(info.st_mode)) {
			return true;
		}
	}

	return false;
}

bool cnx_path_is_fs_root(const CnxPath* restrict path) {
#if CNX_PLATFORM_WINDOWS
	if(cnx_string_length(*path) == 3) {
		return cnx_string_at(*path, 1) == ':' && cnx_string_at(*path, 2) == CNX_PATH_SEPARATOR;
	}

	return false;
#else
	if(cnx_string_length(*path) == 1) {
		return cnx_string_front(*path) == CNX_PATH_SEPARATOR;
	}

	return false;
#endif // CNX_PLATFORM_WINDOWS
}

bool cnx_path_is_symlink(const CnxPath* restrict path) {
#if CNX_PLATFORM_WINDOWS
	let pathname = cnx_string_into_cstring(*path);
	TCHAR dir_path[MAX_PATH];

	_tcscpy(dir_path, pathname);
	WIN32_FIND_DATA file_data;
	HANDLE handle = FindFirstFile(dir_path, &file_data);

	// NOLINTNEXTLINE(performance-no-int-to-ptr)
	if(handle == INVALID_HANDLE_VALUE) {
		return false;
	}

	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	return ((file_data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0)
		   && file_data.dwReserved0 == IO_REPARSE_TAG_SYMLINK;

#else
	struct stat info;
	let str = cnx_string_into_cstring(*path);
	if(lstat(str, &info) == 0) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		if(S_ISLNK(info.st_mode)) {
			return true;
		}
	}

	return false;
#endif // CNX_PLATFORM_WINDOWS
}

CnxResult(CnxPath) cnx_path_get_symlink_target(const CnxPath* restrict path) {
	if(!cnx_path_is_symlink(path)) {
		return Err(CnxPath, cnx_error_new(EINVAL, CNX_POSIX_ERROR_CATEGORY));
	}

#if CNX_PLATFORM_WINDOWS
	let name = cnx_string_into_cstring(*path);

	HANDLE handle = CreateFile(name,
							   GENERIC_READ,
							   FILE_SHARE_READ,
							   NULL,
							   OPEN_EXISTING,
							   FILE_FLAG_BACKUP_SEMANTICS,
							   NULL);
	// NOLINTNEXTLINE(performance-no-int-to-ptr)
	if(handle == INVALID_HANDLE_VALUE) {
		return Err(CnxPath, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}

	DWORD size = GetFinalPathNameByHandle(handle, NULL, 0, FILE_NAME_OPENED);
	if(size == 0) {
		return Err(CnxPath, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}
	let alloc
		= cnx_allocator_allocate_array_t(TCHAR, DEFAULT_ALLOCATOR, static_cast(usize)(size + 1));
	let res = GetFinalPathNameByHandle(handle, alloc, size, FILE_NAME_OPENED);
	if(res == 0) {
		return Err(CnxPath, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}

	CnxScopedString str = win32_tchar_to_char(alloc, static_cast(usize)(size + 1));
	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return Ok(CnxPath, move(str));

#else
	let_mut size = static_cast(usize)(64); // NOLINT
	let_mut alloc = cnx_allocator_allocate_array_t(char, DEFAULT_ALLOCATOR, size);
	let str = cnx_string_into_cstring(*path);

	let_mut size_actual = readlink(str, alloc, size);

	if(size_actual == -1) {
		return Err(CnxPath, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	while(static_cast(usize)(size_actual) >= size) {
		let old_size = size;
		size += size / 2;
		alloc = cnx_allocator_reallocate_array_t(char, DEFAULT_ALLOCATOR, alloc, old_size, size);
		size_actual = readlink(str, alloc, size);
	}

	if(alloc[size_actual - 1] != '\0') {
		alloc[size_actual] = '\0';
	}

	CnxScopedString string = cnx_string_from(alloc);
	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return Ok(CnxPath, move(string));
#endif
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
bool cnx_path_has_file_extension(const CnxPath* restrict path,
								 const CnxString* restrict extension) {
	let is_file = cnx_path_is_file(path);
	if(!is_file) {
		return false;
	}

	CnxScopedString str = cnx_string_clone(*extension);

	cnx_string_prepend(str, ".");

	return cnx_string_ends_with(*path, &str);
}

CnxString cnx_path_get_file_extension(const CnxPath* restrict path) {
	let is_file = cnx_path_is_file(path);
	if(!is_file) {
		return cnx_string_from("");
	}

	let_mut maybe_start = cnx_string_find_last(*path, ".");
	if(cnx_option_is_some(maybe_start)) {
		let start = cnx_option_unwrap(maybe_start);
		return cnx_string_last(*path, cnx_string_length(*path) - start);
	}

	return cnx_string_from("");
}

CnxString cnx_path_get_file_name(const CnxPath* restrict path) {
	let_mut maybe_start = cnx_string_find_last(*path, CNX_PATH_SEPARATOR_AS_STRING);
	if(cnx_option_is_some(maybe_start)) {
		let start = cnx_option_unwrap(maybe_start);
		return cnx_string_last(*path, cnx_string_length(*path) - start);
	}

	return cnx_string_clone(*path);
}

CnxString cnx_path_get_file_name_without_extension(const CnxPath* path) {
	let_mut with_extension = cnx_path_get_file_name(path);

	let_mut maybe_start = cnx_string_find_last(*path, ".");
	if(cnx_option_is_some(maybe_start)) {
		let start = cnx_option_unwrap(maybe_start);
		cnx_string_free(with_extension);
		return cnx_string_first(*path, start);
	}

	return with_extension;
}

CnxResult(CnxPath) cnx_path_get_parent_directory(const CnxPath* restrict path) {
	if(cnx_path_is_absolute(path) && cnx_string_occurrences_of_char(*path, CNX_PATH_SEPARATOR) == 1)
	{
#if CNX_PLATFORM_WINDOWS

		return Ok(CnxPath, cnx_string_first(*path, 3));
#else

		return Ok(CnxPath, cnx_string_from(CNX_PATH_SEPARATOR_AS_STRING));
#endif
	}

	let_mut maybe_last = cnx_string_find_last(*path, CNX_PATH_SEPARATOR_AS_STRING);
	if(cnx_option_is_some(maybe_last)) {
		let last = cnx_option_unwrap(maybe_last);
		return Ok(CnxPath, cnx_string_first(*path, last));
	}

	return Err(CnxPath, cnx_error_new(EINVAL, CNX_POSIX_ERROR_CATEGORY));
}

CnxResult cnx_path_append(CnxPath* restrict path, const CnxString* restrict entry_name) {
	if(cnx_path_is_absolute(entry_name)) {
		return Err(i32, cnx_error_new(EINVAL, CNX_POSIX_ERROR_CATEGORY));
	}

	if(cnx_string_at(*path, cnx_string_length(*path) - 1) != CNX_PATH_SEPARATOR) {
		cnx_string_push_back(*path, CNX_PATH_SEPARATOR);
	}
	cnx_string_append(*path, entry_name);

	return Ok(i32, 0);
}

CnxResult cnx_path_create_file(const CnxPath* restrict file_path, bool overwrite_existing) {
	if(cnx_path_exists(file_path) && !overwrite_existing) {
		return Err(i32, cnx_error_new(EEXIST, CNX_POSIX_ERROR_CATEGORY));
	}

	let res = fopen(cnx_string_into_cstring(*file_path), "w");

	if(res == nullptr) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

#if !CNX_PLATFORM_WINDOWS

int nftw_remove_path(const_cstring path,
					 [[maybe_unused]] const struct stat* stat_ptr,
					 [[maybe_unused]] int type_flag,
					 [[maybe_unused]] struct FTW* ftw_ptr) {
	return remove(path);
}

#endif // !CNX_PLATFORM_WINDOWS

CnxResult cnx_path_remove_file(const CnxPath* restrict path) {
	let pathname = cnx_string_into_cstring(*path);

#if CNX_PLATFORM_WINDOWS

	if(cnx_path_is_file(path)) {
		TCHAR tpathname[MAX_PATH];
		_tcscpy(tpathname, pathname);

		if(DeleteFile(tpathname) == 0) {
			return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
		}

		return Ok(i32, 0);
	}

	return Err(i32, cnx_error_new(EINVAL, CNX_POSIX_ERROR_CATEGORY));
#else

	if(remove(pathname) != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
#endif // CNX_PLATFORM_WINDOWS
}

#if CNX_PLATFORM_WINDOWS
[[nodiscard]] [[not_null(1)]] CnxResult remove_directory_impl(const_cstring restrict pathname, bool recursive, bool guaranteed_empty) {
	WIN32_FIND_DATA file_data;

	TCHAR dir_path[MAX_PATH];
	TCHAR file_name[MAX_PATH];

	_tcscpy(dir_path, pathname);
	_tcscat(dir_path, "\\*");
	_tcscpy(file_name, pathname);
	_tcscat(file_name, "\\");

	HANDLE handle = FindFirstFile(dir_path, &file_data);
	// NOLINTNEXTLINE(performance-no-int-to-ptr)
	if(handle == INVALID_HANDLE_VALUE) {
		return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}

	_tcscpy(dir_path, file_name);

	if(guaranteed_empty) {
		if(RemoveDirectory(dir_path)) {
			return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
		}

		return Ok(i32, 0);
	}

	bool directory_empty = false;
	while(!directory_empty) {
		if(FindNextFile(handle, &file_data) && recursive) {
			// NOLINTNEXTLINE(bugprone-suspicious-string-compare)
			if(!(_tcscmp(file_data.cFileName, ".") && _tcscmp(file_data.cFileName, ".."))) {
				continue;
			}

			_tcscat(file_name, file_data.cFileName);

			// NOLINTNEXTLINE(hicpp-signed-bitwise)
			if((file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				let file_path = cnx_string_from(file_name);
				let res = cnx_path_remove_directory(&file_path, true);
				if(cnx_result_is_err(res)) {
					FindClose(handle);
					return res;
				}

				_tcscpy(file_name, dir_path);
			}
			else {
				// NOLINTNEXTLINE(hicpp-signed-bitwise)
				if(file_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
					_chmod(file_name, _S_IWRITE);
				}

				if(!DeleteFile(file_name)) {
					FindClose(handle);
					return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
				}
				_tcscpy(file_name, dir_path);
			}
		}
		else {
			if(GetLastError() == ERROR_NO_MORE_FILES) {
				directory_empty = true;
			}
			else {
				FindClose(handle);
				return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
			}
		}
	}

	if(RemoveDirectory(dir_path)) {
		return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}
	FindClose(handle);

	return Ok(i32, 0);

}
#endif //CNX_PLATFORM_WINDOWS

// NOLINTNEXTLINE(misc-no-recursion, readability-function-cognitive-complexity))
CnxResult cnx_path_remove_directory(const CnxPath* restrict path, bool recursive) {
	if(!cnx_path_exists(path)) {
		return Err(i32, cnx_error_new(EINVAL, CNX_POSIX_ERROR_CATEGORY));
	}

	let pathname = cnx_string_into_cstring(*path);

#if CNX_PLATFORM_WINDOWS

	return remove_directory_impl(pathname, recursive, false);

#else

	if(recursive) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		let res = nftw(pathname, nftw_remove_path, 64, FTW_DEPTH | FTW_PHYS);
		if(res != 0) {
			return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
		}

		return Ok(i32, 0);
	}
	else {
		let res = remove(pathname);
		if(res != 0) {
			return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
		}

		return Ok(i32, 0);
	}
#endif // CNX_PLATFORM_WINDOWS
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity, misc-no-recursion)
CnxResult remove_path(const CnxPath* restrict path) {

	if(cnx_path_is_file(path)) {
		return cnx_path_remove_file(path);
	}
	else {
		return cnx_path_remove_directory(path, true);
	}
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CnxResult cnx_path_create_symlink(const CnxPath* restrict link_to_create,
								  const CnxPath* restrict link_target,
								  bool overwrite_existing) {

	if(overwrite_existing && cnx_path_exists(link_to_create)) {
		let res = remove_path(link_to_create);
		if(cnx_result_is_err(res)) {
			return res;
		}
	}

#if CNX_PLATFORM_WINDOWS
	let is_dir = static_cast(DWORD)(cnx_path_is_directory(link_target));
	if(!CreateSymbolicLink(cnx_string_into_cstring(*link_to_create),
						   cnx_string_into_cstring(*link_target),
						   is_dir))
	{
		return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}

	return Ok(i32, 0);

#else

	let res
		= symlink(cnx_string_into_cstring(*link_target), cnx_string_into_cstring(*link_to_create));

	if(res != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);

#endif // CNX_PLATFORM_WINDOWS
}

CnxResult cnx_path_remove_symlink(const CnxPath* restrict link_path) {
#if CNX_PLATFORM_WINDOWS
	if(cnx_path_is_file(link_path)) {
		return cnx_path_remove_file(link_path);
	}
	else {
		return remove_directory_impl(cnx_string_into_cstring(*link_path), false, false);
	}
#endif

	if(unlink(cnx_string_into_cstring(*link_path)) != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);
}

CnxResult cnx_path_create_directory(const CnxPath* restrict dir_path, bool overwrite_existing) {
	let exists = cnx_path_exists(dir_path);
	if(exists && !overwrite_existing) {
		return Err(i32, cnx_error_new(EEXIST, CNX_POSIX_ERROR_CATEGORY));
	}
	else if(exists) {
		let res = remove_path(dir_path);
		if(cnx_result_is_err(res)) {
			return res;
		}
	}

#if CNX_PLATFORM_WINDOWS
	if(CreateDirectory(cnx_string_into_cstring(*dir_path), nullptr) == 0) {
		return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}
#else
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	if(mkdir(cnx_string_into_cstring(*dir_path), S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}
#endif // CNX_PLATFORM_WINDOWS
	
	return Ok(i32, 0);
}
