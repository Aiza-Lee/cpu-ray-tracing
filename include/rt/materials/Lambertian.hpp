#pragma once
#include "rt/materials/Material.hpp"

namespace rt {

/**
 * @brief 漫反射材质。
 */
class Lambertian : public Material {
public:
	/**
	 * @brief  构造一个新的漫反射材质对象。
	 * 
	 * @param a 材质的反照率（颜色）。
	 */
	Lambertian(const glm::vec3& a) : albedo(a) {}

	bool scatter(
		const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
	) const override;

	[[nodiscard]] glm::vec3 brdf(
		const Ray& r_in, const HitRecord& rec, const Ray& scattered
	) const override;

	[[nodiscard]] glm::vec3 emitted(const Ray& r_in, const HitRecord& rec) const override {
		return {0,0,0};
	}

public:
	glm::vec3 albedo; ///< 材质的反照率（颜色）。
};

} // namespace rt
