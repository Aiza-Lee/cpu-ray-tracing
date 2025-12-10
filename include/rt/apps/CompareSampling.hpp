#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class CompareSamplingApp : public Application {
public:
	~CompareSamplingApp() override = default;
	void run() override;
	std::string name() const override { return "CompareSampling Scene"; }
};

} // namespace rt