/// @file StdBasicTypes.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief BasicTypes provides simple typedefs and limits for fixed-width types and
/// semantic-intention-communicating typedefs for various other types
/// @version 0.1
/// @date 2021-07-20
///
/// MIT License
/// @copyright Copyright (c) 2021 Braxton Salyer <braxtonsalyer@gmail.com>
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

#include <float.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "StdPlatform.h"

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wnonportable-include-path\"")
#endif

#include "StdDef.h"

#if STD_PLATFORM_COMPILER_CLANG && STD_PLATFORM_APPLE
	_Pragma("GCC diagnostic pop")
#endif

/// @defgroup types Types
/// C2nxt provides its own typdefs for fixed-width types and typedefs for various semantic types

#ifndef STD_BASIC_TYPES
	/// @brief Declarations and definitions related to C2nxt's builtin types
	#define STD_BASIC_TYPES

	/// @brief `nullptr_t` makes using `NULL` to represent a null pointer (aka a `nullptr`) a
	/// first-class type of its own
	/// @ingroup types
	typedef void* nullptr_t;

	/// @brief `nullptr` is a `NULL` pointer
	/// @ingroup types
	#define nullptr static_cast(nullptr_t)(NULL)

/// @brief `u8` is a fixed-width 8 bit unsigned integer
/// @ingroup types
typedef uint8_t u8;
/// @brief `u16` is a fixed-width 16 bit unsigned integer
/// @ingroup types
typedef uint16_t u16;
/// @brief `u32` is a fixed-width 32 bit unsigned integer
/// @ingroup types
typedef uint32_t u32;
/// @brief `u64` is a fixed-width 64 bit unsigned integer
/// @ingroup types
typedef uint64_t u64;
/// @brief `usize` is the unsigned, platform-dependent type to represent a size
/// @ingroup types
typedef size_t usize;

/// @brief a `byte` is an 8 bit unsigned integer
/// @ingroup types
typedef u8 byte;

/// @brief `i8` is a fixed-width 8 bit signed integer
/// @ingroup types
typedef int8_t i8;
/// @brief `i16` is a fixed-width 16 bit signed integer
/// @ingroup types
typedef int16_t i16;
/// @brief `i32` is a fixed-width 32 bit signed integer
/// @ingroup types
typedef int32_t i32;
/// @brief `i64` is a fixed-width 64 bit signed integer
/// @ingroup types
typedef int64_t i64;
	#if STD_PLATFORM_WINDOWS
		#include <BaseTsd.h>
/// @brief `isize` is the platform-dependent type to represent a signed size. It is the signed
/// alternative to `usize`
/// @ingroup types
typedef SSIZE_T isize;
	#else
		#include <sys/types.h>
/// @brief `isize` is the platform-dependent type to represent a signed size. It is the signed
/// alternative to `usize`
/// @ingroup types
typedef ssize_t isize;
	#endif

/// @brief `f32` is a 32-bit floating point number
/// @ingroup types
typedef float f32;
/// @brief `f64` is a 64-bit floating point number
/// @ingroup types
typedef double f64;

/// @brief alias for a `u8` pointer
typedef u8* u8_ptr;
/// @brief alias for a `u16` pointer
typedef u16* u16_ptr;
/// @brief alias for a `u32` pointer
typedef u32* u32_ptr;
/// @brief alias for a `u64` pointer
typedef u64* u64_ptr;
/// @brief alias for a `usize` pointer
typedef usize* usize_ptr;

/// @brief alias for a `i8` pointer
typedef i8* i8_ptr;
/// @brief alias for a `i16` pointer
typedef i16* i16_ptr;
/// @brief alias for a `i32` pointer
typedef i32* i32_ptr;
/// @brief alias for a `i64` pointer
typedef i64* i64_ptr;
/// @brief alias for a `isize` pointer
typedef isize* isize_ptr;

/// @brief alias for a `f32` pointer
typedef f32* f32_ptr;
/// @brief alias for a `f64` pointer
typedef f64* f64_ptr;

/// @brief alias for a const `u8` pointer
typedef const u8* const_u8_ptr;
/// @brief alias for a const `u16` pointer
typedef const u16* const_u16_ptr;
/// @brief alias for a const `u32` pointer
typedef const u32* const_u32_ptr;
/// @brief alias for a const `u64` pointer
typedef const u64* const_u64_ptr;
/// @brief alias for a const `usize` pointer
typedef const usize* const_usize_ptr;

