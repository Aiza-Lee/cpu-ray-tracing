#pragma once
#include "rt/Scene.hpp"
#include "rt/Camera.hpp"
#include <vector>
#include <string>

namespace rt {

/**
 * @brief Main ray tracer class.
 * 
 * Handles the rendering process, including ray generation, color calculation,
 * and image output.
 */
class SoftTracer {
public:
	/**
	 * @brief Construct a new Soft Tracer object.
	 * 
	 * @param width Image width in pixels.
	 * @param height Image height in pixels.
	 * @param samples_per_pixel Number of samples per pixel for antialiasing.
	 * @param max_depth Maximum recursion depth for ray scattering.
	 */
	SoftTracer(int width, int height, int samples_per_pixel, int max_depth);

	/**
	 * @brief Render the scene from the camera's perspective.
	 * 
	 * @param scene The scene to render.
	 * @param cam The camera to view from.
	 * @param filename The output filename for the image.
	 */
	void render(const Scene& scene, const Camera& cam, const std::string& filename);

private:
	/**
	 * @brief Calculate the color of a ray.
	 * 
	 * @param r The ray.
	 * @param world The scene (world) containing objects.
	 * @param depth Current recursion depth.
	 * @return glm::vec3 The calculated color.
	 */
	glm::vec3 ray_color(const Ray& r, const Scene& world, int depth);

	/**
	 * @brief Write a pixel's color to the image data buffer.
	 * 
	 * @param image_data The buffer to write to.
	 * @param index The index in the buffer.
	 * @param pixel_color The color to write.
	 */
	void write_color(std::vector<unsigned char>& image_data, int index, glm::vec3 pixel_color);

	int image_width;       ///< Image width.
	int image_height;      ///< Image height.
	int samples_per_pixel; ///< Samples per pixel.
	int max_depth;         ///< Maximum ray depth.
};

} // namespace rt
