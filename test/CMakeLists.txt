# raylib-app-test
add_executable(raylib-app-test raylib-app-test.c)
target_compile_options(raylib-app-test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(raylib-app-test PUBLIC
    raylib
    raylib-app
)

set_property(TARGET raylib-app-test PROPERTY C_STANDARD 90)
set_property(TARGET raylib-app-test PROPERTY C_STANDARD_REQUIRED ON)

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME raylib-app-test COMMAND raylib-app-test)

# raylib-app-draw-test: covers the wrapper-managed draw path (BeginDrawing/EndDrawing)
add_executable(raylib-app-draw-test raylib-app-draw-test.c)
target_compile_options(raylib-app-draw-test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(raylib-app-draw-test PUBLIC
    raylib
    raylib-app
)

set_property(TARGET raylib-app-draw-test PROPERTY C_STANDARD 90)
set_property(TARGET raylib-app-draw-test PROPERTY C_STANDARD_REQUIRED ON)

add_test(NAME raylib-app-draw-test COMMAND raylib-app-draw-test)
