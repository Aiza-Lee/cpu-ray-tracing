#include "rt/apps/CornerBox.hpp"
#include "rt/apps/MirrorBox.hpp"
#include "rt/apps/RandomSpheres.hpp"
#include "rt/apps/SimpleLight.hpp"
#include "rt/apps/SimpleLightWrong.hpp"
#include "rt/apps/MisComparison.hpp"
#include "rt/apps/Playground.hpp"

#include <string>
#include <fmt/core.h>
#include <iostream>

int main(int argc, char* argv[]) {
	
	std::vector<std::unique_ptr<rt::Application>> apps;
	apps.push_back(std::make_unique<rt::CornerBoxApp>());
	apps.push_back(std::make_unique<rt::MirrorBoxApp>());
	apps.push_back(std::make_unique<rt::RandomSpheresApp>());
	apps.push_back(std::make_unique<rt::SimpleLightApp>());
	apps.push_back(std::make_unique<rt::SimpleLightWrongApp>());
	apps.push_back(std::make_unique<rt::MisComparisonApp>());
	apps.push_back(std::make_unique<rt::PlaygroundApp>());

	fmt::print("Available Applications:\n");
	for (size_t i = 0; i < apps.size(); ++i) {
		fmt::print("{}. {}\n", i + 1, apps[i]->name());
	}

	int choice = 0;
	fmt::print("Select an application to run (1-{}): ", apps.size());
	std::cin >> choice;

	if (choice < 1 || choice > static_cast<int>(apps.size())) {
		fmt::print("Invalid choice. Exiting.\n");
		return 1;
	}

	apps[choice - 1]->run();

	return 0;
}
