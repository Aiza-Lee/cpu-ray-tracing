#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class RandomSpheresApp : public Application {
public:
	virtual ~RandomSpheresApp() = default;
	virtual void run() override;
	virtual std::string name() const override { return "Random Spheres Scene"; }
};

} // namespace rt