/// @file Path.c
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

#include <Cnx/StringExt.h>
#include <Cnx/filesystem/Path.h>
#include <sys/stat.h>
#include <stdio.h>

#if CNX_PLATFORM_WINDOWS

	#define CNX_PATH_SEPARATOR			 '\\'
	#define CNX_PATH_SEPARATOR_AS_STRING "\\"
	#define CNX_SYSTEM_ROOT				 "C:\"

	#include <libloaderapi.h>
	#include <stringapiset.h>
	#include <Cnx/TypeTraits.h>
	#include <winbase.h>
	#include <tchar.h>
	#include <winbase.h>
	#include <fileapi.h>

[[always_inline]] [[nodiscard]] [[not_null(1)]] static inline CnxString
win32_wchar_to_char(TCHAR* wstring, usize wstring_size) {
	let alloc_size = size * sizeof(TCHAR);
	let_mut alloc = cnx_allocator_allocate_array_t(char, DEFAULT_ALLOCATOR, alloc_size);

	ignore(WideCharToMultiByte(CP_UTF8,
							   0,
							   wstring,
							   wstring_size,
							   alloc,
							   alloc_size,
							   nullptr,
							   nullptr));

	let str = cnx_string_from(alloc);

	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return str;
}

#else

	#define CNX_PATH_SEPARATOR			 '/'
	#define CNX_PATH_SEPARATOR_AS_STRING "/"
	#define CNX_SYSTEM_ROOT				 '/'

	#include <pwd.h>
	#include <unistd.h>
	#include <ftw.h>

#endif // CNX_PLATFORM_WINDOWS

