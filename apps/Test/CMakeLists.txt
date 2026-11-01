add_executable(Application main.cpp)

target_link_libraries(Application PRIVATE VOIDCORE)

set_target_properties(Application PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    OUTPUT_NAME "VoidCore Application"
    "${CMAKE_SOURCE_DIR}"
)
