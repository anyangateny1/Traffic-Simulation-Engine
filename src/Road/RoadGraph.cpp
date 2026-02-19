#include "Identifiers/IDs.h"
#include "Road/RoadGraph.h"
#include "Road/RoadIntersection.h"
#include <memory>
#include <span>
#include <stdexcept>
#include <string>

void RoadGraph::AddNode(NodeID id, const Position& pos) {
    if (node_id_to_index_.contains(id)) {
        throw std::runtime_error("Duplicate node ID: " + std::to_string(id.value()));
    }

    size_t index = nodes_.size();
    node_id_to_index_[id] = index;

    nodes_.emplace_back(id, pos);
    adjacency_[id] = {}; // new empty adjacency list for this node
}

const Node& RoadGraph::NodeById(NodeID id) const {
    auto it = node_id_to_index_.find(id);
    if (it == node_id_to_index_.end()) {
        throw std::runtime_error("Node ID not found: " + std::to_string(id.value()));
    }
    return nodes_[it->second];
}

const Road& RoadGraph::RoadById(RoadID id) const {
    auto it = roads_.find(id);
    if (it == roads_.end()) {
        throw std::runtime_error("Road ID not found: " + std::to_string(id.value()));
    }
    return *it->second;
}

void RoadGraph::AddRoad(NodeID from_id,
                        NodeID to_id,
                        double true_distance,
                        std::vector<Position> curve_points,
                        std::span<const LaneConfig> lane_configs) {
    if (!node_id_to_index_.contains(from_id) || !node_id_to_index_.contains(to_id)) {
        throw std::runtime_error("Road refers to unknown node ID.");
    }

    const Node& from_node = NodeById(from_id);
    const Node& to_node = NodeById(to_id);

    curve_points.emplace_back(from_node.Pos());
    curve_points.emplace_back(to_node.Pos());

    auto road_id = RoadID(static_cast<RoadID::value_type>(roads_.size()));
    roads_[road_id] = std::make_unique<Road>(road_id, from_node.Pos(), to_node.Pos(), true_distance,
                                             std::move(curve_points), lane_configs);

    adjacency_[from_id].emplace_back(to_id, road_id);
    adjacency_[to_id].emplace_back(from_id, road_id);
}

void RoadGraph::AddIntersections(std::vector<RoadIntersection>&& intersections) {
    intersections_ = std::move(intersections);
}
