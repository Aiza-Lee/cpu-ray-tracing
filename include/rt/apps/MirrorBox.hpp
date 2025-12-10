#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class MirrorBoxApp : public Application {
public:
	~MirrorBoxApp() override = default;
	void run() override;
	std::string name() const override { return "Mirror Box Scene"; }
};

} // namespace rt