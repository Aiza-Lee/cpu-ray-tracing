#pragma once
#include "rt/Utils.hpp"
#include "rt/Ray.hpp"

namespace rt {

/**
 * @brief Camera class representing a virtual camera in the scene.
 * 
 * The camera is responsible for generating rays from the eye position
 * through the image plane.
 */
class Camera {
public:
	/**
	 * @brief Construct a new Camera object.
	 * 
	 * @param lookfrom The position of the camera.
	 * @param lookat The point the camera is looking at.
	 * @param vup The "up" vector for the camera orientation.
	 * @param vfov Vertical field-of-view in degrees.
	 * @param aspect_ratio The aspect ratio of the image (width / height).
	 */
	Camera(
		glm::vec3 lookfrom,
		glm::vec3 lookat,
		glm::vec3 vup,
		double vfov, // vertical field-of-view in degrees
		double aspect_ratio
	);

	/**
	 * @brief Generate a ray from the camera for a given pixel coordinate.
	 * 
	 * @param s The horizontal coordinate fraction (0 to 1).
	 * @param t The vertical coordinate fraction (0 to 1).
	 * @return Ray The generated ray.
	 */
	Ray get_ray(double s, double t) const;

private:
	glm::vec3 origin;            ///< Camera origin position.
	glm::vec3 lower_left_corner; ///< Lower left corner of the view plane.
	glm::vec3 horizontal;        ///< Horizontal vector of the view plane.
	glm::vec3 vertical;          ///< Vertical vector of the view plane.
};

} // namespace rt
