#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class SimpleLightWrongApp : public Application {
public:
	~SimpleLightWrongApp() override = default;
	void run() override;
	std::string name() const override { return "Simple Light Wrong Lambertian Scene"; }
};

} // namespace rt