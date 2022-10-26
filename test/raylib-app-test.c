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

void Init(App* app) {
    // Initialization
    //--------------------------------------------------------------------------------------
    AppData* appData = (AppData*)app->userData;
    appData->initCalled = true;
    //--------------------------------------------------------------------------------------
}

void UpdateDrawFrame(App* app) {
    // Update
    //----------------------------------------------------------------------------------
    AppData* appData = (AppData*)app->userData;
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
        CloseApp(app);
    }
}

void Close(App* app) {
    // De-Initialization
    //--------------------------------------------------------------------------------------
    AppData* appData = (AppData*)app->userData;
    appData->closeCalled = true;

    // Run the asserts, assuming all callbacks were executed.
    Assert(appData->initCalled);
    Assert(appData->updateCalled);
    Assert(appData->closeCalled);
    AssertEqual(appData->frameCount, 10, "Expected frame counter is not 10, it is %i", appData->frameCount);

    // Clean up the application data.
    MemFree(appData);

    // Since the callbacks were called, close the application without error.
    app->exitStatus = 0;
    //--------------------------------------------------------------------------------------
}

App Main(int argc, char* argv[]) {
    // Ignore unused arguments.
    (void)argc;
    (void)argv;

    // Create the user data.
    AppData* appData = MemAlloc(sizeof(AppData));

    // Create the application data.
    return (App) {
        .width = 640,
        .height = 480,
        .title = "raylib-app-test",
        .init = Init,
        .update = UpdateDrawFrame,
        .close = Close,
        .fps = 60,
        .userData = appData,

        // Assume an error, unless the callbacks were called.
        .exitStatus = 1
    };
}
