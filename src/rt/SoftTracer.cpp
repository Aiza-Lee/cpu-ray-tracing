#include "rt/SoftTracer.hpp"
#include "rt/materials/Material.hpp"
#include "rt/core/Utils.hpp"
#include "rt/pdf/HittablePDF.hpp"
#include <fmt/core.h>
#include <omp.h>
#include <atomic>
#include <cstddef>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace rt {

SoftTracer::SoftTracer(const int width, const int height, const int samples, const int depth) :
	_image_width(width), _image_height(height), m_samples_per_pixel(samples), m_max_depth(depth) {}

glm::vec3 SoftTracer::m_ray_color(const Ray& r_in, const Scene& world, const std::shared_ptr<Hittable>& lights,
								  int depth) {
	// 如果递归深度耗尽，返回黑色
	if (depth <= 0) return {0, 0, 0};

	HitRecord hit_rec;
	if (!world.hit(r_in, RAY_T_MIN, DOUBLE_INF, hit_rec)) {
		// 如果没有击中任何对象，返回背景色
		if (m_use_sky_gradient) {
			glm::vec3 unit_direction = glm::normalize(r_in.direction());
			auto t = 0.5 * (unit_direction.y + 1.0);
			return static_cast<float>(1.0 - t) * glm::vec3(1.0, 1.0, 1.0) +
				   static_cast<float>(t) * glm::vec3(0.5, 0.7, 1.0);
		}
		return m_background_color;
	}

	ScatterRecord srec; // 存储散射信息

	// 材质不散射光线，则返回自发光颜色
	if (!hit_rec.mat_ptr->scatter(r_in, hit_rec, srec) || depth == 1) return hit_rec.mat_ptr->emitted(r_in, hit_rec);

	auto scatter_pdf_ptr = srec.pdf_ptr; // 散射方向分布
	auto mat_ptr = hit_rec.mat_ptr;		 // 击中物体的材质

	float rr_factor = 1.0F;
	// 俄罗斯轮盘赌 (Russian Roulette)
	if (depth < m_max_depth - RR_START_BOUNCE) {
		float p = std::max(srec.attenuation.x, std::max(srec.attenuation.y, srec.attenuation.z));
		p = std::clamp(p, RR_MIN_PROBABILITY, 1.0F); // 限制最小概率
		if (random_double() > p) return hit_rec.mat_ptr->emitted(r_in, hit_rec);
		// 能量补偿
		rr_factor = 1.0F / p;
	}

	std::shared_ptr<HittablePDF> light_pdf_ptr = nullptr;
	if (lights) {
		if (auto scene_ptr = std::dynamic_pointer_cast<Scene>(lights)) {
			if (!scene_ptr->objects.empty()) light_pdf_ptr = std::make_shared<HittablePDF>(lights, hit_rec.p);
		} else {
			light_pdf_ptr = std::make_shared<HittablePDF>(lights, hit_rec.p);
		}
	}

	int n_light_samples = 0;
	int n_mat_samples = 0;

	switch (m_strategy) {
		case SamplingStrategy::MIS:
			n_light_samples = 1;
			n_mat_samples = 1;
			break;
		case SamplingStrategy::LIGHT:
			n_light_samples = 1;
			n_mat_samples = 0;
			break;
		case SamplingStrategy::MATERIAL:
		default:
			n_light_samples = 0;
			n_mat_samples = 1;
			break;
	}

	// 如果没有光源，强制只使用材质采样
	if (!light_pdf_ptr) {
		n_light_samples = 0;
		n_mat_samples = 1;
	}

	// 针对镜面反射（Delta分布）的处理：
	// 镜面反射的 PDF 值为无穷大 (Dirac Delta)。
	// 在数值上，这意味着材质采样的权重应为 1，而光源采样的权重应为 0。
	if (srec.is_specular) {
		n_light_samples = 0;
		// 如果原本是 MIS，保持材质采样
		if (m_strategy == SamplingStrategy::MIS) {
			n_mat_samples = 1;
		}
	}

	// 采样任务结构体
	struct SamplingTask {
		std::shared_ptr<PDF> pdf; ///< 采样 pdf
		int num_samples;		  ///< 采样数量
		int next_depth;			  ///< 递归深度，对于光源采样为 1, 对于材质采样为 depth - 1
	};
	std::vector<SamplingTask> tasks;

	if (n_light_samples > 0 && light_pdf_ptr) {
		tasks.push_back({light_pdf_ptr, n_light_samples, 1});
	}
	if (n_mat_samples > 0) {
		tasks.push_back({scatter_pdf_ptr, n_mat_samples, depth - 1});
	}

	// 采样结果结构体
	struct SampleResult {
		glm::vec3 dir;
		double pdf;
		glm::vec3 radiance;
	};
	std::vector<std::vector<SampleResult>> sample_results(tasks.size());

	// 1. 生成所有样本并计算 Radiance
	for (size_t t_idx = 0; t_idx < tasks.size(); ++t_idx) {
		const auto& task = tasks[t_idx];
		for (int i = 0; i < task.num_samples; ++i) {
			auto dir = task.pdf->generate();
			auto pdf_val = task.pdf->value(dir);

			if (pdf_val <= 0) continue;

			Ray r_next(hit_rec.p, dir);
			auto cos_theta = glm::dot(hit_rec.normal, glm::normalize(dir));
			if (cos_theta <= 0) continue;

			auto Li = m_ray_color(r_next, world, lights, task.next_depth);
			auto brdf = mat_ptr->brdf(r_in, hit_rec, r_next);

			if (cos_theta > 0) {
				// 暂存未加权的贡献: brdf * Li * cos / pdf
				auto unweighted_contrib = brdf * Li * (float)cos_theta * rr_factor / (float)pdf_val;
				sample_results[t_idx].push_back({dir, pdf_val, unweighted_contrib});
			}
		}
	}

	// MIS 启发式函数 (例如 Power Heuristic)
	auto mis_heuristic = [](const double val) { return val; };

	// 计算 MIS 权重的辅助函数
	auto cal_mis_weight = [&](const double cur_pdf, const int cur_n_samples, const glm::vec3& dir) {
		if (tasks.size() <= 1) return 1.0;

		double sum = 0.0;
		for (size_t t_idx = 0; t_idx < tasks.size(); ++t_idx) {
			if (sample_results[t_idx].size() <= 0) continue;
			double p = tasks[t_idx].pdf->value(dir);
			sum += mis_heuristic(sample_results[t_idx].size() * p);
		}
		return (sum > 0) ? mis_heuristic(cur_n_samples * cur_pdf) / sum : 0.0;
	};

	// 2. 计算 MIS 权重并合并结果
	glm::vec3 L_scatter(0.0F);

	for (size_t t_idx = 0; t_idx < tasks.size(); ++t_idx) {
		if (sample_results[t_idx].size() <= 0) continue;

		glm::vec3 L_task(0.0F);
		for (const auto& res : sample_results[t_idx]) {
			double w = cal_mis_weight(res.pdf, sample_results[t_idx].size(), res.dir);
			L_task += (float)w * res.radiance;
		}
		L_scatter += L_task / (float)sample_results[t_idx].size();
	}

	return mat_ptr->emitted(r_in, hit_rec) + L_scatter;
}

