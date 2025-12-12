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
	Metal(const glm::vec3& a, const double f) : _albedo(a), _fuzz(f < 1 ? f : 1) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const override;

	[[nodiscard]] glm::vec3 brdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const override;

	[[nodiscard]] glm::vec3 emitted(const Ray& /*r_in*/, const HitRecord& /*rec*/) const override {
		return {0, 0, 0};
	}

private:
	glm::vec3 _albedo; ///< 材质的反照率（颜色）。
	double _fuzz;	   ///< 模糊度参数，用于模糊反射。
};

} // namespace rt
