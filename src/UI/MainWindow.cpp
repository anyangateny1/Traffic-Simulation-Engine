#include "Renderer/Renderer.h"
#include "SimulationEngine/SimulationConfig.h"
#include "SimulationEngine/SimulationController.h"
#include "SimulationEngine/SimulationEngine.h"
#include "UI/MainWindow.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QShowEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <filesystem>

MainWindow::MainWindow(const std::filesystem::path& mapFilePath, QWidget* parent)
    : QMainWindow(parent) {
    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);
    layout_ = new QVBoxLayout(centralWidget_);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    startButton_ = new QPushButton("Start", this);
    pauseButton_ = new QPushButton("Pause", this);
    stepButton_ = new QPushButton("Step", this);

    pauseButton_->setEnabled(false);

    buttonLayout->addWidget(startButton_);
    buttonLayout->addWidget(pauseButton_);
    buttonLayout->addWidget(stepButton_);
    buttonLayout->addStretch();

    layout_->addLayout(buttonLayout);

    simulationEngine_ = new SimulationEngine();

    try {
        simulationEngine_->LoadMap(mapFilePath);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Map Loading Error",
                              QString("Failed to load map: %1").arg(e.what()));
        throw;
    }

    simulationController_ = new SimulationController(*simulationEngine_);

    renderer_ = new Renderer(this);
    layout_->addWidget(renderer_);

    simulationTimer_ = new QTimer(this);
    simulationTimer_->setInterval(SimulationConfig::GetTimerIntervalMs());

    connect(startButton_, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(pauseButton_, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(stepButton_, &QPushButton::clicked, this, &MainWindow::onStepClicked);

    connect(simulationTimer_, &QTimer::timeout, this, &MainWindow::onSimulationTick);
    connect(simulationController_, &SimulationController::stateChanged, this,
            &MainWindow::onSimulationStateChanged);

    setWindowTitle("Traffic Simulation");
    resize(800, 600);
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);
    static bool firstShow = true;
    if (firstShow) {
        firstShow = false;
        renderer_->updateFromRenderData(simulationEngine_->GetRenderData());
    }
}

MainWindow::~MainWindow() {
    delete simulationController_;
    delete simulationEngine_;
}

void MainWindow::onStartClicked() {
    simulationController_->start();
}

void MainWindow::onPauseClicked() {
    simulationController_->pause();
}

void MainWindow::onStepClicked() {
    simulationController_->step();
    renderer_->updateFromRenderData(simulationEngine_->GetRenderData());
}

void MainWindow::onSimulationTick() {
    simulationController_->step();
    renderer_->updateFromRenderData(simulationEngine_->GetRenderData());
}

void MainWindow::onSimulationStateChanged(SimState state) {
    switch (state) {
        case SimState::RUNNING:
            simulationTimer_->start();
            startButton_->setEnabled(false);
            pauseButton_->setEnabled(true);
            break;
        case SimState::PAUSED:
        case SimState::STOPPED:
            simulationTimer_->stop();
            startButton_->setEnabled(true);
            pauseButton_->setEnabled(false);
            break;
    }
}
