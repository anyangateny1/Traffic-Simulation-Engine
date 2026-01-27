#include "SimulationEngine/SimulationController.h"
#include "SimulationEngine/SimulationEngine.h"

SimulationController::SimulationController(std::unique_ptr<SimulationEngine> engine,
                                           const std::filesystem::path& mapFilePath)
    : engine_(std::move(engine)) {
    engine_->LoadMap(mapFilePath);
}

void SimulationController::start() {
    SimState before = engine_->State();
    engine_->Start();
    if (engine_->State() != before)
        emit stateChanged(engine_->State());
}

void SimulationController::pause() {
    SimState before = engine_->State();
    engine_->Pause();
    if (engine_->State() != before)
        emit stateChanged(engine_->State());
}

void SimulationController::step() {
    engine_->Step();

    emit renderDataUpdated(engine_->GetRenderData());
}

void SimulationController::emitInitialRenderData() {
    emit renderDataUpdated(engine_->GetRenderData());
}
