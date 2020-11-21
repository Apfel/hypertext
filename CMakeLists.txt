# Copyright (c) 2020 The hypertext authors.
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

cmake_minimum_required(VERSION 3.2)
project(hypertext C)

option(BUILD_SHARED "Builds hypertext as a shared library." OFF)
option(BUILD_TESTS  "Builds tests for hypertext."           ON) # TODO: turn off later

if(BUILD_SHARED)
    set(BUILD_MODE SHARED)
else()
    set(BUILD_MODE STATIC)
endif()

if(MSVC)
    add_definitions(-DWIN32_LEAN_AND_MEAN -DUNICODE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /permissive- /WX /Wv:18")
endif()

set(SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Include/hypertext.h

    ${CMAKE_CURRENT_LIST_DIR}/Sources/Internals.h
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Utilities.h
    
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Creation.c
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Instance.c
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Parsing.c
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Output.c
    ${CMAKE_CURRENT_LIST_DIR}/Sources/Utilities.c
)

if(MSVC)
    set(ADDITIONAL_SOURCES ${CMAKE_CURRENT_LIST_DIR}/Sources/Windows/Manifest.rc)
    if(BUILD_SHARED)
        list(APPEND SOURCES ${ADDITIONAL_SOURCES})
    endif()
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/Include)

add_library(hypertext ${BUILD_MODE} ${SOURCES})

if(BUILD_SHARED)
    install(TARGETS hypertext RUNTIME DESTINATION bin)
endif()

install(TARGETS hypertext ARCHIVE DESTINATION lib)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/Include/hypertext.h DESTINATION include)

if(BUILD_TESTS)
    enable_testing()

    project(hypertext_test_response_creation C)
    add_executable(hypertext_test_response_creation
        ${CMAKE_CURRENT_LIST_DIR}/Tests/creation/Response.c
        ${ADDITIONAL_SOURCES}
    )
    target_link_libraries(hypertext_test_response_creation PUBLIC hypertext)
    add_test(hypertext hypertext_test_response_creation)

    project(hypertext_test_request_creation C)
    add_executable(hypertext_test_request_creation
        ${CMAKE_CURRENT_LIST_DIR}/Tests/Creation/Request.c
        ${ADDITIONAL_SOURCES}
    )
    target_link_libraries(hypertext_test_request_creation PUBLIC hypertext)
    add_test(hypertext hypertext_test_request_creation)

    project(hypertext_test_response_parsing C)
    add_executable(hypertext_test_response_parsing
        ${CMAKE_CURRENT_LIST_DIR}/Tests/Parsing/Response.c
        ${ADDITIONAL_SOURCES}
    )
    target_link_libraries(hypertext_test_response_parsing PUBLIC hypertext)
    add_test(hypertext hypertext_test_response_parsing)

    project(hypertext_test_request_parsing C)
    add_executable(hypertext_test_request_parsing
        ${CMAKE_CURRENT_LIST_DIR}/Tests/Parsing/Request.c
        ${ADDITIONAL_SOURCES}
    )
    target_link_libraries(hypertext_test_request_parsing PUBLIC hypertext)
    add_test(hypertext hypertext_test_request_parsing)
endif()
