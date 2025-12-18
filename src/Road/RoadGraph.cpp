#include "Road/RoadGraph.h"
#include <stdexcept>
#include <string>

void RoadGraph::AddNode(int id, double x_coord, double y_coord) {
    if (id_to_index_.contains(id)) {
        throw std::runtime_error("Duplicate node ID: " + std::to_string(id));
    }

    size_t index = nodes_.size();
    id_to_index_[id] = index;

    nodes_.emplace_back(id, x_coord, y_coord);
    adjacency_.emplace_back(); // new empty adjacency list
}

const Node& RoadGraph::NodeById(int id) const {
    auto it = id_to_index_.find(id);
    if (it == id_to_index_.end()) {
        throw std::runtime_error("Node ID not found: " + std::to_string(id));
    }
    return nodes_[it->second];
}

const Node& RoadGraph::NodeByIndex(size_t index) const {
    if (index >= nodes_.size()) {
        throw std::out_of_range("Node index out of range");
    }
    return nodes_[index];
}

const Road& RoadGraph::RoadByIndex(size_t index) const {
    if (index >= roads_.size()) {
        throw std::out_of_range("Road index out of range");
    }
    return roads_[index];
}

void RoadGraph::AddRoad(int from_id,
                        int to_id,
                        double true_distance,
                        const std::vector<Position>& curve_points) {
    auto from_it = id_to_index_.find(from_id);
    auto to_it = id_to_index_.find(to_id);

    if (from_it == id_to_index_.end() || to_it == id_to_index_.end()) {
        throw std::runtime_error("Road refers to unknown node ID.");
    }

    size_t from_index = from_it->second;
    size_t to_index = to_it->second;

    const Node& from_node = nodes_[from_index];
    const Node& to_node = nodes_[to_index];
    Position start_point = {from_node.X(), from_node.Y()};
    Position end_point = {to_node.X(), to_node.Y()};

    size_t road_index = roads_.size();
    roads_.emplace_back(start_point, end_point, true_distance, curve_points);

    adjacency_[from_index].push_back({to_index, road_index});
}
