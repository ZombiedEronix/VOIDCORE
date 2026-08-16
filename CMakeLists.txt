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

add_executable(Engine ${SOURCES} ${HEADERS} ${SHADER_FILES})

set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)

target_include_directories(Engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LIBS_DIR}/glfw/include
    ${LIBS_DIR}/glm/include
    ${LIBS_DIR}/glad/include
    ${LIBS_DIR}/stb/include
    ${LIBS_DIR}/KHR/include
    ${LIBS_DIR}/objparser/obj
)

target_compile_definitions(Engine PRIVATE STB_IMAGE_IMPLEMENTATION)

if(WIN32)
    target_link_libraries(Engine PRIVATE 
        opengl32.lib 
        ${LIBS_DIR}/glfw/lib/Debug/glfw3.lib
    )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

set(CMAKE_CXX_COMPILER "clang-cl")
set(CMAKE_C_COMPILER "clang-cl")

set_target_properties(Engine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")