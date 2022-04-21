/// @file Platform.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module macro defines for identifying the platform being compiled for (operating
/// system, compiler, etc)
/// @version 0.1.1
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

/// @ingroup utils
/// @{
///	@defgroup cnx_platform Platform
/// Cnx provides an assortment of platform detection macros to detect operating system, compiler,
/// little vs big endianness, and architecture.
/// @note while Cnx provides many different platform detection macros, it has currently only been
/// tested on x86_64 Windows, x86_64 linux, and ARM64 (M1) MacOs
/// @}

#ifndef CNX_PLATFORM

#ifndef _GNU_SOURCE
	#define _GNU_SOURCE 1
#endif // _GNU_SOURCE

#include <stdbool.h>

	/// @brief Declarations and definitions related to detecting the compiled-for platform
	#define CNX_PLATFORM

	#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
		/// @brief Whether the compiled-for platform is WINDOWS
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_WINDOWS 1
	#else
		/// @brief Whether the compiled-for platform is WINDOWS
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_WINDOWS 0
	#endif

	#if defined(__APPLE__)
		/// @brief Whether the compiled-for platform is APPLE (MacOS, IOS)
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_APPLE 1
	#else
		/// @brief Whether the compiled-for platform is APPLE (MacOS, IOS)
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_APPLE 0
	#endif

	#if defined(__unix__) || defined(__unix) || defined(unix) \
		|| (CNX_PLATFORM_APPLE && defined(__MACH__)) || defined(__MVS__)
		/// @brief Whether the compiled-for platform is a variant of UNIX
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_UNIX 1
	#else
		/// @brief Whether the compiled-for platform is a variant of UNIX
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_UNIX 0
	#endif

	#if defined(linux) || defined(__linux__) || defined(__linux) \
		|| (defined(__unix__) && !CNX_PLATFORM_APPLE)
		/// @brief Whether the compiled-for platform is LINUX
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_LINUX 1
	#else
		/// @brief Whether the compiled-for platform is LINUX
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_LINUX 0
	#endif

	#if defined(BSD)
		/// @brief Whether the compiled-for platform is BSD
		/// @ingroup cnx_platform
		#define CNX_PLATFOMR_IS_BSD 1
	#else
		/// @brief Whether the compiled-for platform is BSD
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_BSD 0
	#endif

	#if defined(__MVS__)
		/// @brief Whether the compiled-for platform is z/OS
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_ZOS 1
	#else
		/// @brief Whether the compiled-for platform is z/OS
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_ZOS 0
	#endif

	#if defined(__ANDROID__)
		/// @brief Whether the compiled-for platform is ANDROID
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_ANDROID 1
	#else
		/// @brief Whether the compiled-for platform is ANDROID
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_ANDROID 0
	#endif

	#if defined(__hpux)
		/// @brief Whether the compiled-for platform is HPUX
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_HPUX 1
	#else
		/// @brief Whether the compiled-for platform is HPUX
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_HPUX 0
	#endif

	#if defined(_AIX)
		/// @brief Whether the compiled-for platform is AIX
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_AIX 1
	#else
		/// @brief Whether the compiled-for platform is AIX
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_AIX 0
	#endif

	#if defined(__sun) && defined(__SVR4)
		/// @brief Whether the compiled-for platform is SOLARIS
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_SOLARIS 1
	#else
		/// @brief Whether the compiled-for platform is SOLARIS
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_SOLARIS 0
	#endif

	#if defined(__clang__)
		/// @brief Whether the current compiler is CLANG
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_COMPILER_CLANG 1
	#else
		/// @brief Whether the current compiler is CLANG
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_COMPILER_CLANG 0
	#endif

	#if defined(__GNUC__) && !defined(__clang__)
		/// @brief Whether the current compiler is GCC
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_COMPILER_GCC 1
	#else
		/// @brief Whether the current compiler is GCC
		/// @ingroup cnx_platform
		#define CNX_PLATFORM_COMPILER_GCC 0
	#endif

/// @def CNX_PLATFORM_DEBUG
/// @brief Whether the current build mode is DEBUG
/// @ingroup cnx_platform

/// @def CNX_PLATFORM_RELEASE
/// @brief Whether the current build mode is RELEASE
/// @ingroup cnx_platform

	#if defined(DEBUG) || !defined(NDEBUG)
		#define CNX_PLATFORM_DEBUG	 1
		#define CNX_PLATFORM_RELEASE 0
	#else
		#define CNX_PLATFORM_DEBUG	 0
		#define CNX_PLATFORM_RELEASE 1
	#endif // defined(DEBUG) || !defined(NDEBUG)

// BIG OR LITTLE ENDIAN ?

/// @def CNX_PLATFORM_LITTLE_ENDIAN
/// @brief Whether the compiled-for architecture is a little endian architecture
/// @ingroup cnx_platform

/// @def CNX_PLATFORM_BIG_ENDIAN
/// @brief Whether the compiled-for architecture is a big endian architecture
/// @ingroup cnx_platform

	#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || defined(__BIG_ENDIAN__) \
		|| defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__)          \
		|| defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
		#define CNX_PLATFORM_LITTLE_ENDIAN 0
		#define CNX_PLATFORM_BIG_ENDIAN	   1
	#else
		#if defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || defined(__LITTLE_ENDIAN__) \
			|| defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__)                \
			|| defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
			#define CNX_PLATFORM_LITTLE_ENDIAN 1
			#define CNX_PLATFORM_BIG_ENDIAN	   0
		#else
			#define CNX_PLATFORM_LITTLE_ENDIAN 1
			#define CNX_PLATFORM_BIG_ENDIAN	   0
		#endif
	#endif // BIG OR LITTLE ENDIAN ?

