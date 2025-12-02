#pragma once
#include "Road/RoadGraph.h"
#include "Vehicle/Vehicle.h"
#include <memory>
#include <vector>

struct RenderData {
    struct VehicleRenderInfo {
        float x, y;
    };

    struct RoadRenderInfo {
        int from_id;
        int to_id;
        std::vector<std::pair<double, double>> curve_points;
        double length;
    };

    struct NodeRenderInfo {
        int id;
        double x, y;
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

    RenderData GetRenderData() const;

    const std::vector<std::unique_ptr<Vehicle>>& getVehicles() const noexcept { return vehicles_; }

  private:
    RoadGraph road_graph_;
    std::vector<std::unique_ptr<Vehicle>> vehicles_;
};
