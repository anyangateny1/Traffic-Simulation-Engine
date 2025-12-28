#include <Route/Route.h>
#include <print>

const Position Route::Advance(double delta) {
    double remaining = delta;

    while (remaining > 0.0) {
        double laneRemaining = currentLane_->Length() - distance_;

        if (remaining < laneRemaining) {
            distance_ += remaining;
            remaining = 0.0;
        } else {
            remaining -= laneRemaining;
            distance_ = 0.0;

            ChooseNextRoad();
        }
    }

    return currentLane_->GetPositionAtDistance(distance_);
}

void Route::ChooseNextRoad() {
    const auto& adjacency = graph_.GetAdjacency();

    auto it = adjacency.find(destinationNode_);
    if (it == adjacency.end() || it->second.empty()) {
        throw std::runtime_error("No outgoing roads from node");
    }

    // TODO: Currently picks first available road
    const RoadEdge& edge = it->second.back();
    std::print("to_node={}, road={}", edge.to_node_id.value(), edge.road_id.value());

    currentNode_ = destinationNode_;
    destinationNode_ = edge.to_node_id;
    currentRoad_ = &graph_.RoadById(edge.road_id);
    currentLane_ = &currentRoad_->GetDefaultLane();
}
