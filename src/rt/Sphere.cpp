#include "rt/Sphere.hpp"
#include <cmath>

namespace rt {

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
	glm::vec3 oc = r.origin() - center;
	auto a = glm::length(r.direction()) * glm::length(r.direction());
	auto half_b = glm::dot(oc, r.direction());
	auto c = glm::length(oc) * glm::length(oc) - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = std::sqrt(discriminant);

	// 找到最近的根，在 t_min 和 t_max 范围内
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	glm::vec3 outward_normal = (rec.p - center) / static_cast<float>(radius);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}

} // namespace rt
