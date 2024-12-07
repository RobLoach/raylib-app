#include "raylib.h"
#include "raylib-assert.h"

#define RAYLIB_APP_IMPLEMENTATION
#include "raylib-app.h"

typedef struct AppData {
    int frameCount;
    bool initCalled;
    bool updateCalled;
    bool closeCalled;
} AppData;

bool Init(void** userData, int argc, char** argv) {
    // Initialization
    //--------------------------------------------------------------------------------------
    (void)argc;
    (void)argv;

    // Create the user data.
    AppData* appData = MemAlloc(sizeof(AppData));
    if (appData == NULL) {
        return false;
    }

    *userData = (void*)appData;

    appData->initCalled = true;

    return true;
    //--------------------------------------------------------------------------------------
}

bool UpdateDrawFrame(void* userData) {
    // Update
    //----------------------------------------------------------------------------------
    AppData* appData = (AppData*)userData;

    if (appData == NULL) {
        return false;
    }

    appData->updateCalled = true;
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("raylib-app-test", 180, 200, 20, LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------

    // Close after 10 frames
    if (++appData->frameCount >= 10) {
        return false;
    }

    return true;
}

void Close(void* userData) {
    // De-Initialization
    //--------------------------------------------------------------------------------------
    AppData* appData = (AppData*)userData;
    appData->closeCalled = true;

    // Run the asserts, assuming all callbacks were executed.
    Assert(appData->initCalled);
    Assert(appData->updateCalled);
    Assert(appData->closeCalled);
    AssertEqual(appData->frameCount, 10, "Expected frame counter is not 10, it is %i", appData->frameCount);

    // Clean up the application data.
    MemFree(appData);
    //--------------------------------------------------------------------------------------
}

App Main() {
    // Create the application data.
    return (App) {
        .width = 640,
        .height = 480,
        .title = "raylib-app-test",
        .init = Init,
        .update = UpdateDrawFrame,
        .close = Close,
        .fps = 60,
    };
}
