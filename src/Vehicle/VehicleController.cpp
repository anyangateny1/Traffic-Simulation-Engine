#include "Road/Road.h"
#include "Vehicle/Vehicle.h"
#include "Vehicle/VehicleController.h"

VehicleController::VehicleController(const Road& road, double speed) : lane_(&road.GetDefaultLane()), speed_(speed) {}

void VehicleController::Tick(Vehicle& vehicle, float dt) {
    if (!lane_)
        return;

    distance_ += speed_ * dt;
    vehicle.SetPosition(lane_->GetPositionAtDistance(distance_));
}

void VehicleController::SetNextRoad(const Road& road) noexcept {
    lane_ = &road.GetDefaultLane();
    distance_ = 0.0;
}

bool VehicleController::IsAtEndOfRoad() const noexcept {
    if (!lane_)
        return true;
    return distance_ >= lane_->Length();
}
