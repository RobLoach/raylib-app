#include "raylib.h"

#define RAYLIB_APP_IMPLEMENTATION
#include "raylib-app.h"

bool Init(void** userData, int argc, char** argv) {
    // Initialization
    //--------------------------------------------------------------------------------------
    // InitWindow() is automatically called before this function is called.
    //--------------------------------------------------------------------------------------

    return true;
}

bool UpdateDrawFrame(void* userData) {
    // Update
    //----------------------------------------------------------------------------------
    if (IsKeyDown(KEY_Q)) {
        return false;
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first raylib-app!", 180, 200, 20, LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------

    // Return false to end the update loop.
    return true;
}

void Close(void* userData) {
    // De-Initialization
    //--------------------------------------------------------------------------------------
    // CloseWindow() is automatically called after this function completes.
    //--------------------------------------------------------------------------------------
}

App Main() {
    return (App) {
        .width = 800,
        .height = 450,
        .title = "raylib-app [core] example - basic window",
        .init = Init,
        .update = UpdateDrawFrame,
        .close = Close,
        .fps = 60,
    };
}
