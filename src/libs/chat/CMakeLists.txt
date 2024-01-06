cmake_minimum_required(VERSION 3.12)
project(chat VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE STRING "")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()


############



include_directories(${PROJECT_BINARY_DIR})

include_directories(third_party/chatglm.cpp/third_party/ggml/include/ggml)
include_directories(third_party/chatglm.cpp/third_party/ggml/src)
include_directories(third_party/chatglm.cpp/third_party/sentencepiece/src)
include_directories(third_party/chatglm.cpp)
add_subdirectory(third_party/chatglm.cpp)

file(GLOB CPP_SOURCES
    ${PROJECT_SOURCE_DIR}/chat.h
    ${PROJECT_SOURCE_DIR}/chat.cpp)

set_source_files_properties(${CPP_SOURCES} PROPERTIES COMPILE_FLAGS "-pedantic-errors")

add_library(chat STATIC chat.cpp)
target_link_libraries(chat chatglm)