#pragma once
#include <QMainWindow>

class SimulationEngine;
class Renderer;
class QPushButton;
class QVBoxLayout;
class QWidget;
class QTimer;

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private slots:
    void onStartClicked();
    void onPauseClicked();
    void onStepClicked();
    void onSimulationTick();

  private:
    QWidget* centralWidget_;
    QVBoxLayout* layout_;
    QPushButton* startButton_;
    QPushButton* pauseButton_;
    QPushButton* stepButton_;

    SimulationEngine* simulationEngine_;
    Renderer* renderer_;
    QTimer* simulationTimer_;

    bool isRunning_;
};
