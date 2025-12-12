#pragma once
#include "rt/hittables/Hittable.hpp"
#include "rt/materials/Material.hpp"
#include <glm/geometric.hpp>

namespace rt {

/**
 * @brief 四边形 (Quadrilateral) 几何体。
 *
 * 由一个角点 Q 和两个边向量 u, v 定义。
 */
class Quad : public Hittable {
public:
	Quad(const glm::vec3& Q, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> m) :
		_corner(Q), _u(u), _v(v), _mat_ptr(std::move(m)) {
		auto n = glm::cross(u, v);
		_normal = glm::normalize(n);
		_d_param = glm::dot(_normal, Q);
		_w = n / glm::dot(n, n);
		_area = glm::length(n);
	}

	bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

	[[nodiscard]] double pdf_value(const glm::vec3& origin, const glm::vec3& v) const override;
	[[nodiscard]] glm::vec3 random(const glm::vec3& origin) const override;

private:
	glm::vec3 _corner;					///< 角点
	glm::vec3 _u;						///< 边向量 u
	glm::vec3 _v;						///< 边向量 v
	std::shared_ptr<Material> _mat_ptr; ///< 材质
	glm::vec3 _normal{};				///< 法线方向
	glm::vec3 _w{};						///< 法线归一化辅助向量
	double _d_param{};					///< 平面方程中的 D 常数。
	double _area{};						///< 四边形的面积。
};

} // namespace rt
