#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class MirrorBoxApp : public Application {
public:
	void run() override;
	[[nodiscard]] std::string name() const override { return "Mirror Box Scene"; }
};

} // namespace rt