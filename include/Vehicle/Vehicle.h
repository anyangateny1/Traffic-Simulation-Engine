#pragma once
#include "Vehicle/Controller.h"
#include <memory>
#include <utility>

struct VehicleID {
    int id;
};

class Vehicle {
  public:
    Vehicle(VehicleID id, std::unique_ptr<Controller> controller);

    void update(float dt);

    VehicleID getId() const noexcept { return id_; }

    void SetPosition(float x, float y);
    std::pair<float, float> GetPosition() const;

  private:
    VehicleID id_;
    std::unique_ptr<Controller> controller_;
    float x_{0.0f};
    float y_{0.0f};
};
