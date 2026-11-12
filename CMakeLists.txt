cmake_minimum_required(VERSION 3.15)
project(VoidCore LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#=================================================================================

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

find_package(SDL3 REQUIRED)
target_link_libraries(VOIDCORE PUBLIC
    GL
    lz4
    SDL3::SDL3
    pthread
    dl
)

#=================================================================================

target_compile_definitions(VOIDCORE PRIVATE 
    STB_IMAGE_IMPLEMENTATION
)

add_subdirectory(Tools/VoidPacker)