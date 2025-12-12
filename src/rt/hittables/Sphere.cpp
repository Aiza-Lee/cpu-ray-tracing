#include "rt/hittables/Sphere.hpp"
#include "rt/core/Utils.hpp"
#include "rt/core/ONB.hpp"
#include <cmath>

namespace rt {

static void get_sphere_uv(const glm::vec3& p, double& u, double& v) {
	const auto theta = acos(-p.y);
	const auto phi = atan2(-p.z, p.x) + PI;

	u = phi / (2 * PI);
	v = theta / PI;
}

bool Sphere::hit(const Ray& r, const double t_min, const double t_max, HitRecord& rec) const {
	const glm::vec3 co = r.origin() - _center;
	const auto a = glm::dot(r.direction(), r.direction());
	const auto half_b = glm::dot(co, r.direction());
	const auto c = glm::dot(co, co) - _radius * _radius;

	const auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	const auto sqrtd = std::sqrt(discriminant);

	// 找到最近的根，在 t_min 和 t_max 范围内
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	const glm::vec3 outward_normal = (rec.p - _center) / static_cast<float>(_radius);
	rec.set_face_normal(r, outward_normal);
	get_sphere_uv(outward_normal, rec.u, rec.v);
	rec.mat_ptr = _mat_ptr;

	return true;
}

double Sphere::pdf_value(const glm::vec3& origin, const glm::vec3& v) const {
	if (HitRecord rec; !this->hit(Ray(origin, v), 0.001, DOUBLE_INF, rec))
		return 0;

	const auto cos_theta_max = sqrt(1.0 - _radius * _radius / glm::distance2(_center, origin));
	const auto solid_angle = 2.0 * PI * (1.0 - cos_theta_max);

	return  1 / solid_angle;
}

glm::vec3 Sphere::random(const glm::vec3& origin) const {
	const glm::vec3 direction = _center - origin;
	const auto distance_squared = glm::length2(direction);
	ONB uvw;
	uvw.build_from_w(direction);
	// 使用略小于实际半径的值进行采样，避免生成刚好擦过球体边缘的光线。
	// 边缘光线容易因浮点误差导致在 pdf_value 的 hit 检测中失败，从而产生黑点。
	return uvw.transform_to_world(random_to_sphere(0.9999 * _radius, distance_squared));
}

} // namespace rt
