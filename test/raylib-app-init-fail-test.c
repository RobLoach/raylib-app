#include "raylib.h"
#include "raylib-assert.h"

#define RAYLIB_APP_IMPLEMENTATION
#include "raylib-app.h"

static bool closeCalled = false;

bool Init(void** userData, int argc, char** argv) {
    (void)userData;
    (void)argc;
    (void)argv;
    return false;
}

void Close(void* userData) {
    (void)userData;
    closeCalled = true;
    Assert(closeCalled);
}

App Main(void) {
    return (App) {
        .width = 640,
        .height = 480,
        .title = "raylib-app-init-fail-test",
        .init = Init,
        .close = Close,
    };
}
