cmake_minimum_required(VERSION 3.12)
project(ChatGLM.cpp VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE STRING "")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# third-party libraries
add_compile_definitions(GGML_CUDA_MMV_Y=2)  # for large vocab
include_directories(third_party/ggml/include/ggml third_party/ggml/src)
add_subdirectory(third_party/ggml)

set(SPM_ENABLE_SHARED OFF CACHE BOOL "chatglm: disable sentencepiece shared libraries by default")
set(SPM_ENABLE_TCMALLOC OFF CACHE BOOL "chatglm: disable tcmalloc by default")
include_directories(third_party/sentencepiece/src)
add_subdirectory(third_party/sentencepiece)

if (GGML_CUBLAS)
    add_compile_definitions(GGML_USE_CUBLAS)
    set(CUDA_ARCHITECTURES "52;61;70;75;80;86" CACHE STRING "chatglm: cuda architectures to compile")
    set_property(TARGET ggml PROPERTY CUDA_ARCHITECTURES ${CUDA_ARCHITECTURES})
endif ()

if (GGML_METAL)
    add_compile_definitions(GGML_USE_METAL)
    configure_file(third_party/ggml/src/ggml-metal.metal ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/ggml-metal.metal COPYONLY)
endif ()

if (GGML_PERF)
    add_compile_definitions(GGML_PERF)
endif ()

file(GLOB CPP_SOURCES
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.cpp)

set_source_files_properties(${CPP_SOURCES} PROPERTIES COMPILE_FLAGS "-pedantic-errors")

add_library(chatglm STATIC chatglm.cpp)
target_link_libraries(chatglm PUBLIC ggml sentencepiece-static)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    add_definitions("/wd4267 /wd4244 /wd4305 /Zc:strictStrings /utf-8")
endif ()
