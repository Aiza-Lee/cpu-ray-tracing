#pragma once
#include <glm/glm.hpp>

namespace rt {

/**
 * @brief Class representing a ray.
 * 
 * A ray is defined by an origin point and a direction vector.
 * P(t) = origin + t * direction
 */
class Ray {
public:
	Ray() {}
	
	/**
	 * @brief Construct a new Ray object.
	 * 
	 * @param origin The origin of the ray.
	 * @param direction The direction of the ray.
	 */
	Ray(const glm::vec3& origin, const glm::vec3& direction)
		: orig(origin), dir(direction) {}

	/**
	 * @brief Get the origin of the ray.
	 * @return glm::vec3 The origin.
	 */
	glm::vec3 origin() const { return orig; }

	/**
	 * @brief Get the direction of the ray.
	 * @return glm::vec3 The direction.
	 */
	glm::vec3 direction() const { return dir; }

	/**
	 * @brief Calculate the point at parameter t along the ray.
	 * 
	 * @param t The parameter.
	 * @return glm::vec3 The point P(t).
	 */
	glm::vec3 at(double t) const {
		return orig + static_cast<float>(t) * dir;
	}

public:
	glm::vec3 orig; ///< The origin of the ray.
	glm::vec3 dir;  ///< The direction of the ray.
};

} // namespace rt
