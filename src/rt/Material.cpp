#include "rt/Material.hpp"

namespace rt {

bool Lambertian::scatter(
	const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered
) const {
	auto scatter_direction = rec.normal + random_unit_vector();

	// Catch degenerate scatter direction
	if (glm::length(scatter_direction) < 1e-8)
		scatter_direction = rec.normal;

	scattered = Ray(rec.p, scatter_direction);
	attenuation = albedo;
	return true;
}

bool Metal::scatter(
	const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered
) const {
	glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction()), rec.normal);
	scattered = Ray(rec.p, reflected + static_cast<float>(fuzz) * random_in_unit_sphere());
	attenuation = albedo;
	return (glm::dot(scattered.direction(), rec.normal) > 0);
}

} // namespace rt
