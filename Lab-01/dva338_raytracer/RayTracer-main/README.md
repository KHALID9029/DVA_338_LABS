# DVA338 - Ray Tracer

This is a code skeleton for the ray tracer assignment given as part of the course Fundamentals of Computer Graphics - DVA338 at Mälardalen University.

## Building the project

The project is created in C++ and is provided as a set of header and source files.

### CMake

Please note that the CMake configuration is provided **for convenience only**. It is not mandatory! It is not part of the learning outcomes! It is your responsibility to make it work if you want to use it!

[CMake](https://cmake.org/getting-started/) is a build system generator tool. Using CMake you can specify your project configuration once, and then generate whichever build system you prefer.

We have provided a CMake configuration for convenience. You can find details in `CMakeLists.txt`. Please check the link for information on how to install CMake on your system.

To build the project using CMake from the command prompt, running the following commands from the project root directory should create a directory called `build` containing the generated build files.

```CMake
cmake -B build -G <CHOSEN GENERATOR> .
```

The `<CHOSEN GENERATOR>` part of the command shall be replaced by one of the supported generators for your version of CMake. You can find the list of supported generators by running `cmake --help`.

For instance, the command below will create a Visual Studio solution ready to build the project in a folder called `vs_build`.

```CMake
cmake -B vs_build -G "Visual Studio 17 2022" .
```

Once the build system is created, invoking the custom "run" target rule will compile (if necessary) and execute the application.

Alternatively, you can use the graphical CMake-GUI tool, which gives you a graphical UI interface.

#### CMake Configuration

We have added some comments to explain what is going on in `CMakeLists.txt`. 

Please note that it is *intentionally set up to be simple*.

If you use it as part of your solution, **you are expected to figure out how to modify it** to e.g., add new source files or (sub-)libraries if required.

## Licenses Used

### GPL-3.0

[libbmp](https://github.com/marc-q/libbmp/tree/66bec6d7daf254e6dc07d55c9383fd68276a6a39) - used without modifications for exporting image to BMP format.
