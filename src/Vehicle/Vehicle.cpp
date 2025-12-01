#include "Vehicle/Vehicle.h"

Vehicle::Vehicle(const VehicleID& id, const Position& pos, const Velocity& vel) noexcept
    : id_(id), position_(pos), velocity_(vel) {}

void Vehicle::update(const Position& pos, const Velocity& vel) {
    position_.x += velocity_.vx * dt;
    position_.y += velocity_.vy * dt;
    position_.z += velocity_.vz * dt;
}
