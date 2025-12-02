#pragma once
#include "Controller.h"

class Road;  // Forward declaration

class VehicleController : public Controller {
  public:
    ~VehicleController() override = default;
    void Tick(Vehicle& vehicle, float dt) override;

    VehicleController() = delete;
    explicit VehicleController(Road* road, double speed = 5.0);
    
    void SetNextRoad(Road* road);
    bool IsAtEndOfRoad() const;

  private:
    double distance_{0.0};
    double speed_{5.0};
    Road* road_{nullptr};
};
