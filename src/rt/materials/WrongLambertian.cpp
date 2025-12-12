#include "rt/materials/WrongLambertian.hpp"
#include "rt/pdf/UniformPDF.hpp"

namespace rt {

bool WrongLambertian::scatter(const Ray& /*r_in*/, const HitRecord& rec, ScatterRecord& srec) const {
	srec.attenuation = _albedo;
	srec.pdf_ptr = std::make_shared<UniformPDF>(rec.normal);
	return true;
}

glm::vec3 WrongLambertian::brdf(const Ray& /*r_in*/, const HitRecord& /*rec*/, const Ray& /*scattered*/
) const {
	return _albedo / static_cast<float>(PI);
}

} // namespace rt
