#pragma once

#include "Identifiers/IDs.h"
#include "Path.h"
#include "PathFinder.h"
#include "Road/RoadGraph.h"


struct QueueNode {
    NodeID node;
    double cost;

    bool operator>(const QueueNode& other) const { return cost > other.cost; }
};

class Dijkstra : public PathFinder {
  public:
    explicit Dijkstra(const RoadGraph& graph) : graph_(graph) {};

    // No default constructing, no copying and ownership should not be transferred
    Dijkstra() = delete;
    Dijkstra(const Dijkstra& other) = delete;
    Dijkstra& operator=(const Dijkstra& other) = delete;
    Dijkstra(Dijkstra&& other) = delete;
    Dijkstra& operator=(Dijkstra&& other) = delete;

    Path FindPath(const NodeID& source, const NodeID& destination) const override;


  private:
    const RoadGraph& graph_;
};
