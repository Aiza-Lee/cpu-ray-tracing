#pragma once
#include "rt/pdf/PDF.hpp"
#include "rt/core/ONB.hpp"
#include "rt/core/Utils.hpp"

namespace rt {

/**
 * @brief 基于余弦分布的 PDF。
 * 
 * 适用于 Lambertian 材质的采样。
 */
class CosinePDF : public PDF {
public:
	/**
	 * @brief 构造函数。
	 * 
	 * @param w 表面法线方向。
	 */
	CosinePDF(const glm::vec3& w) { uvw.build_from_w(w); }

	virtual double value(const glm::vec3& direction) const override {
		auto cosine = glm::dot(glm::normalize(direction), uvw.w());
		return (cosine <= 0) ? 0 : cosine / pi;
	}

	virtual glm::vec3 generate() const override {
		return uvw.local(random_cosine_direction());
	}

public:
	ONB uvw;
};

} // namespace rt
