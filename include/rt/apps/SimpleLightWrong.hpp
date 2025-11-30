#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class SimpleLightWrongApp : public Application {
public:
	virtual ~SimpleLightWrongApp() = default;
	virtual void run() override;
	virtual std::string name() const override { return "Simple Light Wrong Lambertian Scene"; }
};

} // namespace rt