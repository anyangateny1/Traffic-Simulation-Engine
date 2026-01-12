#include "Geometry/Position.h"
#include "Road/RoadGraph.h"
#include "Route/Route.h"
#include <stdexcept>

Route::Route(const RoadGraph& graph, const Path& path)
    : graph_(graph), roads_(path.roads), currentRoadIndex_(0), distanceAlongRoad_(0.0) {
    if (roads_.empty()) {
        currentRoad_ = nullptr;
        currentLane_ = nullptr;
    } else {
        currentRoad_ = &graph_.RoadById(roads_.front());
        currentLane_ = &currentRoad_->GetDefaultLane();
    }
}

Position Route::Advance(double delta) {
    if (IsFinished()) {
        return finalPosition_;
    }

    distanceAlongRoad_ += delta;

    // Keep moving to next road if delta goes past current road
    while (distanceAlongRoad_ > currentRoad_->Length()) {
        distanceAlongRoad_ -= currentRoad_->Length();
        finalPosition_ = currentRoad_->EndPoint();
        MoveToNextRoad();
        if (IsFinished()) {
            return finalPosition_;
        }
    }

    return currentLane_->GetPositionAtDistance(distanceAlongRoad_);
}

void Route::MoveToNextRoad() {
    currentRoadIndex_++;
    if (currentRoadIndex_ < roads_.size()) {
        currentRoad_ = &graph_.RoadById(roads_[currentRoadIndex_]);
        currentLane_ = &currentRoad_->GetDefaultLane();
    } else {
        currentRoad_ = nullptr;
        currentLane_ = nullptr;
    }
}
