# SomeGraphics
Some graphics using C++ / OpenGL

![SomeGraphicsEditor screenshot](https://github.com/clbrunet/SomeGraphics/assets/53996617/cf2f7241-2656-4070-b582-d60915348a14)

## Building
First, you'll need to install [Git](https://git-scm.com/downloads) and [CMake](https://cmake.org/download/).

Open a command prompt and run :
```sh
git clone -j4 --recurse-submodules https://github.com/clbrunet/SomeGraphics.git && cd SomeGraphics
```

### Unix using Make (macOS is untested, but could work)
```sh
cmake -B build && cmake --build build --parallel 4 && ./build/editor/SomeGraphicsEditor
```

### Windows using Visual Studio
Ensure that you have [Visual Studio](https://visualstudio.microsoft.com/downloads/) installed, then run the following command :
```sh
cmake -B build
```
You can now open the project solution file `build/SomeGraphics.sln` and press the start button.

### Windows using MinGW-w64
Follow the MinGW-w64 installation instructions [here](https://code.visualstudio.com/docs/cpp/config-mingw#_installing-the-mingww64-toolchain), then run :
```sh
cmake -B build -G "MinGW Makefiles" && cmake --build build --parallel 4 && "./build/editor/SomeGraphicsEditor"
```

## Dependencies
* [assimp](https://github.com/assimp/assimp)
* [stb_image](https://github.com/nothings/stb)
* [entt](https://github.com/skypjack/entt)
* [GLFW](https://github.com/glfw/glfw)
* [glad](https://github.com/Dav1dde/glad)
* [glm](https://github.com/g-truc/glm)
* [Dear ImGui](https://github.com/ocornut/imgui)
