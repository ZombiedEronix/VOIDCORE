add_executable(VoidPacker main.cpp)

find_package(PkgConfig REQUIRED)
target_link_libraries(VoidPacker PRIVATE
    VOIDCORE 
    PkgConfig::SHADERC
)



target_include_directories(VoidPacker PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../libs"
    /usr/include/spirv/unified1
)