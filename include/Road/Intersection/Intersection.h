#pragma once

#include "Geometry/Position.h"
#include "Identifiers/IDs.h"
#include "Road/Intersection/IntersectionPolicies/IntersectionPolicy.h"
#include <memory>
#include <unordered_set>

class Intersection {
  public:
    Intersection(IntersectionID id,
                 Position pos,
                 std::vector<RoadID> roads,
                 std::unique_ptr<IntersectionPolicy> policy);

    bool RequestEntry(VehicleID vehicle, RoadID from_road);

    void NotifyExit(VehicleID vehicle);

    void Tick(float dt);

  private:
    IntersectionID id_;
    Position pos_;
    std::vector<RoadID> roads_;
    std::unique_ptr<IntersectionPolicy> policy_;
    std::unordered_set<VehicleID> vehicles_inside_;
};
