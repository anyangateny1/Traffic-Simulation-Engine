#include "Road/Road.h"
#include "Vehicle/Vehicle.h"
#include "Vehicle/VehicleController.h"

VehicleController::VehicleController(Road& road, double speed) : road_(&road), speed_(speed) {}

void VehicleController::Tick(Vehicle& vehicle, float dt) {
    if (!road_)
        return;

    distance_ += speed_ * dt;
    vehicle.SetPosition(road_->GetPositionAtDistance(distance_));
}

void VehicleController::SetNextRoad(Road& road) {
    road_ = &road;
    distance_ = 0.0;
}

bool VehicleController::IsAtEndOfRoad() const {
    if (!road_)
        return true;
    return distance_ >= road_->Length();
}
