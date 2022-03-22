#ifndef STD_ARRAY_i32_10
#define STD_ARRAY_i32_10

#include <C2nxt/StdDef.h>
#define OPTION_INCLUDE_DEFAULT_INSTANTIATIONS TRUE
#include <C2nxt/StdOption.h>
#undef OPTION_INCLUDE_DEFAULT_INSTANTIATIONS

#include <C2nxt/StdBasicTypes.h>

#define ARRAY_T	   i32
#define ARRAY_N	   10
#define ARRAY_DECL TRUE
#include <C2nxt/StdArray.h>
#undef ARRAY_T
#undef ARRAY_N
#undef ARRAY_DECL

#endif // STD_ARRAY_i32_10
