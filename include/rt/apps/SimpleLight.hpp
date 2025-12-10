#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class SimpleLightApp : public Application {
public:
	~SimpleLightApp() override = default;
	void run() override;
	std::string name() const override { return "Simple Light Scene"; }
};

} // namespace rt