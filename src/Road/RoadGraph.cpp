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

void RoadGraph::AddRoad(int from_id,
                        int to_id,
                        double true_distance,
                        const std::vector<std::pair<double, double>>& curve_points) {
    auto from_it = id_to_index_.find(from_id);
    auto to_it = id_to_index_.find(to_id);

    if (from_it == id_to_index_.end() || to_it == id_to_index_.end()) {
        throw std::runtime_error("Road refers to unknown node ID.");
    }

    size_t from_index = from_it->second;
    size_t to_index = to_it->second;

    adjacency_[from_index].emplace_back(from_id, to_id, true_distance, curve_points);
}
