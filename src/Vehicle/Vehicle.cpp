#include "Vehicle/Controller.h"
#include "Vehicle/Vehicle.h"

Vehicle::Vehicle(VehicleID id, std::unique_ptr<Controller> controller)
    : id_(id), controller_(std::move(controller)) {}

void Vehicle::update(float dt) {
    controller_->Tick(*this, dt);
}

void Vehicle::SetPosition(float x, float y) {
    x_ = x;
    y_ = y;
}

std::pair<float, float> Vehicle::GetPosition() const {
    return {x_, y_};
}
