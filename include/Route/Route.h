#pragma once

#include "Road/Lane.h"
#include "Road/Road.h"
#include "Road/RoadGraph.h"

class Route {
  public:
    Route(const RoadGraph& graph, NodeID startNode, NodeID destNode, RoadID startRoad)
        : graph_(graph), currentNode_(startNode), destinationNode_(destNode),
          currentRoad_(&graph.RoadById(startRoad)),
          currentLane_(&currentRoad_->GetDefaultLane()), distance_(0.0) {}

    const Position Advance(double delta);
    void ChooseNextRoad();

  private:
    const RoadGraph& graph_;
    NodeID currentNode_;
    NodeID destinationNode_;
    const Road* currentRoad_;
    const Lane* currentLane_;
    double distance_{};
};
