#pragma once
#include "Geometry/Position.h"
#include "Identifiers/IDs.h"
#include "Pathfinding/Pathfinder.h"
#include "Road/RoadGraph.h"
#include "Vehicle/Vehicle.h"
#include <memory>
#include <vector>

struct RenderData {
    struct VehicleRenderInfo {
        Position pos;
    };

    struct RoadRenderInfo {
        NodeID from_id;
        NodeID to_id;
        std::vector<Position> curve_points;
        double length;
    };

    struct NodeRenderInfo {
        NodeID id;
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

    void Start();
    void Pause();
    void Step();

    // Spawn a vehicle on a specific road between two nodes
    void SpawnVehicle(NodeID from, NodeID to, double speed = 5.0);

    RenderData GetRenderData() const;

    const std::vector<std::unique_ptr<Vehicle>>& getVehicles() const noexcept { return vehicles_; }

  private:
    RoadGraph road_graph_;
    std::vector<std::unique_ptr<Vehicle>> vehicles_;

    bool paused_{false};

    std::unique_ptr<Pathfinder> path_finder_;
};
