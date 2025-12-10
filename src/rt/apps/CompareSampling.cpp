#include "rt/apps/CompareSampling.hpp"
#include <fmt/core.h>

void rt::CompareSamplingApp::run() {
	fmt::println("Running CompareSampling...");

	// Image
	constexpr auto aspect_ratio = 1.0;
	constexpr int image_width = 600;
	constexpr int image_height = static_cast<int>(image_width / aspect_ratio);

	constexpr int max_depth = 70;

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

	auto mat_lambertian = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
	world.add(std::make_shared<Sphere>(glm::vec3(180, 100, 278), 100, mat_lambertian));

	auto mat_wrong_lambertian = std::make_shared<WrongLambertian>(glm::vec3(.73, .73, .73));
	world.add(std::make_shared<Sphere>(glm::vec3(375, 100, 278), 100, mat_wrong_lambertian));

	// Camera
	glm::vec3 lookfrom(278, 278, -780);
	glm::vec3 lookat(278, 278, 0);
	glm::vec3 vup(0, 1, 0);
		
	Camera cam(lookfrom, lookat, vup, 40, aspect_ratio);

	// 16
	{
		fmt::println("16 Samples per Pixel - Material Sampling Only...");
		constexpr int samples_per_pixel = 16; 
		SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
		tracer.set_background(glm::vec3(0,0,0), false);
		tracer.set_sampling_strategy(SamplingStrategy::Material);
		tracer.render(world, lights, cam, "compare_sampling_16.png");
	}
	// 50
	{
		fmt::println("50 Samples per Pixel - Material Sampling Only...");
		constexpr int samples_per_pixel = 50; 
		SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
		tracer.set_background(glm::vec3(0,0,0), false);
		tracer.set_sampling_strategy(SamplingStrategy::Material);
		tracer.render(world, lights, cam, "compare_sampling_50.png");
	
	}
	// 100
	{
		fmt::println("100 Samples per Pixel - Material Sampling Only...");
		constexpr int samples_per_pixel = 100; 
		SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
		tracer.set_background(glm::vec3(0,0,0), false);
		tracer.set_sampling_strategy(SamplingStrategy::Material);
		tracer.render(world, lights, cam, "compare_sampling_100.png");
	}
	// 1000
	{
		fmt::println("1000 Samples per Pixel - Material Sampling Only...");
		constexpr int samples_per_pixel = 1000; 
		SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
		tracer.set_background(glm::vec3(0,0,0), false);
		tracer.set_sampling_strategy(SamplingStrategy::Material);
		tracer.render(world, lights, cam, "compare_sampling_1000.png");
	}

}