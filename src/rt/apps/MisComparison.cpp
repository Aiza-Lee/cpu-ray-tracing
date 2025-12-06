#include "rt/apps/MisComparison.hpp"
#include <iostream>
#include <fmt/core.h>

namespace rt {

inline 

void MisComparisonApp::run() {
	std::cout << "Running MIS Comparison Scene..." << std::endl;

	// Image
	const auto aspect_ratio = 1.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 70;

	// World
	Scene world;

	// Materials for Spheres
	auto red = std::make_shared<DiffuseLight>(glm::vec3(0.8, 0.2, 0.2), 5.0); // Red
	auto blue = std::make_shared<DiffuseLight>(glm::vec3(0.2, 0.2, 0.8), 3.0); // Blue
	auto green = std::make_shared<DiffuseLight>(glm::vec3(0.2, 0.8, 0.2), 2.0); // Green
	auto yellow = std::make_shared<DiffuseLight>(glm::vec3(0.8, 0.8, 0.2), 2.0); // Yellow
	auto white = std::make_shared<DiffuseLight>(glm::vec3(0.9, 0.9, 0.9), 1.0); // White
	auto gray = std::make_shared<Lambertian>(glm::vec3(0.15, 0.15, 0.15)); // Gray

	// Materials for Mirrors (Floor Strips)
	// Increasing roughness from left to right
	auto mirror_0 = std::make_shared<Metal>(glm::vec3(0.95), 0.03); // Perfect Mirror
	auto mirror_1 = std::make_shared<Metal>(glm::vec3(0.95), 0.05); // Slightly Rough
	auto mirror_2 = std::make_shared<Metal>(glm::vec3(0.95), 0.09); // Rough
	auto mirror_3 = std::make_shared<Metal>(glm::vec3(0.95), 0.11); // Very Rough

	// Light Material
	// auto light_mat = std::make_shared<DiffuseLight>(glm::vec3(200, 200, 200));

	auto cam_pos = glm::vec3(0, 250, -500);

	// Lights
	auto lights = std::make_shared<Scene>();

	// --- Geometry ---

	// Mirrors (Floor)
	world.add(m_build_mirror(glm::vec3(0, 400, 280), cam_pos, glm::vec3(0, 280, 420), mirror_0));
	world.add(m_build_mirror(glm::vec3(0, 400, 280), cam_pos, glm::vec3(0, 180, 290), mirror_1));
	world.add(m_build_mirror(glm::vec3(0, 400, 280), cam_pos, glm::vec3(0, 110, 150), mirror_2));
	world.add(m_build_mirror(glm::vec3(0, 400, 280), cam_pos, glm::vec3(0, 70, 0), mirror_3));

	// Spheres
	auto red_sphere = std::make_shared<Sphere>(glm::vec3(-255, 400, 280), 5, red);
	auto blue_sphere = std::make_shared<Sphere>(glm::vec3(-180, 400, 280), 15, blue);
	auto green_sphere = std::make_shared<Sphere>(glm::vec3(-80, 400, 280), 25, green);
	auto yellow_sphere = std::make_shared<Sphere>(glm::vec3(45, 400, 280), 45, yellow);
	auto white_sphere = std::make_shared<Sphere>(glm::vec3(200, 400, 280), 60, white);
	world.add(red_sphere); lights->add(red_sphere);
	world.add(blue_sphere); lights->add(blue_sphere);
	world.add(green_sphere); lights->add(green_sphere);
	world.add(yellow_sphere); lights->add(yellow_sphere);
	world.add(white_sphere); lights->add(white_sphere);
	
	// Light
	// auto light_shape = std::make_shared<Quad>(glm::vec3(-100, 150, -600), glm::vec3(0, 200, 0), glm::vec3(200, 0, 0), light_mat);
	// world.add(light_shape); lights->add(light_shape);

	// // Walls
	// // Back Wall
	// world.add(std::make_shared<Quad>(glm::vec3(-1000, 0, 600), glm::vec3(2000, 0, 0), glm::vec3(0, 1000, 0), gray));
	// // Gound Wall
	// world.add(std::make_shared<Quad>(glm::vec3(-1000, 0, -400), glm::vec3(2000, 0, 0), glm::vec3(0, 0, 1000), gray));
	// // ceiling
	// world.add(std::make_shared<Quad>(glm::vec3(-1000, 1000, 400), glm::vec3(2000, 0, 0), glm::vec3(0, 0, -2500), gray));
	
	// Camera
	Camera cam(cam_pos, glm::vec3(0, 180, 0), glm::vec3(0,1,0), 50, aspect_ratio);

	// Render 1: MIS (Default)
	{
		fmt::print("\nRendering with MIS (Mixture Sampling)...\n");
		SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
		tracer.set_background(glm::vec3(0,0,0), false);
		tracer.set_sampling_strategy(SamplingStrategy::MIS);
		tracer.render(world, lights, cam, "mis_comparison_1_mis.png");
	}

	// Render 2: Light Sampling Only
	{
		fmt::print("\nRendering with Light Sampling Only (NEE)...\n");
		SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
		tracer.set_background(glm::vec3(0,0,0), false);
		tracer.set_sampling_strategy(SamplingStrategy::Light);
		tracer.render(world, lights, cam, "mis_comparison_2_light.png");
	}

	// Render 3: Material Sampling Only
	{
		fmt::print("\nRendering with Material Sampling Only (Standard Path Tracing)...\n");
		SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
		tracer.set_background(glm::vec3(0,0,0), false);
		tracer.set_sampling_strategy(SamplingStrategy::Material);
		tracer.render(world, lights, cam, "mis_comparison_3_material.png");
	}
}

std::shared_ptr<Quad> MisComparisonApp::m_build_mirror(const glm::vec3& S, const glm::vec3& V, const glm::vec3& P, std::shared_ptr<Material> material) {

	auto i = glm::normalize(P - S); // 入射方向
	auto r = glm::normalize(V - P); // 观察方向
	auto n = glm::normalize(i - r); // 法线方向

	auto reflected = i - 2.0f * glm::dot(i, n) * n;

	if (glm::length(reflected - r) > 1e-8 && glm::length(reflected + r) < glm::length(reflected - r)) {
		n = -n;
	}

	auto siz = 120.0f, length = 550.0f;

	auto len = glm::vec3(length, 0, 0);
	auto wid = glm::normalize(glm::cross(n, len)) * siz;

	return std::make_shared<Quad>(P - len * 0.5f - wid * 0.5f, len, wid, material);
}

} // namespace rt
