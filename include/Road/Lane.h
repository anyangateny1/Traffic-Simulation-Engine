#pragma once
#include "Geometry/Position.h"
#include <vector>

class Vehicle;
class Road;

enum class LaneDirection {
    Forward = 1,  // start→ end
    Backward = -1 // end→ start
};

struct LaneConfig {
    LaneDirection dir;
    double offset;
    double width;
};

class Lane {
  public:
    Lane(Road* parent, int index, LaneDirection dir, double offset, double width);

    Position GetPositionAtDistance(double distance) const;
    Position GetHeadingAtDistance(double distance) const;

    double Length() const;

    int Index() const noexcept { return lane_index_; }
    LaneDirection Direction() const noexcept { return direction_; }
    double Offset() const noexcept { return lateral_offset_; }
    double Width() const noexcept { return width_; }
    Road* ParentRoad() const noexcept { return parent_road_; }

    void AddVehicle(Vehicle* v);
    void RemoveVehicle(Vehicle* v);
    const std::vector<Vehicle*>& Vehicles() const { return vehicles_; }

  private:
    Road* parent_road_;
    int lane_index_;
    LaneDirection direction_;
    double lateral_offset_;
    double width_;
    std::vector<Vehicle*> vehicles_;
};
