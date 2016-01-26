cmake_minimum_required(VERSION 3.3)
project(Galeon)

if(CMAKE_BUILD_TYPE EQUAL Debug)
    add_definitions(-D_DEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

if (UNIX)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang")
#set(CMAKE_C_LINK_EXECUTABLE "lld")
#set(CMAKE_CXX_LINK_EXECUTABLE "llvm-link")
endif()

set(CMAKE_CXX_FLAGS -std=c++14)

set(CMAKE_PREFIX_PATH "dependencies")

add_subdirectory(Src)
