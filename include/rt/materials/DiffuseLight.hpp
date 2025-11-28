#pragma once
#include "rt/materials/Material.hpp"

namespace rt {

/**
 * @brief 扩散光材质（发光体）。
 */
class DiffuseLight : public Material {
public:
	/**
	 * @brief Construct a new Diffuse Light material.
	 * 
	 * @param c 发光的颜色。
	 */
	DiffuseLight(const glm::vec3& c) : emit(c) {}

	virtual bool scatter(
		const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
	) const override {
		// 发光体不散射光线
		return false;
	}

	virtual glm::vec3 emitted(
		const Ray& r_in, const HitRecord& rec, double u, double v, const glm::vec3& p
	) const override {
		// 直接返回发光颜色
		return emit;
	}

public:
	glm::vec3 emit;
};

} // namespace rt
