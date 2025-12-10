#pragma once
#include <glm/glm.hpp>

namespace rt {

/**
 * @brief 光线类。
 * 
 * 光线由一个起点和一个方向向量定义。
 * P(t) = origin + t * direction
 */
class Ray {
public:
	/**
	 * @brief 创建一条新的光线。
	 * 
	 * @param origin 光线的起点。
	 * @param direction 光线的方向。
	 */
	Ray(const glm::vec3& origin, const glm::vec3& direction)
		: _orig(origin), _dir(direction) {}

	/**
	 * @brief 获取光线的起点。
	 * @return glm::vec3 起点。
	 */
	[[nodiscard]] glm::vec3 origin() const { return _orig; }

	/**
	 * @brief 获取光线的方向。
	 * @return glm::vec3 方向。
	 */
	[[nodiscard]] glm::vec3 direction() const { return _dir; }

	/**
	 * @brief 计算光线在参数 t 处的点。
	 * 
	 * @param t 参数。
	 * @return glm::vec3 点 P(t)。
	 */
	[[nodiscard]] glm::vec3 at(const double t) const {
		return _orig + static_cast<float>(t) * _dir;
	}

private:
	glm::vec3 _orig; ///< 光线的起点。
	glm::vec3 _dir;  ///< 光线的方向。
};

} // namespace rt