/// @brief alias for a const `i8` pointer
typedef const i8* const_i8_ptr;
/// @brief alias for a const `i16` pointer
typedef const i16* const_i16_ptr;
/// @brief alias for a const `i32` pointer
typedef const i32* const_i32_ptr;
/// @brief alias for a const `i64` pointer
typedef const i64* const_i64_ptr;
/// @brief alias for a const `isize` pointer
typedef const isize* const_isize_ptr;

/// @brief alias for a const `f32` pointer
typedef const f32* const_f32_ptr;
/// @brief alias for a const `f64` pointer
typedef const f64* const_f64_ptr;

/// @brief `cstring` represents a null-terminated legacy C-style string (aka a `char*`) and should
/// be preferred when semantically referring to a string instead of `char*` to differentiate from
/// references to a pointer to single `char`
/// @ingroup types
typedef char* cstring;
/// @brief `const_cstring` represents a const null-terminated legacy C-style string (aka a `const
/// char*`) and should be preferred when semantically referring to a string instead of `char*` to
/// differentiate from references to a pointer to single `const char`
/// @ingroup types
typedef const char* const_cstring;

/// @brief `wcstring` represents a null-terminated legacy C-style wide string (aka a `wchar_t*`) and
/// should be preferred when semantically referring to a wide string instead of `wchar_t*` to
/// differentiate from references to a pointer to single `wchar_t`
/// @ingroup types
typedef wchar_t* wcstring;
/// @brief `const_wcstring` represents a const null-terminated legacy C-style wide string (aka a
/// `const wchar_t*`) and should be preferred when semantically referring to a wide string instead
/// of `const wchar_t*` to differentiate from references to a pointer to single `const wchar_t`
/// @ingroup types
typedef const wchar_t* const_wcstring;

/// @brief `char_ptr` represents a pointer to a single `char` and should be preferred when referring
/// to a pointer to single `char` over `char*` to differentiate from references to a string
/// @ingroup types
typedef char* char_ptr;
/// @brief `const_char_ptr` represents a pointer to a single `const char` and should be preferred
/// when referring to a pointer to single `const char` over `const char*` to differentiate from
/// references to a const string
/// @ingroup types
typedef const char* const_char_ptr;

/// @brief `wchar_t_ptr` represents a pointer to a single `wchar_t` and should be preferred when
/// referring to a pointer to single `wchar_t` over `wchar_t*` to differentiate from references to a
/// wide string
/// @ingroup types
typedef wchar_t* wchar_t_ptr;
/// @brief `const_wchar_t_ptr` represents a pointer to a single `const wchar_t` and should be
/// preferred when referring to a pointer to single `const wchar_t` over `const wchar_t*` to
/// differentiate from references to a wide string
/// @ingroup types
typedef const wchar_t* const_wchar_t_ptr;

/// @brief Alias for `char_ptr*`. This should be preferred wherever possible when semantically
/// referring to a pointer to `cstring`
typedef char_ptr* cstring_ptr;
/// @brief Alias for `const char_ptr*`. This should be preferred wherever possible when semantically
/// referring to a pointer to `const cstring`
typedef const char** const_cstring_ptr;

/// @brief Alias for `wchar_t_ptr*`. This should be preferred wherever possible when semantically
/// referring to a pointer to `wcstring`
typedef wchar_t_ptr* wcstring_ptr;
/// @brief Alias for `const wchar_t_ptr*`. This should be preferred wherever possible when
/// semantically referring to a pointer to `const wcstring`
typedef const wchar_t** const_wcstring_ptr;

/// `Ref` and `ConstRef` aliases for pointer types, used for the inner type of reference-like types
/// (ie iterators), follow

/// @brief Alias for `char_ptr` used for the inner type of reference-like types (ie iterators)
typedef char_ptr Ref(char);

