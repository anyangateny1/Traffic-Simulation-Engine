#pragma once

#include "Geometry/Position.h"
#include "Road/Node.h"
#include "Road/Road.h"
#include "Road/RoadIntersection.h"

#include <cstddef>
#include <memory>
#include <span>
#include <unordered_map>
#include <vector>

// Stores an edge in the adjacency list
struct RoadEdge {
    NodeID to_node_id;
    RoadID road_id;
};

class RoadGraph {
  public:
    RoadGraph() = default;

    RoadGraph(const RoadGraph&) = delete;
    RoadGraph(RoadGraph&&) = delete;
    RoadGraph& operator=(const RoadGraph&) = delete;
    RoadGraph& operator=(RoadGraph&&) = delete;

    void AddNode(NodeID id, const Position& pos);

    void AddIntersections(std::vector<RoadIntersection>&& intersections);

    const std::vector<RoadIntersection>& GetIntersections() const noexcept {
        return intersections_;
    }

    void AddRoad(NodeID from_id,
                 NodeID to_id,
                 double true_distance,
                 std::vector<Position> curve_points,
                 std::span<const LaneConfig> lane_configs);

    const Node& NodeById(NodeID id) const;
    const Road& RoadById(RoadID id) const;

    size_t NodeCount() const noexcept { return nodes_.size(); }
    size_t RoadCount() const noexcept { return roads_.size(); }

    const std::vector<Node>& GetNodes() const noexcept { return nodes_; }
    const std::unordered_map<RoadID, std::unique_ptr<Road>>& GetRoads() const noexcept {
        return roads_;
    }
    const std::unordered_map<NodeID, std::vector<RoadEdge>>& GetAdjacency() const noexcept {
        return adjacency_;
    }


  private:
    std::vector<Node> nodes_;
    std::unordered_map<RoadID, std::unique_ptr<Road>> roads_;
    std::unordered_map<NodeID, std::vector<RoadEdge>> adjacency_;

    std::vector<RoadIntersection> intersections_;

    // Maps user-defined NodeIDs to indices in nodes_ vector
    std::unordered_map<NodeID, size_t> node_id_to_index_;
};
