#include "rt/materials/Lambertian.hpp"
#include "rt/pdf/CosinePDF.hpp"

namespace rt {

bool Lambertian::scatter(
const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
) const {
	srec.is_specular = false;
	srec.attenuation = albedo;
	srec.pdf_ptr = std::make_shared<CosinePDF>(rec.normal);
	return true;
}

double Lambertian::scattering_pdf(
const Ray& r_in, const HitRecord& rec, const Ray& scattered
) const {
	auto cos_theta = glm::dot(rec.normal, glm::normalize(scattered.direction()));
	return cos_theta < 0 ? 0 : cos_theta / pi;
}

} // namespace rt