/// @brief Alias for `u8_ptr` used for the inner type of reference-like types (ie iterators)
typedef u8_ptr Ref(u8);
/// @brief Alias for `u16_ptr` used for the inner type of reference-like types (ie iterators)
typedef u16_ptr Ref(u16);
/// @brief Alias for `u32_ptr` used for the inner type of reference-like types (ie iterators)
typedef u32_ptr Ref(u32);
/// @brief Alias for `u64_ptr` used for the inner type of reference-like types (ie iterators)
typedef u64_ptr Ref(u64);
/// @brief Alias for `usize_ptr` used for the inner type of reference-like types (ie iterators)
typedef usize_ptr Ref(usize);

/// @brief Alias for `i8_ptr` used for the inner type of reference-like types (ie iterators)
typedef i8_ptr Ref(i8);
/// @brief Alias for `i16_ptr` used for the inner type of reference-like types (ie iterators)
typedef i16_ptr Ref(i16);
/// @brief Alias for `i32_ptr` used for the inner type of reference-like types (ie iterators)
typedef i32_ptr Ref(i32);
/// @brief Alias for `i64_ptr` used for the inner type of reference-like types (ie iterators)
typedef i64_ptr Ref(i64);
/// @brief Alias for `isize_ptr` used for the inner type of reference-like types (ie iterators)
typedef isize_ptr Ref(isize);

/// @brief Alias for `f32_ptr` used for the inner type of reference-like types (ie iterators)
typedef f32_ptr Ref(f32);
/// @brief Alias for `f64_ptr` used for the inner type of reference-like types (ie iterators)
typedef f64_ptr Ref(f64);

/// @brief Alias for `const_char_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_char_ptr ConstRef(char);

/// @brief Alias for `const_u8_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_u8_ptr ConstRef(u8);
/// @brief Alias for `const_u16_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_u16_ptr ConstRef(u16);
/// @brief Alias for `const_u32_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_u32_ptr ConstRef(u32);
/// @brief Alias for `const_u64_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_u64_ptr ConstRef(u64);
/// @brief Alias for `const_usize_ptr` used for the inner type of reference-like types (ie
/// iterators)
typedef const_usize_ptr ConstRef(usize);

/// @brief Alias for `const_i8_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_i8_ptr ConstRef(i8);
/// @brief Alias for `const_i16_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_i16_ptr ConstRef(i16);
/// @brief Alias for `const_i32_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_i32_ptr ConstRef(i32);
/// @brief Alias for `const_i64_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_i64_ptr ConstRef(i64);
/// @brief Alias for `const_isize_ptr` used for the inner type of reference-like types (ie
/// iterators)
typedef const_isize_ptr ConstRef(isize);

/// @brief Alias for `const_f32_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_f32_ptr ConstRef(f32);
/// @brief Alias for `const_f64_ptr` used for the inner type of reference-like types (ie iterators)
typedef const_f64_ptr ConstRef(f64);

/// @brief Alias for `cstring*` used for the inner type of reference-like types (ie iterators).
/// This should be preferred wherever possible when semantically referring to a reference to
/// `cstring`
typedef cstring* Ref(cstring);
/// @brief Alias for `const cstring*` used for the inner type of reference-like types (ie
/// iterators). This should be preferred wherever possible when semantically referring to reference
/// to `const cstring`
typedef const cstring* ConstRef(cstring);

/// @brief Alias for `wcstring*` used for the inner type of reference-like types (ie iterators).
/// This should be preferred wherever possible when semantically referring to a reference to
/// `wcstring`
typedef wcstring* Ref(wcstring);
/// @brief Alias for `const wcstring*` used for the inner type of reference-like types (ie
/// iterators). This should be preferred wherever possible when semantically referring to a
/// reference to `const wcstring`
typedef const wcstring* ConstRef(wcstring);

/// @brief Alias for `u8_ptr*` used for the inner type of reference-like types (ie iterators).
typedef u8_ptr* Ref(u8_ptr);
/// @brief Alias for `u16_ptr*` used for the inner type of reference-like types (ie iterators).
typedef u16_ptr* Ref(u16_ptr);
/// @brief Alias for `u32_ptr*` used for the inner type of reference-like types (ie iterators).
typedef u32_ptr* Ref(u32_ptr);
/// @brief Alias for `u64_ptr*` used for the inner type of reference-like types (ie iterators).
typedef u64_ptr* Ref(u64_ptr);
/// @brief Alias for `usize_ptr*` used for the inner type of reference-like types (ie iterators).
typedef usize_ptr* Ref(usize_ptr);

