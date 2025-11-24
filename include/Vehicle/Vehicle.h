#pragma once

struct Position {
    float x, y, z;
};

struct Velocity {
    float vx, vy, vz;
};

class Vehicle {
  public:
    Vehicle() = delete;

    explicit Vehicle(const Position& pos, const Velocity& vel) noexcept;
    void update(float dt);

    Position getPos() const noexcept { return position_; }

  private:
    Position position_;
    Velocity velocity_;
};
