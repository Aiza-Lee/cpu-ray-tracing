#include "rt/apps/CornerBox.hpp"
#include "rt/SoftTracer.hpp"
#include "rt/core/Camera.hpp"
#include "rt/hittables/Quad.hpp"
#include "rt/hittables/Scene.hpp"
#include "rt/hittables/Sphere.hpp"
#include "rt/materials/DiffuseLight.hpp"
#include "rt/materials/Lambertian.hpp"

#include <iostream>

namespace rt {

void CornerBoxApp::run() {
	std::cout << "Running Cornell Box Scene..." << std::endl;

	// Image
	constexpr double ASPECT_RATIO	= 1.0;
	constexpr int IMAGE_WIDTH		= 600;
	constexpr int IMAGE_HEIGHT		= static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);
	constexpr int SAMPLES_PER_PIXEL = 300;
	constexpr int MAX_DEPTH			= 70;

	// World
	Scene world;

	auto red   = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
	auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
	auto green = std::make_shared<Lambertian>(glm::vec3(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(glm::vec3(15, 15, 15));

	// Cornell box walls
	// YZ plane (Left Green)
	world.add(std::make_shared<Quad>(glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), green));
	// YZ plane (Right Red)
	world.add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), red));
	// XZ plane (Light)
	auto light_shape =
		std::make_shared<Quad>(glm::vec3(343, 554, 332), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), light);
	world.add(light_shape);

	auto lights = std::make_shared<Scene>();
	lights->add(light_shape);

	// XZ plane (Floor White)
	world.add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
	// XZ plane (Ceiling White)
	world.add(std::make_shared<Quad>(glm::vec3(0, 555, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
	// XY plane (Back White)
	world.add(std::make_shared<Quad>(glm::vec3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), white));

	// Box 1
	// world.add(box(glm::vec3(130, 0, 65), glm::vec3(295, 165, 230), white));
	// Box 2
	// world.add(box(glm::vec3(265, 0, 295), glm::vec3(430, 330, 460), white));
	// Since I don't have a box helper yet, I'll just add a sphere for now
	world.add(std::make_shared<Sphere>(glm::vec3(278, 100, 278), 100, white));

	// Camera
	Camera cam(glm::vec3(278, 278, -800), glm::vec3(278, 278, 0), glm::vec3(0, 1, 0), 40, ASPECT_RATIO);

	// Render
	SoftTracer tracer(IMAGE_WIDTH, IMAGE_HEIGHT, SAMPLES_PER_PIXEL, MAX_DEPTH);
	tracer.set_background(glm::vec3(0, 0, 0), false);
	tracer.render(world, lights, cam, "corner_box.png");
}
} // namespace rt
