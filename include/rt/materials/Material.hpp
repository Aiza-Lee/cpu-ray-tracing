#pragma once
#include "rt/core/Utils.hpp"
#include "rt/core/Ray.hpp"
#include "rt/core/HitRecord.hpp"
#include "rt/pdf/PDF.hpp"

namespace rt {

struct ScatterRecord {
	Ray specular_ray;             ///< 镜面反射光线（如果有的话）。
	bool is_specular;             ///< 是否为镜面反射。
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
	) const {
		return false;
	}

	/**
	 * @brief 计算散射概率密度函数值。
	 * 
	 * @param r_in 入射光线。
	 * @param rec 击中记录。
	 * @param scattered 散射光线。
	 * @return double PDF 值。
	 */
	virtual double scattering_pdf(
		const Ray& r_in, const HitRecord& rec, const Ray& scattered
	) const {
		return 0;
	}

	/**
	 * @brief 获取材质的自发光颜色。
	 * 
	 * @param r_in 入射光线。
	 * @param rec 击中记录。
	 * @param u 纹理坐标 u。
	 * @param v 纹理坐标 v。
	 * @param p 击中点。
	 * @return glm::vec3 自发光颜色。
	 */
	virtual glm::vec3 emitted(
		const Ray& r_in, const HitRecord& rec, double u, double v, const glm::vec3& p
	) const {
		return glm::vec3(0,0,0);
	}
};

} // namespace rt
