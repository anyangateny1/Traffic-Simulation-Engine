#pragma once

#include "PathFinding/Path.h"
#include "Road/Lane.h"
#include "Road/Road.h"
#include "Road/RoadGraph.h"

#include <vector>

class Route {
  public:
    Route(const RoadGraph& graph, const Path& path);

    Position Advance(double delta);

    void MoveToNextRoad();

    bool IsFinished() const noexcept { return currentRoadIndex_ >= roads_.size(); }

  private:
    const RoadGraph& graph_;

    std::vector<RoadID> roads_; // Precomputed path
    size_t currentRoadIndex_;   // Current road in the path
    const Road* currentRoad_;
    const Lane* currentLane_;
    double distanceAlongRoad_;
    Position finalPosition_; // Cached endpoint when route finishes
};
