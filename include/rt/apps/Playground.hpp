#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class PlaygroundApp : public Application {
public:
	~PlaygroundApp() override = default;
	void run() override;
	[[nodiscard]] std::string name() const override { return "Playground Scene"; }
};

} // namespace rt