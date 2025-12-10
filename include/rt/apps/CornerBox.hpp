#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class CornerBoxApp : public Application {
public:
	~CornerBoxApp() override = default;
	void run() override;
	std::string name() const override { return "Cornell Box Scene"; }
};

} // namespace rt