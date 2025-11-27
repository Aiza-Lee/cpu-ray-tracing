#include "rt/Camera.hpp"

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

    origin = lookfrom;
    horizontal = static_cast<float>(viewport_width) * u;
    vertical = static_cast<float>(viewport_height) * v;
    lower_left_corner = origin - horizontal/2.0f - vertical/2.0f - w;
}

Ray Camera::get_ray(double s, double t) const {
    return Ray(origin, lower_left_corner + static_cast<float>(s)*horizontal + static_cast<float>(t)*vertical - origin);
}

} // namespace rt
