#pragma once

#include "Geometry/Position.h"
#include "Road/Node.h"
#include "Road/Road.h"

#include <cstddef>
#include <unordered_map>
#include <vector>

// Stores an edge in the adjacency list
struct RoadEdge {
    size_t to_node_index;  // Index of the destination node
    size_t road_index;     // Index into the roads_ vector
};

class RoadGraph {
  public:
    RoadGraph() = default;

    RoadGraph(const RoadGraph&) = delete;
    RoadGraph(RoadGraph&&) = delete;
    RoadGraph& operator=(const RoadGraph&) = delete;
    RoadGraph& operator=(RoadGraph&&) = delete;

    void AddNode(int id, double x_coord, double y_coord);

    void AddRoad(int from_id,
                 int to_id,
                 double true_distance,
                 const std::vector<Position>& curve_points);

    const Node& NodeById(int id) const;
    const Node& NodeByIndex(size_t index) const;
    
    const Road& RoadByIndex(size_t index) const;

    size_t NodeCount() const noexcept { return nodes_.size(); }
    size_t RoadCount() const noexcept { return roads_.size(); }

    const std::vector<Node>& GetNodes() const noexcept { return nodes_; }
    const std::vector<Road>& GetRoads() const noexcept { return roads_; }
    const std::vector<std::vector<RoadEdge>>& GetAdjacency() const noexcept { return adjacency_; }

  private:
    std::vector<Node> nodes_;
    std::vector<Road> roads_;
    std::vector<std::vector<RoadEdge>> adjacency_;

    // Maps user-defined IDs to indices in nodes_
    std::unordered_map<int, size_t> id_to_index_;
};
