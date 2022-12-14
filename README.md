# raylib-app

Application wrapper for [raylib](https://raylib.com).

## Why

Building raylib for both the desktop and web need some subtle differences with checks against `PLATFORM`. *raylib-app* aims to clean up those differences, and make the code easier to read for a unified application entry. This pattern is inspired by [sokol_app](https://github.com/floooh/sokol#sokol_apph).

## Usage

``` c
#include "raylib.h"

#define RAYLIB_APP_IMPLEMENTATION
#include "raylib-app.h"

void Init(App* app) {
    // InitWindow() is automatically called prior to this function.
}

void UpdateDrawFrame(App* app) {
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
        .update = UpdateDrawFrame,
        .close = Close,
        .fps = 60,
    };
}
```

## API

Rather than having your own `int main()`, you will define your own `App Main(int argc char* argv[])` function.

``` c
App Main(int argc, char* argv[]) {
    return (App) {
        .width = 800,                          // The width of the window
        .height = 450,                         // The height of the window
        .title = "raylib-app",                 // The window title
        .init = Init,                          // The init callback that is called when the application initializes
        .update = UpdateDrawFrame,             // The update callback that is called when the application should render
        .close = Close,                        // The close callback which is called when the application is closed
        .fps = 60,                             // The target frames-per-second
        .configFlags = FLAG_WINDOW_RESIZABLE,  // The flags that are passed to SetConfigFlags()
        .shouldClose = false,                  // Indicate whether or not the application should be closed
        .exitStatus = 0,                       // When the application closes, this is the exit status that is returned
        .userData = NULL                       // Custom user data that is passed through all the callbacks
    };
}
```

### Functions

Through the update callback, you can call the following function to let the application know that it should stop calling the update callback.

``` c
void CloseApp(App* app);    // Tells the application that it should close.
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
