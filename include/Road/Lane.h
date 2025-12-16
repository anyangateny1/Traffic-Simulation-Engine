#pragma once
#include <vector>

class Vehicle;

enum class LaneDirection {
    Forward = 1,  // start→end
    Backward = -1 // end→start
};

class Lane {
  private:
    int lane_index;
    LaneDirection direction; // Which way traffic flows

    std::vector<Vehicle*> vehicles;

  public:
    explicit Lane(int index, LaneDirection dir = LaneDirection::Forward)
        : lane_index(index), direction(dir) {}

    int DirectionMultiplier() const noexcept { return static_cast<int>(direction); }

    bool IsForward() const noexcept { return direction == LaneDirection::Forward; }
    bool IsBackward() const noexcept { return direction == LaneDirection::Backward; }
};
