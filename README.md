# Learn OpenGL

This is my implementation of learnopengl.com. I am mixing and matching some other libs to the project at a small scale to understand 
rendering concepts and game engine and renderer architecture.

## Build

To build the project just
```bash
mkdir build
cd build
cmake . -S ..
cmake --build .
```
or if you are on windows just use `configure.bat` and `build.bat`.

## TODO
1. State machine abstraction
2. ImGUI integration
3. Test emscripten.
4. Add meshoptimizer.