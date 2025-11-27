#include "rt/SoftTracer.hpp"
#include "rt/Material.hpp"
#include "rt/Utils.hpp"
#include <iostream>
#include <fmt/core.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace rt {

SoftTracer::SoftTracer(int width, int height, int samples, int depth)
    : image_width(width), image_height(height), samples_per_pixel(samples), max_depth(depth) {}

glm::vec3 SoftTracer::ray_color(const Ray& r, const Scene& world, int depth) {
    HitRecord rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return glm::vec3(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        glm::vec3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return glm::vec3(0,0,0);
    }

    glm::vec3 unit_direction = glm::normalize(r.direction());
    auto t = 0.5 * (unit_direction.y + 1.0);
    return static_cast<float>(1.0-t)*glm::vec3(1.0, 1.0, 1.0) + static_cast<float>(t)*glm::vec3(0.5, 0.7, 1.0);
}

void SoftTracer::write_color(std::vector<unsigned char>& image_data, int index, glm::vec3 pixel_color) {
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    image_data[index] = static_cast<unsigned char>(256 * std::clamp(r, 0.0f, 0.999f));
    image_data[index+1] = static_cast<unsigned char>(256 * std::clamp(g, 0.0f, 0.999f));
    image_data[index+2] = static_cast<unsigned char>(256 * std::clamp(b, 0.0f, 0.999f));
}

void SoftTracer::render(const Scene& scene, const Camera& cam, const std::string& filename) {
    std::vector<unsigned char> image_data(image_width * image_height * 3);

    for (int j = image_height-1; j >= 0; --j) {
        fmt::print("\rScanlines remaining: {} ", j);
        std::fflush(stdout);
        for (int i = 0; i < image_width; ++i) {
            glm::vec3 pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, scene, max_depth);
            }
            
            // Calculate index for stb_image (top-left origin, but we loop bottom-up, so we need to flip y)
            int row = image_height - 1 - j;
            int index = (row * image_width + i) * 3;
            write_color(image_data, index, pixel_color);
        }
    }
    fmt::print("\nDone.\n");

    stbi_write_png(filename.c_str(), image_width, image_height, 3, image_data.data(), image_width * 3);
}

} // namespace rt
