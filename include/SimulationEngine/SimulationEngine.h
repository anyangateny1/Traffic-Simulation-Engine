#pragma once
#include "Vehicle/Vehicle.h"
#include <memory>
#include <vector>

class SimulationEngine {
  public:
    SimulationEngine();

    SimulationEngine(const SimulationEngine&) = delete;
    SimulationEngine& operator=(const SimulationEngine&) = delete;

    void start();
    void pause();
    void step();

    const std::vector<std::unique_ptr<Vehicle>>& getVehicles() const noexcept { return vehicles_; }

  private:
    std::vector<std::unique_ptr<Vehicle>> vehicles_;
};
