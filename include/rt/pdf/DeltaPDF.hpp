#pragma once
#include "rt/pdf/PDF.hpp"
#include <glm/glm.hpp>

namespace rt {

/**
 * @brief 单点分布 PDF (Delta Distribution)。
 * 
 * 用于完美镜面反射等情况。
 * 在特定方向上概率为 1 (模拟 Dirac Delta)，其他方向为 0。
 */
class DeltaPDF : public PDF {
public:
	DeltaPDF(const glm::vec3& w) : w(glm::normalize(w)) {}

	virtual double value(const glm::vec3& direction) const override {
		// 只有当方向非常接近预设方向时，才返回 1.0
		// 注意：这在数学上不是严格的 PDF density (应该是无穷大)，
		// 但为了配合 BRDF = albedo / cos_theta 的处理方式，这里返回 1.0。
		auto cosine = glm::dot(glm::normalize(direction), w);
		return (cosine > 0.9999) ? 1.0 : 0.0;
	}

	virtual glm::vec3 generate() const override {
		return w;
	}

private:
	glm::vec3 w;
};

} // namespace rt
