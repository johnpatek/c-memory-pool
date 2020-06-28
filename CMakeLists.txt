cmake_minimum_required(VERSION 2.8)

project(c-memory-pool)

if(WIN32)
    # Windows specific compiler flags
else()
    set(CMAKE_CXX_FLAGS  "-std=c++14")
endif(WIN32)

file(GLOB MEMORY_POOL_SOURCES
    "include/*.h"
    "src/*.c")

add_library(
    memorypool 
    STATIC 
    ${MEMORY_POOL_SOURCES})

target_include_directories(
    memorypool 
    PUBLIC 
    include)

if(DOCS)
    find_package(Doxygen)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

    doxygen_add_docs(
        docs
        ${PROJECT_SOURCE_DIR}/docs/mainpage.md
        ${PROJECT_SOURCE_DIR}/include/
        WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
    )
endif()