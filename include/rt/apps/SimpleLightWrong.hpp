#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class SimpleLightWrongApp : public Application {
public:
	void run() override;
	[[nodiscard]] std::string name() const override { return "Simple Light Wrong Lambertian Scene"; }
};

} // namespace rt