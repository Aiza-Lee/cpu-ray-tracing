#pragma once

#include "rt/SoftTracer.hpp"

#include "rt/hittables/Sphere.hpp"
#include "rt/hittables/Quad.hpp"

#include "rt/materials/Material.hpp"
#include "rt/materials/Lambertian.hpp"
#include "rt/materials/Metal.hpp"
#include "rt/materials/WrongLambertian.hpp"
#include "rt/materials/DiffuseLight.hpp"

#include "rt/hittables/Scene.hpp"

#include "rt/core/Camera.hpp"

#include <string>

namespace rt {

class Application {
public:
	virtual ~Application() = default;
	virtual void run() = 0;
	virtual std::string name() const = 0;
};

} // namespace rt