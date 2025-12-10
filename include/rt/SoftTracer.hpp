#pragma once
#include "rt/hittables/Scene.hpp"
#include "rt/core/Camera.hpp"
#include <vector>
#include <string>

namespace rt {

enum class SamplingStrategy {
	MIS,        ///< 多重重要性采样 (默认)
	Material,   ///< 仅材质采样 (BSDF)
	Light       ///< 仅光源采样 (NEE)
};

/**
 * @brief 路径追踪渲染器类。
 * 
 * 处理渲染过程，包括光线生成、颜色计算和图像输出。
 */
class SoftTracer {
public:
	/**
	 * @brief 构造一个新的 SoftTracer 对象。
	 * 
	 * @param width 图像宽度（像素）。
	 * @param height 图像高度（像素）。
	 * @param samples_per_pixel 每个像素的采样数，用于抗锯齿。
	 * @param max_depth 光线散射的最大递归深度。
	 */
	SoftTracer(int width, int height, int samples_per_pixel, int max_depth);

	/**
	 * @brief 设置背景颜色。
	 * 
	 * @param color 背景颜色。
	 * @param use_gradient 是否使用天空渐变（如果为 true，则忽略 color）。
	 */
	void set_background(const glm::vec3& color, bool use_gradient = false) {
		m_background_color = color;
		m_use_sky_gradient = use_gradient;
	}

	/**
	 * @brief 设置采样策略。
	 */
	void set_sampling_strategy(SamplingStrategy strategy) {
		m_strategy = strategy;
	}

// --- Constants ---
	static constexpr double RAY_T_MIN = 0.001;         ///< 光线相交检测的最小 t 值（防止自相交）。
	static constexpr int RR_START_BOUNCE = 3;          ///< 开始俄罗斯轮盘赌的弹射次数。
	static constexpr float RR_MIN_PROBABILITY = 0.05f; ///< 俄罗斯轮盘赌的最小继续概率。

	/**
	 * @brief 从相机视角渲染场景。
	 * 
	 * @param scene 要渲染的场景。
	 * @param cam 视角相机。
	 * @param filename 输出图像的文件名。
	 */
	void render(const Scene& scene, const std::shared_ptr<Hittable>& lights, const Camera& cam, const std::string& filename);

private:
	/**
	 * @brief 计算光线的颜色。
	 * 
	 * @param r 光线。
	 * @param world 场景。
	 * @param lights 光源列表。
	 * @param depth 当前递归深度。
	 * @return glm::vec3 光线颜色。
	 */
	glm::vec3 m_ray_color(const Ray& r, const Scene& world, const std::shared_ptr<Hittable>& lights, int depth);


	/**
	 * @brief 将像素的颜色写入图像数据缓冲区。
	 * 
	 * @param image_data 要写入的缓冲区。
	 * @param index 缓冲区中的索引。
	 * @param pixel_color 要写入的颜色。
	 */
	void m_write_color(std::vector<unsigned char>& image_data, int index, glm::vec3 pixel_color);

	int m_image_width;       ///< 图像宽度。
	int m_image_height;      ///< 图像高度。
	int m_samples_per_pixel; ///< 每个像素的采样数。
	int m_max_depth;         ///< 最大递归深度。
	glm::vec3 m_background_color = glm::vec3(0,0,0);
	bool m_use_sky_gradient = true;
	SamplingStrategy m_strategy = SamplingStrategy::MIS;
};

} // namespace rt
