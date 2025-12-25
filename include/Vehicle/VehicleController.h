#pragma once
#include "Controller.h"

class Road; // Forward declaration

class VehicleController : public Controller {
  public:
    ~VehicleController() override = default;
    void Tick(Vehicle& vehicle, float dt) override;

    VehicleController() = delete;
    explicit VehicleController(const Road& road, double speed = 5.0);

    void SetNextRoad(const Road& road) noexcept;
    bool IsAtEndOfRoad() const noexcept;

  private:
    double distance_{0.0};
    double speed_{5.0};
    const Road* road_;
};