/// @brief The list of supported architectures to compile for
/// @ingroup cnx_platform
typedef enum CnxPlatformArchitecture
{
	CNX_PLATFORM_x86_64 = 1U << 1U,
	CNX_PLATFORM_X86 = 1U << 2U,
	CNX_PLATFORM_ARMv2 = 1U << 3U,
	CNX_PLATFORM_ARMv3 = 1U << 4U,
	CNX_PLATFORM_ARMv4 = 1U << 5U,
	CNX_PLATFORM_ARMv5 = 1U << 6U,
	CNX_PLATFORM_ARMv6 = 1U << 7U,
	CNX_PLATFORM_ARMv7 = 1U << 8U,
	CNX_PLATFORM_ARMv7a = (unsigned)(CNX_PLATFORM_ARMv7) | 1U << 9U,
	CNX_PLATFORM_ARMv7r
		= (unsigned)(CNX_PLATFORM_ARMv7) | (unsigned)(CNX_PLATFORM_ARMv7a) | 1U << 10U,
	CNX_PLATFORM_ARMv7m = (unsigned)(CNX_PLATFORM_ARMv7) | (unsigned)(CNX_PLATFORM_ARMv7a)
						  | (unsigned)(CNX_PLATFORM_ARMv7r) | 1U << 11U,
	CNX_PLATFORM_ARMv7s = (unsigned)(CNX_PLATFORM_ARMv7) | (unsigned)(CNX_PLATFORM_ARMv7a)
						  | (unsigned)(CNX_PLATFORM_ARMv7r) | 1U << 12U,
	CNX_PLATFORM_ARMv8 = 1U << 13U,
	CNX_PLATFORM_ARM64 = 1U << 13U,
	CNX_PLATFORM_MIPS = 1U << 14U,
	CNX_PLATFORM_SUPERH = 1U << 15U,
	CNX_PLATFORM_POWERPC = 1U << 16U,
	CNX_PLATFORM_POWERPC64 = 1U << 17U,
	CNX_PLATFORM_SPARC = 1U << 18U,
	CNX_PLATFORM_M68K = 1U << 19U,
	CNX_PLATFORM_UNKNOWN = 1U << 20U,
} CnxPlatformArchitecture;

/// @def CNX_PLATFORM_ARCHITECTURE
/// @brief The CPU architecture family being compiled for
///
/// Can be any of `CnxPlatformArchitecture`
/// @ingroup cnx_platform

	#if defined(__X86_64__) || defined(_M_X64)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_X86_64
	#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_X86
	#elif defined(__ARM_ARCH_2__)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARMv2
	#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARMv3
	#elif defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARMv4
	#elif defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5E__)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARMv5
	#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) \
		|| defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARMv6
	#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) \
		|| defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARMv7
		#if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) \
			|| defined(__ARM_ARCH_7S__)
			#undef CNX_PLATFORM_ARCHITECTURE
			#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARMv7a
		#endif
		#if defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			#undef CNX_PLATFORM_ARCHITECTURE
			#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARMv7r
		#endif
		#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			#undef CNX_PLATFORM_ARCHITECTURE
			#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARMv7m
		#endif
		#if defined(__ARM_ARCH_7S__)
			#undef CNX_PLATFORM_ARCHITECTURE
			#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARMv7s
		#endif
	#elif defined(__aarch64__) || defined(aarch64) || defined(__arm64__) || defined(arm64) \
		|| defined(_M_ARM64)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_ARM64
	#elif defined(mips) || defined(__mips__) || defined(__mips)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_MIPS
	#elif defined(__sh__)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_SUPERH
	#elif defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__) || defined(__ppc__) \
		|| defined(__PPC__) || defined(_ARCH_PPC)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_POWERPC
	#elif defined(__powerpc64__) || defined(__POWERPC64__) || defined(__PPC64__) \
		|| defined(__ppc64__) || defined(_ARCH_PPC64)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_POWERPC64
	#elif defined(__sparc__) || defined(__sparc)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_SPARC
	#elif defined(__m68k__)
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_M68K
	#else
		#define CNX_PLATFORM_ARCHITECTURE CNX_PLATFORM_UNKNOWN
	#endif

	/// @brief Determines if the compiled-for architecture is the given one
	///
	/// This will determine if the architecture being compiled for matches or is a super set of
	/// the given architecture to check for.
	///
	/// @param to_check - The platform architecture to check against. Must be one of the enum
	/// `CnxPlatformArchitecture`
	///
	/// @return whether the compiled-for architecture is the given one
	/// @ingroup cnx_platform
	#define IS_CNX_PLATFORM_ARCHITECTURE(to_check) \
		(CNX_PLATFORM_ARCHITECTURE & (to_check)) == (to_check)

/// @def CNX_PLATFORM_CACHE_LINE_SIZE
/// @brief The architecture cache-line size
/// @ingroup cnx_platform

	#if IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_X86_64)       \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_ARM64)     \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_POWERPC)   \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_POWERPC64) \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_SPARC)
		#define CNX_PLATFORM_CACHE_LINE_SIZE 64
	#elif IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_X86)     \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_ARMv2)  \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_ARMv3)  \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_ARMv4)  \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_ARMv5)  \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_ARMv6)  \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_ARMv7)  \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_MIPS)   \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_SUPERH) \
		|| IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_M68K)
		#define CNX_PLATFORM_CACHE_LINE_SIZE 32
	#elif IS_CNX_PLATFORM_ARCHITECTURE(CNX_PLATFORM_UNKOWN)
		#define CNX_PLATFORM_CACHE_LINE_SIZE 128
	#endif

#endif // CNX_PLATFORM
