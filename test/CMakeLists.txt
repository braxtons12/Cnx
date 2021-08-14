cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
include(FetchContent)
include(ExternalProject)

project(C2nxtTest LANGUAGES C VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
#SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

#############################################################################
# Import Unity Test
#############################################################################
FetchContent_Declare(unity
	GIT_REPOSITORY "https://github.com/ThrowTheSwitch/Unity"
	GIT_TAG "v2.5.2"
	)

FetchContent_MakeAvailable(unity)
#############################################################################
#############################################################################

#############################################################################
# Import C2nxt
#############################################################################
FetchContent_Declare(C2nxt SOURCE_DIR "${CMAKE_SOURCE_DIR}/../"
	BINARY_DIR "${CMAKE_SOURCE_DIR}/../build")
FetchContent_MakeAvailable(C2nxt)
#############################################################################
#############################################################################

add_executable(Test
	"${CMAKE_SOURCE_DIR}/src/Test.c"
	"${CMAKE_SOURCE_DIR}/src/StdArrayi32_10.c"
	)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang" OR APPLE)
	set_target_properties(Test PROPERTIES C_CLANG_TIDY clang-tidy)
	SET(CMAKE_C_CLANG_TIDY clang-tidy)
endif()

if(MSVC)
	target_compile_options(Test PRIVATE /WX /W4 /std:c18)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang")
	target_compile_options(Test PRIVATE
		-std=gnu11
		-Wall
		-Wextra
		-Weverything
		-Werror
		-Wno-unused-value
		-Wno-extra-semi
		-Wno-implicit-function-declaration
		-Wno-unused-function
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
		)
else()
	target_compile_options(Test PRIVATE
		-std=gnu11
		-Wall
		-Wextra
		#-Wpedantic
		-Werror
		-Wno-unused-value
		-Wno-implicit-function-declaration
		-Wno-unused-function
		-Wno-missing-braces
		-Wno-stringop-overflow
		)
endif()

target_include_directories(Test PUBLIC
	"${CMAKE_SOURCE_DIR}/"
	"${CMAKE_SOURCE_DIR}/build/_deps/unity-src/src"
	)

if(UNIX AND NOT APPLE)
	target_link_libraries(Test PUBLIC
		curl
		unity
		C2nxt
		)
else()
	target_link_libraries(Test PUBLIC
		unity
		C2nxt
	)
endif()
