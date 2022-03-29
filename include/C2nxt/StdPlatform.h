/// @file StdPlatform.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This module macro defines for identifying the platform being compiled for (operating
/// system, compiler, etc)
/// @version 0.1
/// @date 2021-12-08
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
#include "stdbool.h"

/// @ingroup utils
/// @{
///	@defgroup std_platform Platform
/// C2nxt provides an assortment of platform detection macros to detect operating system, compiler,
/// little vs big endianness, and architecture.
/// @note while C2nxt provides many different platform detection macros, it has currently only been
/// tested on x86_64 Windows, x86_64 linux, and ARM64 (M1) MacOs
/// @}

#ifndef STD_PLATFORM
	/// @brief Declarations and definitions related to detecting the compiled-for platform
	#define STD_PLATFORM

	#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
		/// @brief Whether the compiled-for platform is WINDOWS
		/// @ingroup std_platform
		#define STD_PLATFORM_WINDOWS 1
	#else
		/// @brief Whether the compiled-for platform is WINDOWS
		/// @ingroup std_platform
		#define STD_PLATFORM_WINDOWS 0
	#endif

	#if defined(__APPLE__)
		/// @brief Whether the compiled-for platform is APPLE (MacOS, IOS)
		/// @ingroup std_platform
		#define STD_PLATFORM_APPLE 1
	#else
		/// @brief Whether the compiled-for platform is APPLE (MacOS, IOS)
		/// @ingroup std_platform
		#define STD_PLATFORM_APPLE 0
	#endif

	#if defined(__unix__) || defined(__unix) || defined(unix) \
		|| (STD_PLATFORM_APPLE && defined(__MACH__)) || defined(__MVS__)
		/// @brief Whether the compiled-for platform is a variant of UNIX
		/// @ingroup std_platform
		#define STD_PLATFORM_UNIX 1
	#else
		/// @brief Whether the compiled-for platform is a variant of UNIX
		/// @ingroup std_platform
		#define STD_PLATFORM_UNIX 0
	#endif

	#if defined(linux) || defined(__linux__) || defined(__linux) \
		|| (defined(__unix__) && !STD_PLATFORM_APPLE)
		/// @brief Whether the compiled-for platform is LINUX
		/// @ingroup std_platform
		#define STD_PLATFORM_LINUX 1
	#else
		/// @brief Whether the compiled-for platform is LINUX
		/// @ingroup std_platform
		#define STD_PLATFORM_LINUX 0
	#endif

	#if defined(BSD)
		/// @brief Whether the compiled-for platform is BSD
		/// @ingroup std_platform
		#define STD_PLATFOMR_IS_BSD 1
	#else
		/// @brief Whether the compiled-for platform is BSD
		/// @ingroup std_platform
		#define STD_PLATFORM_BSD 0
	#endif

	#if defined(__MVS__)
		/// @brief Whether the compiled-for platform is z/OS
		/// @ingroup std_platform
		#define STD_PLATFORM_ZOS 1
	#else
		/// @brief Whether the compiled-for platform is z/OS
		/// @ingroup std_platform
		#define STD_PLATFORM_ZOS 0
	#endif

	#if defined(__ANDROID__)
		/// @brief Whether the compiled-for platform is ANDROID
		/// @ingroup std_platform
		#define STD_PLATFORM_ANDROID 1
	#else
		/// @brief Whether the compiled-for platform is ANDROID
		/// @ingroup std_platform
		#define STD_PLATFORM_ANDROID 0
	#endif

	#if defined(__hpux)
		/// @brief Whether the compiled-for platform is HPUX
		/// @ingroup std_platform
		#define STD_PLATFORM_HPUX 1
	#else
		/// @brief Whether the compiled-for platform is HPUX
		/// @ingroup std_platform
		#define STD_PLATFORM_HPUX 0
	#endif

	#if defined(_AIX)
		/// @brief Whether the compiled-for platform is AIX
		/// @ingroup std_platform
		#define STD_PLATFORM_AIX 1
	#else
		/// @brief Whether the compiled-for platform is AIX
		/// @ingroup std_platform
		#define STD_PLATFORM_AIX 0
	#endif

	#if defined(__sun) && defined(__SVR4)
		/// @brief Whether the compiled-for platform is SOLARIS
		/// @ingroup std_platform
		#define STD_PLATFORM_SOLARIS 1
	#else
		/// @brief Whether the compiled-for platform is SOLARIS
		/// @ingroup std_platform
		#define STD_PLATFORM_SOLARIS 0
	#endif

	#if defined(__clang__)
		/// @brief Whether the current compiler is CLANG
		/// @ingroup std_platform
		#define STD_PLATFORM_COMPILER_CLANG 1
	#else
		/// @brief Whether the current compiler is CLANG
		/// @ingroup std_platform
		#define STD_PLATFORM_COMPILER_CLANG 0
	#endif

	#if defined(__GNUC__) && !defined(__clang__)
		/// @brief Whether the current compiler is GCC
		/// @ingroup std_platform
		#define STD_PLATFORM_COMPILER_GCC 1
	#else
		/// @brief Whether the current compiler is GCC
		/// @ingroup std_platform
		#define STD_PLATFORM_COMPILER_GCC 0
	#endif

