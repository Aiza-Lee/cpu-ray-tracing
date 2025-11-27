#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <random>
#include <limits>
#include <memory>

namespace rt {

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = glm::pi<double>();

/**
 * @brief Convert degrees to radians.
 * 
 * @param degrees Angle in degrees.
 * @return double Angle in radians.
 */
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

/**
 * @brief Generate a random double in [0, 1).
 * 
 * @return double Random number.
 */
inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

/**
 * @brief Generate a random double in [min, max).
 * 
 * @param min Minimum value.
 * @param max Maximum value.
 * @return double Random number.
 */
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

/**
 * @brief Generate a random vector with components in [0, 1).
 * 
 * @return glm::vec3 Random vector.
 */
inline glm::vec3 random_vec3() {
	return glm::vec3(random_double(), random_double(), random_double());
}

/**
 * @brief Generate a random vector with components in [min, max).
 * 
 * @param min Minimum value for components.
 * @param max Maximum value for components.
 * @return glm::vec3 Random vector.
 */
inline glm::vec3 random_vec3(double min, double max) {
	return glm::vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

/**
 * @brief Generate a random point inside the unit sphere.
 * 
 * @return glm::vec3 Random point inside unit sphere.
 */
inline glm::vec3 random_in_unit_sphere() {
	while (true) {
		auto p = random_vec3(-1,1);
		if (glm::length(p) * glm::length(p) >= 1) continue;
		return p;
	}
}

/**
 * @brief Generate a random unit vector.
 * 
 * @return glm::vec3 Random unit vector.
 */
inline glm::vec3 random_unit_vector() {
	return glm::normalize(random_in_unit_sphere());
}

} // namespace rt
