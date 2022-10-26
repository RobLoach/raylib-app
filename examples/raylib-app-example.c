#include "raylib.h"

#define RAYLIB_APP_IMPLEMENTATION
#include "raylib-app.h"

void Init(App* app) {
    // Initialization
    //--------------------------------------------------------------------------------------

    // InitWindow() is automatically called before this function is called.

    //--------------------------------------------------------------------------------------
}

void UpdateDrawFrame(App* app) {
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first raylib-app!", 180, 200, 20, LIGHTGRAY);

        if (IsKeyDown(KEY_Q)) {
            DrawCircle(300,200, 200, RED);
        }

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void Close(App* app) {
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
        .update = UpdateDrawFrame,
        .close = Close,
        .fps = 60,
    };
}
