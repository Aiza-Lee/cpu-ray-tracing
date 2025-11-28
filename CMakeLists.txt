cmake_minimum_required(VERSION 3.21)
project(cpu-ray-tracing LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${CMAKE_SOURCE_DIR}/bin/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 			${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

# Library
add_library(rt_lib
    src/rt/hittables/Sphere.cpp
    src/rt/hittables/Scene.cpp
    src/rt/hittables/Quad.cpp
    src/rt/core/Camera.cpp
    src/rt/materials/Lambertian.cpp
    src/rt/materials/Metal.cpp
    src/rt/SoftTracer.cpp
    src/rt/RayTracingApps.cpp
)

target_include_directories(rt_lib PUBLIC include)
target_link_libraries(rt_lib PUBLIC glm::glm fmt::fmt OpenMP::OpenMP_CXX)

# Main Executable
add_executable(cpu-ray-tracing src/main.cpp)
target_link_libraries(cpu-ray-tracing PRIVATE rt_lib)

# Tests
enable_testing()
add_subdirectory(tests)