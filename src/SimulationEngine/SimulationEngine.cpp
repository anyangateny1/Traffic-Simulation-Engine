#include "SimulationEngine/SimulationEngine.h"
#include "Vehicle/Vehicle.h"
#include "Road/MapLoader.h"
#include <iostream>

SimulationEngine::SimulationEngine() {
    // TODO: Fix and have user create engine.
    for (int i = 0; i < 1; i++) {
        vehicles_.emplace_back(
            std::make_unique<Vehicle>(Position{(float)i, 0, (float)i}, Velocity{0.5f, 0, 0.3f}));
    }
}

void SimulationEngine::LoadMap(const std::string& filepath) {
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
    const float deltaTime = 0.016f; // ~60 FPS timestep
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
        data.vehicles.push_back({pos.x, pos.y, pos.z});
    }
    
    // Extract node data
    const auto& nodes = road_graph_.GetNodes();
    data.nodes.reserve(nodes.size());
    for (const auto& node : nodes) {
        data.nodes.push_back({node.id_, node.x_coordinate_, node.y_coordinate_});
    }
    
    // Extract road data
    const auto& adjacency = road_graph_.GetAdjacency();
    for (const auto& road_list : adjacency) {
        for (const auto& road : road_list) {
            data.roads.push_back({
                road.FromId(),
                road.ToId(),
                road.CurvePoints(),
                road.Length()
            });
        }
    }
    
    return data;
}
