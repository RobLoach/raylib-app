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
| `init`   | `bool (*init)(void** userData, int argc, char** argv)` | Called once after `InitWindow()`. Return `false` to abort; `close()` is *not* called on failure, so clean up any partial allocations yourself. |
| `update` | `bool (*update)(void* userData)` | Called each frame for logic. Return `false` to exit. |
| `draw`   | `void (*draw)(void* userData)` | Called each frame for rendering. `BeginDrawing()`/`EndDrawing()` are called automatically. |
| `close`  | `void (*close)(void* userData)` | Called before `CloseWindow()` for cleanup. |

All callbacks are optional. `update` and `draw` can be used independently or together.

## Development

*raylib-app* targets C99 (or newer), and requires raylib 6.0+.

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

#### Web Performance

On the web, *raylib-app* honors the requested `.fps`: a positive value is throttled cooperatively by emscripten via `setTimeout`, and `0` falls back to `requestAnimationFrame` (the display refresh rate). It skips `SetTargetFPS()` on the web so it won't block the browser's main thread with a busy/sleep wait.

Because `.fps = 0` runs at the display refresh rate (which exceeds 60 Hz on high-refresh monitors), drive any motion with `GetFrameTime()` rather than assuming a fixed timestep, so the app behaves consistently across frame rates.

For the rest, performance on the web is mostly determined by your build flags:

- Build with `-DCMAKE_BUILD_TYPE=Release`. Emscripten defaults to `-O0`, so
  this (with `-O3`) is the single biggest runtime-performance win. Use `-Os` or `-Oz` instead if download size matters more than speed
- `-flto`: link-time optimization, pairs well with `-O3`
- `--closure=1`: minifies the generated JS glue for faster startup
- `-sENVIRONMENT=web`: drops the Node/worker glue a browser build never uses
- `-sFILESYSTEM=0`: removes the filesystem runtime if the app does no file I/O
- `-sMALLOC=emmalloc`: a smaller, faster allocator for simple apps

See [examples/CMakeLists.txt](examples/CMakeLists.txt) for these applied to the example's web build.

## License

*raylib-app* is licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.
