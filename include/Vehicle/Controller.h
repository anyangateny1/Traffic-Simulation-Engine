#pragma once

class Vehicle;  // Forward declaration

class Controller {
  public:
    virtual ~Controller() = default;
    virtual void Tick(Vehicle& vehicle, float dt) = 0;
};
