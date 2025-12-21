#include "Road/Lane.h"

Lane::Lane(Road* parent, int index, LaneDirection dir, double offset, double width)
    : parent_road_(parent), lane_index_(index), direction_(dir), lateral_offset_(offset),
      width_(width) {}

void Lane::AddVehicle(Vehicle* v) {
    vehicles_.push_back(v);
}
void Lane::RemoveVehicle(Vehicle* v) {
    std::erase(vehicles_, v);
}
