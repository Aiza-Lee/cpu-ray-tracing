#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "rt/Ray.hpp"

namespace rt {

class Material; // Forward declaration

/**
 * @brief 记录光线与物体相交的信息。
 */
struct HitRecord {
	glm::vec3 p;                       ///< 相交点。
	glm::vec3 normal;                  ///< 相交点处的表面法线。
	std::shared_ptr<Material> mat_ptr; ///< 相交物体的材质指针。
	double t;                          ///< 相交点处的光线参数 t。
	bool front_face;                   ///< 如果光线击中表面的正面，则为真。

	/**
	 * @brief 初始化normal和front_face字段。
	 * 
	 * 判断光线击中的是物体的正面还是背面，并确保法线始终指向光线的反方向。
	 * 
	 * @param r 入射光线。
	 * @param outward_normal 表面的几何外法线。
	 */
	void set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
		front_face = glm::dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

} // namespace rt
