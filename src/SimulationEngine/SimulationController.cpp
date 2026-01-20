#include "SimulationEngine/SimulationController.h"
#include "SimulationEngine/SimulationEngine.h"

SimulationController::SimulationController(SimulationEngine& engine) : engine_(engine) {}

void SimulationController::start() {
    SimState before = engine_.state();
    engine_.Start();
    if (engine_.state() != before)
        emit stateChanged(engine_.state());
}

void SimulationController::pause() {
    SimState before = engine_.state();
    engine_.Pause();
    if (engine_.state() != before)
        emit stateChanged(engine_.state());
}

void SimulationController::step() {
    engine_.Step();
}