/// @brief Alias for `i8_ptr*` used for the inner type of reference-like types (ie iterators).
typedef i8_ptr* Ref(i8_ptr);
/// @brief Alias for `i16_ptr*` used for the inner type of reference-like types (ie iterators).
typedef i16_ptr* Ref(i16_ptr);
/// @brief Alias for `i32_ptr*` used for the inner type of reference-like types (ie iterators).
typedef i32_ptr* Ref(i32_ptr);
/// @brief Alias for `i64_ptr*` used for the inner type of reference-like types (ie iterators).
typedef i64_ptr* Ref(i64_ptr);
/// @brief Alias for `isize_ptr*` used for the inner type of reference-like types (ie iterators).
typedef isize_ptr* Ref(isize_ptr);

/// @brief Alias for `f32_ptr*` used for the inner type of reference-like types (ie iterators).
typedef f32_ptr* Ref(f32_ptr);
/// @brief Alias for `f64_ptr*` used for the inner type of reference-like types (ie iterators).
typedef f64_ptr* Ref(f64_ptr);

/// @brief Alias for `const u8_ptr*` used for the inner type of reference-like types (ie iterators).
typedef const u8_ptr* ConstRef(u8_ptr);
/// @brief Alias for `const u8_ptr*` used for the inner type of reference-like types (ie iterators).
typedef const u16_ptr* ConstRef(u16_ptr);
/// @brief Alias for `const u8_ptr*` used for the inner type of reference-like types (ie iterators).
typedef const u32_ptr* ConstRef(u32_ptr);
/// @brief Alias for `const u8_ptr*` used for the inner type of reference-like types (ie iterators).
typedef const u64_ptr* ConstRef(u64_ptr);
/// @brief Alias for `const u8_ptr*` used for the inner type of reference-like types (ie iterators).
typedef const usize_ptr* ConstRef(usize_ptr);

/// @brief Alias for `const i8_ptr*` used for the inner type of reference-like types (ie iterators).
typedef const i8_ptr* ConstRef(i8_ptr);
/// @brief Alias for `const i16_ptr*` used for the inner type of reference-like types (ie
/// iterators).
typedef const i16_ptr* ConstRef(i16_ptr);
/// @brief Alias for `const i32_ptr*` used for the inner type of reference-like types (ie
/// iterators).
typedef const i32_ptr* ConstRef(i32_ptr);
/// @brief Alias for `const i64_ptr*` used for the inner type of reference-like types (ie
/// iterators).
typedef const i64_ptr* ConstRef(i64_ptr);
/// @brief Alias for `const isize_ptr*` used for the inner type of reference-like types (ie
/// iterators).
typedef const isize_ptr* ConstRef(isize_ptr);

/// @brief Alias for `const f32_ptr*` used for the inner type of reference-like types (ie
/// iterators).
typedef const f32_ptr* ConstRef(f32_ptr);
/// @brief Alias for `const f64_ptr*` used for the inner type of reference-like types (ie
/// iterators).
typedef const f64_ptr* ConstRef(f64_ptr);

/// @brief Alias for `char_ptr*` used for the inner type of reference-like types (ie iterators).
/// This should be preferred when semantically referring to a reference to a pointer to a single
/// `char`
typedef char_ptr* Ref(char_ptr);
/// @brief Alias for `const char_ptr*` used for the inner type of reference-like types (ie
/// iterators). This should be preferred when semantically referring to a reference to a const
/// pointer to a single `char`
typedef const char_ptr* ConstRef(char_ptr);

