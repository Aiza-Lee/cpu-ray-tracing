#include "rt/materials/Metal.hpp"
#include "rt/pdf/CosinePowerPDF.hpp"
#include "rt/pdf/DeltaPDF.hpp"
#include "rt/core/Utils.hpp"

namespace rt {

bool Metal::scatter(
	const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
) const {
	glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction()), rec.normal);
	srec.attenuation = albedo;

	// 针对较小的 fuzz 值（高光泽/近镜面）使用单独的逻辑分支
	// 使用 DeltaPDF 避免高指数带来的数值问题
	if (fuzz < 0.01) {
		srec.pdf_ptr = std::make_shared<DeltaPDF>(reflected);
		srec.is_specular = true;
	} else {
		// 普通粗糙度
		double exponent = 2.0 / (fuzz * fuzz) - 2.0;
		srec.pdf_ptr = std::make_shared<CosinePowerPDF>(reflected, exponent);
	}

	return true;
}

glm::vec3 Metal::brdf(
	const Ray& r_in, const HitRecord& rec, const Ray& scattered
) const {
	const glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction()), rec.normal);
	const glm::vec3 out_dir = glm::normalize(scattered.direction());

	const double cos_theta = glm::dot(out_dir, rec.normal);
	if (cos_theta < 1e-4) return {0,0,0};

	// 针对较小的 fuzz 值提取的逻辑
	if (fuzz < 0.01) {
		// 检查散射方向是否与反射方向一致（允许微小误差）
		if (glm::dot(out_dir, reflected) > 0.9999) {
			// 对于 Delta 分布 (PDF=1)，BRDF 应该是 albedo / cos_theta
			// 这样 estimator = (BRDF * Li * cos) / PDF = (albedo/cos * Li * cos) / 1 = albedo * Li
			return albedo / static_cast<float>(cos_theta);
		}
		return {0,0,0};
	}

	double cos_alpha = glm::dot(out_dir, reflected);
	if (cos_alpha > 1.0) cos_alpha = 1.0;
	if (cos_alpha <= 0) return {0,0,0};

	const double exponent = 2.0 / (fuzz * fuzz) - 2.0;
	const double pdf_val = (exponent + 1) / (2 * pi) * pow(cos_alpha, exponent);

	return albedo * static_cast<float>(pdf_val / cos_theta);
}

} // namespace rt
