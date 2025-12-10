#include "rt/apps/RandomSpheres.hpp"
#include <iostream>

namespace rt {
void RandomSpheresApp::run() {
	std::cout << "Running Random Spheres Scene..." << std::endl;

	// Image
	constexpr auto aspect_ratio = 16.0 / 9.0;
	constexpr int image_width = 400;
	constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
	constexpr int samples_per_pixel = 100;
	constexpr int max_depth = 50;

	// World
	Scene world;

	auto material_ground = std::make_shared<Lambertian>(glm::vec3(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Lambertian>(glm::vec3(0.1, 0.2, 0.5));
	auto material_left   = std::make_shared<Metal>(glm::vec3(0.8, 0.8, 0.8), 0.3);
	auto material_right  = std::make_shared<Metal>(glm::vec3(0.8, 0.6, 0.2), 1.0);

	world.add(std::make_shared<Sphere>(glm::vec3( 0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(std::make_shared<Sphere>(glm::vec3( 0.0,    0.0, -1.0),   0.5, material_center));
	world.add(std::make_shared<Sphere>(glm::vec3(-1.0,    0.0, -1.0),   0.5, material_left));
	world.add(std::make_shared<Sphere>(glm::vec3( 1.0,    0.0, -1.0),   0.5, material_right));

	// Camera
	Camera cam(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0), 90, aspect_ratio);

	// Render
	auto lights = std::make_shared<Scene>();
	SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
	tracer.set_background(glm::vec3(0.70, 0.80, 1.00), true); // Use sky gradient
	tracer.render(world, lights, cam, "random_spheres.png");
}
} // namespace rt