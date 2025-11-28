#pragma once
#include "rt/Utils.hpp"
#include "rt/Ray.hpp"
#include "rt/HitRecord.hpp"

namespace rt {

/**
 * @brief 材质的抽象基类。
 * 
 * 材质决定了光线与表面的相互作用（散射、吸收等）。
 */
class Material {
public:
	virtual ~Material() = default;

	/**
	 * @brief 散射击中材质的光线。
	 * 
	 * @param r_in 入射光线。
	 * @param rec 包含相交细节的击中记录。
	 * @param attenuation [out] 输出颜色衰减。
	 * @param scattered [out] 输出散射光线。
	 * @return true 如果光线被散射。
	 * @return false 如果光线被吸收。
	 */
	virtual bool scatter(
		const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered
	) const = 0;
};

/**
 * @brief 漫反射材质。
 */
class Lambertian : public Material {
public:
	/**
	 * @brief Construct a new Lambertian material.
	 * 
	 * @param a 材质的反照率（颜色）。
	 */
	Lambertian(const glm::vec3& a) : albedo(a) {}

	virtual bool scatter(
		const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered
	) const override;

public:
	glm::vec3 albedo; ///< 材质的反照率（颜色）。
};

/**
 * @brief 金属（反射）材质。
 */
class Metal : public Material {
public:
	/**
	 * @brief Construct a new Metal material.
	 * 
	 * @param a 材质的反照率（颜色）。
	 * @param f 模糊度参数（0 表示完美镜面，1 表示非常模糊）。
	 */
	Metal(const glm::vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered
	) const override;

public:
	glm::vec3 albedo; ///< 材质的反照率（颜色）。
	double fuzz;      ///< 模糊度参数，用于模糊反射。
};

} // namespace rt
