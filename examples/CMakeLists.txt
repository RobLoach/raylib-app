find_package(raylib)

# raylib-app-example
add_executable(raylib-app-example
    raylib-app-example.c
)
target_link_libraries(raylib-app-example PUBLIC
    raylib
    raylib-app
)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(raylib-app-example PROPERTIES SUFFIX ".html")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html")
endif()
