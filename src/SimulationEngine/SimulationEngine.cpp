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
    SpawnVehicle(0, 5.0);
}

void SimulationEngine::SpawnVehicle(size_t road_index, double speed) {
    if (road_index >= road_graph_.RoadCount()) {
        std::cerr << "Invalid road index: " << road_index << "\n";
        return;
    }
    Road& road = const_cast<Road&>(road_graph_.RoadByIndex(road_index));

    auto controller = std::make_unique<VehicleController>(road, speed);
    int vehicle_id = static_cast<int>(vehicles_.size());

    vehicles_.emplace_back(std::make_unique<Vehicle>(VehicleID{vehicle_id}, std::move(controller)));
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
    const auto& all_nodes = road_graph_.GetNodes();

    for (size_t from_idx = 0; from_idx < adjacency.size(); ++from_idx) {
        for (const auto& edge : adjacency[from_idx]) {
            const Road& road = road_graph_.RoadByIndex(edge.road_index);
            const Node& from_node = all_nodes[from_idx];
            const Node& to_node = all_nodes[edge.to_node_index];

            data.roads.push_back({from_node.Id(), to_node.Id(), road.CurvePoints(), road.Length()});
        }
    }

    return data;
}
