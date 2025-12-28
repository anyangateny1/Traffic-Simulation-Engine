#pragma once
#include "Geometry/Position.h"
#include "Identifiers/IDs.h"
#include <memory>

class Controller;

class Vehicle {
  public:
    Vehicle() = delete;
    Vehicle(const Vehicle&) = delete;
    Vehicle(VehicleID id, std::unique_ptr<Controller> controller);
    ~Vehicle();

    void update(float dt);

    VehicleID getId() const noexcept { return id_; }

    void SetPosition(Position p) noexcept { pos_ = p; }
    const Position& GetPosition() const noexcept { return pos_; }

  private:
    VehicleID id_;
    std::unique_ptr<Controller> controller_;
    Position pos_;
};
