cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
include(FetchContent)

project(C2nxt VERSION 0.1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

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

###### We add headers to sources sets because it helps with `#include` lookup for some tooling #####
set(EXPORTS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/C2nxt.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdAllocators.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdArray.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdAssert.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdBasicTypes.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdCollectionData.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdDef.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdError.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdFormat.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdIO.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdIterator.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdLambda.h"
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
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/mpl/StdArgLists.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/mpl/StdPPMath.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/mpl/StdPPStrings.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/mpl/detail/ArgLists.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/time/StdClock.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/time/StdDuration.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/time/StdTimePoint.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdTime.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/std_vector/StdVectorDef.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/std_vector/StdVectorDecl.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/std_vector/StdVectorImpl.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/std_option/StdOptionDef.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/std_option/StdOptionDecl.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/std_option/StdOptionImpl.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdEnum.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt/StdTuple.h"
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
        "${CMAKE_CURRENT_SOURCE_DIR}/src/time/StdClock.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/time/StdDuration.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/time/StdTimePoint.c"
        )

add_library(C2nxt STATIC
        ${EXPORTS} ${IMPLEMENTATIONS})
add_executable(FormatBenchmark "${CMAKE_CURRENT_SOURCE_DIR}/src/benchmark/benchmark.c")
add_executable(Test "${CMAKE_CURRENT_SOURCE_DIR}/src/test/Test.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/test/StdArrayi32_10.c")

if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang" OR APPLE)
    set_target_properties(C2nxt PROPERTIES C_CLANG_TIDY clang-tidy)
    set_target_properties(FormatBenchmark PROPERTIES C_CLANG_TIDY clang-tidy)
    set_target_properties(Test PROPERTIES C_CLANG_TIDY clang-tidy)
    SET(CMAKE_C_CLANG_TIDY clang-tidy)
endif ()

if (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang")
    target_compile_options(FormatBenchmark PRIVATE
            -std=gnu2x
            -Wall
            -Wextra
            -Weverything
            -Werror
            )
    target_compile_options(Test PRIVATE
            -std=gnu2x
            -Wall
            -Wextra
            -Weverything
            -Werror
            )
    target_compile_options(C2nxt PRIVATE
            -std=gnu2x
            -Wall
            -Wextra
            -Weverything
            -Werror
            )
    target_compile_options(C2nxt PUBLIC
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
            )
    if (WIN32)
        target_compile_options(C2nxt PUBLIC
                -D_CRT_SECURE_NO_WARNINGS
                )
        target_compile_options(unity PRIVATE
                -Wno-missing-noreturn
                )
    endif ()
else ()
    target_compile_options(Test PRIVATE
            -std=gnu2x
            -Wall
            -Wextra
            -Werror)
    target_compile_options(FormatBenchmark PRIVATE
            -std=gnu2x
            -Wall
            -Wextra
            -Werror)
    target_compile_options(C2nxt PRIVATE
            -std=gnu2x
            -Wall
            -Wextra
            -Werror)
    target_compile_options(C2nxt PUBLIC
            -Wnonnull
            -Wno-unused-value
            -Wno-unused-function
            -Wno-missing-braces
            -Wno-stringop-overflow
            -Wno-comment
            )
endif ()

if (APPLE)
    target_include_directories(C2nxt BEFORE PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
            )
    target_include_directories(C2nxt BEFORE INTERFACE
            "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/malloc"
            )
else ()
    target_include_directories(C2nxt BEFORE PUBLIC
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
            )
endif ()

if (APPLE)
    target_link_libraries(C2nxt PUBLIC m c)
elseif (NOT WIN32)
    target_link_libraries(C2nxt PUBLIC m)
endif ()

target_link_libraries(FormatBenchmark PRIVATE C2nxt)
target_link_libraries(Test PRIVATE C2nxt unity)

find_package(Doxygen REQUIRED)
set(MCSS_DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile-mcss.in")
set(MCSS_DOXYFILE_OUT "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile-mcss")
set(MCSS_CONF_PY "${CMAKE_CURRENT_SOURCE_DIR}/conf.py")
set(MCSS_TEMPLATES "${CMAKE_CURRENT_SOURCE_DIR}/templates")
set(DOXYGEN_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/C2nxt")
set(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs")
set(DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_DIR}/xml/index.xml")
set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
set(DOXYFILE_OUT "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")

set(DOXYGEN_HTML "${DOXYGEN_OUTPUT_DIR}/html/index.html")

set(COMPILATION_DATABASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(DOXYGEN_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
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

add_custom_target(Doxygen DEPENDS ${DOXYGEN_HTML})
set_target_properties(Doxygen PROPERTIES EXCLUDE_FROM_ALL TRUE)

include(GNUInstallDirs)
install(DIRECTORY ${DOXYGEN_OUTPUT_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})

enable_testing()

add_test(test Test)
