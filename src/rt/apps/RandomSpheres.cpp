#include "rt/apps/RandomSpheres.hpp"
#include "rt/SoftTracer.hpp"
#include "rt/core/Camera.hpp"
#include "rt/hittables/Scene.hpp"
#include "rt/hittables/Sphere.hpp"
#include "rt/materials/Lambertian.hpp"
#include "rt/materials/Metal.hpp"

#include <fmt/base.h>

namespace rt {
void RandomSpheresApp::run() {
	fmt::println("Running Random Spheres Scene...");

	// Image
	constexpr auto ASPECT_RATIO = 16.0 / 9.0;
	constexpr int IMAGE_WIDTH = 400;
	constexpr int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);
	constexpr int SAMPLES_PER_PIXEL = 100;
	constexpr int MAX_DEPTH = 50;

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
	Camera cam(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0), 90, ASPECT_RATIO);

	// Render
	auto lights = std::make_shared<Scene>();
	SoftTracer tracer(IMAGE_WIDTH, IMAGE_HEIGHT, SAMPLES_PER_PIXEL, MAX_DEPTH);
	tracer.set_background(glm::vec3(0.70, 0.80, 1.00), true); // Use sky gradient
	tracer.render(world, lights, cam, "random_spheres.png");
}
} // namespace rt