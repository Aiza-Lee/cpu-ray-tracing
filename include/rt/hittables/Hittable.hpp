#pragma once
#include "rt/core/Ray.hpp"
#include "rt/core/HitRecord.hpp"
#include <glm/glm.hpp>

namespace rt {

/**
 * @brief 描述可被光线击中的对象的抽象基类。
 */
class Hittable {
protected:
	Hittable() = default;

public:
	Hittable(const Hittable&) = default;
	Hittable(Hittable&&) = delete;
	Hittable& operator=(const Hittable&) = default;
	Hittable& operator=(Hittable&&) = delete;
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

	/**
	 * @brief 计算从给定原点沿指定方向采样到该对象的概率密度函数 (PDF) 值。
	 *
	 * @param origin 采样的原点。
	 * @param v 采样方向。
	 */
	[[nodiscard]] virtual double pdf_value(const glm::vec3& origin, const glm::vec3& v) const = 0;

	/**
	 * @brief 从给定的原点产生指向该对象的随机方向。
	 *
	 * @param origin 采样的原点。
	 */
	[[nodiscard]] virtual glm::vec3 random(const glm::vec3& origin) const = 0;
};

} // namespace rt
