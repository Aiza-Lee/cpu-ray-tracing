#pragma once

#include <string>

namespace rt {

class Application {
protected:
	Application() = default;
public:
	Application(const Application&) = default;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = default;
	Application& operator=(Application&&) = delete;
	virtual ~Application() = default;

	virtual void run() = 0;
	[[nodiscard]] virtual std::string name() const = 0;
};

} // namespace rt