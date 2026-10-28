cmake_minimum_required(VERSION 3.15)
project(VoidCore LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp" "src/*.c" "src/*.h" "libs/include/pugixml/pugixml.cpp")
file(GLOB SHADER_FILES
    "Assets/shaders/*.vert"
    "Assets/shaders/*.frag"
)

add_library(VOIDCORE STATIC
    ${ENGINE_SOURCES}
    ${SHADER_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/include/Tracy/TracyClient.cpp
)

target_include_directories(VOIDCORE PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/include
    include
)

target_compile_definitions(VOIDCORE PRIVATE STB_IMAGE_IMPLEMENTATION)

find_package(SDL3 REQUIRED)

if(WIN32)
    target_link_libraries(VOIDCORE PUBLIC
        opengl32.lib
        SDL3::SDL3
    )
else()
    target_link_libraries(VOIDCORE PUBLIC
        GL
        SDL3::SDL3
        pthread
        dl
    )
endif()

add_executable(Engine
    apps/sandbox/main.cpp
)

target_include_directories(Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/include
)

target_link_libraries(Engine PRIVATE
    VOIDCORE
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

set_target_properties(Engine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
