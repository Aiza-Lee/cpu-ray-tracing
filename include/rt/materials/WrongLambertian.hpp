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
	WrongLambertian(const glm::vec3& a) : _albedo(a) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const override;

	[[nodiscard]] glm::vec3 brdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const override;

	[[nodiscard]] glm::vec3 emitted(const Ray& /*r_in*/, const HitRecord& /*rec*/) const override {
		return {0, 0, 0};
	}

private:
	glm::vec3 _albedo; ///< 材质的反照率（颜色）。
};

} // namespace rt
