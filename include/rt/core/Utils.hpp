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
 * @param min 最小值。
 * @param max 最大值。
 * @return glm::vec3 随机向量。
 */
inline glm::vec3 random_vec3(double min, double max) {
	return glm::vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

/**
 * @brief 在单位球体内生成一个随机点。
 * 
 * @return glm::vec3 单位球体内的随机点。
 */
inline glm::vec3 random_in_unit_sphere() {
	while (true) {
		auto p = random_vec3(-1,1);
		if (glm::length2(p) >= 1) continue;
		return p;
	}
}

/**
 * @brief 生成一个随机单位向量。
 * 
 * @return glm::vec3 随机单位向量。
 */
inline glm::vec3 random_unit_vector() {
	return glm::normalize(random_in_unit_sphere());
}

/**
 * @brief 生成一个随机的余弦方向向量。
 * 
 * @return glm::vec3 随机余弦方向向量。
 */
inline glm::vec3 random_cosine_direction() {
	auto r1 = random_double();
	auto r2 = random_double();
	auto z = sqrt(1-r2);

	auto phi = 2*pi*r1;
	auto x = cos(phi)*sqrt(r2);
	auto y = sin(phi)*sqrt(r2);

	return glm::vec3(x, y, z);
}

} // namespace rt