/// @brief Alias for `wchar_t_ptr*` used for the inner type of reference-like types (ie iterators).
/// This should be preferred when semantically referring to a reference to a pointer to a single
/// `wchar_t`
typedef wchar_t_ptr* wchar_t_ptr_ref;
/// @brief Alias for `const wchar_t_ptr*` used for the inner type of reference-like types (ie
/// iterators). This should be preferred when semantically referring to a reference to a const
/// pointer to a single `wchar_t`
typedef const wchar_t_ptr* const_wchar_t_ptr_ref;

	/// @brief `u8` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U8 "%" PRIu8
	/// @brief `u16` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U16 "%" PRIu16
	/// @brief `u32` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U32 "%" PRIu32
	/// @brief `u64` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U64 "%" PRIu64
	/// @brief `usize` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_USIZE "%zu"
	/// @brief `umax` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_UMAX "%ju"
	/// @brief `byte` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_BYTE STD_PFORMAT_U8
	/// @brief `i8` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_I8 "%" PRId8
	/// @brief `i16` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_I16 "%" PRId16
	/// @brief `i32` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_I32 "%" PRId32
	/// @brief `i64` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_I64 "%" PRId64
	/// @brief `isize` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_ISIZE "%zd"
	/// @brief `imax` decimal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_IMAX "%jd"

	/// @brief `u8` octal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U8_OCT "%" PRIo8
	/// @brief `u16` octal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U16_OCT "%" PRIo16
	/// @brief `u32` octal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U32_OCT "%" PRIo32
	/// @brief `u64` octal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U64_OCT "%" PRIo64
	/// @brief `usize` octal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_USIZE_OCT "%zo"
	/// @brief `umax` octal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_UMAX_OCT "%jo"
	/// @brief `byte` octal format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_BYTE_OCT STD_PFORMAT_U8_OCT

	/// @brief `u8` lower-case hex format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U8_HEX "%" PRIx8
	/// @brief `u16` lower-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U16_HEX "%" PRIx16
	/// @brief `u32` lower-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U32_HEX "%" PRIx32
	/// @brief `u64` lower-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U64_HEX "%" PRIx64
	/// @brief `usize` lower-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_USIZE_HEX "%zx"
	/// @brief `umax` lower-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_UMAX_HEX "%jx"
	/// @brief `byte` lower-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_BYTE_HEX STD_PFORMAT_U8_HEX

	/// @brief `u8` upper-case hex format specifier for legacy formatted output functions (`printf`,
	/// `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U8_HEX_UP "%" PRIX8
	/// @brief `u16` upper-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U16_HEX_UP "%" PRIX16
	/// @brief `u32` upper-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U32_HEX_UP "%" PRIX32
	/// @brief `u64` upper-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_U64_HEX_UP "%" PRIX64
	/// @brief `usize` upper-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_USIZE_HEX_UP "%zX"
	/// @brief `umax` upper-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_UMAX_HEX_UP "%jX"
	/// @brief `byte` upper-case hex format specifier for legacy formatted output functions
	/// (`printf`, `fprintf`, etc)
	/// @ingroup types
	#define STD_OFORMAT_BYTE_HEX_UP STD_PFORMAT_U8_HEX_UP

	/// @brief `u8` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U8 "%" SCNu8
	/// @brief `u16` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U16 "%" SCNu16
	/// @brief `u32` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U32 "%" SCNu32
	/// @brief `u64` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U64 "%" SCNu64
	/// @brief `usize` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_USIZE "%zu"
	/// @brief `umax` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_UMAX "%ju"
	/// @brief `byte` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_BYTE STD_SFORMAT_U8
	/// @brief `i8` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_I8 "%" SCNd8
	/// @brief `i16` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_I16 "%" SCNd16
	/// @brief `i32` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_I32 "%" SCNd32
	/// @brief `i64` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_I64 "%" SCNd64
	/// @brief `isize` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_ISIZE "%zd"
	/// @brief `imax` decimal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_IMAX "%jd"

	/// @brief `u8` octal format specifier for legacy formatted input functions (`scanf`, `fscanf`,
	/// etc)
	/// @ingroup types
	#define STD_IFORMAT_U8_OCT "%" SCNo8
	/// @brief `u16` octal format specifier for legacy formatted input functions (`scanf`, `fscanf`,
	/// etc)
	/// @ingroup types
	#define STD_IFORMAT_U16_OCT "%" SCNo16
	/// @brief `u32` octal format specifier for legacy formatted input functions (`scanf`, `fscanf`,
	/// etc)
	/// @ingroup types
	#define STD_IFORMAT_U32_OCT "%" SCNo32
	/// @brief `u64` octal format specifier for legacy formatted input functions (`scanf`, `fscanf`,
	/// etc)
	/// @ingroup types
	#define STD_IFORMAT_U64_OCT "%" SCNo64
	/// @brief `usize` octal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_USIZE_OCT "%zo"
	/// @brief `umax` octal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_UMAX_OCT "%jo"
	/// @brief `byte` octal format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_BYTE_OCT STD_SFORMAT_U8_OCT

	/// @brief `u8` lower-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U8_HEX "%" SCNx8
	/// @brief `u16` lower-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U16_HEX "%" SCNx16
	/// @brief `u32` lower-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U32_HEX "%" SCNx32
	/// @brief `u64` lower-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U64_HEX "%" SCNx64
	/// @brief `usize` lower-case hex format specifier for legacy formatted input functions
	/// (`scanf`, `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_USIZE_HEX "%zx"
	/// @brief `umax` lower-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_UMAX_HEX "%jx"
	/// @brief `byte` lower-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_BYTE_HEX STD_SFORMAT_U8_HEX

	/// @brief `u8` upper-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U8_HEX_UP "%" SCNX8
	/// @brief `u16` upper-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U16_HEX_UP "%" SCNX16
	/// @brief `u32` upper-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U32_HEX_UP "%" SCNX32
	/// @brief `u64` upper-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_U64_HEX_UP "%" SCNX64
	/// @brief `umax` upper-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_UMAX_HEX_UP "%jX"
	/// @brief `byte` upper-case hex format specifier for legacy formatted input functions (`scanf`,
	/// `fscanf`, etc)
	/// @ingroup types
	#define STD_IFORMAT_BYTE_HEX_UP STD_SFORMAT_U8_HEX_UP

	/// @brief Maximum value of a `u8`
	/// @ingroup types
	#define std_max_u8 UINT8_MAX
	/// @brief Maximum value of a `u16`
	/// @ingroup types
	#define std_max_u16 UINT16_MAX
	/// @brief Maximum value of a `u32`
	/// @ingroup types
	#define std_max_u32 UINT32_MAX
	/// @brief Maximum value of a `u64`
	/// @ingroup types
	#define std_max_u64 UINT64_MAX
	/// @brief Maximum value of a `usize`
	/// @ingroup types
	#define std_max_usize SIZE_MAX

	/// @brief Maximum value of an `i8`
	/// @ingroup types
	#define std_max_i8 INT8_MAX
	/// @brief Maximum value of an `i16`
	/// @ingroup types
	#define std_max_i16 INT16_MAX
	/// @brief Maximum value of an `i32`
	/// @ingroup types
	#define std_max_i32 INT32_MAX
	/// @brief Maximum value of an `i64`
	/// @ingroup types
	#define std_max_i64 INT64_MAX
	/// @brief Maximum value of an `isize`
	/// @ingroup types
	#define std_max_isize INT64_MAX

	/// @brief Maximum value of an `f32`
	/// @ingroup types
	#define std_max_f32 FLT_MAX
	/// @brief Maximum value of an `f64`
	/// @ingroup types
	#define std_max_f64 DBL_MAX

	/// @brief Minimum value of a `u8`
	/// @ingroup types
	#define std_min_u8 0
	/// @brief Minimum value of a `u16`
	/// @ingroup types
	#define std_min_u16 0
	/// @brief Minimum value of a `u32`
	/// @ingroup types
	#define std_min_u32 0
	/// @brief Minimum value of a `u64`
	/// @ingroup types
	#define std_min_u64 0
	/// @brief Minimum value of a `usize`
	/// @ingroup types
	#define std_min_usize 0

	/// @brief Minimum value of an `i8`
	/// @ingroup types
	#define std_min_i8 INT8_MIN
	/// @brief Minimum value of an `i16`
	/// @ingroup types
	#define std_min_i16 INT16_MIN
	/// @brief Minimum value of an `i32`
	/// @ingroup types
	#define std_min_i32 INT32_MIN
	/// @brief Minimum value of an `i64`
	/// @ingroup types
	#define std_min_i64 INT64_MIN
	/// @brief Minimum value of an `isize`
	/// @ingroup types
	#define std_min_isize INT64_MIN

	/// @brief Minimum value of an `f32`
	/// @ingroup types
	#define std_min_f32 (-std_max_f32)
	/// @brief Minimum value of an `f64`
	/// @ingroup types
	#define std_min_f64 (-std_max_f64)

	/// @brief Minimum discernable difference value of an `f32`
	/// @ingroup types
	#define std_min_diff_f32 FLT_MIN
	/// @brief Minimum discernable difference value of an `f64`
	/// @ingroup types
	#define std_min_diff_f64 DBL_MIN

