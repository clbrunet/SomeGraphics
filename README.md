# SomeGraphics
Some graphics using C++ / OpenGL

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
