#pragma once
#include "rt/pdf/PDF.hpp"
#include "rt/core/ONB.hpp"
#include "rt/core/Utils.hpp"
#include <algorithm>
#include <cmath>

namespace rt {

/**
 * @brief 基于余弦幂次分布的 PDF (Phong Lobe)。
 * 
 * 概率密度函数为: p(theta) = (n+1)/(2*pi) * cos(theta)^n
 * 其中 n 是幂次 (exponent)，控制波瓣的宽窄。
 */
class CosinePowerPDF : public PDF {
public:
	/**
	 * @brief 构造函数。
	 * 
	 * @param w 波瓣的中心方向（通常是反射向量）。
	 * @param exponent 幂次 n。值越大，波瓣越窄（越光滑）。
	 */
	CosinePowerPDF(const glm::vec3& w, double exponent) : exponent(exponent) { 
		uvw.build_from_w(w); 
	}

	virtual double value(const glm::vec3& direction) const override {
		double cosine = glm::dot(glm::normalize(direction), uvw.w());
		// 钳位余弦值以防止数值不稳定
		if (cosine > 1.0) cosine = 1.0;
		if (cosine <= 0) return 0;
		
		return (exponent + 1) / (2 * pi) * pow(cosine, exponent);
	}

	virtual glm::vec3 generate() const override {
		double r1 = random_double();
		double r2 = random_double();
		double phi = 2 * pi * r2;
		
		// 根据分布生成 cos_theta
		// p(theta) ~ cos^n(theta) -> CDF(theta) = 1 - cos^(n+1)(theta)
		// cos_theta = (1 - r1)^(1/(n+1)) -> 简化为 r1^(1/(n+1))
		double cos_theta = pow(r1, 1.0 / (exponent + 1));
		double sin_theta = sqrt(std::max(0.0, 1.0 - cos_theta * cos_theta));

		return uvw.local(glm::vec3(
			cos(phi) * sin_theta,
			sin(phi) * sin_theta,
			cos_theta
		));
	}

private:
	ONB uvw;
	double exponent;
};

} // namespace rt
