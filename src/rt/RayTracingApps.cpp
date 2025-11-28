#include "rt/RayTracingApps.hpp"
#include "rt/SoftTracer.hpp"
#include "rt/hittables/Sphere.hpp"
#include "rt/hittables/Quad.hpp"
#include "rt/materials/Material.hpp"
#include "rt/materials/Lambertian.hpp"
#include "rt/materials/Metal.hpp"
#include "rt/materials/DiffuseLight.hpp"
#include "rt/hittables/Scene.hpp"
#include "rt/core/Camera.hpp"
#include <iostream>
#include <memory>
#include "rt/RayTracingApps.hpp"

namespace rt {

void RayTracingApps::random_spheres() {
	std::cout << "Running Random Spheres Scene..." << std::endl;

	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

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
	SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
	tracer.set_background(glm::vec3(0.70, 0.80, 1.00), true); // Use sky gradient
	tracer.render(world, cam, "random_spheres.png");
}

void RayTracingApps::simple_light() {
	std::cout << "Running Simple Light Scene..." << std::endl;

	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 400;
	const int max_depth = 50;

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

void RayTracingApps::cornell_box() {
	std::cout << "Running Cornell Box Scene..." << std::endl;

	// Image
	const auto aspect_ratio = 1.0;
	const int image_width = 600;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 300;
	const int max_depth = 60;

	// World
	Scene world;

	auto red   = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
	auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
	auto green = std::make_shared<Lambertian>(glm::vec3(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(glm::vec3(15, 15, 15));

	// Cornell box walls
	// YZ plane (Left Green)
	world.add(std::make_shared<Quad>(glm::vec3(555,0,0), glm::vec3(0,555,0), glm::vec3(0,0,555), green));
	// YZ plane (Right Red)
	world.add(std::make_shared<Quad>(glm::vec3(0,0,0), glm::vec3(0,555,0), glm::vec3(0,0,555), red));
	// XZ plane (Light)
	world.add(std::make_shared<Quad>(glm::vec3(343, 554, 332), glm::vec3(-130,0,0), glm::vec3(0,0,-105), light));
	// XZ plane (Floor White)
	world.add(std::make_shared<Quad>(glm::vec3(0,0,0), glm::vec3(555,0,0), glm::vec3(0,0,555), white));
	// XZ plane (Ceiling White)
	world.add(std::make_shared<Quad>(glm::vec3(0,555,0), glm::vec3(555,0,0), glm::vec3(0,0,555), white));
	// XY plane (Back White)
	world.add(std::make_shared<Quad>(glm::vec3(0,0,555), glm::vec3(555,0,0), glm::vec3(0,555,0), white));

	// Box 1
	// world.add(box(glm::vec3(130, 0, 65), glm::vec3(295, 165, 230), white));
	// Box 2
	// world.add(box(glm::vec3(265, 0, 295), glm::vec3(430, 330, 460), white));
	// Since I don't have a box helper yet, I'll just add a sphere for now
	world.add(std::make_shared<Sphere>(glm::vec3(278, 100, 278), 100, white));

	// Camera
	Camera cam(glm::vec3(278, 278, -800), glm::vec3(278, 278, 0), glm::vec3(0,1,0), 40, aspect_ratio);

	// Render
	SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
	tracer.set_background(glm::vec3(0,0,0), false);
	tracer.render(world, cam, "cornell_box.png");
}

void RayTracingApps::mirror_box() {

	const auto aspect_ratio = 1.0;
	const int image_width = 600;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

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
	world.add(std::make_shared<Quad>(glm::vec3(0,0,0), glm::vec3(555,0,0), glm::vec3(0,0,555), gray));
	// XZ plane ceil
	world.add(std::make_shared<Quad>(glm::vec3(0,555,0), glm::vec3(555,0,0), glm::vec3(0,0,555), mirror_mat));
	// XY plane back
	world.add(std::make_shared<Quad>(glm::vec3(0,0,555), glm::vec3(555,0,0), glm::vec3(0,555,0), mirror_mat));

	world.add(std::make_shared<Quad>(glm::vec3(0,0,0), glm::vec3(555,0,0), glm::vec3(0,555,0), mirror_mat));


	world.add(std::make_shared<Sphere>(glm::vec3(278, 100, 278), 100, red));
	world.add(std::make_shared<Sphere>(glm::vec3(378, 60, 100), 60, light));
	world.add(std::make_shared<Sphere>(glm::vec3(350, 350, 350), 80, perfect_mirror_mat));

	Camera cam(glm::vec3(10, 250, 40), glm::vec3(278, 278, 100), glm::vec3(0,1,0), 40, aspect_ratio);

	SoftTracer tracer(image_width, image_height, samples_per_pixel, max_depth);
	tracer.set_background(glm::vec3(0,0,0), false);
	tracer.render(world, cam, "mirror_box.png");
}

} // namespace rt