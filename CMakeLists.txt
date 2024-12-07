cmake_minimum_required(VERSION 3.11)
project(raylib-app
    DESCRIPTION "raylib-app: Application wrapper for raylib."
    HOMEPAGE_URL "https://github.com/robloach/raylib-app"
    VERSION 1.0.0
    LANGUAGES C
)

# Register the cmake folder for find_package()
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# raylib-app
add_library(raylib-app INTERFACE)
target_include_directories(raylib-app INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(RAYLIB_APP_IS_MAIN TRUE)
else()
    set(RAYLIB_APP_IS_MAIN FALSE)
endif()
option(RAYLIB_APP_BUILD_EXAMPLES "Build Examples" ${RAYLIB_APP_IS_MAIN})

# raylib-umka
if (RAYLIB_APP_BUILD_EXAMPLES)
    add_subdirectory(examples)

    # Testing
    include(CTest)
    enable_testing()
    if (BUILD_TESTING)
        # set(CTEST_CUSTOM_TESTS_IGNORE
        #     pkg-config--static
        # )
        add_subdirectory(test)
    endif()
endif()
