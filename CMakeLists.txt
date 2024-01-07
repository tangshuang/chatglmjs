cmake_minimum_required(VERSION 3.12)

set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum OS X deployment version" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE STRING "")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

project(chatglmjs VERSION 0.0.1 LANGUAGES CXX)

##############

add_subdirectory(src/third_party/chatglm.cpp)

include_directories(
    ${CMAKE_JS_INC}
    src/third_party/chatglm.cpp/third_party/ggml/include/ggml
    src/third_party/chatglm.cpp/third_party/ggml/src
    src/third_party/chatglm.cpp/third_party/sentencepiece/src
    src/third_party/chatglm.cpp
    src
)

add_library(chatglmjs SHARED src/chat.cpp src/napi.cpp ${CMAKE_JS_SRC})
set_target_properties(chatglmjs PROPERTIES PREFIX "" SUFFIX ".node")
target_link_directories(chatglmjs PRIVATE chatglm ${CMAKE_JS_LIB})

# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(chatglmjs PRIVATE
    ${NODE_ADDON_API_DIR}
    ${CMAKE_JS_INC}
    src/third_party/chatglm.cpp/third_party/ggml/include/ggml
    src/third_party/chatglm.cpp/third_party/ggml/src
    src/third_party/chatglm.cpp/third_party/sentencepiece/src
    src/third_party/chatglm.cpp
    src
)

# define NAPI_VERSION
add_definitions(-DNAPI_VERSION=3)


add_executable(demo src/chat.cpp src/demo.cpp)
target_link_libraries(demo chatglm ${CMAKE_JS_LIB})

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    add_definitions("/wd4267 /wd4244 /wd4305 /Zc:strictStrings /utf-8")
endif ()
