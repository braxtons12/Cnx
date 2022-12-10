# Cnx

[![CMake](https://github.com/braxtons12/Cnx/actions/workflows/cmake.yml/badge.svg?branch=master)](https://github.com/braxtons12/Cnx/actions/workflows/cmake.yml)

Cnx is a GNU C (11+) library providing type-safe collections and ergonomic features typical in
higher-level languages, to C. It aims to be a proof-of-concept view of what a more modern standard
(or at least widely used base) library could look like in the future if it only had to support C23,
with much more modern abstractions and ergonomics over the standard C library. It is currently under
active development and has not yet hit a __stable__ release point.

Some features of Cnx include:

- Type-Safe collections implemented (mostly) as manually instantiated templates. Currently
  implemented collections include `CnxString`, `CnxVector(T)`, and `CnxArray(T, N)`
- Error handling facilities similar to Rust and `boost::outcome` with equivalent semantics and
  similar API to Rust, via `CnxError` and `CnxResult(T)`
- Optional value type, `CnxOption(T)` based on Rust's `std::option::Option` with equivalent
  semantics and similar API to Rust
- Facilities for defining, implementing, and using polymorphic interfaces via Traits
- Iterators, Ranges, and `foreach` loops (equivalent to C++'s `for(elem : collection)`)
- Human-readable (no more having to remember which character combination corresonds to which type
  in `printf`), type-safe string formatting and formatted I/O via the `CnxFormat` Trait
- More intuitive, more performant file API with `CnxFile`, along with simple file system interaction
  API with `CnxPath`

##### Why not standard C?

Cnx makes heavy use of `__auto_type` and some of its features are only possible with it. It also
uses statement-expressions for several features and `__attribute__(cleanup)` for scoped destruction.
It could be refactored to be Standard C in C23 or a subsequent standard, if C gets `auto`, `typeof`,
`__VA_OPT__`, lamdas, and defer.

##### Can I use this in production?

Can you? Sure! Should you? Probably not yet. Testing right now is minimal and only enough to ensure
a majority of functionality works in the general case, and is definitely not thorough enough yet to
ensure reliability in production.

## Documentation

You can view the documentation [here](https://braxtons12.github.io/Cnx/)

## Getting Started

Cnx uses CMake, and incorporating it into your project is easy!

First, set up your CMake project. In `CMakeLists.txt`:

```cmake

FetchContent_Declare(Cnx
        GIT_REPOSITORY "https://github.com/braxtons12/Cnx"
        GIT_TAG "0.1.0"
        )

FetchContent_MakeAvailable(Cnx)

### Setup your target......

target_link_libraries(your_target Cnx)

```

Then, include your desired headers, either the main header, `Cnx/Cnx.h`, for everything, or
individual ones for granular imports.

### Example

```c

#include <Cnx/CnxDef.h>
#define VECTOR_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#define RANGE_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <Cnx/Cnx.h>

void transform(i32* restrict elem) {
	*elem *= 3;
}

i32 main(i32 argc, const_cstring* argv) {

	// vector is already instantiated for builtins like `i32` and some provided types like `CnxString`,
	// so, we can just use it directly here
	let_mut vec = cnx_vector_new_with_capacity(i32, 10);

	// insert 10 elements, 0 through 9, into the vector
	ranged_for(i, 0, 10) {
		cnx_vector_push_back(vec, i);
	}

	// print information about the vector (size, capacity, whether it is currently in
	// small-size-optimization mode) to `stdout`, followed by a newline.
	println("{}", as_format_t(CnxVector(i32), vec));

	// print each element, followed by a newline, to `stdout`
	// prints 0 through 9
	foreach(elem, vec) {
		println("{}", elem);	
	}

	// transform the elements in the vector with the above-defined `transform` function
	// and returns a view of the vector as a `CnxRange(i32)` (that we ignore)
	cnx_transform(i32, vec, transform);

	// print each element, followed by a newline, to `stdout`
	// prints multiples of 3 from 0 through 27
	foreach(elem, vec) {
		println("{}", elem);	
	}
}

```

### Collections

Cnx collections provide type-safety, iterators, the ability to use user-provided default and copy
constructors, and destructors, for stored types, and, where applicable, are allocator aware. For
more details on collections, see the documentation on the specific collection.

### Formatting

String formatting uses a very simple syntax. In a format string, `{}` specifies that an argument
should be formatted and placed in that location. Format specifiers can be placed inside the braces
to specify the way the argument should be formatted. Currently supported specifiers are limited:

- `{}`: The default format
- `{d}`: Format the associated argument as a decimal (base-10) number. This is the default for
  integral types. Only applicable to signed, unsigned, and floating point numeric types
- `{dyy}`: Format a floating point number as a decimal (base-10) number, with `yy` digits after the
  decimal point
- `{e}`: Format a floating point number as scientific notation with the default number of
  significant figures (3) after the decimal point. This is the default for floating point types.
- `{eyy}`: Format a floating point number as scientific notation with `yy` number of significant
  figures after the decimal point.
- `{x}`: Format an integral number as lower-case hexadecimal
- `{X}`: Format an integral number as upper-case hexadecimal

Providing a specifier that is invalid for the associated argument will result in a runtime assert in
debug builds and unspecified behavior in release builds (for builtin and provided types).

For more details on string formatting, see the documentation for the `CnxFormat` Trait,
`cnx_format(format_string, ...)`, and the [CnxFormat](@ref format) module.

### Performance

I know, you're probably thinking "So, something that nice can't be fast right? What's the cost?"
And the answer is: not necessarily anything. Generally, performance is faster than traditional
standard C functionality, even with the increased ergonomics and composability gained from using the
library's features.

We have two benchmarks to showcase this. The first pits `println` against `printf` from the standard
library. The second pits the Cnx file API against the standard library's `fprintf`.
For the code used for each benchmark and more detailed results (Std. Dev., Median, individual runs),
see the project/code in the "benchmark" subfolder.


The implementation of `println` makes heavy use of most of the functionality presented in the
example (except for `CnxRange(T)`), as well as the `Result(T)` type, which means it's using many of
the facilities currently provided by the library. Meanwhile, the file API uses everything `println`
does, in addition to providing a safer and more ergonomic API for file manipulation than the
standard library, while also taking better advantage of features like buffering to further improve
performance. So, lets start by taking a look at the benchmark comparing the
relative speed of `println` to `printf`.

This benchmark consisted of printing out N strings to `stdout`, each consisting of:

- the Mth multiple of 1024, unsigned
- the Mth multiple of negative 1024
- the Mth multiple of negative 1024.1024
- a `CnxString` pre-initialized to "This is a string"

where M is in [0, N)

It was run 10 times for each N, with the average of the 10 runs taken. The benchmark was performed
with builds from both Clang 14.0.0 and GCC 11.2.0, and with both the default system allocator and
jemalloc.<br>
All benchmarks were run on an Intel Core i7-8750H with 16GB RAM running EndeavorOS (Arch Linux) with
the Zen Kernel 5.17.5-zen1-1-zen.<br>
Clang builds were compiled with "-flto -Ofast -ffast-math -DNDEBUG".<br>
GCC builds were compiled with "-flto -ffat-lto-objects -Ofast -DNDEBUG"
All numbers are relative performance compared to `printf`

While these numbers are Linux specific, in general you can expect comparable numbers on Windows
using native clang, and even better numbers using MinGW, but benchmark on your specific platform
for platform specific numbers.

#### `println` vs `printf` Results

| N                        |   Clang + System Allocator | Clang + jemalloc | GCC + System Allocator | GCC + jemalloc |
| :----------------------- |--------------------------: | ---------------: | ---------------------: | -------------: |
| 1                        |                     1.6316 |           1.9758 |                 2.9815 |         2.1675 |
| 10                       |                     1.8762 |           1.4501 |                 1.7318 |         1.6026 |
| 100                      |                     1.2305 |           1.2312 |                 1.0868 |         1.0535 |
| 1000                     |                     1.1330 |           1.1666 |                 1.0652 |         1.0045 |
| 10000                    |                     1.3016 |           1.0991 |                 1.0266 |         1.0610 |
| 100000                   |                     1.1154 |           1.1374 |                 1.0509 |         1.0517 |
| average                  |                     1.3814 |           1.3434 |                 1.4905 |         1.3235 |

As you can see, on __average__ you can expect around a 38% performance __boost__ compared to
`printf`. For infrequent I/O you can expect up to a 200% performance __boost__, and for high
frequency I/O you can expect around a 10% performance __boost__. If you dig into the detailed
benchmark results, you'll see that in general you can expect it to be fairly allocator insensitive
with clang (somewhat less so with GCC), and that building with clang and using the default system
allocator is the most __consistently__ fast option to use.

So not only does using Cnx for string formatting and I/O give greatly improved ergonomics and
composability over traditional methods, you also get a performance increase too!

Now for the benchmark pitting the file API against `fprintf`. This is nearly identical to the
previous benchmark as fas as setup and methodologies go. The only difference is that the strings
are printed to two separate files, one for the file API and one for `fprintf`, each using the
API being benchmarked for performing the string formatting and I/O.

#### Cnx File API vs `fprintf` Results

| N                        |   Clang + System Allocator | Clang + jemalloc | GCC + System Allocator | GCC + jemalloc |
| :----------------------- |--------------------------: | ---------------: | ---------------------: | -------------: |
| 1                        |                     5.2710 |           6.6535 |                 3.6145 |         5.0965 |
| 10                       |                     3.6245 |           3.3975 |                 3.2620 |         2.3175 |
| 100                      |                     2.4345 |           2.7620 |                 1.6815 |         1.6745 |
| 1000                     |                     1.5145 |           1.5195 |                 1.1050 |         1.1800 |
| 10000                    |                     1.3485 |           1.3350 |                 0.9645 |         1.0255 |
| 100000                   |                     1.3335 |           1.3115 |                 1.0365 |         1.0005 |
| average                  |                     2.5878 |           2.8298 |                 1.9440 |         2.0491 |

So, on __average__ you can expect around a 100% performance __boost__ compared to `fprintf`.
For infrequent I/O you can expect at least a  250% performance __boost__, or even higher, and for high
frequency I/O you can expect around a 17% performance __boost__. You can see that, particularly
at very sparse workloads, taking better advantage of buffering the way Cnx's file API does leads to
significant performance improvements over the standard functionality. If you dig into the detailed
benchmark results, you'll see that allocator choice plays a bigger role here than with `println`,
in particular at very sparse workloads. Unforntunately, GCC begins to sputter out as the workload
increases, and in some cases actually manages to perform worse than the standard library functions.
However, the gcc + system allocator pairing does the most consistent performance
(lowest average standard deviation and smallest abs(averaged median - averaged average)). That said,
it's also the slowest pairing, and the clang + system allocator pairing comes in at a close second
in terms of consistency while still being the second fastest overall. Lastly, you'll see the
clang + jemalloc pairing is the most __consistently__ fast option to use (highest average and
highest averaged median).

This was the performance for formatting builtin types (u32, i32, f32, cstring. `CnxString`'s
`CnxFormat` implementation simply forwards itself, so it's about as expensive as passing a cstring
to `printf`). That said, it would be reasonable to expect this performance to extend to printing for
custom types as well.

### Testing

Tests are set up as a separate "Cnx-Test" target in the CMake project.<br>
To run the tests, simply configure and build the test target, then run the resulting "Cnx-Test"
executable.<br>
Please feel free to submit new tests!

Inside the Cnx main directory:

```sh

cmake -B build -G "Ninja"
cmake --build build --target Cnx-Test
./build/Cnx-Test

```

### Contributing

Feel free to submit issues, pull requests, etc.!<br>
When contributing code, please adhead to the project `.clang-tidy`, follow the project
`.clang-format` (except in judicious cases of macros ruining things), prefer `let` or `let_mut` over
explicit typing (where possible), and prefer simplicity and correctness over performance by default.

### License

Cnx uses the MIT license.

### Special Thanks

Special thanks should be given to the Rust team and C++ standardization committee, and contributors
of each, for creating great programming languages that have inspired a lot of the functionality
provided by this library. Thanks should also go
to [TotallyNotChase](https://github.com/TotallyNotChase/) for inspiring the design of the Trait
system and Iterators with [c-iterators](https://github.com/TotallyNotChase/c-iterators/)
and to [Hirrolot](https://github.com/Hirrolot/) for also inspiring the Trait system
with [interface99](https://github.com/Hirrolot/interface99).
