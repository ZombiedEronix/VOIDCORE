cmake_minimum_required(VERSION 3.15)
project(EntropyEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCES})

file(GLOB SHADER_FILES
"Assets/shaders/*.vert"
"Assets/shaders/*.frag"
)
source_group("Shader Files" FILES ${SHADER_FILES})

add_executable(Engine 
    ${SOURCES} 
    ${HEADERS} 
    ${SHADER_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/include/Tracy/TracyClient.cpp
)

target_include_directories(Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/include
    include
)
# TRACY_ENABLE TRACY_MAX_FRAME_TIME=100
target_compile_definitions(Engine PRIVATE STB_IMAGE_IMPLEMENTATION )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

find_package(SDL3 REQUIRED)

if(WIN32)
    target_link_libraries(Engine PRIVATE 
        opengl32.lib 
        SDL3::SDL3
    )
else()
    target_link_libraries(Engine PRIVATE 
        GL
        SDL3::SDL3
        pthread
        dl
    )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

set_target_properties(Engine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
