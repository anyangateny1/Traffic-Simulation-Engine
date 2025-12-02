#pragma once

struct Position {
    float x, y;
};

struct VehicleID {
    int id;
};

struct Velocity {
    float vx, vy;
};

class Vehicle {
  public:
    Vehicle() = delete;

    explicit Vehicle(const VehicleID& id, const Position& pos, const Velocity& vel) noexcept;
    void update(float dt);

    Position getPos() const noexcept { return position_; }
    VehicleID getId() const noexcept { return id_; }

  private:
    VehicleID id_;
    Position position_;
    Velocity velocity_;
};
