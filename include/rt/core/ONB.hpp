#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <array>
#include <stdexcept>
#include <cmath>

namespace rt {

/**
 * @brief 正交基 (Orthonormal Basis) 类。
 *
 * 用于构建局部坐标系，常用于采样和着色计算。
 */
class ONB {
public:
	ONB() = default;

	const glm::vec3& operator[](const int i) const {
		switch (i) {
			case 0:
				return _axis[0];
			case 1:
				return _axis[1];
			case 2:
				return _axis[2];
			default:
				throw std::out_of_range("ONB index out of range");
		}
	}

	[[nodiscard]] glm::vec3 u() const { return _axis[0]; }
	[[nodiscard]] glm::vec3 v() const { return _axis[1]; }
	[[nodiscard]] glm::vec3 w() const { return _axis[2]; }

	/**
	 * @brief 将局部坐标系中的向量转换到世界坐标系。
	 *
	 * @param a 局部坐标 x 分量。
	 * @param b 局部坐标 y 分量。
	 * @param c 局部坐标 z 分量。
	 * @return glm::vec3 世界坐标系中的向量。
	 */
	[[nodiscard]] glm::vec3 transform_to_world(const double a, const double b, const double c) const {
		return (float)a * u() + (float)b * v() + (float)c * w();
	}

	/**
	 * @brief 将局部坐标系中的向量转换到世界坐标系。
	 *
	 * @param a 局部坐标向量。
	 * @return glm::vec3 世界坐标系中的向量。
	 */
	[[nodiscard]] glm::vec3 transform_to_world(const glm::vec3& a) const { return a.x * u() + a.y * v() + a.z * w(); }

	/**
	 * @brief 根据给定的法向量 w 构建正交基。
	 *
	 * @param n 法向量（不需要归一化，函数内部会归一化）。
	 */
	void build_from_w(const glm::vec3& n) {
		_axis[2] = glm::normalize(n);
		// Avoid accessing potential union members like .x directly on glm::vec3;
		// use the array-style accessor to prevent union member access issues.
		glm::vec3 a = (std::fabs(w().x) > 0.9F) ? glm::vec3(0.0F, 1.0F, 0.0F) : glm::vec3(1.0F, 0.0F, 0.0F);
		_axis[1] = glm::normalize(glm::cross(w(), a));
		_axis[0] = glm::cross(v(), w());
	}

private:
	std::array<glm::vec3, 3> _axis;
};

} // namespace rt
