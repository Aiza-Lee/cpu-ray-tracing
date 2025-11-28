#pragma once
#include "rt/hittables/Hittable.hpp"
#include "rt/materials/Material.hpp"

namespace rt {

/**
 * @brief 四边形 (Quadrilateral) 几何体。
 * 
 * 由一个角点 Q 和两个边向量 u, v 定义。
 */
class Quad : public Hittable {
public:
	Quad(const glm::vec3& Q, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> m)
		: Q(Q), u(u), v(v), mat_ptr(m)
	{
		auto n = glm::cross(u, v);
		normal = glm::normalize(n);
		D = glm::dot(normal, Q);
		w = n / glm::dot(n, n);
		area = glm::length(n);
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

public:
	glm::vec3 Q;                        ///< 角点
	glm::vec3 u;                        ///< 边向量 u
	glm::vec3 v;                        ///< 边向量 v
	std::shared_ptr<Material> mat_ptr;  ///< 材质
	glm::vec3 normal;                   ///< 法线方向
	glm::vec3 w;                        ///< 法线归一化辅助向量
	double D;                           ///< 平面方程中的 D 常数。
	double area;                        ///< 四边形的面积。
};

} // namespace rt
