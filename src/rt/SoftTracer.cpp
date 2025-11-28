#include "rt/SoftTracer.hpp"
#include "rt/Material.hpp"
#include "rt/Utils.hpp"
#include <iostream>
#include <fmt/core.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace rt {

SoftTracer::SoftTracer(int width, int height, int samples, int depth)
	: _image_width(width), _image_height(height), _samples_per_pixel(samples), _max_depth(depth) {}

glm::vec3 SoftTracer::_ray_color(const Ray& r, const Scene& world, int depth) {
	HitRecord rec;

	// 如果递归深度耗尽，返回黑色
	if (depth <= 0)
		return glm::vec3(0,0,0);

	if (world.hit(r, 0.001, infinity, rec)) {
		Ray scattered;
		glm::vec3 attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * _ray_color(scattered, world, depth-1);
		return glm::vec3(0,0,0);
	}

	return glm::vec3(0,0,0);

	// // 如果没有击中任何对象，返回背景色（渐变天空）
	// glm::vec3 unit_direction = glm::normalize(r.direction());
	// auto t = 0.5 * (unit_direction.y + 1.0);
	// return static_cast<float>(1.0-t)*glm::vec3(1.0, 1.0, 1.0) + static_cast<float>(t)*glm::vec3(0.5, 0.7, 1.0);
}

void SoftTracer::_write_color(std::vector<unsigned char>& image_data, int index, glm::vec3 pixel_color) {
	auto r = pixel_color.x;
	auto g = pixel_color.y;
	auto b = pixel_color.z;

	// gamma 校正，gamma=2.0
	r = sqrt(r);
	g = sqrt(g);
	b = sqrt(b);

	image_data[index] = static_cast<unsigned char>(256 * std::clamp(r, 0.0f, 0.999f));
	image_data[index+1] = static_cast<unsigned char>(256 * std::clamp(g, 0.0f, 0.999f));
	image_data[index+2] = static_cast<unsigned char>(256 * std::clamp(b, 0.0f, 0.999f));
}

void SoftTracer::render(const Scene& scene, const Camera& cam, const std::string& filename) {
	std::vector<unsigned char> image_data(_image_width * _image_height * 3);

	for (int j = _image_height-1; j >= 0; --j) {
		fmt::print("\rScanlines remaining: {} ", j);
		std::fflush(stdout);
		for (int i = 0; i < _image_width; ++i) {
			glm::vec3 pixel_color(0,0,0);
			for (int s = 0; s < _samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (_image_width-1);
				auto v = (j + random_double()) / (_image_height-1);
				Ray r = cam.get_ray(u, v);
				pixel_color += _ray_color(r, scene, _max_depth);
			}
			
			pixel_color = pixel_color / static_cast<float>(_samples_per_pixel);

			// 计算 stb_image 的索引（左上角为原点，但我们是从下往上循环，所以需要翻转 y 轴）
			int row = _image_height - 1 - j;
			int index = (row * _image_width + i) * 3;
			_write_color(image_data, index, pixel_color);
		}
	}
	fmt::print("\nDone.\n");

	stbi_write_png(filename.c_str(), _image_width, _image_height, 3, image_data.data(), _image_width * 3);
}

} // namespace rt
