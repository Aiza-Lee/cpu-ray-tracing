#pragma once
#include "rt/hittables/Hittable.hpp"
#include <memory>
#include <vector>

namespace rt {

using std::shared_ptr;
using std::make_shared;

/**
 * @brief 场景类，包含多个可被光线击中的对象。
 */
class Scene : public Hittable {
public:
	Scene() = default;
	
	/**
	 * @brief 使用单个对象构造新的场景对象。
	 * 
	 * @param object 指向可被光线击中的对象的指针。
	 */
	explicit Scene(const shared_ptr<Hittable>& object) { add(object); }

	/**
	 * @brief 清空场景中的所有对象。
	 */
	void clear() { objects.clear(); }

	/**
	 * @brief 向场景中添加一个可被光线击中的对象。
	 * 
	 * @param object 指向可被光线击中的对象的指针。
	 */
	void add(const shared_ptr<Hittable>& object) { objects.push_back(object); }

	/**
	 * @brief 判断光线是否击中场景中的任何对象。
	 * 
	 * 查找在区间 [t_min, t_max] 内的最近击中。
	 * 
	 * @param r 要测试的光线。
	 * @param t_min 最小的 t 值。
	 * @param t_max 最大的 t 值。
	 * @param rec 用于存储最近相交细节的击中记录引用。
	 * @return true 如果有任何击中。
	 * @return false 否则。
	 */
	bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

	[[nodiscard]] double pdf_value(const glm::vec3& origin, const glm::vec3& v) const override;
	[[nodiscard]] glm::vec3 random(const glm::vec3& origin) const override;

	std::vector<shared_ptr<Hittable>> objects; ///< 场景中的对象列表。
};

} // namespace rt
