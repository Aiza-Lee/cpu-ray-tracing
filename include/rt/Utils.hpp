#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
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
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
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

/**
 * @brief 生成一个分量在 [0, 1) 范围内的随机向量。
 * 
 * @return glm::vec3 随机向量。
 */
inline glm::vec3 random_vec3() {
	return glm::vec3(random_double(), random_double(), random_double());
}

/**
 * @brief 生成一个分量在 [min, max) 范围内的随机向量。
 * 
 * @param min 最小分量值。
 * @param max 最大分量值。
 * @return glm::vec3 随机向量。
 */
inline glm::vec3 random_vec3(double min, double max) {
	return glm::vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

/**
 * @brief 生成单位球内的随机点。
 * 
 * @return glm::vec3 单位球内的随机点。
 */
inline glm::vec3 random_in_unit_sphere() {
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return glm::vec3(r * cos(a), r * sin(a), z);
}

/**
 * @brief 生成一个随机单位向量。
 * 
 * @return glm::vec3 随机单位向量。
 */
inline glm::vec3 random_unit_vector() {
	return glm::normalize(random_in_unit_sphere());
}

} // namespace rt
