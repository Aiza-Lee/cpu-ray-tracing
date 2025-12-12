#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class CompareSamplingApp : public Application {
public:
	void run() override;
	[[nodiscard]] std::string name() const override { return "CompareSampling Scene"; }
};

} // namespace rt