/// @def STD_PLATFORM_DEBUG
/// @brief Whether the current build mode is DEBUG
/// @ingroup std_platform

/// @def STD_PLATFORM_RELEASE
/// @brief Whether the current build mode is RELEASE
/// @ingroup std_platform

	#if defined(DEBUG) || !defined(NDEBUG)
		#define STD_PLATFORM_DEBUG	 1
		#define STD_PLATFORM_RELEASE 0
	#else
		#define STD_PLATFORM_DEBUG	 0
		#define STD_PLATFORM_RELEASE 1
	#endif // defined(DEBUG) || !defined(NDEBUG)

// BIG OR LITTLE ENDIAN ?

/// @def STD_PLATFORM_LITTLE_ENDIAN
/// @brief Whether the compiled-for architecture is a little endian architecture
/// @ingroup std_platform

/// @def STD_PLATFORM_BIG_ENDIAN
/// @brief Whether the compiled-for architecture is a big endian architecture
/// @ingroup std_platform

	#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || defined(__BIG_ENDIAN__) \
		|| defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__)          \
		|| defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
		#define STD_PLATFORM_LITTLE_ENDIAN 0
		#define STD_PLATFORM_BIG_ENDIAN	   1
	#else
		#if defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || defined(__LITTLE_ENDIAN__) \
			|| defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__)                \
			|| defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
			#define STD_PLATFORM_LITTLE_ENDIAN 1
			#define STD_PLATFORM_BIG_ENDIAN	   0
		#else
			#define STD_PLATFORM_LITTLE_ENDIAN 1
			#define STD_PLATFORM_BIG_ENDIAN	   0
		#endif
	#endif // BIG OR LITTLE ENDIAN ?

/// @brief The list of supported architectures to compile for
/// @ingroup std_platform
typedef enum StdPlatformArchitecture
{
	STD_PLATFORM_x86_64 = 1U << 1U,
	STD_PLATFORM_X86 = 1U << 2U,
	STD_PLATFORM_ARMv2 = 1U << 3U,
	STD_PLATFORM_ARMv3 = 1U << 4U,
	STD_PLATFORM_ARMv4 = 1U << 5U,
	STD_PLATFORM_ARMv5 = 1U << 6U,
	STD_PLATFORM_ARMv6 = 1U << 7U,
	STD_PLATFORM_ARMv7 = 1U << 8U,
	STD_PLATFORM_ARMv7a = (unsigned)(STD_PLATFORM_ARMv7) | 1U << 9U,
	STD_PLATFORM_ARMv7r
		= (unsigned)(STD_PLATFORM_ARMv7) | (unsigned)(STD_PLATFORM_ARMv7a) | 1U << 10U,
	STD_PLATFORM_ARMv7m = (unsigned)(STD_PLATFORM_ARMv7) | (unsigned)(STD_PLATFORM_ARMv7a)
						  | (unsigned)(STD_PLATFORM_ARMv7r) | 1U << 11U,
	STD_PLATFORM_ARMv7s = (unsigned)(STD_PLATFORM_ARMv7) | (unsigned)(STD_PLATFORM_ARMv7a)
						  | (unsigned)(STD_PLATFORM_ARMv7r) | 1U << 12U,
	STD_PLATFORM_ARMv8 = 1U << 13U,
	STD_PLATFORM_ARM64 = 1U << 13U,
	STD_PLATFORM_MIPS = 1U << 14U,
	STD_PLATFORM_SUPERH = 1U << 15U,
	STD_PLATFORM_POWERPC = 1U << 16U,
	STD_PLATFORM_POWERPC64 = 1U << 17U,
	STD_PLATFORM_SPARC = 1U << 18U,
	STD_PLATFORM_M68K = 1U << 19U,
	STD_PLATFORM_UNKNOWN = 1U << 20U,
} StdPlatformArchitecture;

