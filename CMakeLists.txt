cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(C2nxt VERSION 0.1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
#SET(CMAKE_C_STANDARD 18) # Cmake doesn't currently support C18 directly, so we'll have to set this
# manually
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

###### We add headers to sources sets because it helps with `#include` lookup for some tooling #####
set(EXPORTS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/C2nxt.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdAllocators.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdArray.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdAssert.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdBasicTypes.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdCollectionsData.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdDef.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdError.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdFormat.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdIO.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdIterator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdMath.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdOption.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdPlatform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdRange.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdRatio.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdResult.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdString.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdTrait.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdTypeTraits.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdVector.h"
	)
set(IMPLEMENTATIONS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdAllocators.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdAssert.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdError.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdFormat.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdIO.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdMath.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdOption.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdRange.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdRatio.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdResult.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdString.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StdVector.c"
	)

add_library(C2nxt STATIC
	${EXPORTS} ${IMPLEMENTATIONS})

if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang" OR APPLE)
	set_target_properties(C2nxt PROPERTIES C_CLANG_TIDY clang-tidy)
	SET(CMAKE_C_CLANG_TIDY clang-tidy)
endif()

if(MSVC)
	target_compile_options(C2nxt PRIVATE /WX /W4 /std:gnu18)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang")
	target_compile_options(C2nxt PRIVATE
		-Wall
		-Wextra
		-Weverything
		-Werror
		)
	target_compile_options(C2nxt PUBLIC
		-std=gnu11
		-Wno-unused-value
		-Wno-unused-function
		-Wno-extra-semi
		-Wno-c++98-compat
		-Wno-cast-align
		-Wno-cast-qual
		-Wno-disabled-macro-expansion
		-Wno-missing-prototypes
		-Wno-used-but-marked-unused
		-Wno-reserved-id-macro
		-Wno-keyword-macro
		-Wno-padded
		-Wno-unused-macros
		-Wno-class-varargs
		-Wno-undef
		-Wno-float-equal
		-Wno-double-promotion
		-Wno-gnu-auto-type
		-Wno-gnu-statement-expression
		-Wno-gnu-zero-variadic-macro-arguments
		-Wno-gnu-empty-initializer
		-Wno-gnu-case-range
		-Wno-pedantic
		-Wno-gcc-compat)
else()
	target_compile_options(C2nxt PRIVATE
		-Wall
		-Wextra
		#-Wpedantic
		-Werror)
	target_compile_options(C2nxt PUBLIC
		-std=gnu11
		-Wno-unused-value
		-Wno-unused-function
		-Wno-missing-braces
		-Wno-stringop-overflow
		-Wno-comment
		)
endif()

if(APPLE)
	target_include_directories(C2nxt BEFORE INTERFACE
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		"/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/malloc"
		)
else()
	target_include_directories(C2nxt BEFORE INTERFACE
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		)
endif()

if(APPLE)
	target_link_libraries(C2nxt PUBLIC m c)
else()
	target_link_libraries(C2nxt PUBLIC m)
endif()

add_subdirectory("docs")
