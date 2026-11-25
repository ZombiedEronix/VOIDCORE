cmake_minimum_required(VERSION 3.20)
project(VoidCore LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SHADERC REQUIRED IMPORTED_TARGET shaderc)

#=================================================================================

file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp" "src/*.c" "src/*.h" "libs/include/pugixml/pugixml.cpp")


add_library(VOIDCORE STATIC
    ${ENGINE_SOURCES}
    ${SHADER_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/include/Tracy/TracyClient.cpp
)

target_include_directories(VOIDCORE PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/include
    ${SHADERC_INCLUDE_DIRS}
    ThirdParty/SPIRV-Reflect
    include
)

target_link_libraries(VOIDCORE PUBLIC
    GL
    lz4
    yaml-cpp::yaml-cpp
    SDL3::SDL3
    pthread
    dl
)

#=================================================================================

target_compile_definitions(VOIDCORE PRIVATE 
    STB_IMAGE_IMPLEMENTATION
)

add_subdirectory(Tools/VoidPacker)

function(voidcore_build_assets TARGET_NAME GAME_ASSETS_DIR)
    set(ENGINE_ASSETS_DIR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Assets")
    set(PAK_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/gamedata.vpak")

    file(GLOB_RECURSE ENGINE_ASSET_FILES CONFIGURE_DEPENDS LIST_DIRECTORIES false "${ENGINE_ASSETS_DIR}/*")
    file(GLOB_RECURSE GAME_ASSET_FILES CONFIGURE_DEPENDS LIST_DIRECTORIES false "${GAME_ASSETS_DIR}/*")

    add_custom_command(
        OUTPUT "${PAK_OUTPUT}"
        COMMAND $<TARGET_FILE:VoidPacker>
            --engine-assets "${ENGINE_ASSETS_DIR}"
            --game-assets   "${GAME_ASSETS_DIR}"
            --output        "${PAK_OUTPUT}"
        DEPENDS VoidPacker ${ENGINE_ASSET_FILES} ${GAME_ASSET_FILES}
        COMMENT "Packing assets into gamedata.vpak for ${TARGET_NAME}..."
    )

    add_custom_target(${TARGET_NAME}_Assets ALL DEPENDS "${PAK_OUTPUT}")
    add_dependencies(${TARGET_NAME} ${TARGET_NAME}_Assets)
endfunction()
