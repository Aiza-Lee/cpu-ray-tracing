#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class SimpleLightApp : public Application {
public:
	void run() override;
	[[nodiscard]] std::string name() const override { return "Simple Light Scene"; }
};

} // namespace rt