# DVA338 Raytracer

This project is a raytracer implementation for the course Fundamentals of Computer Graphics (DVA338).

## Project Structure

```
dva338_raytracer/
  RayTracer-main/
    CMakeLists.txt
    README.md
    external_dependencies/
      libbmp/
        libbmp.cpp
        libbmp.h
        LICENSE
        README.md
    include/
      Image.h
      Light.h
      Material.h
      Object.h
      Plane.h
      Ray.h
      raytrace_features.h
      RayTracer.h
      Scene.h
      Sphere.h
      Triangle.h
      Vec3.h
    src/
      Image.cpp
      Main.cpp
      Plane.cpp
      Ray.cpp
      RayTracer.cpp
      Sphere.cpp
      Triangle.cpp
```

## Features
- Ray tracing for spheres, planes, and triangles
- Material and lighting support
- Scene management
- Image output in PPM format
- Extensible object-oriented design

## Getting Started

### Prerequisites
- C++ compiler (C++11 or higher)
- CMake (for building the project)

### Build Instructions
1. Clone the repository:
   ```
   git clone <repo-url>
   ```
2. Navigate to the project directory:
   ```
   cd dva338_raytracer/RayTracer-main
   ```
3. Create a build directory and run CMake:
   ```
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
4. Run the raytracer executable:
   ```
   ./RayTracer
   ```

### Output
- The rendered image will be saved as `DVA338_Raytracer_output.ppm` in the project root.

## External Dependencies
- [libbmp](https://github.com/christopherpow/bitmap) for BMP image support

## File Overview
- `src/`: Source files for raytracer implementation
- `include/`: Header files for raytracer classes and features
- `external_dependencies/libbmp/`: BMP image library
- `CMakeLists.txt`: Build configuration

## License
This project is for educational purposes. See `external_dependencies/libbmp/LICENSE` for libbmp licensing.

## Contact
For questions or contributions, please contact the course instructor or repository owner.
