#include "Geometry/Position.h"
#include "Vehicle/Controller.h"
#include "Vehicle/Vehicle.h"
#include "catch2/catch_approx.hpp"
#include <catch2/catch_test_macros.hpp>

class DummyController : public Controller {
  public:
    bool wasCalled = false;
    float lastDt = 0.f;

    void Tick(Vehicle&, float dt) override {
        wasCalled = true;
        lastDt = dt;
    }
};

TEST_CASE("Vehicle stores id correctly") {
    VehicleID id(42);

    auto controller = std::make_unique<DummyController>();
    Vehicle v(id, std::move(controller));

    REQUIRE(v.getId() == id);
}

TEST_CASE("Vehicle default position is a default constructed pos") {
    constexpr Position pos;
    VehicleID id(42);

    auto controller = std::make_unique<DummyController>();
    Vehicle v(id, std::move(controller));

    REQUIRE(v.GetPosition() == pos);
}

TEST_CASE("Vehicle position is updated correctly") {
    constexpr Position pos{15.0, 15.0};
    VehicleID id(42);

    auto controller = std::make_unique<DummyController>();
    Vehicle v(id, std::move(controller));

    v.SetPosition(pos);

    REQUIRE(v.GetPosition() == Position{15.0, 15.0});
}

TEST_CASE("Vehicle update forwards to controller") {
    VehicleID id(1);
    auto controller = std::make_unique<DummyController>();
    auto* raw = controller.get();

    Vehicle v(id, std::move(controller));

    v.update(0.5f);

    REQUIRE(raw->wasCalled);
    REQUIRE(raw->lastDt == Catch::Approx(0.5f));
}
