#include "rt/core/Camera.hpp"

namespace rt {

Camera::Camera(
	glm::vec3 lookfrom,
	glm::vec3 lookat,
	glm::vec3 vup,
	double vfov,
	double aspect_ratio
) {
	auto theta = degrees_to_radians(vfov);
	auto h = tan(theta/2);
	auto viewport_height = 2.0 * h;
	auto viewport_width = aspect_ratio * viewport_height;

	auto w = glm::normalize(lookfrom - lookat);
	auto u = glm::normalize(glm::cross(vup, w));
	auto v = glm::cross(w, u);

	m_origin = lookfrom;
	m_horizontal = static_cast<float>(viewport_width) * u;
	m_vertical = static_cast<float>(viewport_height) * v;
	m_lower_left_corner = m_origin - m_horizontal/2.0f - m_vertical/2.0f - w;
}

Ray Camera::get_ray(const double s, const double t) const {
	return Ray(m_origin, m_lower_left_corner + static_cast<float>(s)*m_horizontal + static_cast<float>(t)*m_vertical - m_origin);
}

} // namespace rt
