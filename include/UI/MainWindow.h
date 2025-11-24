#pragma once
#include <QMainWindow>

// Forward declarations
class SimulationEngine;
class Renderer;
class QPushButton;
class QVBoxLayout;
class QWidget;
class QTimer;

/**
 * MainWindow is the Controller in the MVC pattern.
 * It owns both the Model (SimulationEngine) and the View (Renderer).
 * It handles user input, updates the simulation, and pushes state to the view.
 */
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
    // UI Components
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
