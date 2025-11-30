#pragma once

#include "rt/apps/Application.hpp"

namespace rt {

class MirrorBoxApp : public Application {
public:
	virtual ~MirrorBoxApp() = default;
	virtual void run() override;
	virtual std::string name() const override { return "Mirror Box Scene"; }
};

} // namespace rt