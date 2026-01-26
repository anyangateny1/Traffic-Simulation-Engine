#pragma once
#include "Geometry/Position.h"
#include "Identifiers/IDs.h"
#include "PathFinding/PathFinder.h"
#include "Road/RoadGraph.h"
#include "SimulationEngine/SimState.h"
#include "Vehicle/Vehicle.h"
#include <filesystem>
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

    void LoadMap(const std::filesystem::path& mapFilePath);

    void Start();
    void Pause();
    void Step();

    // Spawn a vehicle on a specific road between two nodes
    void SpawnVehicle(NodeID from, NodeID to, double speed = 5.0);

    RenderData GetRenderData() const;

    SimState State() const;
    bool SetState(SimState new_state) noexcept;

    const std::vector<std::unique_ptr<Vehicle>>& getVehicles() const noexcept;

  private:
    RoadGraph road_graph_;
    std::vector<std::unique_ptr<Vehicle>> vehicles_;

    SimState state_{SimState::STOPPED};

    std::unique_ptr<PathFinder> path_finder_;
};
