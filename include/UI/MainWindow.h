#pragma once
#include "SimulationEngine/SimState.h"

#include <QMainWindow>
#include <filesystem>

class SimulationEngine;
class SimulationController;
class Renderer;
class QPushButton;
class QVBoxLayout;
class QWidget;
class QTimer;

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(const std::filesystem::path& mapFilePath, QWidget* parent = nullptr);
    ~MainWindow();

  protected:
    void showEvent(QShowEvent* event) override;

  private slots:

    void onStartClicked();
    void onPauseClicked();
    void onStepClicked();
    void onSpawnVehicleClicked();

    void onSimulationTick();
    void onSimulationStateChanged(SimState state);

  private:
    QWidget* centralWidget_;
    QVBoxLayout* layout_;
    QPushButton* startButton_;
    QPushButton* pauseButton_;
    QPushButton* stepButton_;
    QPushButton* spawnVehicleButton_;

    std::unique_ptr<SimulationController> simulationController_;
    Renderer* renderer_;
    QTimer* simulationTimer_;
};
