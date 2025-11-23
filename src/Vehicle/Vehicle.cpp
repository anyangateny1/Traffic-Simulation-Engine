#include "Vehicle/Vehicle.h"

Vehicle::Vehicle(const Position& pos, const Velocity& vel) noexcept
    : position_(pos), velocity_(vel) {}

void Vehicle::update(float dt) {
    position_.x += velocity_.vx * dt;
    position_.y += velocity_.vy * dt;
    position_.z += velocity_.vz * dt;
}
