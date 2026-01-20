#include "SimulationEngine/SimState.h"
#include <qobject.h>

class SimulationEngine;

class SimulationController : public QObject {
    Q_OBJECT
  public:
    explicit SimulationController(SimulationEngine& engine);

    void start();
    void pause();
    void step();

  signals:
    void stateChanged(SimState state);

  private:
    SimulationEngine& engine_;
};
