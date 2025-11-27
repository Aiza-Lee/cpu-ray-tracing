# CPU Ray Tracing Project

这是一个基于 C++ 实现的 CPU 光线追踪渲染器。该项目旨在通过简洁的代码结构展示光线追踪的核心原理，支持基于物理的渲染 (PBR) 基础特性。

## 📁 项目结构

项目采用标准的 CMake 工程结构：

*   **`include/rt/`**: 核心头文件目录。
    *   `Hittable.hpp`, `Sphere.hpp`, `Scene.hpp`: 定义几何体和场景结构。
    *   `Material.hpp`: 定义材质系统（漫反射、金属等）。
    *   `Camera.hpp`: 定义摄像机和光线生成逻辑。
    *   `Ray.hpp`: 光线类的定义。
    *   `SoftTracer.hpp`: 渲染器核心类。
    *   `Utils.hpp`: 数学工具和随机数生成器。
*   **`src/`**: 源代码目录。
    *   `main.cpp`: 应用程序入口，负责构建场景并调用渲染器。
    *   `rt/`: 对应头文件的具体实现。
*   **`tests/`**: 单元测试目录，使用 Catch2 框架。
*   **`CMakeLists.txt`**: CMake 构建配置文件。

## 💡 实现思路

本项目遵循经典的光线追踪架构，主要包含以下几个核心模块：

1.  **光线生成 (Ray Generation)**:
    *   `Camera` 类负责根据视场角 (FOV) 和宽高比，从视点向图像平面的每个像素发射光线。
    *   支持多重采样 (MSAA) 以实现抗锯齿效果。

2.  **场景与求交 (Scene & Intersection)**:
    *   `Hittable` 是所有可渲染对象的抽象基类，定义了 `hit` 接口。
    *   `Scene` 类维护一个对象列表，并在光线射入时遍历列表寻找最近的交点。
    *   目前主要实现了 `Sphere` (球体) 几何体。

3.  **材质与散射 (Materials & Scattering)**:
    *   `Material` 抽象类定义了光线击中物体后的行为（散射方向、颜色衰减）。
    *   **Lambertian**: 模拟理想漫反射表面，光线随机散射。
    *   **Metal**: 模拟金属表面，支持模糊反射 (Fuzziness)。

4.  **渲染管线 (Rendering Pipeline)**:
    *   `SoftTracer` 类封装了渲染逻辑。
    *   **递归追踪**: 当光线击中物体时，根据材质属性产生散射光线，递归计算颜色，直到达到最大递归深度 (`max_depth`)。
    *   **Gamma 校正**: 最终颜色经过 Gamma=2.0 校正，以适应显示器输出。

5.  **图像输出**:
    *   渲染结果存储在内存缓冲区中，最后使用 `stb_image_write` 库保存为 PNG 格式。

## 📦 依赖库

项目使用 `vcpkg` 进行依赖管理（推荐）：

*   **[glm](https://github.com/g-truc/glm)**: OpenGL Mathematics，用于所有向量 (`vec3`) 和数学运算。
*   **[fmt](https://github.com/fmtlib/fmt)**: 现代 C++ 格式化库，用于控制台输出和日志。
*   **[stb](https://github.com/nothings/stb)**: 单头文件图像库，用于保存渲染结果 (`stb_image_write`)。
*   **[Catch2](https://github.com/catchorg/Catch2)**: 现代 C++ 单元测试框架。

## 🚀 构建与运行

确保已安装 CMake 和 C++ 编译器 (支持 C++17/20)。

```bash
# 配置项目
cmake -B build -S .

# 编译
cmake --build build

# 运行
./bin/Debug/cpu-ray-tracing
```

## 🔮 后期扩展方向

为了进一步提升渲染器的性能和功能，可以考虑以下扩展方向：

### 1. 性能优化
*   **加速结构 (Acceleration Structures)**: 目前求交算法的时间复杂度为 O(N)。引入 **BVH (Bounding Volume Hierarchy)** 或 **KD-Tree** 可以将复杂度降低至 O(log N)，显著提升复杂场景的渲染速度。
*   **多线程并行 (Multithreading)**: 光线追踪是典型的"易并行" (Embarrassingly Parallel) 任务。可以使用 OpenMP 或 C++ `std::thread` 将像素分配给不同线程并行计算。

### 2. 功能增强
*   **更多材质**:
    *   **Dielectric**: 电介质材质（如玻璃、水），支持折射和菲涅尔效应 (Fresnel effect)。
    *   **Emissive**: 发光材质，使物体成为光源。
*   **纹理映射 (Texture Mapping)**: 支持使用图片作为物体表面的纹理，替代单一颜色。
*   **复杂几何体**: 支持三角形网格 (Triangle Meshes) 加载（如 .obj 文件），以渲染更复杂的模型。

### 3. 采样与降噪
*   **重要性采样 (Importance Sampling)**: 优化光线散射方向的选择，减少噪点，加快收敛速度。
*   **去噪 (Denoising)**: 在后处理阶段使用算法去除图像噪点。
