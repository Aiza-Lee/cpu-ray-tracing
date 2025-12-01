#pragma once
#include "rt/core/Utils.hpp"
#include "rt/core/Ray.hpp"
#include "rt/core/HitRecord.hpp"
#include "rt/pdf/PDF.hpp"

namespace rt {

struct ScatterRecord {
	glm::vec3 attenuation;        ///< 衰减系数（颜色）。
	std::shared_ptr<PDF> pdf_ptr; ///< 概率密度函数指针。
};

/**
 * @brief 材质的抽象基类。
 * 
 * 材质决定了光线与表面的相互作用（散射、吸收等）。
 */
class Material {
public:
	virtual ~Material() = default;

	/**
	 * @brief 散射击中材质的光线。
	 * 
	 * @param r_in 入射光线。
	 * @param rec 包含相交细节的击中记录。
	 * @param srec [out] 散射记录。
	 * @return true 如果光线被散射。
	 * @return false 如果光线被吸收。
	 */
	virtual bool scatter(
		const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
	) const = 0;

	/**
	 * @brief 计算双向反射分布函数 (BRDF) 值。
	 * 
	 * @param r_in 入射光线。
	 * @param rec 击中记录。
	 * @param scattered 散射光线。
	 * @return glm::vec3 BRDF 值。
	 */
	virtual glm::vec3 brdf(
		const Ray& r_in, const HitRecord& rec, const Ray& scattered
	) const = 0;

	/**
	 * @brief 获取材质的自发光颜色。
	 * 
	 * @param r_in 入射光线。
	 * @param rec 击中记录。
	 * @return glm::vec3 自发光颜色。
	 */
	virtual glm::vec3 emitted(const Ray& r_in, const HitRecord& rec) const = 0;
};

} // namespace rt