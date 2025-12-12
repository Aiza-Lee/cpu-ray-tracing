#include "rt/apps/Playground.hpp"
#include "rt/SoftTracer.hpp"
#include "rt/hittables/Quad.hpp"
#include "rt/hittables/Scene.hpp"
#include "rt/hittables/Sphere.hpp"
#include "rt/materials/DiffuseLight.hpp"
#include "rt/materials/Lambertian.hpp"
#include "rt/materials/Metal.hpp"
#include <fmt/base.h>
#include <iostream>
#include <fmt/core.h>

namespace rt {
void PlaygroundApp::run() {
	std::cout << "Running Playground..." << std::endl;

	// Image
	constexpr auto ASPECT_RATIO = 1.0;
	constexpr int IMAGE_WIDTH = 600;
	constexpr int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / ASPECT_RATIO);

	constexpr int SAMPLES_PER_PIXEL = 300;
	constexpr int MAX_DEPTH = 50;

	// World
	Scene world;

	// 材质定义
	auto red   = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
	auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
	auto green = std::make_shared<Lambertian>(glm::vec3(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(glm::vec3(15, 15, 15));

	// Cornell Box 墙壁
	world.add(std::make_shared<Quad>(glm::vec3(555,0,0), glm::vec3(0,555,0), glm::vec3(0,0,555), green)); // 左墙
	world.add(std::make_shared<Quad>(glm::vec3(0,0,0), glm::vec3(0,555,0), glm::vec3(0,0,555), red));	 // 右墙
	auto light_shape = std::make_shared<Quad>(glm::vec3(343, 554, 332), glm::vec3(-130,0,0), glm::vec3(0,0,-105), light);
	world.add(light_shape); // 灯光
	world.add(std::make_shared<Quad>(glm::vec3(0,0,0), glm::vec3(555,0,0), glm::vec3(0,0,555), white));   // 地板
	world.add(std::make_shared<Quad>(glm::vec3(555,555,555), glm::vec3(-555,0,0), glm::vec3(0,0,-555), white)); // 天花板
	world.add(std::make_shared<Quad>(glm::vec3(0,0,555), glm::vec3(555,0,0), glm::vec3(0,555,0), white)); // 后墙

	auto lights = std::make_shared<Scene>();
	lights->add(light_shape);

	auto mat_lambertian = std::make_shared<Lambertian>(glm::vec3(0.8, 0.8, 0.8));
	world.add(std::make_shared<Sphere>(glm::vec3(180, 100, 220), 100, mat_lambertian));

	auto mat_metal = std::make_shared<Metal>(glm::vec3(1.0, 1.0, 1.0), 0);
	world.add(std::make_shared<Sphere>(glm::vec3(375, 100, 350), 100, mat_metal));

	// Camera
	glm::vec3 lookfrom(278, 278, -780);
	glm::vec3 lookat(278, 278, 0);
	glm::vec3 vup(0, 1, 0);
		
	Camera cam(lookfrom, lookat, vup, 40, ASPECT_RATIO);

	// Render 1: MIS (Default)
	{
		fmt::print("\nRendering with MIS (Mixture Sampling)...\n");
		SoftTracer tracer(IMAGE_WIDTH, IMAGE_HEIGHT, SAMPLES_PER_PIXEL, MAX_DEPTH);
		tracer.set_background(glm::vec3(0,0,0), false);
		tracer.set_sampling_strategy(SamplingStrategy::MIS);
		tracer.render(world, lights, cam, "playground_mis.png");
	}

	// Render 2: Light Sampling Only
	{
		fmt::print("\nRendering with Light Sampling Only (NEE)...\n");
		SoftTracer tracer(IMAGE_WIDTH, IMAGE_HEIGHT, SAMPLES_PER_PIXEL, MAX_DEPTH);
		tracer.set_background(glm::vec3(0,0,0), false);
		tracer.set_sampling_strategy(SamplingStrategy::LIGHT);
		tracer.render(world, lights, cam, "playground_light.png");
	}

	// Render 3: Material Sampling Only
	{
		fmt::print("\nRendering with Material Sampling Only (Standard Path Tracing)...\n");
		SoftTracer tracer(IMAGE_WIDTH, IMAGE_HEIGHT, SAMPLES_PER_PIXEL, MAX_DEPTH);
		tracer.set_background(glm::vec3(0,0,0), false);
		tracer.set_sampling_strategy(SamplingStrategy::MATERIAL);
		tracer.render(world, lights, cam, "playground_material.png");
	}
}
} // namespace rt