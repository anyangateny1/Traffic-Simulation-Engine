#include "Vehicle/Controller.h"
#include "Vehicle/Vehicle.h"

Vehicle::Vehicle(VehicleID id, std::unique_ptr<Controller> controller)
    : id_(id), controller_(std::move(controller)) {}

Vehicle::~Vehicle() = default;

void Vehicle::update(float dt) {
    controller_->Tick(*this, dt);
}
