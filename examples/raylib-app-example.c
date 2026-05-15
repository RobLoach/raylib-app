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

bool Update(void* userData) {
    // Update
    //----------------------------------------------------------------------------------
    if (IsKeyDown(KEY_Q)) {
        return false;
    }
    //----------------------------------------------------------------------------------

    return true;
}

void Draw(void* userData) {
    // Draw
    //----------------------------------------------------------------------------------
    // BeginDrawing() and EndDrawing() are called automatically by raylib-app.
    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first raylib-app!", 180, 200, 20, LIGHTGRAY);
    //----------------------------------------------------------------------------------
}

void Close(void* userData) {
    // De-Initialization
    //--------------------------------------------------------------------------------------
    // CloseWindow() is automatically called after this function completes.
    //--------------------------------------------------------------------------------------
}

App Main(int argc, char* argv[]) {
    return (App) {
        .width = 800,
        .height = 450,
        .title = "raylib-app [core] example - basic window",
        .init = Init,
        .update = Update,
        .draw = Draw,
        .close = Close,
        .fps = 60,
    };
}
