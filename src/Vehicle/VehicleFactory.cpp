#include "PathFinding/Dijkstra.h"
#include "PathFinding/PathFinder.h"
#include "Vehicle/Controller.h"
#include "Vehicle/VehicleController.h"
#include "Vehicle/VehicleFactory.h"


std::unique_ptr<Vehicle> VehicleFactory::CreateCar(const VehicleInfo& vehicle_info,
                                                   const PathFindingAlgorithm path_algorithm) {
    auto& [from_id, to_id, speed] = vehicle_info;

    std::unique_ptr<PathFinder> path_finder = CreatePathFinder(path_algorithm);
    Path path = path_finder->FindPath(from_id, to_id);
    std::unique_ptr<Route> route = ConstructRoute(road_graph_, path);

    return ConstructCar(NextID(), std::move(ConstructController(std::move(route), speed)));
}

VehicleID VehicleFactory::NextID() {
    auto v = id_count_.fetch_add(1, std::memory_order_relaxed);
    return VehicleID(v);
}

std::unique_ptr<PathFinder> VehicleFactory::CreatePathFinder(const PathFindingAlgorithm algorithm) {
    using enum PathFindingAlgorithm;

    switch (algorithm) {
        case DIJKSTRA:
            return std::make_unique<Dijkstra>(road_graph_);
    }

    std::unreachable();
}

std::unique_ptr<Route> VehicleFactory::ConstructRoute(const RoadGraph& road_graph,
                                                      const Path& path) {
    return std::make_unique<Route>(road_graph, path);
}

std::unique_ptr<Controller> VehicleFactory::ConstructController(std::unique_ptr<Route> route,
                                                                const double speed) {
    return std::make_unique<VehicleController>(std::move(route), speed);
}


std::unique_ptr<Vehicle> VehicleFactory::ConstructCar(const VehicleID vehicle_id,
                                                      std::unique_ptr<Controller> controller) {
    return std::make_unique<Vehicle>(vehicle_id, std::move(controller));
}
