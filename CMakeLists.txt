cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
include(FetchContent)

project(Cnx VERSION 0.1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

####################################################################################################
############ Import Criterion ######################################################################
####################################################################################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Criterion REQUIRED)

####################################################################################################
####################################################################################################

###### We add headers to sources sets because it helps with `#include` lookup for some tooling #####
set(EXPORTS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Cnx.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Allocators.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Array.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Assert.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Atomic.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/BasicTypes.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/CollectionData.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Def.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Enum.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Error.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Format.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/IO.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Iterator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Lambda.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Math.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Option.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Platform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Range.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Ratio.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Result.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/String.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/StringExt.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Trait.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/TypeTraits.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Vector.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/mpl/ArgLists.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/mpl/PPMath.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/mpl/PPStrings.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/mpl/detail/ArgLists.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/filesystem/Path.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/filesystem/File.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/FileSystem.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/time/Clock.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/time/Duration.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/time/TimePoint.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Time.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/array/ArrayDef.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/array/ArrayDecl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/array/ArrayImpl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/vector/VectorDef.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/vector/VectorDecl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/vector/VectorImpl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/option/OptionDef.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/option/OptionDecl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/option/OptionImpl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/result/ResultDef.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/result/ResultDecl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/result/ResultImpl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/smart_ptrs/SmartPtrDef.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/unique_ptr/UniquePtrDef.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/unique_ptr/UniquePtrDecl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/unique_ptr/UniquePtrImpl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Enum.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx/Tuple.h"
	)
set(IMPLEMENTATIONS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Allocators.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Assert.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/AtomicImpl.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Error.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Format.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/IO.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Math.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Option.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Range.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Ratio.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Result.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/String.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/StringExt.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Thread.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Vector.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem/Path.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem/File.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/time/Clock.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/time/Duration.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/time/TimePoint.c"
	)

add_library(Cnx STATIC
	${EXPORTS} ${IMPLEMENTATIONS})
add_executable(PrintlnBenchmark "${CMAKE_CURRENT_SOURCE_DIR}/src/benchmark/println_benchmark.c")
add_executable(FileIOBenchmark "${CMAKE_CURRENT_SOURCE_DIR}/src/benchmark/fileio_benchmark.c")
add_executable(Cnx-Test "${CMAKE_CURRENT_SOURCE_DIR}/src/test/Test.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/test/Arrayi32_10.c")

if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang")
	if(NOT DEFINED CMAKE_C_CLANG_TIDY)
		SET(CMAKE_C_CLANG_TIDY clang-tidy)
	endif()
	set_target_properties(Cnx PROPERTIES C_CLANG_TIDY ${CMAKE_C_CLANG_TIDY})
	set_target_properties(PrintlnBenchmark PROPERTIES C_CLANG_TIDY ${CMAKE_C_CLANG_TIDY})
	set_target_properties(FileIOBenchmark PROPERTIES C_CLANG_TIDY ${CMAKE_C_CLANG_TIDY})
	set_target_properties(Cnx-Test PROPERTIES C_CLANG_TIDY ${CMAKE_C_CLANG_TIDY})
endif ()

if (NOT (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang"))
	UNSET(CMAKE_C_CLANG_TIDY CACHE)
	UNSET(CMAKE_C_CLANG_TIDY)
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang")
	target_compile_options(PrintlnBenchmark PRIVATE
		-std=gnu2x
		-Wall
		-Wextra
		-Weverything
		-Werror
		-Wno-unknown-warning-option
		)
	target_compile_options(FileIOBenchmark PRIVATE
		-std=gnu2x
		-Wall
		-Wextra
		-Weverything
		-Werror
		-Wno-unknown-warning-option
		)
	target_compile_options(Cnx-Test PRIVATE
		-std=gnu2x
		-Wall
		-Wextra
		-Weverything
		-Werror
		-Wno-missing-variable-declarations
		-Wno-unknown-warning-option
		)
	target_compile_options(Cnx PRIVATE
		-std=gnu11
		-Wall
		-Wextra
		-Weverything
		-Werror
		)
	target_compile_options(Cnx PUBLIC
		-fdebug-macro
		-Wnonnull
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
		-Wno-gcc-compat
		-Wno-int-to-pointer-cast
		-Wno-c++-compat
		-Wno-extra-semi-stmt
		-Wno-declaration-after-statement
		-Wno-pre-c2x-compat
		-Wno-documentation-unknown-command
		-Wno-unknown-warning-option
		)
	if (WIN32)
		target_compile_options(Cnx PUBLIC
			-D_CRT_SECURE_NO_WARNINGS
			)
	endif ()
else ()
	target_compile_options(PrintlnBenchmark PRIVATE
		-std=gnu2x
		-Wall
		-Wextra
		-Werror
		-Wno-unknown-warning
		)
	target_compile_options(FileIOBenchmark PRIVATE
		-std=gnu2x
		-Wall
		-Wextra
		-Werror
		-Wno-unknown-warning
		)
	target_compile_options(Cnx-Test PRIVATE
		-std=gnu2x
		-Wall
		-Wextra
		-Werror
		-Wno-unknown-warning
		)
	target_compile_options(Cnx PRIVATE
		-std=gnu11
		-Wall
		-Wextra
		-Werror
		)
	target_compile_options(Cnx PUBLIC
		-Wnonnull
		-Wno-unused-value
		-Wno-unused-function
		-Wno-missing-braces
		-Wno-stringop-overflow
		-Wno-comment
		-Wno-strict-aliasing
		-Wno-unknown-warning
		)
endif ()

if (APPLE AND CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
	target_compile_options(PrintlnBenchmark PRIVATE
		-mcpu=apple-a14
		-mtune=native
		)
	target_compile_options(FileIOBenchmark PRIVATE
		-mcpu=apple-a14
		-mtune=native
		)
else()
	target_compile_options(PrintlnBenchmark PRIVATE
		-march=native
		-mtune=native
		)
	target_compile_options(FileIOBenchmark PRIVATE
		-march=native
		-mtune=native
		)
endif()


if (APPLE)
	target_include_directories(Cnx BEFORE PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		)
	target_include_directories(Cnx BEFORE INTERFACE
		"/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/malloc"
		)
else ()
	target_include_directories(Cnx BEFORE PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		)
endif ()

if (APPLE)
	target_link_libraries(Cnx PUBLIC m c)
elseif (NOT WIN32)
	target_link_libraries(Cnx PUBLIC m)
endif ()

if(NOT WIN32)
	target_link_libraries(Cnx PUBLIC pthread)
endif()

target_link_libraries(PrintlnBenchmark PRIVATE Cnx)
target_link_libraries(FileIOBenchmark PRIVATE Cnx)
target_link_libraries(Cnx-Test PRIVATE Cnx ${CRITERION_LIBRARIES})
target_include_directories(Cnx-Test PRIVATE ${CRITERION_INCLUDE_DIRS})

find_package(Doxygen REQUIRED)
set(MCSS_DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile-mcss.in")
set(MCSS_DOXYFILE_OUT "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile-mcss")
set(MCSS_CONF_PY "${CMAKE_CURRENT_SOURCE_DIR}/conf.py")
set(MCSS_TEMPLATES "${CMAKE_CURRENT_SOURCE_DIR}/templates")
set(DOXYGEN_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/Cnx")
set(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs")
set(DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_DIR}/xml/index.xml")
set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
set(DOXYFILE_OUT "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")

set(DOXYGEN_HTML "${DOXYGEN_OUTPUT_DIR}/html/index.html")

set(COMPILATION_DATABASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(DOXYGEN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(DOXYGEN_COMPILE_OPTIONS "-I${CMAKE_CURRENT_SOURCE_DIR}/include")

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
configure_file(${MCSS_DOXYFILE_IN} ${MCSS_DOXYFILE_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

add_custom_command(OUTPUT ${DOXYGEN_HTML}
	DEPENDS ${EXPORTS} ${CMAKE_CURRENT_SOURCE_DIR}/README.md
	COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/third-party/m.css/documentation/doxygen.py ${MCSS_CONF_PY} --debug --templates ${MCSS_TEMPLATES}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN} ${MCSS_DOXYFILE_OUT} ${MCSS_DOXYFILE_IN}
	COMMENT "Generating themed docs with m.css"
	VERBATIM)

add_custom_target(Cnx-Doxygen DEPENDS ${DOXYGEN_HTML})
set_target_properties(Cnx-Doxygen PROPERTIES EXCLUDE_FROM_ALL TRUE)

include(GNUInstallDirs)
install(DIRECTORY ${DOXYGEN_OUTPUT_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})

enable_testing()

add_test(test Cnx-Test)
