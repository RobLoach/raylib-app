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

# raylib-app-init-fail-test: init() returning false must invoke close()
add_executable(raylib-app-init-fail-test raylib-app-init-fail-test.c)
target_compile_options(raylib-app-init-fail-test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion -Wstrict-prototypes)
target_link_libraries(raylib-app-init-fail-test PUBLIC
    raylib
    raylib-app
)
set_property(TARGET raylib-app-init-fail-test PROPERTY C_STANDARD 90)
set_property(TARGET raylib-app-init-fail-test PROPERTY C_STANDARD_REQUIRED ON)
add_test(NAME raylib-app-init-fail-test COMMAND raylib-app-init-fail-test)
set_tests_properties(raylib-app-init-fail-test PROPERTIES WILL_FAIL TRUE)
