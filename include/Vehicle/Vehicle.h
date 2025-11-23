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

  private:
    Position position_;
    Velocity velocity_;
};
