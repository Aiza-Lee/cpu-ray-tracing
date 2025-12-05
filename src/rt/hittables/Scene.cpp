#include "rt/hittables/Scene.hpp"
#include "rt/core/Utils.hpp"

namespace rt {

bool Scene::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
	HitRecord temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

double Scene::pdf_value(const glm::vec3& origin, const glm::vec3& v) const {
	if (objects.empty()) return 0.0;

	auto sum = 0.0;
	for (const auto& obj : objects)
		sum += obj->pdf_value(origin, v);

	return sum / static_cast<double>(objects.size());
}

glm::vec3 Scene::random(const glm::vec3& origin) const {
	if (objects.empty()) return glm::vec3(1,0,0);

	auto int_size = static_cast<int>(objects.size());
	return objects[random_int(0, int_size-1)]->random(origin);
}

} // namespace rt
