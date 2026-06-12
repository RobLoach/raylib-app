cmake_minimum_required(VERSION 3.11)
project(raylib-app
    DESCRIPTION "raylib-app: Application wrapper for raylib."
    HOMEPAGE_URL "https://github.com/robloach/raylib-app"
    VERSION 2.0.0
    LANGUAGES C
)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Register the cmake folder for find_package()
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# raylib-app
add_library(raylib-app INTERFACE)
add_library(raylib-app::raylib-app ALIAS raylib-app)
target_include_directories(raylib-app INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Install
install(TARGETS raylib-app
    EXPORT raylib-appTargets
)
install(FILES raylib-app.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT raylib-appTargets
    FILE raylib-appTargets.cmake
    NAMESPACE raylib-app::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/raylib-app
)
configure_package_config_file(
    cmake/raylib-appConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/raylib-appConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/raylib-app
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/raylib-appConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/raylib-appConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/raylib-appConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/raylib-app
)

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(RAYLIB_APP_IS_MAIN TRUE)
else()
    set(RAYLIB_APP_IS_MAIN FALSE)
endif()
option(RAYLIB_APP_BUILD_EXAMPLES "Build Examples" ${RAYLIB_APP_IS_MAIN})

# Examples
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
