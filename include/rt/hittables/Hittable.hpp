#pragma once
#include "rt/core/Ray.hpp"
#include "rt/core/HitRecord.hpp"
#include <glm/glm.hpp>

namespace rt {

/**
 * @brief 描述可被光线击中的对象的抽象基类。
 */
class Hittable {
public:
	virtual ~Hittable() = default;

	/**
	 * @brief 判断光线是否击中对象。
	 * 
	 * @param r 待测试的光线。
	 * @param t_min 有效击中范围的最小 t 值。
	 * @param t_max 有效击中范围的最大 t 值。
	 * @param rec [out] 用于存储相交信息的 HitRecord 引用。
	 * @return true 如果光线在范围 [t_min, t_max] 内击中对象。
	 * @return false 否则。
	 */
	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;

	virtual double pdf_value(const glm::vec3& origin, const glm::vec3& v) const {
		return 0.0;
	}

	virtual glm::vec3 random(const glm::vec3& origin) const {
		return glm::vec3(1, 0, 0);
	}
};

} // namespace rt
