#include "rt/hittables/Quad.hpp"
#include "rt/core/Utils.hpp"
#include <cmath>

namespace rt {

bool Quad::hit(const Ray& r, const double t_min, const double t_max, HitRecord& rec) const {
	const auto denom = glm::dot(_normal, r.direction());

	// 如果光线平行于平面，则不相交
	if (std::abs(denom) < 1e-8) return false;

	// 计算交点参数 t
	const auto t = (_d_param - glm::dot(_normal, r.origin())) / denom;
	if (t < t_min || t > t_max) return false;

	// 确定交点是否在四边形内
	const auto intersection = r.at(t);
	const glm::vec3 planar_hitpt_vector = intersection - _corner;
	const auto alpha = glm::dot(_w, glm::cross(planar_hitpt_vector, _v));
	const auto beta = glm::dot(_w, glm::cross(_u, planar_hitpt_vector));

	if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1) return false;

	// 记录击中信息
	rec.t = t;
	rec.p = intersection;
	rec.mat_ptr = _mat_ptr;
	rec.set_face_normal(r, _normal);
	rec.u = alpha;
	rec.v = beta;

	return true;
}

double Quad::pdf_value(const glm::vec3& origin, const glm::vec3& v) const {
	HitRecord rec;
	if (!this->hit(Ray(origin, v), 0.001, DOUBLE_INF, rec)) {
		return 0;
	}

	const auto distance_squared = rec.t * rec.t * glm::length2(v);
	const auto cosine = std::fabs(glm::dot(v, rec.normal) / glm::length(v));

	return distance_squared / (cosine * _area);
}

glm::vec3 Quad::random(const glm::vec3& origin) const {
	// 范围从 [0, 1] 变为 [0.00005, 0.99995]，避免边缘浮点误差
	const auto r1 = 0.00005 + 0.9999 * random_double();
	const auto r2 = 0.00005 + 0.9999 * random_double();
	const auto p = _corner + static_cast<float>(r1) * _u + static_cast<float>(r2) * _v;
	return p - origin;
}

} // namespace rt
