/**********************************************************************************************
*
*   raylib-app v0.0.1 - Application wrapper for raylib.
*
*       https://github.com/RobLoach/raylib-app
*
*   DEPENDENCIES:
*       - raylib 4.x https://www.raylib.com
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

typedef struct App App;

struct App {
    void (*init)(App* app);
    void (*update)(App* app);
    void (*close)(App* app);

    int width;
    int height;
    const char* title;
    int configFlags;
    int fps;
    bool shouldClose;
    int exitStatus;

    void* userData;
};

void CloseApp(App* app);        // Tells the application that it should close.

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
 * Define a Main() function as your entry point of your application.
 *
 * @param argc The length of the argument vector.
 * @param argv The array of arguments.
 * @see App
 * @example examples/core_basic_window.c
 *
 * @return The App description for your Application.
 */
extern App Main(int argc, char* argv[]);
#endif

/**
 * Informs the application that it should close.
 *
 * @param app The application that should close.
 */
void CloseApp(App* app) {
    app->shouldClose = true;
}

#if defined(PLATFORM_WEB)
/**
 * The update callback for web.
 */
void RaylibAppWebUpdate(void* app) {
    App* application = (App*)app;
    if (application == NULL) {
        return;
    }

    if (application->shouldClose) {
        // Tell emscripten that it should stop doing the main loop.
        emscripten_cancel_main_loop();
        return;
    }

    // Call the update function.
    if (application->update != NULL) {
        application->update(application);
    }
}
#endif

#if !defined(RAYLIB_APP_NO_ENTRY)
/**
 * The main entry point for raylib-app.
 */
int main(int argc, char* argv[]) {
    // Get the user-defined App from their Main() function.
    App app = Main(argc, argv);

    // Allow exiting early if desired.
    if (app.exitStatus != 0) {
        return app.exitStatus;
    }

    // Config Flags
    if (app.configFlags != 0) {
        SetConfigFlags(app.configFlags);
    }

    // Initialize
    InitWindow(app.width, app.height, app.title);
    if (app.fps > 0) {
        SetTargetFPS(app.fps);
    }
    else {
        app.fps = 0;
    }

    // Call the init callback.
    if (app.init != NULL) {
        app.init(&app);
    }

    // Start the update loop
    if (app.update != NULL) {
#if defined(PLATFORM_WEB)
        emscripten_set_main_loop_arg(RaylibAppWebUpdate, &app, app.fps, 1);
#else
        // Stop running if the Window or App have been told to close.
        while (!WindowShouldClose() && !app.shouldClose) {
            app.update(&app);
        }
#endif
    }

    // Close the App and Window
    if (app.close != NULL) {
        app.close(&app);
    }
    CloseWindow();

    return app.exitStatus;
}
#endif // RAYLIB_APP_NO_ENTRY

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_APP_IMPLEMENTATION_ONCE
#endif // RAYLIB_APP_IMPLEMENTATION
