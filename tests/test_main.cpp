#include <catch2/catch_test_macros.hpp>
#include "rt/Ray.hpp"
#include "rt/Sphere.hpp"
#include "rt/Material.hpp"

TEST_CASE("Ray At", "[ray]") {
    rt::Ray r(glm::vec3(0,0,0), glm::vec3(1,0,0));
    glm::vec3 p = r.at(2.0);
    REQUIRE(p.x == 2.0);
    REQUIRE(p.y == 0.0);
    REQUIRE(p.z == 0.0);
}

TEST_CASE("Sphere Hit", "[sphere]") {
    auto mat = std::make_shared<rt::Lambertian>(glm::vec3(0.5,0.5,0.5));
    rt::Sphere s(glm::vec3(0,0,-2), 1.0, mat);
    rt::Ray r(glm::vec3(0,0,0), glm::vec3(0,0,-1));
    rt::HitRecord rec;
    
    bool hit = s.hit(r, 0, 100, rec);
    REQUIRE(hit == true);
    REQUIRE(rec.t == 1.0);
}
