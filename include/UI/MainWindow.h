#pragma once
#include "SimulationEngine/SimState.h"
#include <QMainWindow>
#include <QString>

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
    explicit MainWindow(const QString& mapFilePath, QWidget* parent = nullptr);
    ~MainWindow();

  protected:
    void showEvent(QShowEvent* event) override;

  private slots:
    void onStartClicked();
    void onPauseClicked();
    void onStepClicked();
    void onSimulationTick();
    void onSimulationStateChanged(SimState state);

  private:
    QWidget* centralWidget_;
    QVBoxLayout* layout_;
    QPushButton* startButton_;
    QPushButton* pauseButton_;
    QPushButton* stepButton_;

    SimulationEngine* simulationEngine_;
    SimulationController* simulationController_;
    Renderer* renderer_;
    QTimer* simulationTimer_;
};
