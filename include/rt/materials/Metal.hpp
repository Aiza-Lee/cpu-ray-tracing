#pragma once
#include "rt/materials/Material.hpp"

namespace rt {

/**
 * @brief 金属（反射）材质。
 */
class Metal : public Material {
public:
	/**
	 * @brief  构造一个新的金属材质对象。
	 * 
	 * @param a 材质的反照率（颜色）。
	 * @param f 模糊度参数（0 表示完美镜面，1 表示非常模糊）。
	 */
	Metal(const glm::vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
	) const override;

public:
	glm::vec3 albedo; ///< 材质的反照率（颜色）。
	double fuzz;      ///< 模糊度参数，用于模糊反射。
};

} // namespace rt
