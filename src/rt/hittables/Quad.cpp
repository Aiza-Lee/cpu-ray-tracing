#include "rt/hittables/Quad.hpp"
#include <cmath>

namespace rt {

bool Quad::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
	auto denom = glm::dot(normal, r.direction());

	// 如果光线平行于平面，则不相交
	if (std::abs(denom) < 1e-8)
		return false;

	// 计算交点参数 t
	auto t = (D - glm::dot(normal, r.origin())) / denom;
	if (t < t_min || t > t_max)
		return false;

	// 确定交点是否在四边形内
	auto intersection = r.at(t);
	glm::vec3 planar_hitpt_vector = intersection - Q;
	auto alpha = glm::dot(w, glm::cross(planar_hitpt_vector, v));
	auto beta = glm::dot(w, glm::cross(u, planar_hitpt_vector));

	if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1)
		return false;

	// 记录击中信息
	rec.t = t;
	rec.p = intersection;
	rec.mat_ptr = mat_ptr;
	rec.set_face_normal(r, normal);
	rec.u = alpha;
	rec.v = beta;

	return true;
}

} // namespace rt
