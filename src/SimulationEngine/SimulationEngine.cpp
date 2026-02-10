#include "Map/MapLoader.h"
#include "Route/Route.h"
#include "SimulationEngine/SimulationConfig.h"
#include "SimulationEngine/SimulationEngine.h"
#include "Vehicle/Vehicle.h"
#include "Vehicle/VehicleController.h"

#include <iostream>

SimulationEngine::SimulationEngine() {}

void SimulationEngine::LoadMap(const std::filesystem::path& filepath) {
    if (!MapLoader::LoadMapFromJson(road_graph_, filepath)) {
        std::cerr << "Failed to load map from: " << filepath << "\n";
        throw std::runtime_error("Map loading failed");
    }
    std::cout << "Successfully loaded map with " << road_graph_.NodeCount() << " nodes\n";
    vehicle_factory_ = std::make_unique<VehicleFactory>(road_graph_);
}

void SimulationEngine::SpawnVehicle(const VehicleInfo& info) {
    auto vehicle = vehicle_factory_->CreateCar(info);
    if (vehicle) {
        vehicles_.push_back(std::move(vehicle));
    }
}

void SimulationEngine::Start() {
    SetState(SimState::RUNNING);
}

void SimulationEngine::Pause() {
    SetState(SimState::PAUSED);
}

void SimulationEngine::Step() {
    if (state_ != SimState::RUNNING) {
        return;
    }

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
            data.roads.push_back(
                {from_node_id, edge.to_node_id, road.CurvePoints(), road.Length()});
        }
    }

    return data;
}

SimState SimulationEngine::State() const {
    return state_;
}
bool SimulationEngine::SetState(SimState new_state) noexcept {
    if (state_ == new_state) {
        return false;
    }
    state_ = new_state;
    return true;
}

const std::vector<std::unique_ptr<Vehicle>>& SimulationEngine::getVehicles() const noexcept {
    return vehicles_;
}
