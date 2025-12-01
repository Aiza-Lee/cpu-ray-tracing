#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class PlaygroundApp : public Application {
public:
	virtual ~PlaygroundApp() = default;
	virtual void run() override;
	virtual std::string name() const override { return "Playground Scene"; }
};

} // namespace rt