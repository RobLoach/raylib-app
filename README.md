# raylib-app

Application wrapper for [raylib](https://raylib.com).

## Why

Building raylib for both the desktop and web need some subtle differences with checks against `PLATFORM`. *raylib-app* aims to clean up those differences, and make the code easier to read for a unified application entry.

## Usage

``` c
#include "raylib.h"

#define RAYLIB_APP_IMPLEMENTATION
#include "raylib-app.h"

void Init(App* app) {
    // InitWindow() is automatically called prior to this function.
}

void Update(App* app) {
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first raylib-app!", 180, 200, 20, LIGHTGRAY);

    EndDrawing();
}

void Close(App* app) {
    // CloseWindow() is automatically called after this function completes.
}

App Main(int argc, char* argv[]) {
    return (App) {
        .width = 800,
        .height = 450,
        .title = "raylib-app [core] example - basic window",
        .init = Init,
        .update = Update,
        .close = Close,
        .fps = 60,
    };
}
```

## API

``` c
void CloseApp(App* app);
```

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
