#include "Road/MapLoader.h"
#include "SimulationEngine/SimulationConfig.h"
#include "SimulationEngine/SimulationEngine.h"
#include "Vehicle/Vehicle.h"
#include "Vehicle/VehicleController.h"
#include <iostream>

SimulationEngine::SimulationEngine() {}

void SimulationEngine::LoadMap(std::string_view filepath) {
    if (!MapLoader::LoadMapFromJson(road_graph_, filepath)) {
        std::cerr << "Failed to load map from: " << filepath << "\n";
        throw std::runtime_error("Map loading failed");
    }
    std::cout << "Successfully loaded map with " << road_graph_.NodeCount() << " nodes\n";
    SpawnVehicle(RoadID(0), 5.0);
}

void SimulationEngine::SpawnVehicle(RoadID road_id, double speed) {
    const Road& road = road_graph_.RoadById(road_id);
    auto controller = std::make_unique<VehicleController>(road, speed);
    auto vehicle_id = VehicleID(static_cast<VehicleID::value_type>(vehicles_.size()));

    vehicles_.emplace_back(std::make_unique<Vehicle>(vehicle_id, std::move(controller)));
}

void SimulationEngine::start() {
    // TODO: Start the simulation - could set a running flag here if needed
}

void SimulationEngine::pause() {
    // TODO: Pause the simulation - could set a paused flag here if needed
}

void SimulationEngine::step() {
    const float deltaTime = SimulationConfig::GetDeltaTimeSeconds();
    for (auto& vehicle : vehicles_) {
        vehicle->update(deltaTime);
    }
}

RenderData SimulationEngine::GetRenderData() const {
    RenderData data;

    // Extract vehicle data
    data.vehicles.reserve(vehicles_.size());
    for (const auto& vehicle : vehicles_) {
        data.vehicles.emplace_back(vehicle->GetPosition());
    }

    // Extract node data
    const auto& nodes = road_graph_.GetNodes();
    data.nodes.reserve(nodes.size());
    for (const auto& node : nodes) {
        data.nodes.push_back({node.Id(), node.Pos()});
    }

    // Extract road data
    const auto& adjacency = road_graph_.GetAdjacency();

    for (const auto& [from_node_id, edges] : adjacency) {
        for (const auto& edge : edges) {
            const Road& road = road_graph_.RoadById(edge.road_id);
            data.roads.push_back({from_node_id, edge.to_node_id, road.CurvePoints(), road.Length()});
        }
    }

    return data;
}
