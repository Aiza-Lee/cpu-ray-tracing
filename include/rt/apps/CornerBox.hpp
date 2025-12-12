#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class CornerBoxApp : public Application {
public:
	void run() override;
	[[nodiscard]] std::string name() const override { return "Cornell Box Scene"; }
};

} // namespace rt