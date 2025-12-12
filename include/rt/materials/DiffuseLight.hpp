#pragma once
#include "rt/materials/Material.hpp"

namespace rt {

/**
 * @brief 扩散光材质（发光体）。
 */
class DiffuseLight : public Material {
public:
	/**
	 * @brief  构造一个新的扩散光材质对象。
	 *
	 * @param c 发光的颜色。
	 */
	DiffuseLight(const glm::vec3& c) : _emit(c) {}

	/**
	 * @brief 构造一个新的扩散光材质对象。
	 *
	 * @param c 发光的颜色。
	 * @param intensity 发光强度。
	 */
	DiffuseLight(const glm::vec3& c, double intensity) : _emit(c * (float)intensity) {}

	bool scatter(const Ray& /*r_in*/, const HitRecord& /*rec*/, ScatterRecord& /*srec*/
	) const override {
		// 发光体不散射光线
		return false;
	}

	[[nodiscard]] glm::vec3 emitted(const Ray& /*r_in*/, const HitRecord& /*rec*/
	) const override {
		// 直接返回发光颜色
		return _emit;
	}

	[[nodiscard]] glm::vec3 brdf(const Ray& /*r_in*/, const HitRecord& /*rec*/, const Ray& /*scattered*/) const override {
		return {0, 0, 0};
	}

private:
	glm::vec3 _emit;
};

} // namespace rt
