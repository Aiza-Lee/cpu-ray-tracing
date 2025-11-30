#include "rt/SoftTracer.hpp"
#include "rt/materials/Material.hpp"
#include "rt/core/Utils.hpp"
#include <iostream>
#include <fmt/core.h>
#include <omp.h>
#include <atomic>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace rt {

SoftTracer::SoftTracer(int width, int height, int samples, int depth)
	: m_image_width(width), m_image_height(height), m_samples_per_pixel(samples), m_max_depth(depth) {}

glm::vec3 SoftTracer::m_ray_color(const Ray& r, const Scene& world, int depth) {
	HitRecord hit_rec;

	// 如果递归深度耗尽，返回黑色
	if (depth <= 0)
		return glm::vec3(0,0,0);

	if (!world.hit(r, 0.001, infinity, hit_rec)) {
		// 如果没有击中任何对象，返回背景色
		if (m_use_sky_gradient) {
			glm::vec3 unit_direction = glm::normalize(r.direction());
			auto t = 0.5 * (unit_direction.y + 1.0);
			return static_cast<float>(1.0-t)*glm::vec3(1.0, 1.0, 1.0) + static_cast<float>(t)*glm::vec3(0.5, 0.7, 1.0);
		} else {
			return m_background_color;
		}
	}

	ScatterRecord srec; // 存储散射信息

	// 材质不散射光线，则返回自发光颜色
	if (!hit_rec.mat_ptr->scatter(r, hit_rec, srec))
		return hit_rec.mat_ptr->emitted(r, hit_rec);

	// 镜面反射，直接递归追踪反射光线
	if (srec.is_specular)
		return srec.attenuation * m_ray_color(srec.specular_ray, world, depth-1);

	auto pdf_ptr = srec.pdf_ptr;    // 散射方向分布
	auto mat_ptr = hit_rec.mat_ptr; // 击中物体的材质

	float rr_factor = 1.0f;
	// 俄罗斯轮盘赌 (Russian Roulette)
	if (depth < m_max_depth - 3) {
		float p = std::max(srec.attenuation.x, std::max(srec.attenuation.y, srec.attenuation.z));
		p = std::clamp(p, 0.05f, 1.0f); // 限制最小概率
		if (random_double() > p)
			return hit_rec.mat_ptr->emitted(r, hit_rec);
		// 能量补偿
		rr_factor = 1.0f / p;
	}
	
	Ray scatter_ray{ hit_rec.p, pdf_ptr->generate() };      // 散射光线
	auto pdf_val = pdf_ptr->value(scatter_ray.direction()); // 光线方向的 PDF 值
	// 如果 PDF 值为零，说明该方向不可达，返回自发光颜色
	if (pdf_val <= 0) return mat_ptr->emitted(r, hit_rec);

	// 计算 BRDF 值
	glm::vec3 brdf_val = mat_ptr->brdf(r, hit_rec, scatter_ray);

	// 计算余弦项
	auto cos_theta = glm::dot(hit_rec.normal, glm::normalize(scatter_ray.direction()));
	if (cos_theta < 0) cos_theta = 0;

	glm::vec3 L_i = m_ray_color(scatter_ray, world, depth-1);
	
	// 渲染方程的蒙特卡洛估计：
	// Lr = Emitted + (BRDF * Li * CosTheta) / PDF
	glm::vec3 L_r = (brdf_val * L_i * (float)cos_theta * rr_factor) / (float)pdf_val;

	return mat_ptr->emitted(r, hit_rec) + L_r;
}

void SoftTracer::m_write_color(std::vector<unsigned char>& image_data, int index, glm::vec3 pixel_color) {
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
	std::vector<unsigned char> image_data(m_image_width * m_image_height * 3);

	std::atomic<int> scanlines_finished = 0;

	#pragma omp parallel for schedule(dynamic, 1)
	for (int j = m_image_height-1; j >= 0; --j) {
		int finished = ++scanlines_finished;
		if (finished % 10 == 0) {
			fmt::print("\rScanlines remaining: {}   ", m_image_height - finished);
			std::fflush(stdout);
		}
		for (int i = 0; i < m_image_width; ++i) {
			glm::vec3 pixel_color(0,0,0);
			for (int s = 0; s < m_samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (m_image_width-1);
				auto v = (j + random_double()) / (m_image_height-1);
				Ray r = cam.get_ray(u, v);
				pixel_color += m_ray_color(r, scene, m_max_depth);
			}
			
			pixel_color = pixel_color / static_cast<float>(m_samples_per_pixel);

			// 计算 stb_image 的索引（左上角为原点，但我们是从下往上循环，所以需要翻转 y 轴）
			int row = m_image_height - 1 - j;
			int index = (row * m_image_width + i) * 3;
			m_write_color(image_data, index, pixel_color);
		}
	}
	fmt::print("\nDone.\n");

	stbi_write_png(filename.c_str(), m_image_width, m_image_height, 3, image_data.data(), m_image_width * 3);
}

} // namespace rt
