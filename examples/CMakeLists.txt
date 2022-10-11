find_package(raylib)

# core_basic_window
add_executable(core_basic_window
    core_basic_window.c
)
target_link_libraries(core_basic_window PUBLIC
    raylib
    raylib-app
)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(core_basic_window PROPERTIES SUFFIX ".html")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html")
endif()