// clang-format off

#if STD_PLATFORM_APPLE
	/// @brief Returns the maximum possible value of the given type
	///
	/// @param T - The type to get the maximum value for
	///
	/// @return the maximum value
	/// @ingroup types
	#define std_max_value(T) _Generic(((T)(0)), 	\
			u8:  std_max_u8, 	/** NOLINT **/		\
			u16: std_max_u16, 	/** NOLINT **/		\
			u32: std_max_u32, 	/** NOLINT **/		\
			u64: std_max_u64, 	/** NOLINT **/		\
			usize: std_max_usize, 	/** NOLINT **/		\
			i8:  std_max_i8, 	/** NOLINT **/		\
			i16: std_max_i16, 	/** NOLINT **/		\
			i32: std_max_i32, 	/** NOLINT **/		\
			i64: std_max_i64, 	/** NOLINT **/		\
			isize: std_max_isize, 	/** NOLINT **/		\
			f32: std_max_f32, 	/** NOLINT **/		\
			f64: std_max_f64 	/** NOLINT **/)
#else
	/// @brief Returns the maximum possible value of the given type
	///
	/// @param T - The type to get the maximum value for
	///
	/// @return the maximum value
	/// @ingroup types
	#define std_max_value(T) _Generic(((T)(0)), 	\
			u8:  std_max_u8, 	/** NOLINT **/		\
			u16: std_max_u16, 	/** NOLINT **/		\
			u32: std_max_u32, 	/** NOLINT **/		\
			u64: std_max_u64, 	/** NOLINT **/		\
			i8:  std_max_i8, 	/** NOLINT **/		\
			i16: std_max_i16, 	/** NOLINT **/		\
			i32: std_max_i32, 	/** NOLINT **/		\
			i64: std_max_i64, 	/** NOLINT **/		\
			f32: std_max_f32, 	/** NOLINT **/		\
			f64: std_max_f64 	/** NOLINT **/)
