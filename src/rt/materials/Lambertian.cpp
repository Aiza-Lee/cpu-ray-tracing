#include "rt/materials/Lambertian.hpp"
#include "rt/pdf/CosinePDF.hpp"

namespace rt {

bool Lambertian::scatter(
	const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
) const {
	srec.attenuation = albedo;
	srec.pdf_ptr = std::make_shared<CosinePDF>(rec.normal);
	return true;
}

glm::vec3 Lambertian::brdf(
	const Ray& r_in, const HitRecord& rec, const Ray& scattered
) const {
	return albedo / static_cast<float>(pi);
}

} // namespace rt
