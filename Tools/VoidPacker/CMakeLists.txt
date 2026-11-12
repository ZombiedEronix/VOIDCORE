add_executable(VoidPacker main.cpp)

target_link_libraries(VoidPacker PRIVATE VOIDCORE)

target_include_directories(VoidPacker PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../libs"
)