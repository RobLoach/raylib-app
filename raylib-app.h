/**********************************************************************************************
*
*   raylib-app v2.0.0 - Application wrapper for raylib.
*
*       https://github.com/RobLoach/raylib-app
*
*   DEPENDENCIES:
*       - raylib 6.0+ https://www.raylib.com
*
*   LICENSE: zlib/libpng
*
*   raylib-app is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2026 Rob Loach (@RobLoach)
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

#define RAYLIB_APP_VERSION_MAJOR 2
#define RAYLIB_APP_VERSION_MINOR 0
#define RAYLIB_APP_VERSION_PATCH 0
#define RAYLIB_APP_VERSION "2.0.0"

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
     * If init() returns false, close() is NOT called, so init() must clean up
     * any partial allocations it made before returning false.
     *
     * @return True if initialization was successful.
     */
    bool (*init)(void** userData, int argc, char** argv);

    /**
     * The update callback for per-frame logic.
     *
     * @param userData The App information for the currently running application.
     *
     * @return True if the application is to continue running, false otherwise.
     */
    bool (*update)(void* userData);

    /**
     * The draw callback for rendering. Called each frame after update().
     *
     * BeginDrawing() and EndDrawing() are called by the wrapper around draw().
     *
     * @param userData The App information for the currently running application.
     */
    void (*draw)(void* userData);

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

#if !defined(RAYLIB_APP_NO_ENTRY)
/**
 * The main entry point defining the application behavior.
 *
 * @see App
 * @example examples/raylib-app-example.c
 *
 * @return The App description for your Application.
 */
extern App Main(void);
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(PLATFORM_WEB)
/**
 * The update callback for web.
 *
 * WindowShouldClose() is not called here because it is always false on web,
 * and raylib's web backend implements it with emscripten_sleep() which
 * requires ASYNCIFY (breaks JS->wasm callbacks).
 */
void RaylibAppWebUpdate(void* app) {
    App* application = (App*)app;
    if (application == NULL) {
        return;
    }

    // Call the update function.
    if (application->update != NULL) {
        if (!application->update(application->userData)) {
            if (application->close != NULL) {
                application->close(application->userData);
            }
            CloseWindow();
            emscripten_cancel_main_loop();
            return;
        }
    }

    // Call the draw function.
    if (application->draw != NULL) {
        BeginDrawing();
        application->draw(application->userData);
        EndDrawing();
    }
}
#endif

/**
 * Run the application lifecycle: init window, call callbacks, run the loop.
 *
 * This is available regardless of RAYLIB_APP_NO_ENTRY, so users who provide
 * their own main() can still use raylib-app's lifecycle management.
 *
 * @param app The application description.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 *
 * @return 0 on success, 1 on failure.
 */
int RaylibAppRun(App* app, int argc, char* argv[]) {
    if (app == NULL) {
        return 1;
    }

    // Config Flags
    if (app->configFlags != 0) {
        SetConfigFlags(app->configFlags);
    }

    // Initialize
    InitWindow(app->width, app->height, app->title);

    // Error checking
    if (!IsWindowReady()) {
        return 1;
    }

    // Handle the FPS
    if (app->fps < 0) {
        app->fps = 0;
    }
#if !defined(PLATFORM_WEB)
    // On web the loop is driven by requestAnimationFrame (see below), so
    // SetTargetFPS() is skipped: it would make raylib's EndDrawing() block the
    // browser's main thread with WaitTime()/nanosleep() every frame.
    if (app->fps > 0) {
        SetTargetFPS(app->fps);
    }
#endif

    // Call the init callback.
    if (app->init != NULL) {
        if (!app->init(&app->userData, argc, argv)) {
            // close() is not called here: init() failed, so there is nothing
            // fully initialized to tear down. init() is responsible for
            // cleaning up any of its own partial allocations on failure.
            CloseWindow();
            return 1;
        }
    }

    // Start the update loop
    if (app->update != NULL || app->draw != NULL) {
#if defined(PLATFORM_WEB)
        // fps > 0: emscripten throttles cooperatively via setTimeout, honoring
        // the requested target without blocking the browser's main thread.
        // fps == 0: emscripten uses requestAnimationFrame (display refresh rate).
        emscripten_set_main_loop_arg(RaylibAppWebUpdate, app, app->fps, 1);
#else
        while (!WindowShouldClose()) {
            if (app->update != NULL) {
                if (!app->update(app->userData)) {
                    break;
                }
            }
            if (app->draw != NULL) {
                BeginDrawing();
                app->draw(app->userData);
                EndDrawing();
            }
        }
#endif
    }

    // Close the App and Window
    if (app->close != NULL) {
        app->close(app->userData);
    }

    CloseWindow();

    return 0;
}

#if !defined(RAYLIB_APP_NO_ENTRY)
/**
 * The main entry point for raylib-app.
 */
int main(int argc, char* argv[]) {
    App app = Main();
    return RaylibAppRun(&app, argc, argv);
}
#endif // RAYLIB_APP_NO_ENTRY

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_APP_IMPLEMENTATION_ONCE
#endif // RAYLIB_APP_IMPLEMENTATION
