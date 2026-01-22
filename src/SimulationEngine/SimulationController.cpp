#include "SimulationEngine/SimulationController.h"
#include "SimulationEngine/SimulationEngine.h"

SimulationController::SimulationController(SimulationEngine& engine) : engine_(engine) {}

void SimulationController::start() {
    SimState before = engine_.State();
    engine_.Start();
    if (engine_.State() != before)
        emit stateChanged(engine_.State());
}

void SimulationController::pause() {
    SimState before = engine_.State();
    engine_.Pause();
    if (engine_.State() != before)
        emit stateChanged(engine_.State());
}

void SimulationController::step() {
    engine_.Step();
}
