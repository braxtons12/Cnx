cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
include(FetchContent)
include(ExternalProject)

project(C2nxtBenchmark LANGUAGES C VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
#SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

#############################################################################
# Import C2nxt
#############################################################################
FetchContent_Declare(C2nxt SOURCE_DIR "${CMAKE_SOURCE_DIR}/../"
	BINARY_DIR "${CMAKE_SOURCE_DIR}/../build")
FetchContent_MakeAvailable(C2nxt)
#############################################################################
#############################################################################

add_executable(Benchmark
	"${CMAKE_SOURCE_DIR}/src/benchmark.c"
	)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang" OR APPLE)
	set_target_properties(Benchmark PROPERTIES C_CLANG_TIDY clang-tidy)
	SET(CMAKE_C_CLANG_TIDY clang-tidy)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "clang")
	target_compile_options(Benchmark PRIVATE
		-Wall
		-Wextra
		-Wpedantic
		-Weverything
		-Werror
		)
else()
	target_compile_options(Benchmark PRIVATE
		-Wall
		-Wextra
		#-Wpedantic
		-Werror
		)
endif()

target_include_directories(Benchmark PUBLIC
	"${CMAKE_SOURCE_DIR}/"
	)

if(UNIX AND NOT APPLE)
	target_link_libraries(Benchmark PUBLIC
		C2nxt
		)
else()
	target_link_libraries(Benchmark PUBLIC
		C2nxt
		)
endif()
