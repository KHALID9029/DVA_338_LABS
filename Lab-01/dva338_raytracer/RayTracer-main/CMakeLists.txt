# Setting some reasonable cmake version. 
# Any version you get in 2024 onwards should be WAAAY newer than this
cmake_minimum_required(VERSION 3.10)
# This defines the name of the project - it is required
project(DVA338_RayTracer)

# This sets up the local include directories for the project
## PROJECT_SOURCE_DIR is a placeholder variable that _rougly_ maps
## to where the CMakeLists.txt file is located
include_directories("${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/external_dependencies/libbmp")

# The project is built as a set of libraries that are linked together
add_library(Ray "${PROJECT_SOURCE_DIR}/src/Ray.cpp" "include/Material.h" "include/Light.h" "include/Plane.h" "src/Plane.cpp" "include/Triangle.h" "src/Triangle.cpp")

add_library(Image 
"${PROJECT_SOURCE_DIR}/external_dependencies/libbmp/libbmp.cpp" 
"${PROJECT_SOURCE_DIR}/src/Image.cpp" "include/Material.h" "include/Light.h" "include/Plane.h" "src/Plane.cpp" "include/Triangle.h" "src/Triangle.cpp")

add_library(Objects "${PROJECT_SOURCE_DIR}/src/Sphere.cpp" "include/Material.h" "include/Light.h" "include/Plane.h" "src/Plane.cpp" "include/Triangle.h" "src/Triangle.cpp")

add_library(RayTracer "${PROJECT_SOURCE_DIR}/src/RayTracer.cpp" "include/Material.h" "include/Light.h" "include/Plane.h" "src/Plane.cpp" "include/Triangle.h" "src/Triangle.cpp")

target_link_libraries(RayTracer Ray Objects Image)

# Finally, we set the executable
add_executable(RayTraceRunner "${PROJECT_SOURCE_DIR}/src/Main.cpp" "include/Material.h" "include/Light.h" "include/Plane.h" "src/Plane.cpp" "include/Triangle.h" "src/Triangle.cpp")

# .. which is linked together with the ray tracer library
target_link_libraries(RayTraceRunner RayTracer)

set_property(TARGET RayTraceRunner Image RayTracer Objects Ray PROPERTY CXX_STANDARD 11)
set_property(TARGET RayTraceRunner Image RayTracer Objects Ray PROPERTY CXX_STANDARD_REQUIRED On)

# This creates a custom build target that 
## (if needed builds) and runs the executable
add_custom_target(run RayTraceRunner)
