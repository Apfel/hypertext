# Copyright (c) 2020 Apfel
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software.
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.12.0)
project(hypertext LANGUAGES C VERSION 1.0.4)

include(CMakePackageConfigHelpers)

option(BUILD_SHARED "Builds hypertext as a shared library." OFF)
option(BUILD_TESTS  "Builds tests for hypertext."           OFF)

if(BUILD_SHARED)
    message("-- > Building mode set to shared.")
    set(BUILD_MODE SHARED)
else()
    message("-- > Building mode set to static.")
    set(BUILD_MODE STATIC)
endif()

if(BUILD_TESTS)
    message("-- > Tests enabled.")
else()
    message("-- > Tests disabled.")
endif()

add_library(hypertext ${BUILD_MODE}
    ${CMAKE_CURRENT_LIST_DIR}/Include/hypertext.h

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Internals.h
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Utilities.h

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Creation.c
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Fetching.c
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance.c
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Modifying.c
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Parsing.c
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Output.c
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Utilities.c
)

if(MSVC)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Sources/Windows/Manifest.rc.in ${CMAKE_CURRENT_BINARY_DIR}/Manifest.rc @ONLY NEWLINE_STYLE LF)
endif()

if(MSVC)
    target_compile_definitions(hypertext
        PRIVATE "WIN32_LEAN_AND_MEAN"
        PRIVATE "UNICODE"
        PRIVATE "_CRT_SECURE_NO_WARNINGS"
    )

    target_compile_options(hypertext
        PRIVATE "/W4"
        PRIVATE "/WX"
    )
else()
    target_compile_options(hypertext
        PRIVATE "-Wall"
        PRIVATE "-Werror"
        PRIVATE "-Wno-error=restrict"
        PRIVATE "-Wno-restrict"
        PRIVATE "-Wno-attributes"
    )
endif()

target_include_directories(hypertext PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Include>)

if(BUILD_SHARED AND MSVC)
    target_sources(hypertext PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Manifest.rc)
endif()

export(PACKAGE hypertext)

install(TARGETS hypertext EXPORT hypertextTargets LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin INCLUDES DESTINATION include)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/Include/hypertext.h DESTINATION include)

configure_file(CMake/hypertextConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/hypertextConfig.cmake" COPYONLY)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/hypertextConfigVersion.cmake" VERSION "${hypertext_VERSION_MAJOR}.${hypertext_VERSION_MINOR}.${hypertext_VERSION_PATCH}" COMPATIBILITY AnyNewerVersion)

install(EXPORT hypertextTargets FILE hypertextTargets.cmake DESTINATION lib/cmake/hypertext)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/hypertextConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/hypertextConfigVersion.cmake" DESTINATION lib/cmake/hypertext)

if(BUILD_TESTS)
    enable_testing()

    project(hypertext_test_response_creation C)
    add_executable(hypertext_test_response_creation ${CMAKE_CURRENT_LIST_DIR}/Tests/Creation/Response.c)
    if(MSVC)
        target_sources(hypertext_test_response_creation PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Manifest.rc)
    endif()
    target_link_libraries(hypertext_test_response_creation PRIVATE hypertext)
    add_test(hypertext hypertext_test_response_creation)

    project(hypertext_test_request_creation C)
    add_executable(hypertext_test_request_creation ${CMAKE_CURRENT_LIST_DIR}/Tests/Creation/Request.c)
    if(MSVC)
        target_sources(hypertext_test_request_creation PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Manifest.rc)
    endif()
    target_link_libraries(hypertext_test_request_creation PRIVATE hypertext)
    add_test(hypertext hypertext_test_request_creation)

    project(hypertext_test_response_parsing C)
    add_executable(hypertext_test_response_parsing ${CMAKE_CURRENT_LIST_DIR}/Tests/Parsing/Response.c)
    if(MSVC)
        target_sources(hypertext_test_response_parsing PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Manifest.rc)
    endif()
    target_link_libraries(hypertext_test_response_parsing PRIVATE hypertext)
    add_test(hypertext hypertext_test_response_parsing)

    project(hypertext_test_request_parsing C)
    add_executable(hypertext_test_request_parsing ${CMAKE_CURRENT_LIST_DIR}/Tests/Parsing/Request.c)
    if(MSVC)
        target_sources(hypertext_test_request_parsing PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Manifest.rc)
    endif()
    target_link_libraries(hypertext_test_request_parsing PRIVATE hypertext)
    add_test(hypertext hypertext_test_request_parsing)
endif()
