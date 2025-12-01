#pragma once

struct Position {
    float x, y, z;
};

struct VehicleID {
    int id;
};

struct Velocity {
    float vx, vy, vz;
};

class Vehicle {
  public:
    Vehicle() = delete;

    explicit Vehicle(const VehicleID& id, const Position& pos, const Velocity& vel) noexcept;
    void update(const Position& pos, const Velocity& vel);

    Position getPos() const noexcept { return position_; }

  private:
    VehicleID id_;
    Position position_;
    Velocity velocity_;
};
