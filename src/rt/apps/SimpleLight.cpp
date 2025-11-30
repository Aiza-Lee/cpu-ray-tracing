#include "rt/apps/SimpleLight.hpp"
#include <iostream>

namespace rt {
void SimpleLightApp::run() {
	std::cout << "Running Simple Light Scene..." << std::endl;

	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 800;
	const int max_depth = 200;

	// World
	Scene world;

	auto material_ground = std::make_shared<Lambertian>(glm::vec3(0.5, 0.5, 0.5)); // Grey ground
	world.add(std::make_shared<Sphere>(glm::vec3(0,-1000,0), 1000, material_ground));

	auto material_sphere = std::make_shared<Lambertian>(glm::vec3(0.7, 0.1, 0.1)); // Red sphere
	world.add(std::make_shared<Sphere>(glm::vec3(0, 2, 0), 2, material_sphere));

	auto difflight = std::make_shared<DiffuseLight>(glm::vec3(4, 4, 4)); // Bright light
	// Light source: a small sphere above
	world.add(std::make_shared<Sphere>(glm::vec3(0, 7, 0), 2, difflight));
	// And a rect light
	world.add(std::make_shared<Quad>(glm::vec3(3, 1, -2), glm::vec3(2, 0, 0), glm::vec3(0, 2, 0), difflight));

	// Camera
	Camera cam(glm::vec3(26,3,6), glm::vec3(0,2,0), glm::vec3(0,1,0), 20, aspect_ratio);

	// Render
	SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
	tracer.set_background(glm::vec3(0,0,0), false); // Black background
	tracer.render(world, cam, "simple_light.png");
}
} // namespace rt