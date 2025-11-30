#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class CornerBoxApp : public Application {
public:
	virtual ~CornerBoxApp() = default;
	virtual void run() override;
	virtual std::string name() const override { return "Cornell Box Scene"; }
};

} // namespace rt