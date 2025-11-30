#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class SimpleLightApp : public Application {
public:
	virtual ~SimpleLightApp() = default;
	virtual void run() override;
	virtual std::string name() const override { return "Simple Light Scene"; }
};

} // namespace rt