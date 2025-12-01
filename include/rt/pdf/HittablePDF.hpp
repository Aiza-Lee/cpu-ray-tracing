#pragma once
#include "rt/pdf/PDF.hpp"
#include "rt/hittables/Hittable.hpp"

namespace rt {

class HittablePDF : public PDF {
public:
	HittablePDF(std::shared_ptr<Hittable> p, const glm::vec3& origin) : ptr(p), o(origin) {}

	virtual double value(const glm::vec3& direction) const override {
		return ptr->pdf_value(o, direction);
	}

	virtual glm::vec3 generate() const override {
		return ptr->random(o);
	}

public:
	glm::vec3 o;
	std::shared_ptr<Hittable> ptr;
};

} // namespace rt
