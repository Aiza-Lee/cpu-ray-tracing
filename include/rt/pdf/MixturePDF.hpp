#pragma once
#include "rt/pdf/PDF.hpp"
#include "rt/core/Utils.hpp"

namespace rt {

/**
 * @brief 混合概率密度函数 (PDF)。
 * 
 * 结合两个 PDF，p_0 和 p_1，
 * 
 * 得到新的 PDF，p = 0.5 * p_0 + 0.5 * p_1。
 */
class MixturePDF : public PDF {
public:
	MixturePDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1) {
		_p[0] = p0;
		_p[1] = p1;
	}

	virtual double value(const glm::vec3& direction) const override {
		return 0.5 * _p[0]->value(direction) + 0.5 * _p[1]->value(direction);
	}

	virtual glm::vec3 generate() const override {
		if (random_double() < 0.5)
			return _p[0]->generate();
		else
			return _p[1]->generate();
	}

private:
	std::shared_ptr<PDF> _p[2];
};

} // namespace rt
