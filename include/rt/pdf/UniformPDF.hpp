#pragma once
#include "rt/pdf/PDF.hpp"
#include "rt/core/ONB.hpp"
#include "rt/core/Utils.hpp"

namespace rt {

/**
 * @brief 均匀分布的 PDF。
 */
class UniformPDF : public PDF {
public:
	/**
	 * @brief 构造函数。
	 * 
	 * @param w 表面法线方向。
	 */
	UniformPDF(const glm::vec3& w) { _uvw.build_from_w(w); }

	virtual double value(const glm::vec3& direction) const override {
		return 1 / (2 * pi);
	}

	virtual glm::vec3 generate() const override {
		return _uvw.transform_to_world(random_unit_vector_hemisphere());
	}

private:
	ONB _uvw;
};

} // namespace rt
