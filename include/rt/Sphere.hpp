#pragma once
#include "rt/Hittable.hpp"
#include <glm/glm.hpp>

namespace rt {

/**
 * @brief Class representing a sphere object.
 */
class Sphere : public Hittable {
public:
	Sphere() {}
	
	/**
	 * @brief Construct a new Sphere object.
	 * 
	 * @param cen Center of the sphere.
	 * @param r Radius of the sphere.
	 * @param m Material of the sphere.
	 */
	Sphere(glm::vec3 cen, double r, std::shared_ptr<Material> m)
		: center(cen), radius(r), mat_ptr(m) {};

	/**
	 * @brief Determine if a ray hits the sphere.
	 * 
	 * @param r The ray to test.
	 * @param t_min The minimum t value.
	 * @param t_max The maximum t value.
	 * @param rec Reference to a HitRecord to store intersection details.
	 * @return true If the ray hits the sphere.
	 * @return false Otherwise.
	 */
	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

public:
	glm::vec3 center;                  ///< Center of the sphere.
	double radius;                     ///< Radius of the sphere.
	std::shared_ptr<Material> mat_ptr; ///< Material of the sphere.
};

} // namespace rt
