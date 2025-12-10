#pragma once
#include <glm/glm.hpp>

namespace rt {

/**
 * @brief 概率密度函数 (Probability Density Function) 抽象基类。
 */
class PDF {
public:
	virtual ~PDF() = default;

	/**
	 * @brief 计算给定方向的 PDF 值。
	 * 
	 * @param direction 采样方向。
	 * @return double PDF 值。
	 */
	[[nodiscard]] virtual double value(const glm::vec3& direction) const = 0;

	/**
	 * @brief 根据 PDF 分布生成一个随机方向。
	 * 
	 * @return glm::vec3 生成的随机方向向量。
	 */
	[[nodiscard]] virtual glm::vec3 generate() const = 0;
};

} // namespace rt
