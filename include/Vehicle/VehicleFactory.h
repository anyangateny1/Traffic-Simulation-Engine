#pragma once

#include "PathFinding/PathFinder.h"
#include "Route/Route.h"
#include "Vehicle/Vehicle.h"
#include "Vehicle/VehicleInfo.h"

#include <atomic>
#include <memory>

class RoadGraph;


class VehicleFactory {
  public:
    explicit VehicleFactory(const RoadGraph& road_graph) : road_graph_(road_graph) {}

    VehicleFactory() = delete;
    VehicleFactory(const VehicleFactory&) = delete;
    VehicleFactory& operator=(const VehicleFactory&) = delete;
    VehicleFactory(VehicleFactory&&) = delete;
    VehicleFactory& operator=(VehicleFactory&&) = delete;


    std::unique_ptr<Vehicle>
    CreateCar(const VehicleInfo& vehicle_info,
              const PathFindingAlgorithm path_algorithm = PathFindingAlgorithm::DIJKSTRA);

  private:
    VehicleID NextID();
    std::unique_ptr<PathFinder> CreatePathFinder(const PathFindingAlgorithm algorithm);
    std::unique_ptr<Route> ConstructRoute(const RoadGraph& road_graph, const Path& path);
    std::unique_ptr<Controller> ConstructController(std::unique_ptr<Route> route,
                                                    const double speed);


    std::unique_ptr<Vehicle> ConstructCar(const VehicleID vehicle_id,
                                          std::unique_ptr<Controller> controller);

    std::atomic<std::uint32_t> id_count_{0};

    const RoadGraph& road_graph_;
};
