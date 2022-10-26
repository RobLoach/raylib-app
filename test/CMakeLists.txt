# raylib-app-test
add_executable(raylib-app-test raylib-app-test.c)
target_compile_options(raylib-app-test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(raylib-app-test PUBLIC
    raylib
    raylib-app
)

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME raylib-app-test COMMAND raylib-app-test)
