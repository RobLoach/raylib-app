# raylib-app

Application wrapper for [raylib](https://raylib.com).

## Why

Building raylib for both the desktop and web need some subtle differences with checks against `PLATFORM`. *raylib-app* aims to clean up those differences, and make the code easier to read for a unified application entry. This pattern is inspired by [sokol_app](https://github.com/floooh/sokol#sokol_apph) and [SDL3's Main Functions](https://wiki.libsdl.org/SDL3/README/main-functions).

## Usage

``` c
#include "raylib.h"

#define RAYLIB_APP_IMPLEMENTATION
#include "raylib-app.h"

bool Init(void** userData, int argc, char** argv) {
    // InitWindow() is automatically called prior to this function.
    return true;
}

bool Update(void* userData) {
    // Return false to exit the application.
    return !IsKeyDown(KEY_Q);
}

void Draw(void* userData) {
    // BeginDrawing() and EndDrawing() are called automatically by raylib-app.
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first raylib-app!", 180, 200, 20, LIGHTGRAY);
}

void Close(void* userData) {
    // CloseWindow() is automatically called after this function completes.
}

App Main(void) {
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
```

## API

Rather than having your own `int main()`, you will define your own `App Main(void)` function.

``` c
App Main(void) {
    return (App) {
        .width = 800,                          // The width of the window
        .height = 450,                         // The height of the window
        .title = "raylib-app",                 // The window title
        .init = Init,                          // Called once after InitWindow(); receives argc/argv
        .update = Update,                      // Called each frame; return false to exit
        .draw = Draw,                          // Called each frame; BeginDrawing/EndDrawing are automatic
        .close = Close,                        // Called before CloseWindow()
        .fps = 60,                             // The target frames-per-second
        .configFlags = FLAG_WINDOW_RESIZABLE   // The flags that are passed to SetConfigFlags()
    };
}
```

### Callbacks

| Callback | Signature | Description |
|----------|-----------|-------------|
| `init`   | `bool (*init)(void** userData, int argc, char** argv)` | Called once after `InitWindow()`. Return `false` to abort. |
| `update` | `bool (*update)(void* userData)` | Called each frame for logic. Return `false` to exit. |
| `draw`   | `void (*draw)(void* userData)` | Called each frame for rendering. `BeginDrawing()`/`EndDrawing()` are called automatically. |
| `close`  | `void (*close)(void* userData)` | Called before `CloseWindow()` for cleanup. |

All callbacks are optional. `update` and `draw` can be used independently or together.

## Development

There are a few ways to build raylib-app.

### Desktop

``` bash
cmake -B build
cmake --build build
```

### Web

``` bash
mkdir build
cd build
emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3"
emmake make
```

## License

*raylib-app* is licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.
