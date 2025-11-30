#include "rt/materials/Metal.hpp"
#include "rt/core/Utils.hpp"

namespace rt {

bool Metal::scatter(
	const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
) const {
	glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction()), rec.normal);
	srec.specular_ray = Ray(rec.p, reflected + static_cast<float>(fuzz) * random_in_unit_sphere());
	srec.attenuation = albedo;
	srec.is_specular = true;
	srec.pdf_ptr = nullptr;
	return (glm::dot(srec.specular_ray.direction(), rec.normal) > 0);
}

} // namespace rt
