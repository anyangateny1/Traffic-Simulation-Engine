#pragma once
#include "Road/Node.h"
#include "Road/Road.h"
#include <vector>

class RoadGraph {
    std::vector<Node> nodes_;
    std::vector<std::vector<Road>> adjList_;

    RoadGraph(const RoadGraph&) = delete;
    RoadGraph(RoadGraph&&) = delete;
    RoadGraph& operator=(const RoadGraph&) = delete;
    RoadGraph& operator=(RoadGraph&&) = delete;

    void addNode(const int id, const double x_coord, const int y_coord);
    void addRoad(const Node& from,
                 const Node& to,
                 double true_distance,
                 const std::vector<std::pair<double, double>>& curve_points);

  private:
    int currentId_{0};
};
