#pragma once
#include "rt/hittables/Hittable.hpp"
#include "rt/materials/Material.hpp"
#include <glm/glm.hpp>
#include <utility>

namespace rt {

/**
 * @brief 球体类，表示三维空间中的一个球体对象。
 */
class Sphere : public Hittable {
public:
	Sphere() : radius(0){}

	/**
	 * @brief 构造一个新的球体对象。
	 * 
	 * @param cen 球体的中心。
	 * @param r 球体的半径。
	 * @param m 球体的材质。
	 */
	Sphere(const glm::vec3 cen, const double r, std::shared_ptr<Material> m)
		: center(cen), radius(r), mat_ptr(std::move(m)) {};

	/**
	 * @brief 判断光线是否击中球体。
	 * 
	 * @param r 要测试的光线。
	 * @param t_min 最小的 t 值。
	 * @param t_max 最大的 t 值。
	 * @param rec 用于存储击中细节的击中记录引用。
	 * @return true 如果光线击中球体。
	 * @return false 否则。
	 */
	bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

	[[nodiscard]] double pdf_value(const glm::vec3& origin, const glm::vec3& v) const override;
	[[nodiscard]] glm::vec3 random(const glm::vec3& origin) const override;

public:
	glm::vec3 center{};   ///< 球心
	double radius;                     ///< 球体的半径。
	std::shared_ptr<Material> mat_ptr; ///< 球体的材质。
};

} // namespace rt
