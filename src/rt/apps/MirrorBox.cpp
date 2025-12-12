#include "rt/apps/MirrorBox.hpp"
#include "rt/SoftTracer.hpp"
#include "rt/core/Camera.hpp"
#include "rt/hittables/Quad.hpp"
#include "rt/hittables/Scene.hpp"
#include "rt/hittables/Sphere.hpp"
#include "rt/materials/DiffuseLight.hpp"
#include "rt/materials/Lambertian.hpp"
#include "rt/materials/Metal.hpp"
#include <iostream>

namespace rt {
void MirrorBoxApp::run() {
	std::cout << "Running Mirror Box Scene..." << std::endl;
	constexpr double ASPECT_RATIO = 1.0;
	constexpr int IMAGE_WIDTH = 1024;
	constexpr int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);
	constexpr int SAMPLES_PER_PIXEL = 6000;
	constexpr int MAX_DEPTH = 400;

	Scene world;

	auto mirror_mat = std::make_shared<Metal>(glm::vec3(0.9, 0.9, 0.9), 0.002);
	auto perfect_mirror_mat = std::make_shared<Metal>(glm::vec3(0.9, 0.9, 0.9), 0);

	auto red = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
	auto gray = std::make_shared<Lambertian>(glm::vec3(.2, .2, .2));
	auto green = std::make_shared<Lambertian>(glm::vec3(.12, .45, .15));

	auto light = std::make_shared<DiffuseLight>(glm::vec3(12, 12, 12));

	// mirror cornell box walls
	// YZ plane left
	world.add(std::make_shared<Quad>(glm::vec3(555,0,0), glm::vec3(0,555,0), glm::vec3(0,0,555), mirror_mat));
	// YZ plane right
	world.add(std::make_shared<Quad>(glm::vec3(0,0,0), glm::vec3(0,555,0), glm::vec3(0,0,555), mirror_mat));
	// // XZ plane light
	// world.add(std::make_shared<Quad>(glm::vec3(343, 554, 332), glm::vec3(-130,0,0), glm::vec3(0,0,-105), light));
	// XZ plane floor
	world.add(std::make_shared<Quad>(glm::vec3(0,0,0), glm::vec3(700,0,0), glm::vec3(0,0,700), gray));
	// XZ plane ceil
	world.add(std::make_shared<Quad>(glm::vec3(0,555,0), glm::vec3(700,0,0), glm::vec3(0,0,700), mirror_mat));
	// XY plane back
	world.add(std::make_shared<Quad>(glm::vec3(0,0,555), glm::vec3(555,0,0), glm::vec3(0,555,0), mirror_mat));
	// XY plane front
	world.add(std::make_shared<Quad>(glm::vec3(0,0,0), glm::vec3(700,0,0), glm::vec3(0,700,0), mirror_mat));

	auto light_sphere = std::make_shared<Sphere>(glm::vec3(378, 70, 100), 70, light);
	world.add(std::make_shared<Sphere>(glm::vec3(278, 100, 278), 100, red));
	world.add(light_sphere);
	world.add(std::make_shared<Sphere>(glm::vec3(350, 350, 350), 80, perfect_mirror_mat));

	auto lights = std::make_shared<Scene>();
	lights->add(light_sphere);

	Camera cam(glm::vec3(10, 250, 40), glm::vec3(278, 278, 100), glm::vec3(0,1,0), 40, ASPECT_RATIO);

	SoftTracer tracer(IMAGE_WIDTH, IMAGE_HEIGHT, SAMPLES_PER_PIXEL, MAX_DEPTH);
	tracer.set_sampling_strategy(SamplingStrategy::MIS);
	tracer.set_background(glm::vec3(0,0,0), false);
	tracer.render(world, lights, cam, "mirror_box.png");
}
} // namespace rt