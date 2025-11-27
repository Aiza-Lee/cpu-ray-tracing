#pragma once
#include "rt/Hittable.hpp"
#include <memory>
#include <vector>

namespace rt {

using std::shared_ptr;
using std::make_shared;

/**
 * @brief Class representing a scene (a list of hittable objects).
 */
class Scene : public Hittable {
public:
	Scene() {}
	
	/**
	 * @brief Construct a new Scene object with a single object.
	 * 
	 * @param object Pointer to the hittable object to add.
	 */
	Scene(shared_ptr<Hittable> object) { add(object); }

	/**
	 * @brief Clear all objects from the scene.
	 */
	void clear() { objects.clear(); }

	/**
	 * @brief Add a hittable object to the scene.
	 * 
	 * @param object Pointer to the hittable object.
	 */
	void add(shared_ptr<Hittable> object) { objects.push_back(object); }

	/**
	 * @brief Determine if a ray hits any object in the scene.
	 * 
	 * Finds the closest hit in the range [t_min, t_max].
	 * 
	 * @param r The ray to test.
	 * @param t_min The minimum t value.
	 * @param t_max The maximum t value.
	 * @param rec Reference to a HitRecord to store the closest intersection details.
	 * @return true If anything was hit.
	 * @return false Otherwise.
	 */
	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

public:
	std::vector<shared_ptr<Hittable>> objects; ///< List of objects in the scene.
};

} // namespace rt
