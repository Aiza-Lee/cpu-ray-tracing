#include "rt/apps/SimpleLightWrong.hpp"
#include <iostream>

namespace rt {
void SimpleLightWrongApp::run() {
	std::cout << "Running Comparison Scene (Cosine vs Uniform)..." << std::endl;

	// Image
	constexpr auto aspect_ratio = 1.0;
	constexpr int image_width = 600;
	constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
		
	// !!! 关键：使用低采样数来凸显噪声差异 !!!
	constexpr int samples_per_pixel = 1000;
	constexpr int max_depth = 50;

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

	// --- 对比测试对象 ---
		
	// 左球：标准 Lambertian (Cosine Weighted Sampling)
	// 预期结果：表面比较平滑，噪声较少
	auto mat_correct = std::make_shared<Lambertian>(glm::vec3(0.8, 0.8, 0.8));
	world.add(std::make_shared<Sphere>(glm::vec3(180, 100, 250), 100, mat_correct));

	// 右球：Wrong Lambertian (Uniform Sampling)
	// 预期结果：表面会有明显的噪点，特别是在阴影过渡区域
	auto mat_wrong = std::make_shared<WrongLambertian>(glm::vec3(0.8, 0.8, 0.8));
	world.add(std::make_shared<Sphere>(glm::vec3(375, 100, 250), 100, mat_wrong));

	// Camera
	glm::vec3 lookfrom(278, 278, -780);
	glm::vec3 lookat(278, 278, 0);
	glm::vec3 vup(0, 1, 0);
		
	Camera cam(lookfrom, lookat, vup, 40, aspect_ratio);

	// Render
	SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
	tracer.set_background(glm::vec3(0,0,0), false);
	tracer.render(world, lights, cam, "comparison_sampling.png");
}
} // namespace rt