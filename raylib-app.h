/**********************************************************************************************
*
*   raylib-app v0.0.3 - Application wrapper for raylib.
*
*       https://github.com/RobLoach/raylib-app
*
*   DEPENDENCIES:
*       - raylib 5.x https://www.raylib.com
*
*   LICENSE: zlib/libpng
*
*   raylib-app is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2022 Rob Loach (@RobLoach)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef RAYLIB_APP_H_
#define RAYLIB_APP_H_

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Application data that is used to manage the window.
 */
typedef struct App {
    /**
     * The initiatialization callback for the application.
     *
     * InitWindow() will be called prior to init() being called.
     *
     * @param userData If the application uses custom data, initialize the memory here.
     * @param argc The number of arguments passed through the command line.
     * @param argv A string array of command line arguments.
     *
     * @return True if initialization was successful.
     */
    bool (*init)(void** userData, int argc, char** argv);

    /**
     * The update callback to update and draw the application.
     *
     * @param userData The App information for the currently running application.
     *
     * @return True if the application is to continue running, false otherwise.
     */
    bool (*update)(void* userData);

    /**
     * The close callback used to deinitialize all application data.
     *
     * CloseWindow() is called after the callback is run.
     *
     * @param userData The App information for the currently running application.
     */
    void (*close)(void* userData);

    /**
     * The desired width of the application window.
     */
    int width;

    /**
     * The desired height of the application window.
     */
    int height;

    /**
     * The title of the application window.
     */
    const char* title;

    /**
     * Configuration flags that are set prior to initializing the window.
     *
     * @see ConfigFlags
     * @see SetConfigFlags()
     */
    unsigned int configFlags;

    /**
     * The target frames-per-second that will be used for the application.
     *
     * @see SetTargetFPS()
     */
    int fps;

    /**
     * Custom user data that is passed through the application callbacks.
     *
     * This is helpful if you have context that needs to be passed through all the callbacks.
     *
     * @example test/raylib-app-test.c
     */
    void* userData;
} App;

#if defined(__cplusplus)
}
#endif

#endif  // RAYLIB_APP_H_

#ifdef RAYLIB_APP_IMPLEMENTATION
#ifndef RAYLIB_APP_IMPLEMENTATION_ONCE
#define RAYLIB_APP_IMPLEMENTATION_ONCE

// raylib.h
#ifndef RAYLIB_APP_RAYLIB_H
#define RAYLIB_APP_RAYLIB_H "raylib.h"
#endif
#include RAYLIB_APP_RAYLIB_H

// emscripten.h
#if defined(PLATFORM_WEB)
#ifndef RAYLIB_APP_EMSCRIPTEN_H
#define RAYLIB_APP_EMSCRIPTEN_H "emscripten.h"
#endif
#include RAYLIB_APP_EMSCRIPTEN_H
#endif

#ifndef NULL
#include <stddef.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(RAYLIB_APP_NO_ENTRY)
/**
 * The main entry point defining the application behavior.
 *
 * @see App
 * @example examples/core_basic_window.c
 *
 * @return The App description for your Application.
 */
extern App Main();
#endif

#if defined(PLATFORM_WEB)
/**
 * The update callback for web.
 */
void RaylibAppWebUpdate(void* app) {
    App* application = (App*)app;
    if (application == NULL) {
        return;
    }

    // Call the update function.
    if (application->update != NULL) {
        if (!application->update(application->userData)) {
            emscripten_cancel_main_loop();
        }
    }
}
#endif

#if !defined(RAYLIB_APP_NO_ENTRY)
/**
 * The main entry point for raylib-app.
 */
int main(int argc, char* argv[]) {
    // Get the user-defined App from their Main() function.
    App app = Main();

    // Config Flags
    if (app.configFlags != 0) {
        SetConfigFlags(app.configFlags);
    }

    // Initialize
    InitWindow(app.width, app.height, app.title);

    // Error checking
    if (!IsWindowReady()) {
        return 1;
    }

    // Handle the FPS
    if (app.fps > 0) {
        SetTargetFPS(app.fps);
    }
    else {
        app.fps = 0;
    }

    // Call the init callback.
    if (app.init != NULL) {
        if (!app.init(&app.userData, argc, argv)) {
            CloseWindow();
            return 1;
        }
    }

    // Start the update loop
    if (app.update != NULL) {
#if defined(PLATFORM_WEB)
        emscripten_set_main_loop_arg(RaylibAppWebUpdate, &app, app.fps, 1);
#else
        // Stop running if the Window or App have been told to close.
        while (!WindowShouldClose()) {
            if (!app.update(app.userData)) {
                break;
            }
        }
#endif
    }

    // Close the App and Window
    if (app.close != NULL) {
        app.close(app.userData);
    }

    CloseWindow();

    return 0;
}
#endif // RAYLIB_APP_NO_ENTRY

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_APP_IMPLEMENTATION_ONCE
#endif // RAYLIB_APP_IMPLEMENTATION
