#include "Vehicle/Controller.h"
#include "Vehicle/Vehicle.h"
#include <catch2/catch_test_macros.hpp>

class DummyController : public Controller {
  public:
    ~DummyController() override = default;
    void Tick(Vehicle&, float) override {}
};

TEST_CASE("Vehicle stores id correctly") {
    VehicleID id(42);

    auto controller = std::make_unique<DummyController>();
    Vehicle v(id, std::move(controller));

    REQUIRE(v.getId() == id);
}
