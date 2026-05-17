#include "raylib.h"
#include "raylib-assert.h"

#define RAYLIB_APP_IMPLEMENTATION
#include "raylib-app.h"

typedef struct AppData {
    int updateCount;
    int drawCount;
    bool initCalled;
    bool closeCalled;
} AppData;

bool Init(void** userData, int argc, char** argv) {
    AppData* appData;
    (void)argc;
    (void)argv;

    appData = (AppData*)MemAlloc(sizeof(AppData));
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

    appData->updateCount++;

    /* Stop after 10 frames. */
    return appData->updateCount < 10;
}

void Draw(void* userData) {
    AppData* appData = (AppData*)userData;
    if (appData == NULL) {
        return;
    }

    ClearBackground(RAYWHITE);
    appData->drawCount++;
}

void Close(void* userData) {
    AppData* appData = (AppData*)userData;

    appData->closeCalled = true;

    Assert(appData->initCalled);
    Assert(appData->closeCalled);

    /* Both update and draw should have been called exactly 10 times. */
    AssertEqual(appData->updateCount, 10, "Expected 10 update calls, got %i", appData->updateCount);
    AssertEqual(appData->drawCount, 10, "Expected 10 draw calls via wrapper, got %i", appData->drawCount);

    MemFree(appData);
}

App Main() {
    return (App) {
        .width = 640,
        .height = 480,
        .title = "raylib-app-draw-test",
        .fps = 60,
        .init = Init,
        .update = Update,
        .draw = Draw,
        .close = Close,
    };
}
