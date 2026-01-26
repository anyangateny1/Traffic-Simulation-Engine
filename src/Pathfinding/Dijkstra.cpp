#include "PathFinding/Dijkstra.h"

#include <queue>


Path Dijkstra::FindPath(const NodeID& source, const NodeID& destination) const {
    using DistanceMap = std::unordered_map<NodeID, double>;
    using CameFromMap = std::unordered_map<NodeID, NodeID>;
    using CameViaRoadMap = std::unordered_map<NodeID, RoadID>;

    DistanceMap distance;
    CameFromMap came_from;
    CameViaRoadMap came_via_road;

    std::priority_queue<QueueNode, std::vector<QueueNode>, std::greater<>> open;

    for (const auto& node : graph_.GetNodes()) {
        distance[node.Id()] = std::numeric_limits<double>::infinity();
    }

    distance[source] = 0.0;
    open.push({source, 0.0});

    const auto& adjacency = graph_.GetAdjacency();

    while (!open.empty()) {
        NodeID current = open.top().node;
        double currentCost = open.top().cost;
        open.pop();

        if (currentCost > distance[current])
            continue;

        if (current == destination)
            break;

        for (const RoadEdge& edge : adjacency.at(current)) {
            NodeID next = edge.to_node_id;
            const Road& road = graph_.RoadById(edge.road_id);

            double newCost = currentCost + road.Length();

            if (newCost < distance[next]) {
                distance[next] = newCost;
                came_from[next] = current;
                came_via_road[next] = edge.road_id;

                open.push({next, newCost});
            }
        }
    }

    Path path;
    path.total_distance = distance[destination];

    if (!came_from.contains(destination)) {
        return path;
    }

    NodeID current = destination;
    while (current != source) {
        path.roads.push_back(came_via_road[current]);
        current = came_from[current];
    }

    std::reverse(path.roads.begin(), path.roads.end());
    return path;
}
