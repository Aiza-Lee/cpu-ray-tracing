#pragma once
#include "rt/Scene.hpp"
#include "rt/Camera.hpp"
#include <vector>
#include <string>

namespace rt {

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
	 * @brief 从相机视角渲染场景。
	 * 
	 * @param scene 要渲染的场景。
	 * @param cam 视角相机。
	 * @param filename 输出图像的文件名。
	 */
	void render(const Scene& scene, const Camera& cam, const std::string& filename);

private:
	/**
	 * @brief 计算光线的颜色。
	 * 
	 * @param r 光线。
	 * @param world 包含对象的场景。
	 * @param depth 当前递归深度。
	 * @return glm::vec3 计算得到的颜色。
	 */
	glm::vec3 _ray_color(const Ray& r, const Scene& world, int depth);

	/**
	 * @brief 将像素的颜色写入图像数据缓冲区。
	 * 
	 * @param image_data 要写入的缓冲区。
	 * @param index 缓冲区中的索引。
	 * @param pixel_color 要写入的颜色。
	 */
	void _write_color(std::vector<unsigned char>& image_data, int index, glm::vec3 pixel_color);

	int _image_width;       ///< 图像宽度。
	int _image_height;      ///< 图像高度。
	int _samples_per_pixel; ///< 每个像素的采样数。
	int _max_depth;         ///< 最大递归深度。
};

} // namespace rt
