#pragma once
#include "SimulationEngine/SimState.h"
#include "SimulationEngine/SimulationEngine.h"
#include <QObject>
#include <filesystem>
#include <memory>

class SimulationEngine;

class SimulationController : public QObject {
    Q_OBJECT
  public:
    explicit SimulationController(std::unique_ptr<SimulationEngine> engine,
                                  const std::filesystem::path& mapFilePath);

    void start();
    void pause();
    void step();
    void emitInitialRenderData();

  signals:
    void stateChanged(SimState state);
    void renderDataUpdated(RenderData data);

  private:
    std::unique_ptr<SimulationEngine> engine_;
};
