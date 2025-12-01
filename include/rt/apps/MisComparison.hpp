#pragma once
#include "rt/apps/Application.hpp"

namespace rt {

class MisComparisonApp : public Application {
public:
	virtual void run() override;
	virtual std::string name() const override { return "MIS Comparison (MIS vs Light vs Material)"; }
private:
	std::shared_ptr<Quad> m_build_mirror(const glm::vec3& light, const glm::vec3& eye, const glm::vec3& pos, std::shared_ptr<Material> mat);
};

} // namespace rt
