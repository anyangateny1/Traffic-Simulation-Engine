#include "Road/Road.h"
#include "Route/Route.h"
#include "Vehicle/Vehicle.h"
#include "Vehicle/VehicleController.h"

VehicleController::VehicleController(std::unique_ptr<Route> route, double speed)
    : route_(std::move(route)), speed_(speed) {}

void VehicleController::Tick(Vehicle& vehicle, float dt) {
    double distance = speed_ * dt;
    vehicle.SetPosition(route_->Advance(distance));
}
