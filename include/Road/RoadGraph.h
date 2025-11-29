#pragma once

#include "Road/Node.h"
#include "Road/Road.h"

#include <cstddef>
#include <unordered_map>
#include <vector>

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
                 const std::vector<std::pair<double, double>>& curve_points);

    const Node& NodeById(int id) const;

    size_t NodeCount() const noexcept { return nodes_.size(); }

    const std::vector<Node>& GetNodes() const noexcept { return nodes_; }
    const std::vector<std::vector<Road>>& GetAdjacency() const noexcept { return adjacency_; }

  private:
    std::vector<Node> nodes_;
    std::vector<std::vector<Road>> adjacency_;

    // Maps user-defined IDs to indices in nodes_
    std::unordered_map<int, size_t> id_to_index_;
};
