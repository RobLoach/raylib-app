#include "raylib.h"
#include "raylib-assert.h"

#define RAYLIB_APP_IMPLEMENTATION
#include "raylib-app.h"

typedef struct AppData {
    int frameCount;
    int drawCount;
    bool initCalled;
    bool updateCalled;
    bool closeCalled;
} AppData;

bool Init(void** userData, int argc, char** argv) {
    AppData* appData;
    (void)argc;
    (void)argv;

    appData = MemAlloc(sizeof(AppData));
    if (appData == NULL) {
        return false;
    }

    *userData = (void*)appData;
    appData->initCalled = true;

    return true;
}

bool Update(void* userData) {
    AppData* appData = (AppData*)userData;

    if (appData == NULL) {
        return false;
    }

    appData->updateCalled = true;

    if (++appData->frameCount >= 10) {
        return false;
    }

    return true;
}

void Draw(void* userData) {
    AppData* appData = (AppData*)userData;
    if (appData == NULL) {
        return;
    }

    ClearBackground(RAYWHITE);
    DrawText("raylib-app-test", 180, 200, 20, LIGHTGRAY);
    appData->drawCount++;
}

void Close(void* userData) {
    AppData* appData = (AppData*)userData;
    appData->closeCalled = true;

    Assert(appData->initCalled);
    Assert(appData->updateCalled);
    Assert(appData->closeCalled);
    AssertEqual(appData->frameCount, 10, "Expected frame counter is not 10, it is %i", appData->frameCount);
    AssertEqual(appData->drawCount, appData->frameCount, "Expected draw count %i, got %i", appData->frameCount, appData->drawCount);

    MemFree(appData);
}

App Main(void) {
    // Create the application data.
    return (App) {
        .width = 640,
        .height = 480,
        .title = "raylib-app-test",
        .fps = 60,
        .init = Init,
        .update = Update,
        .draw = Draw,
        .close = Close,
    };
}
