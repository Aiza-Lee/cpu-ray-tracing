#pragma once

#include "rt/pdf/PDF.hpp"
#include "rt/core/ONB.hpp"
#include "rt/core/Utils.hpp"

#include <fmt/core.h>

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
	CosinePDF(const glm::vec3& w) { _uvw.build_from_w(w); }

	[[nodiscard]] double value(const glm::vec3& direction) const override {
		const auto cosine = glm::dot(glm::normalize(direction), _uvw.w());
		return (cosine <= 0) ? 0 : cosine / pi;
	}

	[[nodiscard]] glm::vec3 generate() const override {
		const auto dbg_mid = random_cosine_direction();
		auto cosine_theta = glm::dot(glm::normalize(_uvw.transform_to_world(dbg_mid)), _uvw.w());
		// fmt::println(stderr, 
		// 	"CosinePDF::generate() - cosine_theta: {}\n  - return ({},{},{})", 
		// 	cosine_theta, _uvw.transform_to_world(dbg_mid).x, _uvw.transform_to_world(dbg_mid).y, _uvw.transform_to_world(dbg_mid).z);
		return _uvw.transform_to_world(dbg_mid);
	}

private:
	ONB _uvw;
};

} // namespace rt
