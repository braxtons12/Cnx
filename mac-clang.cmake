SET(CMAKE_SYSTEM_NAME Darwin)
SET(CMAKE_C_COMPILER clang CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER clang++ CACHE INTERNAL "")

SET(CMAKE_C_FLAGS_DEBUG "")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")
SET(CMAKE_CXX_FLAGS_DEBUG "")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

SET(CMAKE_C_FLAGS_RELEASE "")
SET(CMAKE_C_FLAGS_RELEASE "-flto -Ofast -ffast-math -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "")
SET(CMAKE_CXX_FLAGS_RELEASE "-flto -Ofast -ffast-math -DNDEBUG")

SET(CMAKE_LINKER "ld.lld")
SET(CMAKE_AR "llvm-ar" CACHE PATH "AR" FORCE)
SET(CMAKE_RANLIB "llvm-ranlib" CACHE PATH "RANLIB" FORCE)
