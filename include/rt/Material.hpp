#pragma once
#include "rt/Utils.hpp"
#include "rt/Ray.hpp"
#include "rt/HitRecord.hpp"

namespace rt {

/**
 * @brief Abstract base class for materials.
 * 
 * Materials determine how rays interact with surfaces (scattering, absorption, etc.).
 */
class Material {
public:
	virtual ~Material() = default;

	/**
	 * @brief Scatter a ray hitting the material.
	 * 
	 * @param r_in The incoming ray.
	 * @param rec The hit record containing intersection details.
	 * @param attenuation Output color attenuation.
	 * @param scattered Output scattered ray.
	 * @return true If the ray is scattered.
	 * @return false If the ray is absorbed.
	 */
	virtual bool scatter(
		const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered
	) const = 0;
};

/**
 * @brief Lambertian (diffuse) material.
 */
class Lambertian : public Material {
public:
	/**
	 * @brief Construct a new Lambertian material.
	 * 
	 * @param a The albedo (color) of the material.
	 */
	Lambertian(const glm::vec3& a) : albedo(a) {}

	virtual bool scatter(
		const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered
	) const override;

public:
	glm::vec3 albedo; ///< The albedo (reflectance) of the material.
};

/**
 * @brief Metal (reflective) material.
 */
class Metal : public Material {
public:
	/**
	 * @brief Construct a new Metal material.
	 * 
	 * @param a The albedo (color) of the material.
	 * @param f The fuzziness factor (0 for perfect mirror, 1 for very fuzzy).
	 */
	Metal(const glm::vec3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered
	) const override;

public:
	glm::vec3 albedo; ///< The albedo (reflectance) of the material.
	double fuzz;      ///< Fuzziness parameter for blurry reflections.
};

} // namespace rt
