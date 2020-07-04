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
    # check if Doxygen is installed
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif()

if(TEST)
    enable_testing()
    add_subdirectory(test)
    include(CTest)
endif()