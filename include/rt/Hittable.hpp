#pragma once
#include "rt/Ray.hpp"
#include "rt/HitRecord.hpp"

namespace rt {

/**
 * @brief Abstract base class for objects that can be hit by a ray.
 */
class Hittable {
public:
	virtual ~Hittable() = default;

	/**
	 * @brief Determine if a ray hits the object.
	 * 
	 * @param r The ray to test.
	 * @param t_min The minimum t value to consider for a valid hit.
	 * @param t_max The maximum t value to consider for a valid hit.
	 * @param rec Reference to a HitRecord to store intersection details.
	 * @return true If the ray hits the object within the range [t_min, t_max].
	 * @return false Otherwise.
	 */
	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};

} // namespace rt
