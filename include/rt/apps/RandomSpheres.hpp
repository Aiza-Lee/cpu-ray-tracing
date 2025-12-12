#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class RandomSpheresApp : public Application {
public:
	void run() override;
	[[nodiscard]] std::string name() const override { return "Random Spheres Scene"; }
};

} // namespace rt