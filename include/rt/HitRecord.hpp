#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "rt/Ray.hpp"

namespace rt {

class Material; // Forward declaration

/**
 * @brief Structure to record details of a ray-object intersection.
 */
struct HitRecord {
	glm::vec3 p;                  ///< Point of intersection.
	glm::vec3 normal;             ///< Surface normal at the intersection point.
	std::shared_ptr<Material> mat_ptr; ///< Pointer to the material of the hit object.
	double t;                     ///< Ray parameter t at the intersection point.
	bool front_face;              ///< True if the ray hit the front face of the surface.

	/**
	 * @brief Set the face normal based on the ray direction and outward normal.
	 * 
	 * Determines if the hit is on the front or back face and ensures the normal
	 * always points against the ray.
	 * 
	 * @param r The incoming ray.
	 * @param outward_normal The geometric outward normal of the surface.
	 */
	inline void set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
		front_face = glm::dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

} // namespace rt