bool cnx_path_is_valid(const CnxPath* restrict path) {
	let occurrences = cnx_string_find_occurrences_of_char_with_allocator(*path,
																		 CNX_PATH_SEPARATOR,
																		 path->m_allocator);
#if CNX_PLATFORM_WINDOWS
	let_mut previous = static_cast(usize)(0);
	let_mut started = false;
	foreach(elem, occurrences) {
		if(!started) {
			if(elem != 2) {
				return false;
			}

			started = true;
		}
		else {
			if(elem - previous < 2) {
				return false;
			}
		}
		previous = elem;
	}

	return true;
#else
	let_mut previous = static_cast(usize)(0);
	let_mut started = false;
	foreach(elem, occurrences) {
		if(!started) {
			if(elem != 0) {
				return false;
			}

			started = true;
		}
		else {
			if(elem - previous < 2) {
				return false;
			}
		}
		previous = elem;
	}

	return true;
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

CnxPath cnx_path_new(const CnxString* restrict path) {
	if(cnx_path_is_valid(path)) {
		return cnx_string_clone(*path);
	}

	let capacity = cnx_string_capacity(*path);
	let_mut split
		= cnx_string_split_on_with_allocator(*path, CNX_PATH_SEPARATOR, path->m_allocator);

	let_mut new_path = cnx_string_new_with_capacity_with_allocator(capacity, path->m_allocator);

#if CNX_PLATFORM_WINDOWS
	foreach_ref(string, split) {
		cnx_string_append(new_path, string);
		cnx_string_push_back(new_path, CNX_PATH_SEPARATOR);
	}
#else
	foreach_ref(string, split) {
		cnx_string_push_back(new_path, CNX_PATH_SEPARATOR);
		cnx_string_append(new_path, string);
	}
#endif // CNX_PLATFORM_WINDOWS

	return new_path;
}

CnxPath cnx_user_home_directory(void) {
	let user_name = get_user_name();

	let_mut path = cnx_string_new_with_allocator(DEFAULT_ALLOCATOR);

#if CNX_PLATFORM_WINDOWS

	cnx_string_append(path, "C:\\Users\\");
	cnx_string_append(path, user_name);

#else

	cnx_string_append(path, "/home/");
	cnx_string_append(path, user_name);

#endif // CNX_PLATFORM_WINDOWS

	return path;
}

CnxPath cnx_user_application_data_directory(void) {
	let_mut path = cnx_user_home_directory();

#if CNX_PLATFORM_WINDOWS

	cnx_string_append(path, "\\AppData");

#elif CNX_PLATFORM_APPLE

	cnx_string_append(path, "/Library/Preferences");

#else // CNX_PLATFORM_APPLE

	cnx_string_append(path, "/.local/share");

#endif // CNX_PLATFORM_WINDOWS

	return path;
}

CnxPath cnx_user_documents_directory(void) {
	let_mut path = cnx_user_home_directory();

	cnx_string_push_back(path, CNX_PATH_SEPARATOR);
	cnx_string_append(path, "Documents");

	return path;
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

	let_mut path = cnx_user_application_data_directory();
	cnx_string_append(path, "\\Local\\Temp");
	return path;

#else

	return cnx_string_from("/tmp");

#endif // CNX_PLATFORM_WINDOWS
}

#if CNX_PLATFORM_WINDOWS

CnxPath cnx_current_executable_file(void) {
	let_mut size = static_cast(usize)(64); // NOLINT
	let_mut alloc = cnx_allocator_allocate_array_t(TCHAR, DEFAULT_ALLOCATOR, size);
	let_mut actual_size = GetModuleFileNameA(NULL, alloc, size);

	while(GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		let old_size = size;
		size += size / 2;
		alloc = cnx_allocator_reallocate_array_t(TCHAR, DEFAULT_ALLOCATOR, alloc, old_size, size);
		actual_size = GetModuleFileNameA(NULL, alloc, size);
	}

	// if TCHAR is WCHAR, we need to convert from widechars (likely UTF16/UCS-2) to char/utf8
	if(cnx_types_equal(TCHAR, WCHAR)) {
		let str = win32_wchar_to_char(alloc, actual_size);
		cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);
		return str;
	}
	else {
		let str = cnx_string_from(alloc);

		cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

		return str;
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

	ignore(readlink(CNX_PROC_EXE_PATH, alloc, static_cast(usize)(size + 1)));

	let str = cnx_string_from(alloc);

	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return str;
}

#elif CNX_PLATFORM_APPLE

	#include <mach-o/dyld.h>

CnxPath cnx_current_executable_file(void) {
	let_mut size = static_cast(u32)(0);

	ignore(_NSGetExecutablePath(nullptr, &size));

	let_mut alloc = cnx_allocator_allocate_array_t(char, DEFAULT_ALLOCATOR, size); // NOLINT

	ignore(_NSGetExecutablePath(alloc, size));

	let str = cnx_string_from(alloc);

	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return str;
}

#else

	#error cnx_current_executable_file not supported on this platform

#endif // CNX_PLATFORM_WINDOWS

#if CNX_PLATFORM_APPLE

CnxPath cnx_current_application_file(void) {
	path_scoped executable = cnx_current_executable_file();

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

	let str = cnx_string_from(alloc);

	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return str;
#elif CNX_PLATFORM_WINDOWS

	let_mut size = static_cast(usize)(64);
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
		let str = win32_wchar_to_char(alloc, actual_size);
		cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);
		return str;
	}
	else {
		let str = cnx_string_from(alloc);

		cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

		return str;
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
		 || (first >= '0' || first <= '9')))
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
	struct stat info;
	let str = cnx_string_into_cstring(*path);
	if(stat(str, &info) == 0) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		if(S_ISLNK(info.st_mode)) {
			return true;
		}
	}

	return false;
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
	if(handle == INVALID_HANDLE_VALUE) {
		return Err(CnxPath, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}

	DWORD size = GetFinalPathNameByHandle(handle, NULL, 0, FILE_NAME_OPENED);
	if(size == 0) {
		return Err(CnxPath, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}
	let alloc = cnx_allocator_allocate_array_t(TCHAR, DEFAULT_ALLOCATOR, static_cast(usize)(len + 1);
	let res = GetFinalPathNameByHandle(handle, alloc, size, FILE_NAME_OPENED);
	if(res == 0) {
		return Err(CnxPath, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}

	let str = win32_wchar_to_char(alloc, static_cast(usize)(size + 1));	
	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return Ok(CnxPath, str);

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

	let string = cnx_string_from(alloc);
	cnx_allocator_deallocate(DEFAULT_ALLOCATOR, alloc);

	return Ok(CnxPath, string);
#endif
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
bool cnx_path_has_file_extension(const CnxPath* restrict path,
								 const CnxString* restrict extension) {
	let is_file = cnx_path_is_file(path);
	if(!is_file) {
		return false;
	}

	cnx_string_scoped str = cnx_string_clone(*extension);

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

	cnx_string_push_back(*path, CNX_PATH_SEPARATOR);
	cnx_string_append(*path, entry_name);

	return Ok(i32, 0);
}

#if CNX_PLATFORM_WINDOWS

CnxResult remove_path(const CnxPath* restrict path) {

	let pathname = cnx_string_into_cstring(*path);

	if(cnx_path_is_file(path)) {
		TCHAR tpathname[MAX_PATH];
		_tcscpy(tpathname, pathname);

		if(DeleteFile(tpathname) == 0) {
			return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
		}
	}
	else {
		HANDLE hFind;
		WIN32_FIND_DATA FindFileData;

		TCHAR DirPath[MAX_PATH];
		TCHAR FileName[MAX_PATH];

		_tcscpy(DirPath, pathname);
		_tcscat(DirPath, "\\*");
		_tcscpy(FileName, pathname);
		_tcscat(FileName, "\\");

		hFind = FindFirstFile(DirPath, &FindFileData);
		if(hFind == INVALID_HANDLE_VALUE) {
			return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
		}

		_tcscpy(DirPath, FileName);

		bool bSearch = true;
		while(bSearch) {
			if(FindNextFile(hFind, &FindFileData)) {
				if(!(_tcscmp(FindFileData.cFileName, ".") && _tcscmp(FindFileData.cFileName, "..")))
					continue;
				_tcscat(FileName, FindFileData.cFileName);
				if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					let res = directory_delete(FileName);
					if(cnx_result_is_err(res)) {
						FindClose(hFind);
						return res;
					}

					RemoveDirectory(FileName);
					_tcscpy(FileName, DirPath);
				}
				else {
					if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
						_chmod(FileName, _S_IWRITE);

					if(!DeleteFile(FileName)) {
						FindClose(hFind);
						return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
					}
					_tcscpy(FileName, DirPath);
				}
			}
			else {
				if(GetLastError() == ERROR_NO_MORE_FILES)
					bSearch = false;
				else {
					FindClose(hFind);
					return Err(i32, cnx_error_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
				}
			}
		}
		FindClose(hFind);
	}

	return Ok(i32, 0);
}

#else

int remove_path(const_cstring path,
				[[maybe_unused]] const struct stat* stat_ptr,
				[[maybe_unused]] int type_flag,
				[[maybe_unused]] struct FTW* ftw_ptr) {
	return remove(path);
}

#endif // CNX_PLATFORM_WINDOWS

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
CnxResult cnx_path_create_symlink(const CnxPath* restrict link_to_create,
								  const CnxPath* restrict link_target,
								  bool overwrite_existing) {
#if CNX_PLATFORM_WINDOWS
	if(overwrite_existing && cnx_path_exists(link_to_create)) {
		let res = remove_path(link_to_create);
		if(cnx_result_is_err(res)) {
			return res;
		}
	}

	let is_dir = static_cast(DWORD)(cnx_path_is_directory(link_target));
	if(!CreateSymbolicLink(cnx_string_into_cstring(*link_to_create),
						   cnx_string_into_cstring(*link_target),
						   is_dir))
	{
		return Err(i32, cnx_erro_new(GetLastError(), CNX_WIN32_ERROR_CATEGORY));
	}

	return Ok(i32, 0);

#else

	let to_create_cstr = cnx_string_into_cstring(*link_target);

	if(overwrite_existing && cnx_path_exists(link_to_create)) {
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		let res = nftw(to_create_cstr, remove_path, 64, FTW_DEPTH | FTW_PHYS);
		if(res != 0) {
			return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
		}
	}

	let res = symlink(to_create_cstr, cnx_string_into_cstring(*link_to_create));

	if(res != 0) {
		return Err(i32, cnx_error_new(errno, CNX_POSIX_ERROR_CATEGORY));
	}

	return Ok(i32, 0);

#endif // CNX_PLATFORM_WINDOWS
}
