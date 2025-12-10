#pragma once

#include <utility>

#include "rt/pdf/PDF.hpp"
#include "rt/hittables/Hittable.hpp"

namespace rt {

class HittablePDF : public PDF {
public:
	HittablePDF(std::shared_ptr<Hittable> p, const glm::vec3& origin) : _origin(origin), _hittable_ptr(std::move(p)) {}

	[[nodiscard]] double value(const glm::vec3& direction) const override {
		return _hittable_ptr->pdf_value(_origin, direction);
	}

	[[nodiscard]] glm::vec3 generate() const override {
		return _hittable_ptr->random(_origin);
	}

private:
	glm::vec3 _origin;							///< 采样的原点
	std::shared_ptr<Hittable> _hittable_ptr;	///< 被采样的 Hittable 对象指针
};

} // namespace rt
