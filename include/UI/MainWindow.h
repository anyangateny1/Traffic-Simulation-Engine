#pragma once
#include <QMainWindow>
#include <QString>

class SimulationEngine;
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
