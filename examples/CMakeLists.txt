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

    # --shell-file is a link-time flag; passing it at compile time is ignored.
    target_link_options(raylib-app-example PRIVATE
        "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
        # Drop runtime glue that a browser-only build never uses.
        "-sENVIRONMENT=web"
        "-sFILESYSTEM=0"
        "-sMALLOC=emmalloc"
    )

    # Optimize release web builds. Emscripten defaults to -O0, which is the
    # single biggest runtime-performance lever for the web.
    target_compile_options(raylib-app-example PRIVATE
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
    )
    target_link_options(raylib-app-example PRIVATE
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
        $<$<CONFIG:Release>:--closure=1>
    )
endif()