#endif

#if STD_PLATFORM_APPLE
	/// @brief Returns the minimum possible value of the given type
	///
	/// @param T - The type to get the minimum value for
	///
	/// @return the minimum value
	/// @ingroup types
	#define std_min_value(T) _Generic(((T)(0)), 	\
			u8:  std_min_u8, 	/** NOLINT **/		\
			u16: std_min_u16, 	/** NOLINT **/		\
			u32: std_min_u32, 	/** NOLINT **/		\
			u64: std_min_u64, 	/** NOLINT **/		\
			usize: std_min_usize, 	/** NOLINT **/		\
			i8:  std_min_i8, 	/** NOLINT **/		\
			i16: std_min_i16, 	/** NOLINT **/		\
			i32: std_min_i32, 	/** NOLINT **/		\
			i64: std_min_i64, 	/** NOLINT **/		\
			isize: std_min_isize, 	/** NOLINT **/		\
			f32: std_min_f32,	/** NOLINT **/ 		\
			f64: std_min_f64 	/** NOLINT **/)
#else
	/// @brief Returns the minimum possible value of the given type
	///
	/// @param T - The type to get the minimum value for
	///
	/// @return the minimum value
	/// @ingroup types
	#define std_min_value(T) _Generic(((T)(0)), 	\
			u8:  std_min_u8, 	/** NOLINT **/		\
			u16: std_min_u16, 	/** NOLINT **/		\
			u32: std_min_u32, 	/** NOLINT **/		\
			u64: std_min_u64, 	/** NOLINT **/		\
			i8:  std_min_i8, 	/** NOLINT **/		\
			i16: std_min_i16, 	/** NOLINT **/		\
			i32: std_min_i32, 	/** NOLINT **/		\
			i64: std_min_i64, 	/** NOLINT **/		\
			f32: std_min_f32,	/** NOLINT **/ 		\
			f64: std_min_f64 	/** NOLINT **/)
#endif

// clang-format on

#endif // STD_BASIC_TYPES
