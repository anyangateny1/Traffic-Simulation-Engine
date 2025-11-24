#include "SimulationEngine/SimulationEngine.h"
#include "Vehicle/Vehicle.h"

SimulationEngine::SimulationEngine() {
    // TODO: Fix and have user create engine.
    for (int i = 0; i < 5; i++) {
        vehicles_.emplace_back(
            std::make_unique<Vehicle>(Position{(float)i, 0, (float)i}, Velocity{0.5f, 0, 0.3f}));
    }
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
