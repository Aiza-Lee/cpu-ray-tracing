#pragma once
#include <glm/glm.hpp>
#include <cmath>

namespace rt {

/**
 * @brief 正交基 (Orthonormal Basis) 类。
 * 
 * 用于构建局部坐标系，常用于采样和着色计算。
 */
class ONB {
public:
	ONB() {}

	inline glm::vec3 operator[](int i) const { return axis[i]; }

	glm::vec3 u() const { return axis[0]; }
	glm::vec3 v() const { return axis[1]; }
	glm::vec3 w() const { return axis[2]; }

	/**
	 * @brief 将局部坐标系中的向量转换到世界坐标系。
	 * 
	 * @param a 局部坐标 x 分量。
	 * @param b 局部坐标 y 分量。
	 * @param c 局部坐标 z 分量。
	 * @return glm::vec3 世界坐标系中的向量。
	 */
	glm::vec3 local(double a, double b, double c) const {
		return (float)a * u() + (float)b * v() + (float)c * w();
	}

	/**
	 * @brief 将局部坐标系中的向量转换到世界坐标系。
	 * 
	 * @param a 局部坐标向量。
	 * @return glm::vec3 世界坐标系中的向量。
	 */
	glm::vec3 local(const glm::vec3& a) const {
		return a.x * u() + a.y * v() + a.z * w();
	}

	/**
	 * @brief 根据给定的法向量 w 构建正交基。
	 * 
	 * @param n 法向量（不需要归一化，函数内部会归一化）。
	 */
	void build_from_w(const glm::vec3& n) {
		axis[2] = glm::normalize(n);
		glm::vec3 a = (fabs(w().x) > 0.9) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
		axis[1] = glm::normalize(glm::cross(w(), a));
		axis[0] = glm::cross(w(), v());
	}

public:
	glm::vec3 axis[3];
};

} // namespace rt
