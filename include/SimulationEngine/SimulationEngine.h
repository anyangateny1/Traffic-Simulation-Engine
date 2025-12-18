#pragma once
#include "Geometry/Position.h"
#include "Road/RoadGraph.h"
#include "Vehicle/Vehicle.h"
#include <memory>
#include <vector>

struct RenderData {
    struct VehicleRenderInfo {
        Position pos;
    };

    struct RoadRenderInfo {
        int from_id;
        int to_id;
        std::vector<Position> curve_points;
        double length;
    };

    struct NodeRenderInfo {
        int id;
        Position pos;
    };

    std::vector<VehicleRenderInfo> vehicles;
    std::vector<RoadRenderInfo> roads;
    std::vector<NodeRenderInfo> nodes;
};

class SimulationEngine {
  public:
    SimulationEngine();

    SimulationEngine(const SimulationEngine&) = delete;
    SimulationEngine& operator=(const SimulationEngine&) = delete;

    void LoadMap(std::string_view filepath);

    void start();
    void pause();
    void step();

    // Spawn a vehicle on a road (by road index)
    void SpawnVehicle(size_t road_index, double speed = 5.0);

    RenderData GetRenderData() const;

    const std::vector<std::unique_ptr<Vehicle>>& getVehicles() const noexcept { return vehicles_; }

  private:
    RoadGraph road_graph_;
    std::vector<std::unique_ptr<Vehicle>> vehicles_;
};
