#include "rt/RayTracingApps.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	std::cout << "Select app to run:" << std::endl;
	std::cout << "1. Random Spheres" << std::endl;
	std::cout << "2. Simple Light" << std::endl;
	std::cout << "3. Cornell Box" << std::endl;
	
	int choice = 1;
	if (argc > 1) {
		choice = std::stoi(argv[1]);
	} else {
		std::cout << "Enter choice (1-4): ";
		std::cin >> choice;
	}

	switch(choice) {
		case 1: rt::RayTracingApps::random_spheres(); break;
		case 2: rt::RayTracingApps::simple_light(); break;
		case 3: rt::RayTracingApps::cornell_box(); break;
		case 4: rt::RayTracingApps::mirror_box(); break;
		default: rt::RayTracingApps::random_spheres(); break;
	}

	return 0;
}
