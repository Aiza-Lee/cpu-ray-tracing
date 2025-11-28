#pragma once
#include "rt/core/Utils.hpp"
#include "rt/core/Ray.hpp"

namespace rt {

/**
 * @brief 表示场景中摄像机的类。
 * 
 * 摄像机负责从相机位置通过图像平面生成光线。
 */
class Camera {
public:
	/**
	 * @brief 构造一个新的 Camera 对象。
	 * 
	 * @param lookfrom 摄像机的位置。
	 * @param lookat 摄像机所看的点。
	 * @param vup 摄像机的“上”向量。
	 * @param vfov 垂直视场角，单位为度。
	 * @param aspect_ratio 图像的宽高比（宽/高）。
	 */
	Camera(
		glm::vec3 lookfrom,
		glm::vec3 lookat,
		glm::vec3 vup,
		double vfov,
		double aspect_ratio
	);

	/**
	 * @brief 对于给定的图像平面坐标 (s, t)，生成一条光线。
	 * 
	 * @param s 水平坐标分数（0 到 1）。
	 * @param t 垂直坐标分数（0 到 1）。
	 * @return Ray 生成的光线。
	 */
	Ray get_ray(double s, double t) const;

private:
	glm::vec3 m_origin;            ///< 摄像机原点位置。
	glm::vec3 m_lower_left_corner; ///< 视图平面的左下角。
	glm::vec3 m_horizontal;        ///< 视图平面的水平向量。
	glm::vec3 m_vertical;          ///< 视图平面的垂直向量。
};

} // namespace rt
