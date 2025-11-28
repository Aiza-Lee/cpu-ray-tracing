#include "rt/SoftTracer.hpp"
#include "rt/Sphere.hpp"
#include "rt/Material.hpp"
#include "rt/Scene.hpp"
#include "rt/Camera.hpp"
#include <iostream>

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1000;
    const int max_depth = 100;

    // World
    rt::Scene world;

    auto material_ground = std::make_shared<rt::Lambertian>(glm::vec3(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<rt::Lambertian>(glm::vec3(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<rt::Metal>(glm::vec3(0.8, 0.8, 0.8), 0.3);
    auto material_right  = std::make_shared<rt::Metal>(glm::vec3(0.8, 0.6, 0.2), 1.0);

    world.add(std::make_shared<rt::Sphere>(glm::vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<rt::Sphere>(glm::vec3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<rt::Sphere>(glm::vec3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<rt::Sphere>(glm::vec3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    rt::Camera cam(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0), 90, aspect_ratio);

    // Render
    rt::SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
    tracer.render(world, cam, "image.png");

    return 0;
}
