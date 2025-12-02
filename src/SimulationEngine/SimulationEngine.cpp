#include "SimulationEngine/SimulationEngine.h"
#include "SimulationEngine/SimulationConfig.h"
#include "Vehicle/Vehicle.h"
#include "Road/MapLoader.h"
#include <iostream>

SimulationEngine::SimulationEngine() {
    // TODO: Fix and have user create engine.
    for (int i = 0; i < 1; i++) {
        vehicles_.emplace_back(
            std::make_unique<Vehicle>(VehicleID{i}, Position{(float)i * 5.0f, 10.0f}, Velocity{0.5f, 0.3f}));
    }
}

void SimulationEngine::LoadMap(std::string_view filepath) {
    if (!MapLoader::LoadMapFromJson(road_graph_, filepath)) {
        std::cerr << "Failed to load map from: " << filepath << "\n";
        throw std::runtime_error("Map loading failed");
    }
    std::cout << "Successfully loaded map with " << road_graph_.NodeCount() << " nodes\n";
}

void SimulationEngine::start() {
    // TODO: Start the simulation - could set a running flag here if needed
}

void SimulationEngine::pause() {
    // TODO:Pause the simulation - could set a paused flag here if needed
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
        auto pos = vehicle->getPos();
        data.vehicles.push_back({pos.x, pos.y});
    }
    
    // Extract node data
    const auto& nodes = road_graph_.GetNodes();
    data.nodes.reserve(nodes.size());
    for (const auto& node : nodes) {
        data.nodes.push_back({node.Id(), node.X(), node.Y()});
    }
    
    // Extract road data
    const auto& adjacency = road_graph_.GetAdjacency();
    const auto& all_nodes = road_graph_.GetNodes();
    
    for (size_t from_idx = 0; from_idx < adjacency.size(); ++from_idx) {
        for (const auto& edge : adjacency[from_idx]) {
            const Road& road = road_graph_.RoadByIndex(edge.road_index);
            const Node& from_node = all_nodes[from_idx];
            const Node& to_node = all_nodes[edge.to_node_index];
            
            data.roads.push_back({
                from_node.Id(),
                to_node.Id(),
                road.CurvePoints(),
                road.Length()
            });
        }
    }
    
    return data;
}
