#include "rt/hittables/Sphere.hpp"
#include "rt/core/Utils.hpp"
#include "rt/core/ONB.hpp"
#include <cmath>

namespace rt {

static void get_sphere_uv(const glm::vec3& p, double& u, double& v) {
	auto theta = acos(-p.y);
	auto phi = atan2(-p.z, p.x) + pi;

	u = phi / (2 * pi);
	v = theta / pi;
}

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
	get_sphere_uv(outward_normal, rec.u, rec.v);
	rec.mat_ptr = mat_ptr;

	return true;
}

double Sphere::pdf_value(const glm::vec3& origin, const glm::vec3& v) const {
	HitRecord rec;
	if (!this->hit(Ray(origin, v), 0.001, infinity, rec))
		return 0;

	auto cos_theta_max = sqrt(1 - radius*radius/glm::distance2(center, origin));
	auto solid_angle = 2*pi*(1-cos_theta_max);

	return  1 / solid_angle;
}

glm::vec3 Sphere::random(const glm::vec3& origin) const {
	glm::vec3 direction = center - origin;
	auto distance_squared = glm::length2(direction);
	ONB uvw;
	uvw.build_from_w(direction);
	return uvw.local(random_to_sphere(radius, distance_squared));
}

} // namespace rt
