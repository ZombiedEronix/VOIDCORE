file(GLOB_RECURSE VEDITOR_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_executable(VEditor ${VEDITOR_SOURCES})

target_link_libraries(VEditor PRIVATE
    VOIDCORE
)

target_include_directories(VEditor PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/..src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../libs/include"
)

voidcore_build_assets(VEditor "${CMAKE_CURRENT_SOURCE_DIR}/Assets")
