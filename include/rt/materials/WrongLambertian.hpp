#pragma once
#include "rt/materials/Material.hpp"

namespace rt {

/**
 * @brief 错误的漫反射（未使用关于余弦的重要性采样）材质。
 */
class WrongLambertian : public Material {
public:
	/**
	 * @brief  构造一个新的漫反射材质对象。
	 * 
	 * @param a 材质的反照率（颜色）。
	 */
	WrongLambertian(const glm::vec3& a) : albedo(a) {}

	virtual bool scatter(
		const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
	) const override;

	virtual glm::vec3 brdf(
		const Ray& r_in, const HitRecord& rec, const Ray& scattered
	) const override;

public:
	glm::vec3 albedo; ///< 材质的反照率（颜色）。
};

} // namespace rt