void SoftTracer::_write_color(std::vector<unsigned char>& image_data, const int index, const glm::vec3 pixel_color) {
	auto r = pixel_color.x;
	auto g = pixel_color.y;
	auto b = pixel_color.z;

	// gamma 校正，gamma=2.0
	r = sqrt(r);
	g = sqrt(g);
	b = sqrt(b);

	image_data[index] = static_cast<unsigned char>(256 * std::clamp(r, 0.0F, 0.999F));
	image_data[index + 1] = static_cast<unsigned char>(256 * std::clamp(g, 0.0F, 0.999F));
	image_data[index + 2] = static_cast<unsigned char>(256 * std::clamp(b, 0.0F, 0.999F));
}

void SoftTracer::render(const Scene& scene, const std::shared_ptr<Hittable>& lights, const Camera& cam,
						const std::string& filename) {
	std::vector<unsigned char> image_data(static_cast<size_t>(_image_width * _image_height * 3));

	std::atomic<int> scanlines_finished = 0;

#pragma omp parallel for schedule(dynamic, 1)
	for (int j = _image_height - 1; j >= 0; --j) {
		int finished = ++scanlines_finished;
		if (finished % 10 == 0) {
			fmt::print("\rScanlines remaining: {}   ", _image_height - finished);
			std::fflush(stdout);
		}
		for (int i = 0; i < _image_width; ++i) {
			glm::vec3 pixel_color(0, 0, 0);
			for (int s = 0; s < m_samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (_image_width - 1);
				auto v = (j + random_double()) / (_image_height - 1);
				Ray r = cam.get_ray(u, v);
				pixel_color += m_ray_color(r, scene, lights, m_max_depth);
			}

			pixel_color = pixel_color / static_cast<float>(m_samples_per_pixel);

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
