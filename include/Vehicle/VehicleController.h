#pragma once
#include "Controller.h"
#include "Road/Lane.h"
#include "Route/Route.h"

class Road; // Forward declaration

class VehicleController : public Controller {
  public:
    explicit VehicleController(std::unique_ptr<Route>, double speed = 5.0);
    void Tick(Vehicle& vehicle, float dt) override;

    // deleted constructors
    ~VehicleController() override = default;
    VehicleController() = delete;

  private:
    std::unique_ptr<Route> route_;
    double speed_{5.0};
};
