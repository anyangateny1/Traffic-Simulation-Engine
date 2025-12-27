#include "Road/Lane.h"
#include "Road/Road.h"

Lane::Lane(Road* parent, LaneID id, LaneDirection dir, double offset, double width)
    : parent_road_(parent), id_(id), direction_(dir), lateral_offset_(offset), width_(width) {}

Position Lane::GetPositionAtDistance(double distance) const {
    return parent_road_->GetPositionAtDistance(distance);
}

Position Lane::GetHeadingAtDistance(double distance) const {
    const double epsilon = 0.01;
    Position p1 = GetPositionAtDistance(distance);
    Position p2 = GetPositionAtDistance(distance + epsilon);
    Position diff = p2 - p1;
    Position heading = diff.normalized();
    if (heading.magnitude() > 0.0) {
        return heading;
    }
    return Position{1.0, 0.0}; // Default heading
}

double Lane::Length() const {
    return parent_road_->Length();
}

void Lane::AddVehicle(Vehicle* v) {
    vehicles_.push_back(v);
}
void Lane::RemoveVehicle(Vehicle* v) {
    std::erase(vehicles_, v);
}
