#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class CompareSamplingApp : public Application {
public:
	virtual ~CompareSamplingApp() = default;
	virtual void run() override;
	virtual std::string name() const override { return "CompareSampling Scene"; }
};

} // namespace rt