#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>
#include <random>
#include <limits>
#include <memory>

namespace rt {

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = glm::pi<double>();

/**
 * @brief 角度转换为弧度。
 * 
 * @param degrees 角度值。
 * @return double 弧度值。
 */
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

/**
 * @brief 生成一个在 [0, 1) 范围内的随机双精度浮点数。
 * 
 * @return double 随机数。
 */
inline double random_double() {
	static thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static thread_local std::mt19937 generator(std::random_device{}());
	return distribution(generator);
}

/**
 * @brief 生成一个在 [min, max) 范围内的随机双精度浮点数。
 * 
 * @param min 最小值。
 * @param max 最大值。
 * @return double 随机数。
 */
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
	return static_cast<int>(random_double(min, max + 1));
}

/**
 * @brief 生成一个分量在 [0, 1) 范围内的随机向量。
 */
inline glm::vec3 random_vec3() {
	return glm::vec3(random_double(), random_double(), random_double());
}

/**
 * @brief 生成一个分量在 [min, max) 范围内的随机向量。
 * 
 * @param min 最小值。
 * @param max 最大值。
 */
inline glm::vec3 random_vec3(double min, double max) {
	return glm::vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

/**
 * @brief 生成一个随机的余弦方向向量。
 */
inline glm::vec3 random_cosine_direction() {
	auto r01 = random_double();
	auto z = sqrt(1 - r01);
	
	auto phi = 2 * pi * random_double();
	auto x = cos(phi) * sqrt(r01);
	auto y = sin(phi) * sqrt(r01);
	
	return glm::vec3(x, y, z);
}

/**
 * @brief 生成一个指向球体表面的随机向量。返回局部坐标下的向量，z轴(0,0,1)指向球心方向。
 */
inline glm::vec3 random_to_sphere(double radius, double distance_squared) {
	auto z = random_double(std::sqrt(1 - radius * radius / distance_squared), 1);
	
	auto phi = 2 * pi * random_double();
	auto x = cos(phi) * sqrt(1 - z * z);
	auto y = sin(phi) * sqrt(1 - z * z);
	
	return glm::vec3(x, y, z);
}

/**
 * @brief 生成一个均匀分布在单位球体表面的随机向量。
 */
inline glm::vec3 random_unit_to_sphere() {
	auto z = random_double(-1, 1);
	auto r = std::sqrt(1 - z * z);
	auto phi = 2 * pi * random_double();
	return glm::vec3(r * std::cos(phi), r * std::sin(phi), z);
}

/**
 * @brief 在单位球体内生成一个随机点。
 */
inline glm::vec3 random_in_unit_sphere() {
	float r = std::cbrt(random_double());
	return r * random_unit_to_sphere();
}

/**
 * @brief 生成上半单位球面上的向量
 */
inline glm::vec3 random_unit_vector_hemisphere() {
	auto on_sphere = random_unit_to_sphere();
	return on_sphere.z >= 0 ? on_sphere : -on_sphere;
}

} // namespace rt