/// @def STD_PLATFORM_ARCHITECTURE
/// @brief The CPU architecture family being compiled for
///
/// Can be any of `StdPlatformArchitecture`
/// @ingroup std_platform

	#if defined(__X86_64__) || defined(_M_X64)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_X86_64
	#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_X86
	#elif defined(__ARM_ARCH_2__)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARMv2
	#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARMv3
	#elif defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARMv4
	#elif defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5E__)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARMv5
	#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) \
		|| defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARMv6
	#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) \
		|| defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARMv7
		#if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) \
			|| defined(__ARM_ARCH_7S__)
			#undef STD_PLATFORM_ARCHITECTURE
			#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARMv7a
		#endif
		#if defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			#undef STD_PLATFORM_ARCHITECTURE
			#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARMv7r
		#endif
		#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
			#undef STD_PLATFORM_ARCHITECTURE
			#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARMv7m
		#endif
		#if defined(__ARM_ARCH_7S__)
			#undef STD_PLATFORM_ARCHITECTURE
			#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARMv7s
		#endif
	#elif defined(__aarch64__) || defined(aarch64) || defined(__arm64__) || defined(arm64) \
		|| defined(_M_ARM64)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_ARM64
	#elif defined(mips) || defined(__mips__) || defined(__mips)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_MIPS
	#elif defined(__sh__)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_SUPERH
	#elif defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__) || defined(__ppc__) \
		|| defined(__PPC__) || defined(_ARCH_PPC)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_POWERPC
	#elif defined(__powerpc64__) || defined(__POWERPC64__) || defined(__PPC64__) \
		|| defined(__ppc64__) || defined(_ARCH_PPC64)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_POWERPC64
	#elif defined(__sparc__) || defined(__sparc)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_SPARC
	#elif defined(__m68k__)
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_M68K
	#else
		#define STD_PLATFORM_ARCHITECTURE STD_PLATFORM_UNKNOWN
	#endif

	/// @brief Determines if the compiled-for architecture is the given one
	///
	/// This will determine if the architecture being compiled for matches or is a super set of
	/// the given architecture to check for.
	///
	/// @param to_check - The platform architecture to check against. Must be one of the enum
	/// `StdPlatformArchitecture`
	///
	/// @return whether the compiled-for architecture is the given one
	/// @ingroup std_platform
	#define IS_STD_PLATFORM_ARCHITECTURE(to_check) \
		(STD_PLATFORM_ARCHITECTURE & (to_check)) == (to_check)

/// @def STD_PLATFORM_CACHE_LINE_SIZE
/// @brief The architecture cache-line size
/// @ingroup std_platform

	#if IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_X86_64)       \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_ARM64)     \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_POWERPC)   \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_POWERPC64) \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_SPARC)
		#define STD_PLATFORM_CACHE_LINE_SIZE 64
	#elif IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_X86)     \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_ARMv2)  \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_ARMv3)  \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_ARMv4)  \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_ARMv5)  \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_ARMv6)  \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_ARMv7)  \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_MIPS)   \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_SUPERH) \
		|| IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_M68K)
		#define STD_PLATFORM_CACHE_LINE_SIZE 32
	#elif IS_STD_PLATFORM_ARCHITECTURE(STD_PLATFORM_UNKOWN)
		#define STD_PLATFORM_CACHE_LINE_SIZE 128
	#endif

#endif // STD_PLATFORM
