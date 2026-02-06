#pragma once
#include "SimulationEngine/SimState.h"
#include "SimulationEngine/SimulationEngine.h"
#include <QObject>
#include <filesystem>
#include <memory>
#include <span>

class SimulationEngine;
class Node;

class SimulationController : public QObject {
    Q_OBJECT
  public:
    explicit SimulationController(std::unique_ptr<SimulationEngine> engine,
                                  const std::filesystem::path& mapFilePath);

    void start();
    void pause();
    void step();
    void emitInitialRenderData();

    void spawnVehicle(const VehicleInfo& info);

    std::span<const Node> getNodes() const;

  signals:
    void stateChanged(SimState state);
    void renderDataUpdated(RenderData data);

  private:
    std::unique_ptr<SimulationEngine> engine_;
};
